#include "graphics.hpp"
#include "h265_slice_parser.h"
#include "h265_bitstream_parser_state.h"
#include "h265_common.h"

#include <cstring>

inline bool check_result(wis::Result result, const char* where)
{
    if (result.status == wis::Status::Ok) {
        return true;
    }
    std::printf(
        "%s failed: %d, platform_code: %d, error: %s\n",
        where,
        static_cast<int>(result.status),
        result.platform_code,
        result.error ? result.error : "None"
    );
    return false;
}

void Graphics::log_callback(wis::Severity severity, const char* message, uint64_t, void* data)
{
    const CallbackData* callback_data = static_cast<const CallbackData*>(data);
    if (!callback_data || !callback_data->device_created.load(std::memory_order::relaxed)) {
        return;
    }

    const char* severity_str = "UNKNOWN";
    switch (severity) {
    case wis::Severity::Verbose:
        severity_str = "VERBOSE";
        break;
    case wis::Severity::Info:
        severity_str = "INFO";
        break;
    case wis::Severity::Warning:
        severity_str = "WARNING";
        break;
    case wis::Severity::Error:
        severity_str = "ERROR";
        break;
    case wis::Severity::Fatal:
        severity_str = "FATAL";
        break;
    default:
        break;
    }
    std::printf("[%s] %s\n", severity_str, message ? message : "");
}

std::optional<Graphics> Graphics::Create(
    wis::StdCodecProfile codec_profile,
    wis::DataFormat output_format,
    uint32_t width,
    uint32_t height,
    uint64_t max_slice_size,
    h265nal::H265BitstreamParserState* parser_state,
    const wis::VideoDecodeH265Desc* h265_params
)
{
    std::optional<Graphics> out;
    out.emplace();
    auto& g = *out;

    // stash dimensions / format for later use
    g.framewidth = width;
    g.frameheight = height;
    g.codec_profile = codec_profile;
    g.out_format = output_format;
    g.parser_state = parser_state;
    g.callback_data = std::make_unique<CallbackData>();

    if (!g.platform.Init()) {
        std::printf("Failed to initialize SDL platform\n");
        out.reset();
        return out;
    }

    wis::DebugDesc debug_desc = {
        .enable_debug_layer = true,
        .callback = log_callback,
        .user_data = g.callback_data.get(),
    };

    wis::InstanceExtensionHeader* instance_exts[] = {g.platform.Extension()};
    wis::Result result;
    wis::Instance instance = wis::CreateInstance(&debug_desc, wis::span{instance_exts}, result);
    if (!check_result(result, "CreateInstance")) {
        out.reset();
        return out;
    }

    g.video_ext = wis::VideoDecodingExtension(wis::VideoCodecFlags::H265);
    wis::DeviceExtensionHeader* device_exts[] = {&g.video_ext};

    wis::CommandQueueDesc queue_descs[] = {
        {wis::CommandQueueType::Graphics, wis::CommandQueuePriority::High},
        {wis::CommandQueueType::VideoDecode, wis::CommandQueuePriority::Normal},
    };

    wis::DeviceRequirements requirements{};
    requirements.queue_descs = {queue_descs, 2};
    requirements.extensions = {device_exts, 1};

    wis::AdapterQuery adapters = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
    if (!check_result(result, "QueryAdapters")) {
        out.reset();
        return out;
    }

    for (size_t i = 0; i < adapters.GetAdapterCount(); ++i) {
        g.device = adapters.CreateDevice(i, requirements, result);
        if (result.status == wis::Status::Ok) {
            std::printf("Created device on adapter %zu\n", i);
            break;
        }
    }

    g.callback_data->device_created.store(true, std::memory_order::relaxed);

    if (!g.device) {
        std::printf("Failed to create device with VideoDecode queue\n");
        out.reset();
        return out;
    }

    wis::VideoCodecDesc codec_query{
        .codec_profile = codec_profile,
        .image_format = output_format,
        .width = width,
        .height = height,
    };
    uint64_t decode_input_buffer_size = max_slice_size;
    wis::Result caps_result;
    auto caps = g.video_ext.QueryCodecCaps(codec_query, caps_result);
    if (caps_result.status == wis::Status::Ok && caps.supported) {
        g.bitstream_alignment = caps.min_bitstream_buffer_size_alignment;
        decode_input_buffer_size = wis::aligned_size(max_slice_size, g.bitstream_alignment);
        if (decode_input_buffer_size == 0) {
            decode_input_buffer_size = g.bitstream_alignment;
        }
        std::printf(
            "Codec %dx%d supported (bitstream alignment %llu)\n",
            width,
            height,
            static_cast<unsigned long long>(caps.min_bitstream_buffer_size_alignment)
        );
    } else {
        std::printf("Codec %dx%d NOT supported\n", width, height);
        out.reset();
        return out;
    }

    g.video_queue = g.device.CreateCommandQueue(wis::CommandQueueType::VideoDecode, result);
    if (!check_result(result, "CreateCommandQueue(VideoDecode)")) {
        out.reset();
        return out;
    }

    g.fence = g.device.CreateFence(0, result);
    if (!check_result(result, "CreateFence")) {
        out.reset();
        return out;
    }

    g.video_alloc = g.device.CreateCommandAllocator(wis::CommandQueueType::VideoDecode, result);
    if (!check_result(result, "CreateCommandAllocator(VideoDecode)")) {
        out.reset();
        return out;
    }

    g.video_cl = g.video_ext.CreateCommandList(g.video_alloc, result);
    if (!check_result(result, "CreateCommandList(VideoDecode)")) {
        out.reset();
        return out;
    }

    wis::VideoDecoderDesc decoder_desc{
        .width = width,
        .height = height,
        .image_format = output_format,
        .codec_profile = codec_profile,
        .decode_picture_buffer_count = 16,
    };

    g.decoder = g.video_ext.CreateDecoder(decoder_desc, result);
    if (!check_result(result, "CreateDecoder")) {
        out.reset();
        return out;
    }

    {
        wis::VideoDecodeH265Desc default_params{
            .max_vps_count = h265_params ? h265_params->max_vps_count : 2,
            .max_sps_count = h265_params ? h265_params->max_sps_count : 2,
            .max_pps_count = h265_params ? h265_params->max_pps_count : 16,
            .vps = h265_params ? h265_params->vps : nullptr,
            .vps_count = h265_params ? h265_params->vps_count : 0,
            .sps = h265_params ? h265_params->sps : nullptr,
            .sps_count = h265_params ? h265_params->sps_count : 0,
            .pps = h265_params ? h265_params->pps : nullptr,
            .pps_count = h265_params ? h265_params->pps_count : 0,
        };
        wis::VideoDecodeParameterDesc param_desc{
            .codec = codec_profile,
            .av1 = nullptr,
            .h265 = &default_params,
        };
        g.decoder_params = g.video_ext.CreateParameters(g.decoder, param_desc, result);
        if (!check_result(result, "CreateParameters")) {
            out.reset();
            return out;
        }
    }

    // Get the resource allocator for texture / buffer creation
    g.allocator = g.device.GetResourceAllocator(result);
    if (!check_result(result, "GetResourceAllocator")) {
        out.reset();
        return out;
    }

    // Create the decode-output texture (NV12 / P010 etc.)
    {
        wis::TextureDesc tex_desc{
            .width = width,
            .height = height,
            .depth_or_array_size = 1,
            .mip_levels = 1,
            .format = output_format,
            .sample_count = wis::SampleCount::S1,
            .layout = wis::TextureLayout::Texture2D,
            .usage_flags = wis::TextureUsageFlags::VideoDecodeDst,
            .flags = wis::TextureFlags::None,
            .memory_type = wis::MemoryType::Default,
            .memory_flags = wis::MemoryFlags::None,
        };
        g.decode_output = g.allocator.CreateTexture(tex_desc, result);
        if (!check_result(result, "CreateTexture(decode output)")) {
            out.reset();
            return out;
        }
    }

    // Create the decode-input buffer for bitstream data
    {
        wis::BufferDesc buf_desc{
            .size_bytes = decode_input_buffer_size,
            .usage_flags = wis::BufferUsageFlags::VideoDecodeSrc,
            .memory_type = wis::MemoryType::Upload,
            .memory_flags = wis::MemoryFlags::Mapped,
        };
        g.decode_input = g.allocator.CreateBuffer(buf_desc, result);
        if (!check_result(result, "CreateBuffer(decode input)")) {
            out.reset();
            return out;
        }
    }

    std::printf("H.265 decoder created: %dx%d, dpb=16\n", width, height);
    std::printf("H.265 session parameters created\n");
    std::printf("H.265 input buffer created: %llu bytes\n", static_cast<unsigned long long>(decode_input_buffer_size));
    return out;
}

int Graphics::DecodeFrame(const SliceData& slice)
{
    wis::Result result;

    result = video_cl.Begin();
    if (!check_result(result, "VideoDecodeCommandList::Begin")) {
        return -1;
    }

    // Upload slice data to the decode input buffer
    {
        const uint64_t padded_size =
            (static_cast<uint64_t>(slice.data.size()) + bitstream_alignment - 1) / bitstream_alignment *
            bitstream_alignment;
        void* mapped_ptr = decode_input.Map();
        if (!mapped_ptr) {
            std::printf("Failed to map decode input buffer\n");
            return -1;
        }
        std::memcpy(mapped_ptr, slice.data.data(), slice.data.size());
        if (padded_size > slice.data.size()) {
            std::memset(
                static_cast<std::byte*>(mapped_ptr) + slice.data.size(),
                0,
                static_cast<size_t>(padded_size - slice.data.size())
            );
        }
    }

    // Transition output texture: Undefined -> VideoDecodeWrite
    {
        wis::TextureBarrier barrier{
            .sync_before = wis::BarrierSync::None,
            .sync_after = wis::BarrierSync::VideoDecode,
            .access_before = wis::ResourceAccess::None,
            .access_after = wis::ResourceAccess::VideoDecodeWrite,
            .state_before = wis::TextureState::Undefined,
            .state_after = wis::TextureState::VideoDecodeWrite,
            .flags = wis::BarrierFlags::PlanarImage,
            .texture = decode_output.GetView(),
            .subresource_range = {0, 1, 0, 1, 0, 2},
            .queue_type_before = wis::CommandQueueType::VideoDecode,
            .queue_type_after = wis::CommandQueueType::VideoDecode,
        };
        wis::BarrierGroup barrier_group{
            .texture_barriers = wis::span{&barrier, 1},
        };
        video_cl.InsertBarriers(barrier_group);
    }

    // Build picture info from slice header
    wis::StdVideoDecodeH265PictureInfo pic_info{};
    wis::StdVideoDecodeH265ReferenceInfo ref_info{};
    {
        // Parse slice header
        auto slice_header = h265nal::H265SliceSegmentHeaderParser::ParseSliceSegmentHeader(
            slice.data.data() + 2,
            slice.data.size() - 2,
            slice.nal_unit_type,
            parser_state
        );
        
        if (slice_header) {
            pic_info.flags.IrapPicFlag = (slice.nal_unit_type >= 16 && slice.nal_unit_type <= 21) ? 1 : 0;
            pic_info.flags.IdrPicFlag = (slice.nal_unit_type == 19 || slice.nal_unit_type == 20) ? 1 : 0;
            pic_info.flags.IsReference = (slice.nal_unit_type <= 21) ? 1 : 0;
            pic_info.flags.short_term_ref_pic_set_sps_flag = slice_header->short_term_ref_pic_set_sps_flag;
            pic_info.sps_video_parameter_set_id = 0;
            pic_info.pps_seq_parameter_set_id = slice_header->slice_pic_parameter_set_id;
            pic_info.pps_pic_parameter_set_id = slice_header->slice_pic_parameter_set_id;
            pic_info.PicOrderCntVal = static_cast<int32_t>(slice_header->slice_pic_order_cnt_lsb);
            pic_info.NumDeltaPocsOfRefRpsIdx = 0;
            pic_info.NumBitsForSTRefPicSetInSlice = 0;
        } else {
            // Default for first frame (IDR)
            pic_info.flags.IrapPicFlag = 1;
            pic_info.flags.IdrPicFlag = 1;
            pic_info.flags.IsReference = 1;
            pic_info.flags.short_term_ref_pic_set_sps_flag = 1;
            pic_info.sps_video_parameter_set_id = 0;
            pic_info.pps_seq_parameter_set_id = 0;
            pic_info.pps_pic_parameter_set_id = 0;
            pic_info.PicOrderCntVal = 0;
            pic_info.NumDeltaPocsOfRefRpsIdx = 0;
            pic_info.NumBitsForSTRefPicSetInSlice = 0;
        }
    }

    ref_info.PicOrderCntVal = pic_info.PicOrderCntVal;

    wis::VideoDecodeOutputDesc output_desc{
        .output_texture = decode_output.GetView(),
        .format = out_format,
        .subresource = 0,
    };

    wis::VideoDecodePictureDesc picture_desc{
        .codec = codec_profile,
        .av1_picture_info = nullptr,
        .h265_picture_info = &pic_info,
        .av1_reference_info = nullptr,
        .h265_reference_info = &ref_info,
        .reference_frame_count = 0,
    };

    const uint64_t input_size =
        (static_cast<uint64_t>(slice.data.size()) + bitstream_alignment - 1) / bitstream_alignment * bitstream_alignment;
    wis::VideoDecodeInputDesc input_desc{
        .bitstream_buffer = decode_input,
        .offset = 0,
        .size = input_size,
    };
    video_cl.DecodeFrame(decoder, decoder_params, input_desc, output_desc, picture_desc);

    // Transition output texture: VideoDecodeWrite -> Common
    {
        wis::TextureBarrier barrier{
            .sync_before = wis::BarrierSync::VideoDecode,
            .sync_after = wis::BarrierSync::None,
            .access_before = wis::ResourceAccess::VideoDecodeWrite,
            .access_after = wis::ResourceAccess::None,
            .state_before = wis::TextureState::VideoDecodeWrite,
            .state_after = wis::TextureState::Common,
            .flags = wis::BarrierFlags::PlanarImage,
            .texture = decode_output.GetView(),
            .subresource_range = {0, 1, 0, 1, 0, 2},
            .queue_type_before = wis::CommandQueueType::VideoDecode,
            .queue_type_after = wis::CommandQueueType::VideoDecode,
        };
        wis::BarrierGroup barrier_group{
            .texture_barriers = wis::span{&barrier, 1},
        };
        video_cl.InsertBarriers(barrier_group);
    }

    result = video_cl.End();
    if (!check_result(result, "VideoDecodeCommandList::End")) {
        return -1;
    }

    wis::CommandListView cmd_view = video_cl;
    result = video_queue.Submit({&cmd_view, 1});
    if (!check_result(result, "VideoQueue::Submit")) {
        return -1;
    }

    result = video_queue.SignalFence(fence.GetView(), 1);
    if (!check_result(result, "VideoQueue::SignalFence")) {
        return -1;
    }

    result = fence.Wait(1, UINT64_MAX);
    if (!check_result(result, "Fence::Wait")) {
        return -1;
    }

    std::printf("Slice decoded successfully (type=%u, size=%zu bytes)\n", 
                slice.nal_unit_type, slice.data.size());
    return 0;
}
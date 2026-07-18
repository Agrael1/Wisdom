#include "graphics.hpp"

void Graphics::log_callback(wis::Severity severity, const char* message, uint64_t, void*)
{
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

    if (!g.platform.Init()) {
        std::printf("Failed to initialize SDL platform\n");
        out.reset();
        return out;
    }

    wis::DebugDesc debug_desc = {
        .enable_debug_layer = true,
        .callback = log_callback,
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
    auto caps_result = g.video_ext.QueryCodecCaps(codec_query);
    if (caps_result.status == wis::Status::Ok) {
        std::printf("Codec %dx%d supported\n", width, height);
    } else {
        std::printf("Codec %dx%d NOT supported\n", width, height);
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

    std::printf("H.265 decoder created: %dx%d, dpb=16\n", width, height);
    std::printf("H.265 session parameters created\n");
    return out;
}

int Graphics::Frame()
{
    wis::Result result;

    result = video_cl.Begin();
    if (!check_result(result, "VideoDecodeCommandList::Begin")) {
        return -1;
    }

    // Transition output texture: Undefined → VideoDecodeWrite
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

    // Frame-level picture info for H.265
    wis::StdVideoDecodeH265PictureInfo pic_info{};
    pic_info.flags.IrapPicFlag = 1;
    pic_info.flags.IdrPicFlag = 1;
    pic_info.flags.IsReference = 1;
    pic_info.flags.short_term_ref_pic_set_sps_flag = 1;
    pic_info.sps_video_parameter_set_id = 0;
    pic_info.pps_seq_parameter_set_id = 0;
    pic_info.pps_pic_parameter_set_id = 0;
    pic_info.PicOrderCntVal = 0;

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
        .h265_reference_info = nullptr,
        .reference_frame_count = 0,
    };

    wis::VideoDecodeInputDesc input_desc{
        .bitstream_buffer = decode_input,
        .offset = 0,
        .size = 0,
    };
    video_cl.DecodeFrame(decoder, input_desc, output_desc, picture_desc);

    // Transition output texture: VideoDecodeWrite → Common
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

    std::printf("Video decode command list submitted and completed\n");
    return 0;
}

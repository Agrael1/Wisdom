#include "graphics.hpp"
#include "h265_bitstream_parser_state.h"
#include "h265_common.h"
#include "h265_slice_parser.h"

#include <cstring>
#include <fstream>

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

static wis::Shader create_shader(const wis::Device* device, std::string_view basename)
{
    wis::Shader shader{};

    constexpr const char* extension = wis::shader_intermediate == wis::ShaderIntermediate::SPIRV ? ".spv" : ".cso";

    std::string filename;
    filename.reserve(basename.size() + 8);
    filename.append(basename);
    filename.append(extension);

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return shader;
    }

    std::vector<uint8_t> bytes{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
    if (bytes.empty()) {
        return shader;
    }

    wis::Result result;
    shader = device->CreateShader({bytes.data(), bytes.size()}, result);
    if (result.status == wis::Status::Ok) {
        return shader;
    }

    printf(
        "Failed to create shader from file %s: %d, platform_code: %d, error: %s\n",
        filename.c_str(),
        result.status,
        result.platform_code,
        result.error ? result.error : "None"
    );

    return shader;
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
    SDL_Window* window,
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
        .decode_picture_buffer_count = 0,
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
        wis::DataFormat cast_formats[] = {wis::DataFormat::R8Unorm, wis::DataFormat::RG8Unorm};
        wis::TextureDesc tex_desc{
            .width = width,
            .height = height,
            .depth_or_array_size = 1,
            .mip_levels = 1,
            .format = output_format,
            .sample_count = wis::SampleCount::S1,
            .layout = wis::TextureLayout::Texture2D,
            .usage_flags = wis::TextureUsageFlags::VideoDecodeDst | wis::TextureUsageFlags::ShaderResource,
            .flags = wis::TextureFlags::None,
            .memory_type = wis::MemoryType::Default,
            .memory_flags = wis::MemoryFlags::None,
            .cast_formats = {cast_formats},
        };
        g.decode_output = g.allocator.CreateTexture(tex_desc, result);
        if (!check_result(result, "CreateTexture(decode output)")) {
            out.reset();
            return out;
        }
    }

    g.view_heap = g.device.CreateViewHeap(
        wis::ViewHeapType::RenderTarget,
        1 + kSwapchainImages,
        wis::ViewHeapFlags::AllowVideoTargets,
        result
    );
    if (!check_result(result, "CreateViewHeap(decode output)")) {
        out.reset();
        return out;
    }
    {
        wis::RenderTargetDesc view_desc{
            .format = output_format,
            .layout = wis::TextureLayout::Texture2D,
            .mip_level = 0,
            .base_array_layer = 0,
            .array_layer_count = 1,
            .plane_slice = 0,
        };
        g.decode_output_view = g.view_heap.WriteVideoDecodeTarget(g.decode_output, view_desc, 0);
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

    std::printf("H.265 decoder created: %dx%d, dpb=0\n", width, height);
    std::printf("H.265 session parameters created\n");
    std::printf("H.265 input buffer created: %llu bytes\n", static_cast<unsigned long long>(decode_input_buffer_size));

    out->InitRenderingResources(window);

    return out;
}

int Graphics::DecodeFrame(const SliceData& slice)
{
    wis::Result result;

    result = video_alloc.Reset();
    if (!check_result(result, "VideoDecodeCommandAllocator::Reset")) {
        return -1;
    }

    result = video_cl.Begin();
    if (!check_result(result, "VideoDecodeCommandList::Begin")) {
        return -1;
    }

    // Upload slice data to the decode input buffer
    {
        const uint64_t padded_size = (static_cast<uint64_t>(slice.data.size()) + bitstream_alignment - 1)
                                   / bitstream_alignment * bitstream_alignment;
        char* mapped_ptr = (char*)decode_input.Map();
        if (!mapped_ptr) {
            std::printf("Failed to map decode input buffer\n");
            return -1;
        }
        std::memcpy(mapped_ptr, slice.data.data(), slice.data.size());
        if (padded_size > slice.data.size()) {
            std::memset(
                mapped_ptr + slice.data.size(),
                0,
                static_cast<size_t>(padded_size - slice.data.size())
            );
        }
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

    wis::VideoDecodePictureDesc picture_desc{
        .codec = codec_profile,
        .av1_picture_info = nullptr,
        .h265_picture_info = &pic_info,
        .av1_reference_info = nullptr,
        .h265_reference_info = &ref_info,
        .reference_frame_count = 0,
    };

    const uint64_t input_size = (static_cast<uint64_t>(slice.data.size()) + bitstream_alignment - 1)
                              / bitstream_alignment * bitstream_alignment;
    wis::VideoDecodeInputDesc input_desc{
        .bitstream_buffer = decode_input,
        .offset = 0,
        .size = input_size,
    };

    // Transition output texture into the layout required for this frame.
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
    video_cl.DecodeFrame(decoder, decoder_params, input_desc, picture_desc, decode_output_view);

    result = video_cl.End();
    if (!check_result(result, "VideoDecodeCommandList::End")) {
        return -1;
    }

    wis::CommandListView cmd_view = video_cl;
    result = video_queue.Submit({&cmd_view, 1});
    if (!check_result(result, "VideoQueue::Submit")) {
        return -1;
    }

    result = video_queue.SignalFence(fence.GetView(), ++decode_fence_value);
    if (!check_result(result, "VideoQueue::SignalFence")) {
        return -1;
    }

    return 0;
}

void Graphics::InitRenderingResources(SDL_Window* window)
{
    // Create a descriptor heap for rendering
    wis::Result result;

    // Create Command Queue and Command Allocator for rendering
    {
        render_queue = device.CreateCommandQueue(wis::CommandQueueType::Graphics, result);
        if (!check_result(result, "CreateCommandQueue(Graphics)")) {
            return;
        }
        render_alloc = device.CreateCommandAllocator(wis::CommandQueueType::Graphics, result);
        if (!check_result(result, "CreateCommandAllocator(Graphics)")) {
            return;
        }
        render_cl = render_alloc.CreateCommandList(result);
        if (!check_result(result, "CreateCommandList(Graphics)")) {
            return;
        }
        render_fence = device.CreateFence(0, result);
        if (!check_result(result, "CreateFence(Graphics)")) {
            return;
        }
    }

    // Get the swapchain format for the window
    {
        int win_width = 0, win_height = 0;
        SDL_GetWindowSize(window, &win_width, &win_height);
        swapchain_width = static_cast<uint32_t>(win_width);
        swapchain_height = static_cast<uint32_t>(win_height);

        auto surface = platform.CreateWindowSurface(window);
        render_format = device.GetFormatPresentationSupport(surface.GetView(), wis::DataFormat::RGB10A2Unorm)
                          ? wis::DataFormat::RGB10A2Unorm
                          : wis::DataFormat::BGRA8Unorm;

        wis::SwapchainDesc swapchain_desc = {
            .width = swapchain_width,
            .height = swapchain_height,
            .image_count = kSwapchainImages,
            .texture_usage_flags = wis::TextureUsageFlags::RenderTarget,
            .format = render_format,
            .scaling = wis::SwapchainScaling::None,
            .flags = wis::SwapchainFlags::VSync,
            .composite_alpha = wis::CompositeAlpha::Opaque,
        };

        swapchain = device.CreateSwapchain(surface, render_queue, swapchain_desc, result);
        if (!check_result(result, "Device::CreateSwapchain")) {
            return;
        }

        result = swapchain.GetTextures(swapchain_textures);
        if (!check_result(result, "Swapchain::GetTextures")) {
            return;
        }

        for (size_t i = 0; i < kSwapchainImages; ++i) {
            wis::RenderTargetDesc view_desc{
                .format = render_format,
                .layout = wis::TextureLayout::Texture2D,
                .mip_level = 0,
                .base_array_layer = 0,
                .array_layer_count = 1,
                .plane_slice = 0,
            };

            swapchain_views[i] = view_heap.WriteRenderTarget(
                swapchain_textures[i],
                view_desc,
                i + 1
            ); // +1 because 0 is reserved for the decode output view
            if (!check_result(result, "CreateRenderTargetView")) {
                return;
            }
        }
    }

    {
        wis::DescriptorHeapDesc heap_desc{
            .type = wis::DescriptorHeapType::Descriptor,
            .memory_type = wis::DescriptorMemoryType::ShaderVisible,
            .descriptor_count = 2,
            .flags = wis::DescriptorHeapFlags::None,
        };
        wis::DescriptorHeapDesc samp_desc{
            .type = wis::DescriptorHeapType::Sampler,
            .memory_type = wis::DescriptorMemoryType::ShaderVisible,
            .descriptor_count = 1,
            .flags = wis::DescriptorHeapFlags::DisallowEmbeddedSamplers,
        };

        desc_heap = device.CreateDescriptorHeap(heap_desc, result);
        if (!check_result(result, "CreateDescriptorHeap")) {
            return;
        }

        sampler_heap = device.CreateDescriptorHeap(samp_desc, result);
        if (!check_result(result, "CreateSamplerHeap")) {
            return;
        }
    }

    // Write descriptors for the decoded texture and the swapchain textures
    {
        wis::TextureBinding srv_desc_Y{
            .format = wis::DataFormat::R8Unorm,
            .layout = wis::TextureLayout::Texture2D,
            .range = {0, 1, 0, 1, 1, 1},
        };
        wis::TextureBinding srv_desc_UV{
            .format = wis::DataFormat::RG8Unorm,
            .layout = wis::TextureLayout::Texture2D,
            .range = {0, 1, 0, 1, 2, 1},
        };

        result = desc_heap.WriteTexture(decode_output, srv_desc_Y, 0);
        if (!check_result(result, "WriteShaderResource(decode output)")) {
            return;
        }

        result = desc_heap.WriteTexture(decode_output, srv_desc_UV, 1);
        if (!check_result(result, "WriteShaderResource(decode output)")) {
            return;
        }
    }

    // Create a sampler for the decoded texture
    {
        result = sampler_heap.WriteSampler(
            wis::SamplerDesc{
                .min_filter = wis::Filter::Point,
                .mag_filter = wis::Filter::Point,
                .mip_filter = wis::Filter::Point,
                .reduction_mode = wis::ReductionMode::Standard,
                .address_u = wis::AddressMode::ClampToEdge,
                .address_v = wis::AddressMode::ClampToEdge,
                .address_w = wis::AddressMode::ClampToEdge,
            },
            0
        );
        if (!check_result(result, "WriteSampler")) {
            return;
        }
    }

    // Create a root signature for rendering the decoded texture
    {
        wis::DescriptorTableEntry table_entries[] = {
            {
                .type = wis::DescriptorType::Texture,
                .bind_register = 0,
                .count = 1,
            },
            {.type = wis::DescriptorType::Texture, .bind_register = 1, .count = 1, .descriptor_offset = 1},
            {
                .type = wis::DescriptorType::Sampler,
                .bind_register = 0,
                .count = 1,
            },
        };
        wis::DescriptorTable table_descs[] = {
            {
                .visibility = wis::ShaderVisibility::Pixel,
                .entries = {table_entries, 2},
            },
            {
                .visibility = wis::ShaderVisibility::Pixel,
                .entries = {table_entries + 2, 1},
            },
        };
        wis::RootSignatureDesc root_desc{
            .descriptor_tables = {table_descs, 2},
        };
        root_signature = device.CreateRootSignature(root_desc, result);
        if (!check_result(result, "CreateRootSignature")) {
            return;
        }
    }

    // Create a simple pipeline state for rendering the decoded texture
    {
        wis::Shader vs = create_shader(&device, "fullscreen.vs.hlsl");
        wis::Shader ps = create_shader(&device, "video.ps.hlsl");

        wis::RenderAttachmentsDesc attachments = {};
        attachments.attachment_formats[0] = render_format;
        attachments.attachments_count = 1;

        wis::GraphicsPipelineDesc pipeline_desc{
            .root_signature = root_signature,
            .vertex_shader = vs,
            .pixel_shader = ps,
            .topology_type = wis::TopologyType::Triangle,
        };
        pipeline_state = device.CreateGraphicsPipeline(pipeline_desc, result);

        if (!check_result(result, "CreateGraphicsPipeline")) {
            return;
        }
    }
}

void Graphics::RenderFrame()
{
    wis::Result result;
    result = render_alloc.Reset();

    result = render_cl.Begin();

    result = render_cl.End();

    // Submit the render command list and present the swapchain
    wis::CommandListView list_view = render_cl.GetView();
    result = render_queue.Submit({&list_view, 1});
    if (!check_result(result, "Submit render command list")) {
        return;
    }

    result = swapchain.Present(wis::PresentFlags::None, {});
    if (!check_result(result, "Swapchain::Present")) {
        return;
    }

    result = render_queue.SignalFence(render_fence.GetView(), ++fence_value);
    if (!check_result(result, "SignalFence")) {
        return;
    }

    result = render_fence.Wait(fence_value, UINT64_MAX);
    if (!check_result(result, "WaitFence")) {
        return;
    }
}

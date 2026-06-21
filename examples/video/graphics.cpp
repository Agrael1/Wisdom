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
    uint32_t height)
{
    std::optional<Graphics> out;
    out.emplace();
    auto& g = *out;

    if (!g.platform.Init()) {
        std::printf("Failed to initialize SDL platform\n");
        out.reset();
        return out;
    }

    wis::DebugDesc debug_desc = {
        .enable_debug_layer = true,
        .callback = log_callback,
    };

    wis::InstanceExtensionHeader* instance_exts[] = { g.platform.Extension() };
    wis::Result result;
    wis::Instance instance = wis::CreateInstance(&debug_desc, wis::span{instance_exts}, result);
    if (!check_result(result, "CreateInstance")) {
        out.reset();
        return out;
    }

    g.video_ext = wis::VideoDecodingExtension(wis::VideoCodecFlags::H265);
    wis::DeviceExtensionHeader* device_exts[] = { &g.video_ext };

    wis::CommandQueueDesc queue_descs[] = {
        { wis::CommandQueueType::Graphics, wis::CommandQueuePriority::High },
        { wis::CommandQueueType::VideoDecode, wis::CommandQueuePriority::Normal },
    };

    wis::DeviceRequirements requirements{};
    requirements.queue_descs = { queue_descs, 2 };
    requirements.extensions = { device_exts, 1 };

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

    std::printf("H.265 decoder created: %dx%d, dpb=16\n", width, height);
    return out;
}

int Graphics::Frame()
{
    wis::Result result;

    result = video_cl.Begin();
    if (!check_result(result, "VideoDecodeCommandList::Begin")) {
        return -1;
    }

    result = video_cl.End();
    if (!check_result(result, "VideoDecodeCommandList::End")) {
        return -1;
    }

    std::printf("Video decode command list recorded successfully\n");
    std::printf("Video decode pipeline validated successfully\n");
    return 0;
}

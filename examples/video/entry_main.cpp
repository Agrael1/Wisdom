#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <format>

static bool check_result(wis::Result result, const char* where)
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

class Application
{
    static void log_callback(wis::Severity severity, const char* message, uint64_t, void* data)
    {
        static_cast<Application*>(data)->log_callback(severity, message, 0);
    }
    void log_callback(wis::Severity severity, const char* message, uint64_t)
    {
        if (!_device.IsValid()) {
            // Avoid logging messages before instance creation
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
        std::printf("[Member %s] %s\n", severity_str, message ? message : "");
    }

public:
    Application()
        : _device(CreateDevice())
        , _video_decoder(CreateDecoder())
    {
        wis::Result result{};

        _video_queue = _device.CreateCommandQueue(wis::CommandQueueType::VideoDecode, result);
        if (!check_result(result, "CreateCommandQueue")) {
            return;
        }

        _video_command_allocator = _device.CreateCommandAllocator(wis::CommandQueueType::VideoDecode, result);
        if (!check_result(result, "CreateCommandAllocator")) {
            return;
        }
        _vcmd = _video_extension.CreateCommandList(_video_command_allocator, result);
        if (!check_result(result, "CreateVideoDecodeCommandList")) {
            return;
        }
    }

private:
    wis::Device CreateDevice()
    {
        wis::Result result{};
        wis::Device device{};

        wis::DebugDesc debug_desc = {
            .enable_debug_layer = true,
            .callback = log_callback,
            .user_data = this,
        };

        wis::Instance instance = wis::CreateInstance(&debug_desc, {}, result);
        if (!check_result(result, "CreateInstance")) {
            return device;
        }

        // Query adapters
        wis::AdapterQuery adapters = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
        if (!check_result(result, "QueryAdapters")) {
            return device;
        }

        // Cycle through adapters and create device
        wis::DeviceExtensionHeader* extensions[] = {&_video_extension};
        wis::CommandQueueDesc queue_descs[] = {
            {wis::CommandQueueType::VideoDecode, wis::CommandQueuePriority::Normal},
        };
        wis::DeviceRequirements requirements{
            .queue_descs = {queue_descs},
            .extensions = {extensions},
        };

        for (size_t i = 0; i < adapters.GetAdapterCount(); ++i) {
            device = adapters.CreateDevice(i, requirements, result);
            if (result.status == wis::Status::Ok) {
                // Get adapter description for logging purposes
                wis::AdapterDesc adapter_desc = adapters.GetAdapterDesc(i, result);
                std::printf(
                    "Successfully created device for adapter: %s, vendor_id: %u, device_id: %u\n",
                    adapter_desc.description.data(),
                    adapter_desc.vendor_id,
                    adapter_desc.device_id
                );
                PrintCapability();
                return device;
            }
        }

        assert(_device.IsValid() && "Failed to create device with video decoding support");
        return device;
    }
    wis::VideoDecoder CreateDecoder()
    {
        wis::Result result{};
        wis::VideoDecoderDesc decoder_desc{
            .width = 1920,
            .height = 1080,
            .image_format = wis::DataFormat::NV12,
            .codec_profile = wis::StdCodecProfile::AV1Main,
            .decode_picture_buffer_count = 4,
        };
        wis::VideoDecoder video_decoder = _video_extension.CreateDecoder(decoder_desc, result);
        if (!check_result(result, "CreateDecoder")) {
            return video_decoder;
        }
        //assert(video_decoder.IsValid() && "Failed to create video decoder for AV1 Main profile with NV12 format");
        return video_decoder;
    }
    void PrintCapability()
    {
        wis::Result result{};

        result = _video_extension.QueryCodecCaps({
            .codec_profile = wis::StdCodecProfile::AV1Main,
            .image_format = wis::DataFormat::NV12,
            .width = 1920,
            .height = 1080,
        });
        auto str = std::format(
            "Codec: AV1 Main; Format: NV12; {}",
            result.status == wis::Status::Ok ? "Supported" : "Not Supported"
        );
        std::printf("%s\n", str.c_str());
        assert(result.status == wis::Status::Ok && "AV1 Main profile with NV12 format should be supported");
    }

private:
    wis::VideoDecodingExtension _video_extension{wis::VideoCodecFlags::AV1};
    wis::Device _device;
    wis::VideoDecoder _video_decoder;
    wis::CommandQueue _video_queue;
    wis::CommandAllocator _video_command_allocator;
    wis::VideoDecodeCommandList _vcmd;
};

int main()
{
    Application app;
    return 0;
}

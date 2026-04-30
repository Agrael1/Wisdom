#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>

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
    static void log_callback(wis::Severity severity, const char* message, uint64_t, void*)
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

public:
    Application()
        : video_extension{wis::VideoCodecFlags::AV1}
        , device{CreateDevice()}
    {}

private:
    wis::Device CreateDevice()
    {
        wis::Device device{};
        wis::Result result{};

        wis::DebugDesc debug_desc = {
            .enable_debug_layer = false,
            .callback = log_callback,
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
        wis::DeviceExtensionHeader* extensions[] = {&video_extension};
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

                break;
            }
        }

        if (video_extension.Supported()) {
            std::printf("Video decoding is supported on this device.\n");
        } else {
            std::printf("Video decoding is not supported on this device.\n");
        }

        return device;
    }

private:
    wis::VideoDecodingExtension video_extension;
    wis::Device device;
};

int main()
{
    Application app;
    return 0;
}

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
    Application() { CreateDevice(); }

private:
    wis::Device CreateDevice()
    {
        wis::Device device{};
        wis::VideoDecodingExtension video_extension{
            wis::VideoCodecFlags::AV1 | wis::VideoCodecFlags::H264 | wis::VideoCodecFlags::H265
            | wis::VideoCodecFlags::VP9
        };
        wis::Result result{};

        wis::DebugDesc debug_desc = {
            //.enable_debug_layer = true,
            //.callback = log_callback,
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

                PrintCapabilities(video_extension);
            }
        }

        return device;
    }

    void PrintCapabilities(wis::VideoDecodingExtension& video_extension)
    {
        wis::Result result{};

        constexpr wis::DataFormat formats[] = {
            wis::DataFormat::NV12,
            wis::DataFormat::P010,
            wis::DataFormat::P012,
            wis::DataFormat::P016,
        };
        constexpr wis::StdCodecProfile profiles[] = {
            wis::StdCodecProfile::H264Baseline,
            wis::StdCodecProfile::H264Main,
            wis::StdCodecProfile::H264High,
            wis::StdCodecProfile::H264HighPredictive,
            wis::StdCodecProfile::H265Main,
            wis::StdCodecProfile::H265Main10,
            wis::StdCodecProfile::H265Main12,
            wis::StdCodecProfile::H265Main16,
            wis::StdCodecProfile::H265FormatRangeExt,
            wis::StdCodecProfile::AV1Main,
            wis::StdCodecProfile::AV1High,
            wis::StdCodecProfile::AV1Professional,
            wis::StdCodecProfile::VP9Profile0,
            wis::StdCodecProfile::VP9Profile1,
            wis::StdCodecProfile::VP9Profile2,
            wis::StdCodecProfile::VP9Profile3,
        };
        constexpr std::string_view profile_strs[] = {
            "H.264 Baseline",
            "H.264 Main",
            "H.264 High",
            "H.264 High Predictive",
            "H.265 Main",
            "H.265 Main 10",
            "H.265 Main 12",
            "H.265 Main 16",
            "H.265 Format Range Extension",
            "AV1 Main",
            "AV1 High",
            "AV1 Professional",
            "VP9 Profile 0",
            "VP9 Profile 1",
            "VP9 Profile 2",
            "VP9 Profile 3",
        };
        static_assert(
            std::size(profiles) == std::size(profile_strs),
            "Profiles and profile strings arrays must be of the same size"
        );

        constexpr std::string_view format_strs[] = {
            "NV12",
            "P010",
            "P012",
            "P016",
        };

        for (uint32_t i = 0; i < std::size(profile_strs); i++) {
            auto codec = profiles[i];
            auto codec_str = profile_strs[i];

            for (uint32_t j = 0; j < std::size(format_strs); j++) {
                auto format = formats[j];
                auto format_str = format_strs[j];

                result = video_extension.QueryCodecCaps(
                    {
                        .codec_profile = codec,
                        .data_format = format,
                        .width = 1920,
                        .height = 1080,
                    }
                );
                auto str = std::format(
                    "Codec: {}; Format: {}; {}",
                    codec_str,
                    format_str,
                    result.status == wis::Status::Ok ? "Supported" : "Not Supported"
                );
                std::printf("%s\n", str.c_str());
            }
            std::printf("\n");
        }
    }
};

int main()
{
    Application app;
    return 0;
}

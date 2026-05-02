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
    Application()
        : video_extension{wis::VideoCodecFlags::AV1 | wis::VideoCodecFlags::H264 | wis::VideoCodecFlags::H265 | wis::VideoCodecFlags::VP9}
        , device{CreateDevice()}
    {
        wis::Result result{};

        constexpr wis::ComponentBitDepth bit_depths[] = {
            wis::ComponentBitDepth::Bit8,
            wis::ComponentBitDepth::Bit10,
            wis::ComponentBitDepth::Bit12,
        };
        constexpr wis::ChromaSubsampling chroma_subsamplings[] = {
            wis::ChromaSubsampling::C420,
            wis::ChromaSubsampling::C422,
            wis::ChromaSubsampling::C444,
        };
        constexpr wis::StdCodecProfile profiles[] = {
            wis::StdCodecProfile::H264Baseline,
            wis::StdCodecProfile::H264Main,
            wis::StdCodecProfile::H264High,
            wis::StdCodecProfile::H264HighPredictive,
            wis::StdCodecProfile::H265Main,
            wis::StdCodecProfile::H265Main10,
            wis::StdCodecProfile::H265FormatRangeExt,
            wis::StdCodecProfile::H265SCCExt,
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
            "H.265 Format Range Extension",
            "H.265 Screen Content Coding Extension",
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

        constexpr std::string_view bit_depth_strs[] = {
            "8-bit",
            "10-bit",
            "12-bit",
        };
        constexpr std::string_view chroma_subsampling_strs[] = {
            "4:2:0",
            "4:2:2",
            "4:4:4",
        };

        for (uint32_t i = 0; i < std::size(profile_strs); i++) {
            auto codec = profiles[i];
            auto codec_str = profile_strs[i];

            for (uint32_t j = 0; j < std::size(bit_depth_strs); j++) {
                auto bit_depth = bit_depths[j];
                auto bit_depth_str = bit_depth_strs[j];


                for (uint32_t k = 0; k < std::size(chroma_subsampling_strs); k++) {
                    auto caps = video_extension.QueryCodecCaps(
                        {
                            .codec_profile = codec,
                            .bit_depth = bit_depth,
                            .chroma_subsampling = chroma_subsamplings[k],
                        },
                        result
                    );
                    auto str = std::format(
                        "Codec: {}; Bit Depth: {}; Chroma Subsampling: {}; Max Resolution: {}x{}; {}",
                        codec_str,
                        bit_depth_str,
                        chroma_subsampling_strs[k],
                        caps.max_width,
                        caps.max_height,
                        caps.max_width != 0 && caps.max_height != 0 ? "Supported" : "Not Supported"
                    );
                    std::printf("%s\n", str.c_str());
                }
                std::printf("\n");
            }
            std::printf("\n");
        }
    }

private:
    wis::Device CreateDevice()
    {
        wis::Device device{};
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

                break;
            }
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

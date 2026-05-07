#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <catch2/catch_test_macros.hpp>
#include <format>
#include <iostream>

static bool check_result(wis::Result result, const char*)
{
    if (result.status == wis::Status::Ok) {
        return true;
    }
    return false;
}

static void log_callback(wis::Severity severity, const char* message, uint64_t, void* data)
{
    if ((severity == wis::Severity::Error || severity == wis::Severity::Fatal) && (*(bool*)data)) {
        FAIL("Error or Critical message in log: " << (message ? message : ""));
    }
}

TEST_CASE("video.check_support")
{
    wis::Device device{};
    wis::VideoDecodingExtension video_extension{
        wis::VideoCodecFlags::AV1 | wis::VideoCodecFlags::H264 | wis::VideoCodecFlags::H265 | wis::VideoCodecFlags::VP9
    };
    wis::Result result{};

    bool instance_created = false;
    wis::DebugDesc debug_desc = {
        .enable_debug_layer = true,
        .callback = log_callback,
        .user_data = &instance_created,
    };

    wis::Instance instance = wis::CreateInstance(&debug_desc, {}, result);
    if (!check_result(result, "CreateInstance")) {
        return;
    }
    instance_created = true;

    wis::AdapterQuery adapters = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
    if (!check_result(result, "QueryAdapters")) {
        return;
    }

    wis::DeviceExtensionHeader* extensions[] = {&video_extension};
    wis::CommandQueueDesc queue_descs[] = {
        {wis::CommandQueueType::VideoDecode, wis::CommandQueuePriority::Normal},
    };
    wis::DeviceRequirements requirements{
        .queue_descs = {queue_descs},
        .extensions = {extensions},
    };

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

    bool any_supported = false;
    for (size_t i = 0; i < adapters.GetAdapterCount(); ++i) {
        device = adapters.CreateDevice(i, requirements, result);
        if (result.status == wis::Status::Ok) {
            for (auto profile : profiles) {
                for (auto format : formats) {
                    result = video_extension.QueryCodecCaps({
                        .codec_profile = profile,
                        .image_format = format,
                        .width = 1920,
                        .height = 1080,
                    });
                    if (result.status == wis::Status::Ok) {
                        std::cout << std::format(
                            "Codec supported: profile {}, format {}\n",
                            static_cast<int>(profile),
                            static_cast<int>(format)
                        );
                        any_supported = true;
                    }
                }
            }
            break; // only test the first adapter that supports video decoding
        }
    }

    if (!any_supported) {
        return; // got nothing to test here
    }

    // If there is any support for video, H264 Main profile with NV12 format should be supported on all hardware, so
    // we can test creating a decoder with it.
    wis::VideoDecoder decoder = video_extension.CreateDecoder(
        {
            .width = 1920,
            .height = 1080,
            .image_format = wis::DataFormat::NV12,
            .codec_profile = wis::StdCodecProfile::H264Main,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);
    //REQUIRE(decoder.IsValid()); // TODO: initialize first
}

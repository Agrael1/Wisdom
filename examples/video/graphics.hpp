#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <cstdio>
#include <optional>
#include <sdl_backend_cpp.h>

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

class Graphics
{
    static void log_callback(wis::Severity severity, const char* message, uint64_t, void*);

public:
    static std::optional<Graphics> Create(
        wis::StdCodecProfile codec_profile,
        wis::DataFormat output_format,
        uint32_t width,
        uint32_t height);

    int Frame();

private:
    SDLPlatformCpp platform;
    wis::Device device;
    wis::VideoDecodingExtension video_ext{wis::VideoCodecFlags::H265};
    wis::CommandQueue video_queue;
    wis::Fence fence;
    wis::CommandAllocator video_alloc;
    wis::VideoDecodeCommandList video_cl;
    wis::VideoDecoder decoder;
};

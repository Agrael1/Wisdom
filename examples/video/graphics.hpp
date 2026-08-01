#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <cstdio>
#include <optional>
#include <sdl_backend_cpp.h>
#include <span>
#include <vector>
#include <cstdint>

struct SliceData {
    std::vector<uint8_t> data;
    uint32_t nal_unit_type;
    uint32_t temporal_id;
};

class Graphics
{
    static void log_callback(wis::Severity severity, const char* message, uint64_t, void*);

public:
    static std::optional<Graphics> Create(
        wis::StdCodecProfile codec_profile,
        wis::DataFormat output_format,
        uint32_t width,
        uint32_t height,
        const wis::VideoDecodeH265Desc* h265_params = nullptr);

    // Decode a single frame with the given slice NAL unit
    int DecodeFrame(const SliceData& slice);

private:
    SDLPlatformCpp platform;
    wis::Device device;
    wis::VideoDecodingExtension video_ext{wis::VideoCodecFlags::H265};
    wis::CommandQueue video_queue;
    wis::Fence fence;
    wis::CommandAllocator video_alloc;
    wis::VideoDecodeCommandList video_cl;
    wis::VideoDecoder decoder;
    wis::VideoDecoderParameters decoder_params;
    wis::ResourceAllocator allocator;
    wis::Texture decode_output;
    wis::Buffer decode_input;
    uint32_t framewidth = 0;
    uint32_t frameheight = 0;
    wis::StdCodecProfile codec_profile{};
    wis::DataFormat out_format{};
};

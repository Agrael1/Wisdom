#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <sdl_backend_cpp.h>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <span>
#include <vector>

namespace h265nal {
struct H265BitstreamParserState;
}

struct SliceData {
    std::vector<uint8_t> data;
    uint32_t nal_unit_type = 0;
    uint32_t temporal_id = 0;
};

struct CallbackData {
    std::atomic<bool> device_created{false};
};

class Graphics
{
    static void log_callback(wis::Severity severity, const char* message, uint64_t, void*);
    static constexpr uint32_t kSwapchainImages = 3;

public:
    static std::optional<Graphics> Create(
        wis::StdCodecProfile codec_profile,
        wis::DataFormat output_format,
        uint32_t width,
        uint32_t height,
        uint64_t max_slice_size,
        h265nal::H265BitstreamParserState* parser_state,
        SDL_Window* window,
        const wis::VideoDecodeH265Desc* h265_params = nullptr
    );

    // Decode a single frame with the given slice NAL unit
    int DecodeFrame(const SliceData& slice);
    void InitRenderingResources(SDL_Window* window);
    void RenderFrame();

private:
    std::unique_ptr<CallbackData> callback_data;

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
    wis::ViewHeap view_heap;
    uint64_t decode_output_view = 0;
    wis::Texture decode_output;
    wis::Buffer decode_input;
    uint32_t framewidth = 0;
    uint32_t frameheight = 0;
    wis::StdCodecProfile codec_profile{};
    wis::DataFormat out_format{};
    uint64_t bitstream_alignment = 256;
    h265nal::H265BitstreamParserState* parser_state = nullptr;

    // Rendering resources
    wis::CommandQueue render_queue;
    wis::CommandAllocator render_alloc;
    wis::CommandList render_cl;
    wis::DataFormat render_format{};
    wis::Swapchain swapchain;
    uint32_t swapchain_width = 0;
    uint32_t swapchain_height = 0;
    wis::DataFormat luma_sample_format{};
    wis::DataFormat chroma_sample_format{};

    wis::DescriptorHeap desc_heap;
    wis::DescriptorHeap sampler_heap;
    wis::RootSignature root_signature;
    wis::Pipeline pipeline_state;
    wis::Fence render_fence;
    uint64_t fence_value = 0;
    uint64_t decode_fence_value = 0;
    wis::Buffer plane_copy_buffer;
    wis::Texture luma_texture;
    wis::Texture chroma_texture;

    wis::Texture swapchain_textures[kSwapchainImages];
    uint64_t swapchain_views[kSwapchainImages]{};
};

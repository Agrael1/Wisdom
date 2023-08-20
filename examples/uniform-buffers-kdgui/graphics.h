#pragma once
#include <wisdom/wisdom.h>
#include <iostream>
#include <glm/glm.hpp>
#include <filesystem>
#include <fstream>

template<class ShaderTy>
inline auto LoadShader(std::filesystem::path p)
{
    if constexpr (ShaderTy::language == wis::ShaderLang::dxil)
        p += ".cso";
    else if constexpr (ShaderTy::language == wis::ShaderLang::spirv)
        p += ".spv";

    std::ifstream t{ p, std::ios::binary };
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    wis::shared_blob ret{ size };
    t.seekg(0);
    t.read(ret.data<char>(), size);
    return ret;
}

class Graphics
{
    static constexpr wis::ApplicationInfo app_info{
        .application_name = "example",
        .engine_name = "none",
    };

public:
    Graphics(uint32_t width, uint32_t height, wis::SurfaceParameters surface)
        : factory(app_info)
    {
        for (auto&& a : factory.EnumerateAdapters(wis::AdapterPreference::Performance)) {
            auto desc = a.GetDesc();

            if (desc.IsSoftware())
                wis::lib_warn("Loading WARP adapter");

            std::cout << desc.to_string();

            if (device.Initialize(factory, a)) {
                allocator = wis::ResourceAllocator{ device };
                break;
            }
        }

        queue = device.CreateCommandQueue();

        swap = device.CreateSwapchain(queue, wis::SwapchainOptions{ uint32_t(width), uint32_t(height), wis::SwapchainOptions::default_frames, wis::SwapchainOptions::default_format, true }, surface);

        fence = device.CreateFence();
        context = device.CreateCommandList(wis::QueueType::direct);
        constants_heap = device.CreateDescriptorHeap(2, wis::PoolType::CBV_SRV_UAV);
    }

public:
    void OnResize(uint32_t width, uint32_t height)
    {
        if (!swap.Resize(width, height))
            return;

        constexpr static std::array cas2{
            wis::ColorAttachment{
                .format = wis::SwapchainOptions::default_format,
                .load = wis::PassLoadOperation::clear },
            wis::ColorAttachment{
                .format = wis::SwapchainOptions::default_format,
                .load = wis::PassLoadOperation::clear }
        };
        wis::DepthStencilAttachment dsa{
            .format = wis::DataFormat::d32_float,
            .depth_load = wis::PassLoadOperation::clear,
        };

        // needs to be recreated for vulkan for now
        render_pass = device.CreateRenderPass({ width, height }, { cas2.data(), static_cast<unsigned int>(swap.StereoSupported()) + 1u }, dsa);

        // needs to be recreated for vulkan for now
        static constexpr std::array ia{
            wis::InputLayoutDesc{ 0, "POSITION", 0, wis::DataFormat::r32g32b32_float, 0, 0, wis::InputClassification::vertex, 0 },
            wis::InputLayoutDesc{ 1, "NORMAL", 0, wis::DataFormat::r32g32b32_float, 0, 12, wis::InputClassification::vertex, 0 },
            wis::InputLayoutDesc{ 2, "COLOR", 0, wis::DataFormat::r32g32b32a32_float, 0, 24, wis::InputClassification::vertex, 0 }
        };

        auto x = swap.GetRenderTargets();
        for (size_t i = 0; i < x.size(); i++) {
            rtvs[i] = device.CreateRenderTargetView(x[i]);
            if (swap.StereoSupported())
                rtvs2[i] = device.CreateRenderTargetView(x[i], { .base_layer = 1 });
        }

        for (size_t i = 0; i < 2; i++) {
            depth_buffers[i] = allocator.CreateDepthStencilTexture({ width, height, wis::DataFormat::d32_float });
            dsv[i] = device.CreateDepthStencilView(depth_buffers[i]);
        }
    }

    void WaitForGPU()
    {
        const uint64_t vfence = fence_value;
        queue.Signal(fence, vfence);
        fence_value++;
        fence.Wait(vfence);
    }

public:
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

public:
    wis::Factory factory;

    wis::Device device;
    wis::CommandQueue queue;
    wis::SwapChain swap;

    wis::CommandList context;
    wis::ResourceAllocator allocator;
    wis::DescriptorHeap constants_heap;
    wis::RenderPass render_pass;

    wis::RenderTargetView rtvs[2];
    wis::RenderTargetView rtvs2[2];
    wis::Texture depth_buffers[2];
    wis::DepthStencilView dsv[2];

    wis::Fence fence;
    uint64_t fence_value = 1;
};

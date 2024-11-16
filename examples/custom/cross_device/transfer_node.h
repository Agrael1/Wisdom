#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_extended_allocation.h>
#include <wisdom/wisdom_descriptor_buffer.h>
#include <span>
#include <array>
#include <string_view>
#include <expected>

struct TransferNode {
    static constexpr uint32_t kFrameCount = 2;

public:
    void InitSwapchain(wis::SwapChain&& swap);
    void Resize(uint32_t width, uint32_t height);
    void VKCreateInputBuffer(wis::Size2D frame);
    void WaitForGPU() noexcept;

    void Frame();

public:
    uint32_t width;
    uint32_t height;

    wis::Device transfer_device;
    wis::ResourceAllocator allocator;
    wis::SwapChain swap;
    std::span<const wis::Texture> back_buffers;
    wis::CommandQueue queue;
    wis::CommandList cmd_list;

    wis::Buffer input_buffer; // GPU to GPU copy, using GPUUpload heap

    wis::Fence fence;
    uint64_t fence_value = 1;
};

std::expected<TransferNode, std::string_view>
CreateTransferNode(wis::Adapter&& adapter);

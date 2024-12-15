#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_extended_allocation.hpp>
#include <wisdom/wisdom_descriptor_buffer.hpp>
#include <expected>

struct WorkNode {
    WorkNode() = default;
    WorkNode(const WorkNode&) = default;
    WorkNode(WorkNode&&) noexcept = default;
    WorkNode& operator=(const WorkNode&) = default;
    WorkNode& operator=(WorkNode&&) noexcept = default;
    ~WorkNode() noexcept
    {
        if (mapping) {
            out_buffer.Unmap();
        }
    }

public:
    void PrepareResources();
    void CreateOutputTexture(wis::Size2D frame);
    void WaitForGPU() noexcept;
    void Frame();

    void DumpFrame(const char* name);

public:
    uint32_t width = 0;
    uint32_t height = 0;

    wis::Device work_device;
    wis::ExtendedAllocation extended_alloc;
    wis::DescriptorBufferExtension desc_buffer_ext;
    wis::ResourceAllocator allocator;

    wis::Texture lut;
    wis::Texture texture;

    wis::Buffer out_buffer;
    wis::Texture out_texture;

    wis::ShaderResource srv_lut;
    wis::ShaderResource srv;

    wis::Sampler sampler;

    wis::Shader vertex_shader;
    wis::Shader pixel_shader;

    wis::RootSignature root;
    wis::PipelineState pipeline;

    wis::RenderTarget render_target;
    wis::CommandQueue queue;
    wis::CommandList cmd_list;

    wis::DescriptorBuffer desc_buffer;
    wis::DescriptorBuffer sampler_buffer;

    wis::Fence fence;
    uint64_t fence_value = 1;

    void* mapping = nullptr;
};

std::expected<WorkNode, std::string_view>
CreateWorkNode(wis::Adapter&& adapter);

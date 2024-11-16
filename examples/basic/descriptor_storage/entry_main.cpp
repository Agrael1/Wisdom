#include <wis_helper.h>
#include <wis_swapchain.h>
#include <window.h>
#include <iostream>

#include <glm/vec3.hpp>

class App
{
    // Standard setup
    ex::Window window;
    ex::ExampleSetup setup;
    ex::Swapchain swap;
    ex::FramedCommandList cmd_list;

    // Resources
    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::Shader vs; // vertex shader
    wis::Shader ps; // pixel shader

    wis::Buffer vertex_buffer; // vertex buffer for triangle
    wis::Buffer constant_buffers[ex::flight_frames]; // constant buffer for triangle

    float* constant_data[ex::flight_frames]; // constant buffer data
    float offset = 0.0f; // x offset for the triangle

    // Descriptor buffers
    wis::DescriptorStorage desc_storage;

public:
    App()
        : window("Descriptor Storage", 800, 600)
    {
        setup.InitDefault(window.GetPlatformExtension());
        auto [w, h] = window.PixelSize();
        auto swapx = window.CreateSwapchain(setup);
        std::construct_at(&swap, setup.device, std::move(swapx), w, h);
        cmd_list = setup.CreateLists();

        // Only a single descriptor table with 1 descriptor
        wis::DescriptorStorageDesc desc{
            .cbuffer_count = ex::flight_frames, // one cbuffer per frame
            .memory = wis::DescriptorMemory::ShaderVisible,
        };
        desc_storage = ex::Unwrap(setup.device.CreateDescriptorStorage(desc));
    }

public:
    void Run()
    {
        CreateResources();
        while (true) {
            if (!ProcessEvents())
                break;

            Frame();
        }
    }
    // Process window events
    bool ProcessEvents()
    {
        // Window events
        while (true) {
            auto event = window.PollEvents();
            switch (event) {
            case ex::WindowEvent::Resize: {
                auto [w, h] = window.PixelSize();
                swap.Resize(setup.device, w, h);
                break;
            }
            case ex::WindowEvent::Quit:
                swap.Throttle(); // wait for GPU to finish, then exit
                for (size_t i = 0; i < ex::flight_frames; i++) {
                    constant_buffers[i].Unmap();
                }
                return false;
            case ex::WindowEvent::NoEvent:
                return true;
            default:
                break;
            }
        }
        // Mouse events
        // Keyboard events
    }

    // Render a frame
    void Frame()
    {
        uint32_t frame_index = swap.CurrentFrame();

        offset += 0.01f;
        if (offset > 1.0f)
            offset = -1.0f;

        // ------------------------------
        // Second pass
        auto& cmd2 = cmd_list[frame_index];
        wis::RenderPassRenderTargetDesc targets2[] {
            {   .target = swap.GetRenderTarget(frame_index),
                              .load_op = wis::LoadOperation::Clear,
                              .store_op = wis::StoreOperation::Store,
                .clear_value = { 0.5f, 0.5f, 0.5f, 1.0f }
            }
        };
        wis::RenderPassDesc rp2{
            .flags = wis::RenderPassFlags::None,
            .target_count = 1,
            .targets = targets2,
        };

        constant_data[frame_index][0] = offset;

        // Begin recording
        ex::CheckResult(cmd2.Reset(pipeline));

        // Insert barriers for the swapchain render target
        cmd2.TextureBarrier(
        {   .sync_before = wis::BarrierSync::None,
            .sync_after = wis::BarrierSync::Draw,
            .access_before = wis::ResourceAccess::NoAccess,
            .access_after = wis::ResourceAccess::RenderTarget,
            .state_before = wis::TextureState::Present,
            .state_after = wis::TextureState::RenderTarget },
        swap.GetTexture(frame_index));

        cmd2.BeginRenderPass(&rp2);
        cmd2.SetRootSignature(root); // always set root signature before binding resources

        // Bind descriptor storage
        cmd2.SetDescriptorStorage(desc_storage);

        cmd2.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
        wis::VertexBufferBinding vertex_binding{
            .buffer = vertex_buffer,
            .size = sizeof(glm::vec3) * 3,
            .stride = sizeof(glm::vec3),
        };
        cmd2.IASetVertexBuffers(&vertex_binding, 1);

        auto [w, h] = window.PixelSize();
        cmd2.RSSetViewport({ 0, 0, float(w), float(h), 0, 1 });
        cmd2.RSSetScissor({ 0, 0, w, h });
        cmd2.DrawInstanced(3);
        cmd2.EndRenderPass();

        // Insert barriers for the swapchain render target
        cmd2.TextureBarrier(
        {   .sync_before = wis::BarrierSync::Draw,
            .sync_after = wis::BarrierSync::Draw,
            .access_before = wis::ResourceAccess::RenderTarget,
            .access_after = wis::ResourceAccess::Common,
            .state_before = wis::TextureState::RenderTarget,
            .state_after = wis::TextureState::Present },
        swap.GetTexture(frame_index));

        // End recording
        cmd2.Close();

        wis::CommandListView lists[] = { cmd2 };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        swap.Present(setup.queue);
    }
    void CreateResources()
    {
        // Load shaders
        auto vs_code = ex::LoadShader("shaders/desc_storage.vs");
        auto ps_code = ex::LoadShader("shaders/desc_storage.ps");
        vs = ex::Unwrap(setup.device.CreateShader(vs_code.data(), vs_code.size()));
        ps = ex::Unwrap(setup.device.CreateShader(ps_code.data(), ps_code.size()));

        // Create root for storage (it is bindless, so no reason to use tables anymore)
        wis::RootConstant root_constants[] {
            { .stage = wis::ShaderStages::Vertex, .size_bytes = sizeof(uint32_t) }
        };
        root = ex::Unwrap(setup.device.CreateRootSignature(root_constants, std::size(root_constants)));

        // Create pipeline
        {
            wis::InputSlotDesc input_slots[] = {
                { .slot = 0, .stride_bytes = sizeof(glm::vec3), .input_class = wis::InputClass::PerVertex },
            };
            wis::InputAttribute input_attributes[] = {
                { .input_slot = 0, .semantic_name = "POSITION", .semantic_index = 0, .location = 0, .format = wis::DataFormat::RGB32Float, .offset_bytes = 0 }
            };
            wis::DataFormat attachment_formats[] = { ex::swapchain_format };
            wis::GraphicsPipelineDesc desc{
                .root_signature = root,
                .input_layout = {
                    .slots = input_slots,
                    .slot_count = 1,
                    .attributes = input_attributes,
                    .attribute_count = 1,
                },
                .shaders = { .vertex = vs, .pixel = ps },
                .attachments = {
                    .attachment_formats = attachment_formats,
                    .attachments_count = 1,
                },
            };
            pipeline = ex::Unwrap(setup.device.CreateGraphicsPipeline(&desc));
        }

        // Create vertex buffer
        {
            glm::vec3 triangle_vertices[] = {
                { 0.0f, 0.5f, 0.0f },
                { 0.5f, -0.5f, 0.0f },
                { -0.5f, -0.5f, 0.0f }
            };
            vertex_buffer = setup.CreateAndUploadBuffer(std::span<glm::vec3> { triangle_vertices }, wis::BufferUsage::VertexBuffer);
        }

        // Create constant buffer
        {
            using namespace wis;
            constant_buffers[0] = ex::Unwrap(setup.allocator.CreateBuffer(sizeof(float), wis::BufferUsage::CopySrc | wis::BufferUsage::ConstantBuffer, wis::MemoryType::Upload, wis::MemoryFlags::Mapped));
            constant_buffers[1] = ex::Unwrap(setup.allocator.CreateBuffer(sizeof(float), wis::BufferUsage::CopySrc | wis::BufferUsage::ConstantBuffer, wis::MemoryType::Upload, wis::MemoryFlags::Mapped));
            desc_storage.WriteConstantBuffer(0, constant_buffers[0], sizeof(float));
            desc_storage.WriteConstantBuffer(1, constant_buffers[1], sizeof(float));

            for (size_t i = 0; i < ex::flight_frames; i++) {
                constant_data[i] = static_cast<float*>(constant_buffers[i].Map());
                constant_data[i][0] = 0.0f;
            }
        }
    }
};

int main(int argc, char** argv)
{
    try {
        App{}.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

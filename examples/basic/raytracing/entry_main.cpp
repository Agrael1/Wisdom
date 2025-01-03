#include <wis_helper.h>
#include <wis_swapchain.h>
#include <wisdom/wisdom_raytracing.hpp>
#include <window.h>
#include <iostream>

#include <glm/vec3.hpp>

// In order to render with multiview, we need to have texture with array layers.
// In case of stereo rendering, we need to have 2 array layers.
// Stereo example is not implemented here, but it is possible to implement it by
// creating a swapchain with stereo=true. Stereo rendering is not supported by all graphics cards.
// You can check if stereo is supported by calling swap.StereoSupported().

// In this example, we will render to the texture with 2 array layers, imitating stereo rendering.
// One layer will be rendered with red color, and the other with blue color.

class App
{
    // Standard setup
    ex::Window window;
    ex::ExampleSetup setup;
    ex::Swapchain swap;
    ex::FramedCommandList cmd_list;

    // Resources
    wis::Raytracing raytracing_extension;

    wis::Texture uav_texture;
    wis::Shader raygen_shader;

    wis::Buffer vertex_buffer;
    wis::Buffer index_buffer;

    wis::Buffer rtas_buffer;
    wis::Buffer rtas_scratch_buffer;
    wis::Buffer rtas_update_buffer;
    wis::Buffer rtas_instance_buffer;
    wis::AccelerationStructure top_rtas;
    wis::AccelerationStructure bottom_rtas;

public:
    App()
        : window("Raytracing", 800, 600)
    {
        wis::Result result = wis::success;
        wis::DeviceExtension* device_exts[] = { &raytracing_extension };
        setup.InitDefault(window.GetPlatformExtension(), device_exts);
        auto [w, h] = window.PixelSize();
        std::construct_at(&swap, setup.device, window.CreateSwapchain(result, setup, ex::swapchain_format), w, h);
        cmd_list = setup.CreateLists();

        // Create resources
        CreateSizeDependentResources(w, h);

        // Load shaders
        auto buf = ex::LoadShader("shaders/raytracing.lib");
        raygen_shader = setup.device.CreateShader(result, buf.data(), uint32_t(buf.size()));

        // Create ...
        CreatePrimitives();
        CreateAccelerationStructures();
        MakeTransitions();
    }

public:
    void Run()
    {
        while (true) {
            if (!ProcessEvents()) {
                break;
            }

            Frame();
        }
    }
    // Process window events
    bool ProcessEvents()
    {
        // Window events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_EVENT_WINDOW_RESIZED: {
                auto [w, h] = window.PixelSize();
                swap.Resize(setup.device, w, h);
                break;
            }
            case SDL_EVENT_QUIT:
                swap.Throttle(); // wait for GPU to finish, then exit
                return false;
            case SDL_EVENT_KEY_DOWN:
                OnKeyPressed(e);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                OnMouseButtonDown(e);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                OnMouseMove(e);
                break;
            default:
                break;
            }
        }
        return true;
    }
    void OnKeyPressed(const SDL_Event& event)
    {
        // Keyboard events
    }
    void OnMouseButtonDown(const SDL_Event& event)
    {
        // Mouse events
    }
    void OnMouseMove(const SDL_Event& event)
    {
        // Mouse events
    }

    // Render a frame
    void Frame()
    {
        uint32_t frame_index = swap.CurrentFrame();
        auto& cmd = cmd_list[frame_index];

        swap.Present(setup.queue);
    }

private:
    void CreateSizeDependentResources(uint32_t width, uint32_t height)
    {
        using namespace wis; // for flag operators
        wis::Result result = wis::success;

        // Create UAV texture
        wis::TextureDesc desc{
            .format = ex::swapchain_format,
            .size = { width, height, 1 },
            .usage = wis::TextureUsage::CopySrc | wis::TextureUsage::UnorderedAccess,
        };
        uav_texture = setup.allocator.CreateTexture(result, desc);
    }
    void CreatePrimitives()
    {
        // clang-format off
        constexpr static float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };
        // clang-format on
        constexpr static uint16_t indices[] = { 0, 1, 2 };

        wis::Result result = wis::success;
        vertex_buffer = setup.allocator.CreateBuffer(result, sizeof(vertices), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);
        index_buffer = setup.allocator.CreateBuffer(result, sizeof(indices), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);

        auto memory = vertex_buffer.Map<float>();
        std::copy_n(vertices, std::size(vertices), memory);
        vertex_buffer.Unmap();

        auto memory2 = index_buffer.Map<uint16_t>();
        std::copy_n(indices, std::size(indices), memory);
        index_buffer.Unmap();
    }

    void MakeTransitions()
    {
        auto& cmd = cmd_list[swap.CurrentFrame()];
        // Transition UAV texture to UAV state
        cmd.TextureBarrier({ .sync_before = wis::BarrierSync::None,
                             .sync_after = wis::BarrierSync::None,
                             .access_before = wis::ResourceAccess::NoAccess,
                             .access_after = wis::ResourceAccess::NoAccess,
                             .state_before = wis::TextureState::Undefined,
                             .state_after = wis::TextureState::Common },
                           uav_texture);
        cmd.Close();

        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        setup.WaitForGPU();
    }
    void CreateAccelerationStructures()
    {
        wis::Result result = wis::success;

        rtas_instance_buffer = setup.allocator.CreateBuffer(result, sizeof(wis::AccelerationInstance), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);

        // get tlas size
        wis::TopLevelASBuildDesc build_desc{
            .flags = wis::AccelerationStructureFlags::AllowUpdate,
            .instance_count = 1,
            .gpu_address = rtas_instance_buffer.GetGPUAddress(),
            .indirect = false,
            .update = false,
        };
        auto as_size = raytracing_extension.GetTopLevelASSize(build_desc);

        // get blas size
        wis::AcceleratedGeometryInput geometry_input{
            .geometry_type = wis::ASGeometryType::Triangles,
            .flags = wis::ASGeometryFlags::Opaque,
            .vertex_or_aabb_buffer_address = vertex_buffer.GetGPUAddress(),
            .vertex_or_aabb_buffer_stride = sizeof(float[3]),
            .index_buffer_address = index_buffer.GetGPUAddress(),
            .transform_matrix_address = 0,
            .vertex_count = 3,
            .triangle_or_aabb_count = 1,
            .vertex_format = wis::DataFormat::RGB32Float,
            .index_format = wis::IndexType::UInt16,
        };
        wis::AcceleratedGeometryDesc geometry_desc = wis::CreateGeometryDesc(geometry_input);
        wis::BottomLevelASBuildDesc blas_desc{
            .flags = wis::AccelerationStructureFlags::None,
            .geometry_count = 1,
            .geometry_array = &geometry_desc,
            .update = false,
        };
        auto blas_size = raytracing_extension.GetBottomLevelASSize(blas_desc);

        rtas_buffer = setup.allocator.CreateBuffer(result, as_size.result_size + blas_size.result_size, wis::BufferUsage::AccelerationStructureBuffer);
        rtas_scratch_buffer = setup.allocator.CreateBuffer(result, as_size.scratch_size + blas_size.scratch_size, wis::BufferUsage::StorageBuffer);
        rtas_update_buffer = setup.allocator.CreateBuffer(result, as_size.update_size, wis::BufferUsage::StorageBuffer);

        top_rtas = raytracing_extension.CreateAccelerationStructure(result, rtas_buffer, 0, as_size.result_size, wis::ASLevel::Top);
        bottom_rtas = raytracing_extension.CreateAccelerationStructure(result, rtas_buffer, as_size.result_size, blas_size.result_size, wis::ASLevel::Bottom);

        // Fill instance buffer
        rtas_instance_buffer.Map<wis::AccelerationInstance>()[0] = {
            .transform = {
                    { 1.0f, 0.0f, 0.0f, 0.0f },
                    { 0.0f, 1.0f, 0.0f, 0.0f },
                    { 0.0f, 0.0f, 1.0f, 0.0f },
            },
            .instance_id = 0,
            .mask = 0xFF,
            .instance_offset = 0,
            .flags = +wis::ASInstanceFlags::TriangleCullDisable,
            .acceleration_structure_handle = raytracing_extension.GetAccelerationStructureDeviceAddress(bottom_rtas),
        };
        rtas_instance_buffer.Unmap();

        // Build acceleration structures
        auto& cmd = cmd_list[0];
        raytracing_extension.BuildTopLevelAS(cmd, build_desc, top_rtas, rtas_scratch_buffer.GetGPUAddress());
        raytracing_extension.BuildBottomLevelAS(cmd, blas_desc, bottom_rtas, rtas_scratch_buffer.GetGPUAddress() + as_size.scratch_size);
        cmd.Close();

        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        setup.WaitForGPU();
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

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

    wis::Buffer rtas_buffer;
    wis::Buffer rtas_instance_buffer;
    wis::AccelerationStructure top_rtas;

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

        rtas_instance_buffer = setup.allocator.CreateBuffer(result, sizeof(wis::AccelerationInstance), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload);
        
        // Create top level acceleration structure
        wis::TopLevelASBuildDesc build_desc{
            .flags = wis::AccelerationStructureFlags::AllowUpdate,
            .instance_count = 1,
            .gpu_address = rtas_instance_buffer.GetGPUAddress(),
            .indirect = false,
            .update = false,
        };
        auto as_size = raytracing_extension.GetTopLevelASSize(build_desc);

        rtas_buffer = setup.allocator.CreateBuffer(result, as_size.result_size, wis::BufferUsage::AccelerationStructureBuffer);

        top_rtas = raytracing_extension.CreateAccelerationStructure(result, rtas_buffer, 0, as_size.result_size, wis::ASLevel::Top);
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

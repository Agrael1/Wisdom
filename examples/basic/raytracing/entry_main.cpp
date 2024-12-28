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

public:
    App()
        : window("Raytracing", 800, 600)
    {
        wis::Result result = wis::success;
        wis::DeviceExtension* device_exts[] = { &raytracing_extension };
        setup.InitDefault(window.GetPlatformExtension(), device_exts);
        auto [w, h] = window.PixelSize();
        std::construct_at(&swap, setup.device, window.CreateSwapchain(result, setup), w, h);
        cmd_list = setup.CreateLists();
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

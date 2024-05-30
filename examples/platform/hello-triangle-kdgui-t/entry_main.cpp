// #include "app.h"
#include <app.h>
#include <iostream>
#include "window.h"

#include <wisdom/wisdom_platform.h>

int main()
{
    XApp app;
    Window window = app.createWindow(1920, 1080);

    std::array<wis::FactoryExtension*, 1> required_extensions = {
        window.GetFactoryExtension()
    };
    Test::App gfx_app(required_extensions);

    auto [res, chain] = window.CreateSwapchain(gfx_app.GetDevice(), gfx_app.GetQueue());
    if (res.status != wis::Status::Ok) {
        std::cerr << "Failed to create swapchain" << std::endl;
        return 1;
    }

    gfx_app.SetSwapChain(std::move(chain), window.width(), window.height());
    gfx_app.CreateResources();

    while (true) {
        app.ProcessEvents();
        if (!window.visible()) {
            break;
        }

        if (window.resized()) {
            gfx_app.OnResize(window.width(), window.height());
        }
        gfx_app.Frame();
    }
    return 0;
}

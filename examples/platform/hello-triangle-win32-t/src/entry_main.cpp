#include <example/window.h>
#include <app.h>
#include <wisdom/platform/win32.h>


int main()
{
    Window wnd(1920, 1080, "Example Win32 Wisdom");
    Test::App app;

    wis::SwapchainDesc desc{
        .size = { uint32_t(wnd.GetWidth()), uint32_t(wnd.GetHeight()) },
        .format = wis::DataFormat::BGRA8Unorm,
        .buffer_count = 2,
        .stereo = true,
        .vsync = true,
    };

    auto [res3, hswap] = wis::CreateSwapchainWin32(app.GetDevice(), app.GetQueue(), &desc,
                         wnd.GetHandle());
    app.SetSwapChain(std::move(hswap), desc.size.width, desc.size.height);
    app.CreateResources();

    while (true) {
        if (const auto a = wnd.ProcessMessages())
            return (int)a.value();

        for (auto e : wnd.GetEvents())
            switch (e) {
            case Event::Resize:
                app.OnResize(wnd.GetWidth(), wnd.GetHeight());
            }

        app.Frame();
    }
}

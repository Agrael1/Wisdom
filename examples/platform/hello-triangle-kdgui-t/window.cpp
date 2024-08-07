#include <KDFoundation/config.h> // for KD_PLATFORM
#include <KDGui/gui_application.h>
#include <KDGui/window.h>
#include <KDGui/gui_application.h>
#include <wisdom/wisdom_platform.h>

// #if defined(KD_PLATFORM_WIN32) && WISDOM_LINUX
// #error WSL is not supported with KDGui
// #endif

#if defined(KD_PLATFORM_WIN32)
#include <KDGui/platform/win32/win32_platform_window.h>
#endif
#if defined(KD_PLATFORM_LINUX)
#include <KDGui/platform/linux/xcb/linux_xcb_platform_window.h>
#include <KDGui/platform/linux/wayland/linux_wayland_platform_window.h>
#include <KDGui/platform/linux/wayland/linux_wayland_platform_integration.h>
#endif
#include "window.h"

enum class Platform {
    Win32,
    Xcb,
    Wayland,
};

class WindowP : public KDGui::Window
{
public:
    virtual void resizeEvent(KDFoundation::ResizeEvent* ev) override
    {
        resized = true;
        KDGui::Window::resizeEvent(ev);
    }
    bool resized = false;
    std::unique_ptr<wis::FactoryExtension> ext; // Platform specific extension
    Platform platform = Platform::Win32;
};

Window::Window(uint32_t width, uint32_t height, WindowP* p)
    : p(p)
{
    p->width = width;
    p->height = height;
    p->visible = true;

#ifdef KD_PLATFORM_WIN32
    p->ext = std::make_unique<wis::platform::WindowsExtension>();
#elif defined(KD_PLATFORM_LINUX)
    if (KDGui::LinuxWaylandPlatformIntegration::checkAvailable()) {
        p->ext = std::make_unique<wis::platform::WaylandExtension>();
        p->platform = Platform::Wayland;
    } else {
        p->ext = std::make_unique<wis::platform::XCBExtension>();
        p->platform = Platform::Xcb;
    }
#endif // KD_PLATFORM_WIN32
}
Window::~Window()
{
    p->destroy();
}

uint32_t Window::width() const noexcept
{
    return p->width.get();
}
uint32_t Window::height() const noexcept
{
    return p->height.get();
}

wis::FactoryExtension*
Window::GetFactoryExtension() noexcept
{
    return p->ext.get();
}

wis::ResultValue<wis::SwapChain>
Window::CreateSwapchain(const wis::Device& device, const wis::CommandQueue& queue) const noexcept
{
    wis::SwapchainDesc desc{
        .size = { width(), height() },
        .format = wis::DataFormat::BGRA8Unorm,
        .buffer_count = 2,
        .stereo = false,
        .vsync = true,
    };

#if defined(KD_PLATFORM_WIN32)
    auto win32Window = dynamic_cast<KDGui::Win32PlatformWindow*>(p->platformWindow());
    return static_cast<wis::platform::WindowsExtension*>(p->ext.get())->CreateSwapchain(device, queue, &desc, win32Window->handle());
#elif defined(KD_PLATFORM_LINUX)
    if (p->platform == Platform::Wayland) {
        auto* platformIntegration = KDGui::GuiApplication::instance()->guiPlatformIntegration();
        auto* waylandPlatformIntegration = dynamic_cast<KDGui::LinuxWaylandPlatformIntegration*>(platformIntegration);
        auto* waylandWindow = dynamic_cast<KDGui::LinuxWaylandPlatformWindow*>(p->platformWindow());

        return static_cast<wis::platform::WaylandExtension*>(p->ext.get())
                ->CreateSwapchain(device, queue, &desc, waylandPlatformIntegration->display(), waylandWindow->surface());
    } else {
        auto* xcbWindow = dynamic_cast<KDGui::LinuxXcbPlatformWindow*>(p->platformWindow());
        return static_cast<wis::platform::XCBExtension*>(p->ext.get())
                ->CreateSwapchain(device, queue, &desc, xcbWindow->connection(), xcbWindow->handle());
    }
#endif
    return wis::Result{
        wis::Status::InvalidArgument,
        "No platform was selected"
    };
}
bool Window::resized() const noexcept
{
    return std::exchange(p->resized, false);
}
bool Window::visible() const noexcept
{
    return p->visible.get();
}

class XAppP
{
public:
    KDGui::GuiApplication app;
};

XApp::XApp()
    : p(new XAppP())
{
}
XApp::~XApp()
{
}

void XApp::ProcessEvents()
{
    p->app.processEvents();
}
Window XApp::createWindow(uint32_t width, uint32_t height)
{
    return Window(width, height, p->app.createChild<WindowP>());
}

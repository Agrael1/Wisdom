module;
#include <memory>
#include <KDFoundation/config.h> // for KD_PLATFORM
#include <KDGui/gui_application.h>

#if defined(KD_PLATFORM_WIN32)
#include <KDGui/platform/win32/win32_platform_window.h>
#endif
#if defined(KD_PLATFORM_LINUX)
#include <KDGui/platform/linux/xcb/linux_xcb_platform_window.h>
#include <KDGui/platform/linux/wayland/linux_wayland_platform_window.h>
#include <KDGui/platform/linux/wayland/linux_wayland_platform_integration.h>
#endif
#if defined(KD_PLATFORM_MACOS)
extern CAMetalLayer* createMetalLayer(KDGui::Window* window);
#endif

#include <KDGui/window.h>
#include <KDGui/gui_application.h>
export module window;

import wisdom.api;

class WindowP;
export class Window
{
	friend class XApp;
private:
	Window(uint32_t width, uint32_t height, WindowP* p);
public:
	~Window();
public:
	bool visible()const noexcept;
	uint32_t width()const noexcept;
	uint32_t height()const noexcept;
	wis::SurfaceParameters GetSurfaceOptions()const noexcept;
private:
	WindowP* p;
};

class XAppP;
export class XApp
{
public:
	XApp();
	~XApp();
public:
	void ProcessEvents();
	Window createWindow(uint32_t width, uint32_t height);
private:
	std::unique_ptr<XAppP> p;
};



class WindowP : public KDGui::Window
{

};


Window::Window(uint32_t width, uint32_t height, WindowP* p)
	: p(p)
{
	p->width = 1920;
	p->height = 1080;
	p->visible = true;
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

wis::SurfaceParameters Window::GetSurfaceOptions()const noexcept
{
#if defined(KD_PLATFORM_WIN32)
	auto win32Window = dynamic_cast<KDGui::Win32PlatformWindow*>(p->platformWindow());
	return wis::SurfaceParameters{
		win32Window->handle()
	};
#elif defined(KD_PLATFORM_LINUX)
    if (KDGui::LinuxWaylandPlatformIntegration::checkAvailable()) {
        auto *platformIntegration = KDGui::GuiApplication::instance()->guiPlatformIntegration();
        auto *waylandPlatformIntegration = dynamic_cast<KDGui::LinuxWaylandPlatformIntegration *>(platformIntegration);
        auto waylandWindow = dynamic_cast<KDGui::LinuxWaylandPlatformWindow *>(p->platformWindow());
        return wis::SurfaceParameters{
            waylandPlatformIntegration->display(),
            waylandWindow->surface(),
        };
    } else {
        auto *xcbWindow = dynamic_cast<KDGui::LinuxXcbPlatformWindow *>(p->platformWindow());
        return wis::SurfaceParameters{
            xcbWindow->connection(),
            xcbWindow->handle(),
        };
    }
#elif defined(KD_PLATFORM_MACOS)
	return wis::SurfaceParameters{
		.layer = createMetalLayer(this)
	};
#endif
	return {};
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
	:p(new XAppP())
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

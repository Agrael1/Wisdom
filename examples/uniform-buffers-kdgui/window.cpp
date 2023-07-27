/*
   This file is part of KDGpu.

   SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

   SPDX-License-Identifier: MIT

   Contact KDAB at <info@kdab.com> for commercial licensing options.
 */

#include "window.h"

#include <KDFoundation/config.h> // for KD_PLATFORM
#include <KDGui/gui_application.h>

#if defined(KD_PLATFORM_WIN32)
#include <KDGui/platform/win32/win32_platform_window.h>
#endif
#if defined(KD_PLATFORM_LINUX)
#include <KDGui/platform/linux/xcb/linux_xcb_platform_window.h>
#endif
#if defined(KD_PLATFORM_MACOS)
extern CAMetalLayer *createMetalLayer(KDGui::Window *window);
#endif

#include <KDGui/window.h>
#include <KDGui/gui_application.h>

class WindowP : public KDGui::Window
{
};

Window::Window(uint32_t width, uint32_t height, WindowP *p)
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

wis::SurfaceParameters Window::GetSurfaceOptions() const noexcept
{
#if defined(KD_PLATFORM_WIN32)
    auto win32Window = dynamic_cast<KDGui::Win32PlatformWindow *>(p->platformWindow());
    return wis::SurfaceParameters{
        win32Window->handle()
    };
#elif defined(KD_PLATFORM_LINUX)
    auto xcbWindow = dynamic_cast<KDGui::LinuxXcbPlatformWindow *>(p->platformWindow());
    return wis::SurfaceParameters{
        xcbWindow->connection(),
        xcbWindow->handle()
    };
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

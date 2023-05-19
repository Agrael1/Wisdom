/*
   This file is part of KDGpu.

   SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

   SPDX-License-Identifier: MIT

   Contact KDAB at <info@kdab.com> for commercial licensing options.
 */

#include "window.h"

#include <KDFoundation/config.h> // for KD_PLATFORM
#include <KDFoundation/core_application.h>

#if defined(KD_PLATFORM_WIN32)
#include <KDGui/platform/win32/win32_platform_window.h>
#endif
#if defined(KD_PLATFORM_LINUX)
#include <KDGui/platform/linux/xcb/linux_xcb_platform_window.h>
#endif
#if defined(KD_PLATFORM_MACOS)
extern CAMetalLayer* createMetalLayer(KDGui::Window* window);
#endif


Window::Window(uint32_t width, uint32_t height)
	: KDGui::Window()
{
	this->width = 1920;
	this->height = 1080;
	visible = true;
}


wis::SurfaceParameters Window::GetSurfaceOptions()const noexcept
{
#if defined(KD_PLATFORM_WIN32)
	auto win32Window = dynamic_cast<KDGui::Win32PlatformWindow*>(platformWindow());
	return wis::SurfaceParameters{
		win32Window->handle()
	};
#elif defined(KD_PLATFORM_LINUX)
	auto xcbWindow = dynamic_cast<KDGui::LinuxXcbPlatformWindow*>(platformWindow());
	return wis::SurfaceParameters{
		xcbWindow->connection(),
		xcbWindow->handle()
	};
#elif defined(KD_PLATFORM_MACOS)
	return KDGpu::SurfaceOptions{
		.layer = createMetalLayer(this)
	};
#endif
	return {};
}

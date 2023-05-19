/*
  This file is part of KDGpu.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once

#include <KDGui/window.h>
#include <wisdom/api/api_swapchain.h>

namespace KDGpu {
	class Instance;
}


class Window : public KDGui::Window
{
public:
	Window(uint32_t width, uint32_t height);
public:
	wis::SurfaceParameters GetSurfaceOptions()const noexcept;
	//KDGpu::Surface createSurface(KDGpu::Instance& instance);
};
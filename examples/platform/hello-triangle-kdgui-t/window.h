/*
  This file is part of KDGpu.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once
#include <wisdom/wisdom.hpp>
#include <memory>

class WindowP;
class Window
{
    friend class XApp;

private:
    Window(uint32_t width, uint32_t height, WindowP* p);

public:
    ~Window();

    [[nodiscard]] bool visible() const noexcept;
    [[nodiscard]] uint32_t width() const noexcept;
    [[nodiscard]] uint32_t height() const noexcept;

    [[nodiscard]] wis::ResultValue<wis::SwapChain>
    CreateSwapchain(const wis::Device& device, const wis::CommandQueue& queue) const noexcept;

    [[nodiscard]] bool resized() const noexcept;

private:
    WindowP* p;
};

class XAppP;
class XApp
{
public:
    XApp();
    ~XApp();

    void ProcessEvents();
    Window createWindow(uint32_t width, uint32_t height);

private:
    std::unique_ptr<XAppP> p;
};

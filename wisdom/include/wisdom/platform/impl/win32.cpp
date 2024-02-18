#pragma once
#ifndef WISDOM_PLATFORM_HEADER_ONLY
#include <wisdom/platform/win32.h>
#endif // !WISDOM_PLATFORM_HEADER_ONLY

#include <wisdom/xdx12/dx12_device.h>
#include <wisdom/util/log_layer.h>
#include <d3d11.h>

namespace wis::detail {
void ToSwapchainDesc(DXGI_SWAP_CHAIN_DESC1& swap_desc, const wis::SwapchainDesc* desc) noexcept
{
    swap_desc.Width = desc->size.width;
    swap_desc.Height = desc->size.height;
    swap_desc.Format = convert_dx(desc->format);
    swap_desc.Stereo = desc->stereo;
    swap_desc.SampleDesc.Count = 1u;
    swap_desc.SampleDesc.Quality = 0u;
    swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_desc.BufferCount = desc->buffer_count;
    swap_desc.Scaling = DXGI_SCALING_STRETCH;
    swap_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swap_desc.Flags = 0;
}
wis::com_ptr<ID3D11Device> CreateD3D11Device() noexcept
{
    constexpr D3D_FEATURE_LEVEL featureLevels[]{
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    wis::com_ptr<ID3D11Device> device11;
    D3D11CreateDevice(nullptr,
                      D3D_DRIVER_TYPE_HARDWARE,
                      nullptr, 0,
                      featureLevels, 3, D3D11_SDK_VERSION, device11.put(), nullptr, nullptr);
    return device11;
}

} // namespace wis::detail

std::pair<wis::Result, wis::DX12SwapChain>
wis::DX12CreateSwapchainWin32(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) noexcept
{
    DXGI_SWAP_CHAIN_DESC1 swap_desc;
    detail::ToSwapchainDesc(swap_desc, desc);
    auto& devicei = device.GetInternal();

    HRESULT hr;

    wis::com_ptr<IDXGISwapChain1> swap;

    // until microsoft fixes this
    if (desc->stereo && !wis::succeeded(hr = devicei.factory->CreateSwapChainForHwnd(detail::CreateD3D11Device().get(), hwnd, &swap_desc, nullptr, nullptr, swap.put()))) {
        return std::pair{
            wis::make_result<FUNC, "Failed to create D3D11 device for stereo mode">(hr), DX12SwapChain{}
        };
    }

    hr = devicei.factory->CreateSwapChainForHwnd(
            std::get<0>(main_queue), // Swap chain needs the queue so that it can force a flush on it.
            hwnd,
            &swap_desc,
            nullptr,
            nullptr,
            swap.put());

    if (!wis::succeeded(hr)) {
        return std::pair{
            wis::make_result<FUNC, "Failed to create swapchain for hwnd">(hr), DX12SwapChain{}
        };
    }
    auto [hrx, swap4] = swap.as<IDXGISwapChain4>();
    return wis::succeeded(hrx)
            ? std::pair{ wis::success, DX12SwapChain{ std::move(swap4) } }
            : std::pair{ wis::make_result<FUNC, "Failed to create swapchain">(hr), DX12SwapChain{} };
}

std::pair<wis::Result, wis::DX12SwapChain>
wis::DX12CreateSwapchainUWP(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, IUnknown* window) noexcept
{
    DXGI_SWAP_CHAIN_DESC1 swap_desc;
    detail::ToSwapchainDesc(swap_desc, desc);
    auto& devicei = device.GetInternal();

    HRESULT hr;

    wis::com_ptr<IDXGISwapChain1> swap;

    // until microsoft fixes this
    if (desc->stereo && !wis::succeeded(hr = devicei.factory->CreateSwapChainForCoreWindow(detail::CreateD3D11Device().get(), window, &swap_desc, nullptr, swap.put()))) {
        return std::pair{
            wis::make_result<FUNC, "Failed to create D3D11 device for stereo mode">(hr), DX12SwapChain{}
        };
    }

    hr = devicei.factory->CreateSwapChainForCoreWindow(
            std::get<0>(main_queue), // Swap chain needs the queue so that it can force a flush on it.
            window,
            &swap_desc,
            nullptr,
            swap.put());

    if (!wis::succeeded(hr)) {
        return std::pair{
            wis::make_result<FUNC, "Failed to create swapchain for core window">(hr), DX12SwapChain{}
        };
    }
    auto [hrx, swap4] = swap.as<IDXGISwapChain4>();
    return wis::succeeded(hrx)
            ? std::pair{ wis::success, DX12SwapChain{ std::move(swap4) } }
            : std::pair{ wis::make_result<FUNC, "Failed to create swapchain">(hr), DX12SwapChain{} };
}

#ifdef WISDOM_VULKAN
#include <wisdom/xvulkan/vk_device.h>

std::pair<wis::Result, wis::VKSwapChain>
wis::VKCreateSwapchainWin32(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) noexcept
{
    VkWin32SurfaceCreateInfoKHR surface_desc{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = GetModuleHandle(nullptr),
        .hwnd = hwnd
    };
    wis::lib_info("Initializing Win32 Surface");

    auto& devicei = device.GetInternal();
    const auto* instance_table = devicei.GetInstanceTable();
    VkSurfaceKHR surface;
    auto result = instance_table->vkCreateWin32SurfaceKHR(device.GetInternal().instance.get(), &surface_desc, nullptr, &surface);
    if (!wis::succeeded(result)) {
        return std::pair{
            wis::make_result<FUNC, "Failed to create Win32 surface">(result), VKSwapChain{}
        };
    }
    wis::shared_handle<VkSurfaceKHR> surface_handle{ surface, device.GetInternal().instance, instance_table->vkDestroySurfaceKHR };
    return device.VKCreateSwapChain(surface_handle, desc);
}
#endif // WISDOM_VULKAN
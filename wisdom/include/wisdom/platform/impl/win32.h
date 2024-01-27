#pragma once
#ifndef WISDOM_PLATFORM_HEADER_ONLY
#include <wisdom/platform/win32.h>
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#include <wisdom/dx12/xdx12_device.h>
#include <wisdom/util/log_layer.h>


std::pair<wis::Result, wis::DX12SwapChain>
wis::DX12CreateSwapchainWin32(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) noexcept
{
    DXGI_SWAP_CHAIN_DESC1 swap_desc;
    DX12SwapchainHelpers::ToSwapchainDesc(swap_desc, desc);
    auto& devicei = device.GetInternal();

    HRESULT hr;

    wis::com_ptr<IDXGISwapChain1> swap;

    // until microsoft fixes this
    if (desc->stereo && !wis::succeeded(hr = devicei.factory->CreateSwapChainForHwnd(DX12SwapchainHelpers::CreateD3D11Device().get(), hwnd, &swap_desc, nullptr, nullptr, swap.put()))) {
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
    DX12SwapchainHelpers::ToSwapchainDesc(swap_desc, desc);
    auto& devicei = device.GetInternal();

    HRESULT hr;

    wis::com_ptr<IDXGISwapChain1> swap;

    // until microsoft fixes this
    if (desc->stereo && !wis::succeeded(hr = devicei.factory->CreateSwapChainForCoreWindow(DX12SwapchainHelpers::CreateD3D11Device().get(), window, &swap_desc, nullptr, swap.put()))) {
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
#include <wisdom/vulkan/xvk_device.h>

std::pair<wis::Result, wis::VKSwapChain>
wis::VKCreateSwapchainWin32(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) noexcept
{
    VkWin32SurfaceCreateInfoKHR surface_desc{
        .pNext = nullptr,
        .flags = 0,
        .hinstance = GetModuleHandle(nullptr),
        .hwnd = hwnd
    };
    lib_info("Initializing Win32 Surface");

    auto& devicei = device.GetInternal();
    const auto* instance_table = devicei.GetInstanceTable();
    VkSurfaceKHR surface;
    auto result = instance_table->vkCreateWin32SurfaceKHR(device.GetInternal().instance.get(), &surface_desc, nullptr, &surface);
    if (!wis::succeeded(result)) {
        return std::pair{
            wis::make_result<FUNC, "Failed to create Win32 surface">(result), VKSwapChain{}
        };
    }
    wis::shared_handle<VkSurfaceKHR> surface_handle{ surface, device.GetInternal().instance };

    // to device
    // 
    //int32_t present_queue = -1;
    //for (uint16_t i = 0; i < max_count; i++) {
    //    const auto& x = queues.available_queues[i];
    //    if (x.Empty())
    //        continue;

    //    auto [result, value] = adapter.getSurfaceSupportKHR(x.family_index, surface.get());

    //    if (value) {
    //        present_queue = i;
    //        lib_info(format("Present queue {} selected", i));
    //        break;
    //    }
    //}
    //if (present_queue == -1) {
    //    lib_error("None of the queues support presenting to the surface");
    //    return {}; // Presentation is not supported
    //}

    //const auto& queue = queues.available_queues[present_queue];
    //vk::DeviceQueueInfo2 info{};
    //{
    //    info.queueFamilyIndex = queue.family_index,
    //    info.queueIndex = queue.GetNextInLine();
    //};
    //vk::Queue qpresent_queue = device->getQueue2(info);

    //auto [result2, surface_formats] = adapter.getSurfaceFormatsKHR(surface.get());
    //if (!succeeded(result2))
    //    return {};

    //auto format = std::ranges::find_if(surface_formats,
    //                                   [=](const vk::SurfaceFormatKHR& fmt) {
    //    return fmt.format == vk_format(options.format);
    //                                   });

    //if (format == surface_formats.end() || format->format == vk::Format::eUndefined) {
    //    lib_error(wis::format("Supplied format {} is not supported by surface", data_format_strings[+options.format]));
    //    return {}; // Format specified is not supported
    //}

    //auto [result3, cap] = adapter.getSurfaceCapabilitiesKHR(surface.get());
    //if (!succeeded(result3))
    //    return {};

    //bool stereo = cap.maxImageArrayLayers > 1;
    //if (options.stereo && stereo)
    //    lib_info(wis::format("Stereo mode is ativated"));

    //uint32_t layers = options.stereo && stereo ? 2u : 1u;

    //auto present_mode = GetPresentMode(surface.get(), vsync);

    //vk::SwapchainCreateInfoKHR desc{};
    //{
    //    desc.surface = surface.get(),
    //    desc.minImageCount = options.frame_count,
    //    desc.imageFormat = format->format,
    //    desc.imageColorSpace = format->colorSpace,
    //    desc.imageExtent = vk::Extent2D{ options.width, options.height },
    //    desc.imageArrayLayers = layers,
    //    desc.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
    //    desc.imageSharingMode = vk::SharingMode::eExclusive,
    //    desc.queueFamilyIndexCount = 0u,
    //    desc.pQueueFamilyIndices = nullptr,
    //    desc.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
    //    desc.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
    //    desc.presentMode = present_mode,
    //    desc.clipped = true;
    //};

    //auto [result4, swapchain] = device->createSwapchainKHR(desc);
    //return succeeded(result4)
    //        ? VKSwapChain{
    //              shared_handle<vk::SwapchainKHR>{
    //                      swapchain,
    //                      device,
    //                      surface },
    //              render_queue,
    //              VKCommandQueue{ qpresent_queue },
    //              CreateCommandList(QueueType::Direct),
    //              *format,
    //              present_mode,
    //              stereo
    //          }
    //        : VKSwapChain{};
}
#endif // WISDOM_VULKAN
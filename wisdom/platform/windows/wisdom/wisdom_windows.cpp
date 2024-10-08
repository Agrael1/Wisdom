#ifndef WISDOM_WINDOWS_CPP
#define WISDOM_WINDOWS_CPP
#include <wisdom/wisdom_windows.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/dx12/dx12_device.h>
#include <d3d11.h>

namespace wis::detail {
inline void ToSwapchainDesc(DXGI_SWAP_CHAIN_DESC1& swap_desc, const wis::SwapchainDesc* desc) noexcept
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
    swap_desc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
}
inline wis::com_ptr<ID3D11Device> CreateD3D11Device() noexcept
{
    constexpr D3D_FEATURE_LEVEL featureLevels[]{
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    wis::com_ptr<ID3D11Device> device11;
    D3D11CreateDevice(nullptr,
                      D3D_DRIVER_TYPE_HARDWARE,
                      nullptr, 0,
                      featureLevels, 2, D3D11_SDK_VERSION, device11.put(), nullptr, nullptr);
    return device11;
}

} // namespace wis::detail

wis::ResultValue<wis::DX12SwapChain>
wis::platform::DX12WindowsExtension::CreateSwapchain(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) const noexcept
{
    DXGI_SWAP_CHAIN_DESC1 swap_desc;
    detail::ToSwapchainDesc(swap_desc, desc);
    auto& devicei = device.GetInternal();

    swap_desc.Stereo &= devicei.factory->IsWindowedStereoEnabled();

    bool tearing = [&]() {
        auto [hr, factory5] = devicei.factory.as<IDXGIFactory5>();
        if (!wis::succeeded(hr)) {
            return false;
        }
        BOOL xtearing = FALSE;
        factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &xtearing, sizeof(xtearing));
        return bool(xtearing);
    }();
    if (tearing)
        swap_desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    HRESULT hr;
    wis::com_ptr<IDXGISwapChain1> swap;

    // until microsoft fixes this
    if (desc->stereo && !wis::succeeded(hr = devicei.factory->CreateSwapChainForHwnd(detail::CreateD3D11Device().get(), hwnd, &swap_desc, nullptr, nullptr, swap.put()))) {
        return wis::make_result<FUNC, "Failed to create D3D11 device for stereo mode">(hr);
    }

    hr = devicei.factory->CreateSwapChainForHwnd(
            std::get<0>(main_queue), // Swap chain needs the queue so that it can force a flush on it.
            hwnd,
            &swap_desc,
            nullptr,
            nullptr,
            swap.put());

    if (!wis::succeeded(hr)) {
        return wis::make_result<FUNC, "Failed to create swapchain for hwnd">(hr);
    }
    auto [hrx, swap4] = swap.as<IDXGISwapChain4>();
    if (!wis::succeeded(hrx)) {
        return wis::make_result<FUNC, "Failed to create swapchain for HWND">(hr);
    }

    auto hnd = swap4->GetFrameLatencyWaitableObject();

    wis::detail::DX12SwapChainCreateInfo create_info{
        .chain = std::move(swap4),
        .present_event{ hnd },
        .stereo = desc->stereo,
        .vsync = desc->vsync,
        .tearing = tearing,
    };
    if (auto resw = create_info.InitBackBuffers(); resw.status != wis::Status::Ok)
        return resw;

    return DX12SwapChain{ std::move(create_info) };
}

wis::ResultValue<wis::DX12SwapChain>
wis::platform::DX12WindowsExtension::CreateSwapchainUWP(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, IUnknown* window) const noexcept
{
    DXGI_SWAP_CHAIN_DESC1 swap_desc;
    detail::ToSwapchainDesc(swap_desc, desc);
    auto& devicei = device.GetInternal();

    swap_desc.Stereo &= devicei.factory->IsWindowedStereoEnabled();
    bool tearing = [&]() {
        auto [hr, factory5] = devicei.factory.as<IDXGIFactory5>();
        if (!wis::succeeded(hr)) {
            return false;
        }
        BOOL xtearing = FALSE;
        factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &xtearing, sizeof(xtearing));
        return bool(xtearing);
    }();
    if (tearing)
        swap_desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    HRESULT hr;
    wis::com_ptr<IDXGISwapChain1> swap;

    // until microsoft fixes this
    if (desc->stereo && !wis::succeeded(hr = devicei.factory->CreateSwapChainForCoreWindow(detail::CreateD3D11Device().get(), window, &swap_desc, nullptr, swap.put()))) {
        return wis::make_result<FUNC, "Failed to create D3D11 device for stereo mode">(hr);
    }

    hr = devicei.factory->CreateSwapChainForCoreWindow(
            std::get<0>(main_queue), // Swap chain needs the queue so that it can force a flush on it.
            window,
            &swap_desc,
            nullptr,
            swap.put());

    if (!wis::succeeded(hr)) {
        return wis::make_result<FUNC, "Failed to create swapchain for core window">(hr);
    }
    auto [hrx, swap4] = swap.as<IDXGISwapChain4>();
    if (!wis::succeeded(hrx)) {
        return wis::make_result<FUNC, "Failed to create swapchain for core window">(hr);
    }
    auto hnd = swap4->GetFrameLatencyWaitableObject();

    wis::detail::DX12SwapChainCreateInfo create_info{
        .chain = std::move(swap4),
        .present_event{ hnd },
        .stereo = desc->stereo,
        .vsync = desc->vsync,
        .tearing = tearing,
    };
    if (auto resw = create_info.InitBackBuffers(); resw.status != wis::Status::Ok)
        return resw;

    return DX12SwapChain{ std::move(create_info) };
}

#ifdef WISDOM_VULKAN
#include <wisdom/vulkan/vk_device.h>

// #error error

wis::ResultValue<wis::VKSwapChain>
wis::platform::VKWindowsExtension::CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) const noexcept
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
    const auto& instance_table = instance.table();
    VkSurfaceKHR surface;
    auto result = vkCreateWin32SurfaceKHR(instance.get(), &surface_desc, nullptr, &surface);
    if (!wis::succeeded(result)) {
        return wis::make_result<FUNC, "Failed to create Win32 surface">(result);
    }
    wis::SharedSurface surface_handle{ surface, instance, instance_table.vkDestroySurfaceKHR };
    return device.VKCreateSwapChain(surface_handle, desc, std::get<0>(main_queue));
}

bool wis::platform::VKInteropDeviceExtension::GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                                                               std::unordered_set<std::string_view>& ext_name_set,
                                                               std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                                               std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    if (available_extensions.contains(VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME)) {
        ext_name_set.emplace(VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME);
    }
    if (available_extensions.contains(VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME)) {
        ext_name_set.emplace(VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
    }
    // TODO: check?
    return true;
}

wis::Result
wis::platform::VKInteropDeviceExtension::Init(const wis::VKDevice& instance,
                                              const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                              const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    device = instance.GetInternal().device;
    vkGetMemoryWin32HandleKHR = device.GetDeviceProcAddr<PFN_vkGetMemoryWin32HandleKHR>("vkGetMemoryWin32HandleKHR");
    vkGetSemaphoreWin32HandleKHR = device.GetDeviceProcAddr<PFN_vkGetSemaphoreWin32HandleKHR>("vkGetSemaphoreWin32HandleKHR");

    // Tell the device that memory and semaphores should support interop
    const_cast<wis::XInternalFeatures&>(instance.GetInternal().ext1.GetInternal().features).interop_device = Supported();
    return {};
}
#endif // WISDOM_VULKAN
#endif // WISDOM_WINDOWS_CPP

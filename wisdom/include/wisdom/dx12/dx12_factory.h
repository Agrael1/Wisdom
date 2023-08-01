#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_factory.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/bridge/generator.h>

#include <dxgi1_6.h>
#endif

WIS_EXPORT namespace wis
{
    class DX12Factory;

    template<>
    class Internal<DX12Factory>
    {
    public:
        Internal()
        {
            if (factory)
                factory->AddRef();
        }
        ~Internal()
        {
            if (!factory->Release())
                factory.detach();
        }

    public:
        [[nodiscard]] static IDXGIFactory4* GetFactory() noexcept
        {
            return factory.get();
        }

    protected:
        static inline winrt::com_ptr<IDXGIFactory4> factory{};
    };

    /// @brief Main Factory class, since we don't need more than one factory it is a static resource
    /// @note Not thread safe on creation
    class DX12Factory final : public QueryInternal<DX12Factory>
    {
        friend class DX12Device;
        friend class DX12Surface;
        static inline constexpr uint32_t debug_flag = wis::debug_mode & DXGI_CREATE_FACTORY_DEBUG;

    public:
        /// @brief Creates a new factory
        /// @param app_info Application info, not used
        ///	@param use_preference Use the preference when enumerating adapters
        WIS_INLINE DX12Factory([[maybe_unused]] const ApplicationInfo& app_info, bool use_preference = true);

    public:
        /// @brief Enumerates all adapters on the system
        /// @param preference Preference to use when enumerating adapters, changes the order of the adapters
        /// @return coroutine that yields DX12Adapter
        WIS_INLINE [[nodiscard]] wis::generator<DX12Adapter> EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance) const noexcept;

    public:
        /// @brief Used as a direct bridge to the IDXGIFactory, used only by internal classes
        /// @return Factory interface
        [[nodiscard]] static auto GetFactory() noexcept
        {
            return factory;
        }

    private:
        /// @brief enables the debug layer if debug mode is enabled by compiler
        WIS_INLINE void EnableDebugLayer() noexcept;

        /// @brief Enumerates all adapters on the system by GPU preference
        /// @param preference Specifies the GPU preference
        /// @return Coroutine that yields IDXGIAdapter1
        WIS_INLINE wis::generator<winrt::com_ptr<IDXGIAdapter1>>
        AdaptersByGPUPreference(DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) const noexcept;

        /// @brief Enumerates all adapters on the system
        /// @return Coroutine that yields IDXGIAdapter1
        WIS_INLINE wis::generator<winrt::com_ptr<IDXGIAdapter1>>
        Adapters() const noexcept;

        /// @brief Creates a swapchain for a core window
        /// @param desc Description of the swapchain
        /// @param core_window Core window to create the swapchain for
        /// @param queue Queue to use for the swapchain
        /// @return Handle to the swapchain
        WIS_INLINE [[nodiscard]] static winrt::com_ptr<IDXGISwapChain4>
        SwapChainForCoreWindow(const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue);

        /// @brief Creates a swapchain for a win32 window
        /// @param desc Description of the swapchain
        /// @param hwnd Window handle to create the swapchain for
        /// @param queue Queue to use for the swapchain
        /// @return Handle to the swapchain
        WIS_INLINE [[nodiscard]] static winrt::com_ptr<IDXGISwapChain4>
        SwapChainForWin32(const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue);

    private:
        static inline bool has_preference = false; //< if the user has specified a preference for adapters
        DX12Info info; //< infromation queue for debugging and error messages
    };
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_factory.inl"
#endif

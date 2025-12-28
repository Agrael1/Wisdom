#ifndef WIS_DX12_TYPES_HPP
#define WIS_DX12_TYPES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <dxgi1_6.h>
#include <d3d12.h>
#include <cassert>
#include <wrl/implements.h>

namespace wis {
//-----------------------------------------------------------------------------
constexpr inline wis::Result convert_result(WisResult result) noexcept
{
    return { static_cast<wis::Status>(result.status), result.platform_code, result.error };
}

//-----------------------------------------------------------------------------
namespace detail {
class DX12DebugLayer : public Microsoft::WRL::RuntimeClass<
                               Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom | Microsoft::WRL::InhibitRoOriginateError>,
                               IUnknown>
{
public:
    WisDebugCallback callback  = nullptr;
    void*            user_data = nullptr;
};
} // namespace detail

namespace impl {
struct DX12InstanceImpl {
    IDXGIFactory6*               factory;
    wis::detail::DX12DebugLayer* debug_layer;
};

struct DX12AdapterQueryImpl {
    IDXGIAdapter4**              physical_devices;
    std::size_t                  adapter_count;
    IDXGIFactory6*               factory;
    wis::detail::DX12DebugLayer* debug_layer;
};

struct DX12DeviceImpl {
    ID3D12Device10* device;
    IDXGIAdapter4*  physical_device;
    IDXGIFactory6*  factory;
};

struct DX12CommandQueueImpl {
    ID3D12CommandQueue* queue;
};

struct DX12CommandListImpl {
    ID3D12GraphicsCommandList7* list;
};

struct DX12FenceImpl {
    ID3D12Fence* fence;
    HANDLE       event;
};
} // namespace impl

// Manual variants of generated structures with virtual functions
struct DX12InstanceExtensionHeader {
    explicit DX12InstanceExtensionHeader(WisResult (*init_fptr)(void* self, const impl::DX12InstanceImpl& instance) noexcept) noexcept
        : init_fptr(init_fptr)
    {
    }

private:
    WisResult (*init_fptr)(void* self, const impl::DX12InstanceImpl& instance) noexcept = &DX12InstanceExtensionHeader::Init;

    static WisResult Init(void* self, const impl::DX12InstanceImpl& instance) noexcept
    {
        return {};
    }

public:
    WisResult CallInit(void* self, const impl::DX12InstanceImpl& instance) noexcept
    {
        return init_fptr(self, instance);
    }
};

// Manual variants of generated structures with virtual functions
struct DX12DeviceExtensionHeader {
    // TODO: add Init function pointer and CallInit method when device extensions are supported
};

// TODO: move to an extension header file
template<typename T, typename... Impls>
struct DX12InstanceExtensionImpl : public DX12InstanceExtensionHeader, public Impls... {
    DX12InstanceExtensionImpl() noexcept
        : DX12InstanceExtensionHeader(&DX12InstanceExtensionImpl<T>::Init)
    {
        assert(std::uintptr_t(static_cast<T*>(this)) == std::uintptr_t(static_cast<DX12InstanceExtensionHeader*>(this)) && "DX12InstanceExtensionImpl must be the first base class!");
    }

    static WisResult Init(void* self, const impl::DX12InstanceImpl& instance) noexcept
    {
        return reinterpret_cast<T*>(self)->Init(instance);
    }
};

struct DX12TestExtension : public DX12InstanceExtensionImpl<DX12TestExtension> {
    WisResult Init(const impl::DX12InstanceImpl& instance) noexcept
    {
        // Test extension initialization logic
        return WisResult{ WisStatus::WisStatusOk, 0, "DX12TestExtension initialized successfully." };
    }
};

} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "dx12_instance.cpp"
#endif // WISDOM_BUILD_BINARIES
#endif // DX12_FACTORY_HPP

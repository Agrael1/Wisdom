#ifndef WIS_DX12_DETAIL_HPP
#define WIS_DX12_DETAIL_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/bridge/span.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/util/com_ptr.hpp>

#include <array>
#include <bit>

namespace wis::detail {

/**
 * @brief Internal class implementing the debug layer for DirectX 12. This class is not exposed to users and is used
 * internally to handle debug messages from the DirectX 12 API.
 * @details The DX12DebugLayer class implements the IUnknown interface and holds a callback function and user data for
 * handling debug messages. The DX12DebugLayerThunk class is responsible for registering the callback with the DirectX
 * 12 info queue and translating DirectX 12 debug messages into the format expected by the user's callback. The thunk
 * class also manages the lifetime of the callback registration and ensures that the callback is properly unregistered
 * when the debug layer is destroyed.
 */
struct DX12DebugLayer final : public IUnknownImpl<DX12DebugLayer, IUnknown> {
public:
    ~DX12DebugLayer() = default;

public:
    WisDebugCallback callback = nullptr;
    void* user_data = nullptr;
};

/**
 * @brief Internal class responsible for registering the debug callback with the DirectX 12 info queue and translating
 * messages.
 * @details The DX12DebugLayerThunk class registers a callback with the ID3D12
 * InfoQueue1 interface to receive debug messages from DirectX 12. It translates the severity of the messages into the
 * WisSeverity format and invokes the user's callback with the appropriate parameters. The class also manages the
 * registration cookie to ensure that the callback is properly unregistered when the DX12DebugLayerThunk instance is
 * destroyed. This class is not exposed to users and is used internally by the DX12DebugLayer to handle debug messages.
 */
struct DX12DebugLayerThunk final : public IUnknownImpl<DX12DebugLayerThunk, IUnknown> {
public:
    DX12DebugLayerThunk(
        ID3D12InfoQueue1* in_info_queue,
        uint64_t device,
        WisDebugCallback in_callback,
        void* in_user_data
    ) noexcept
        : info_queue(in_info_queue)
        , user_data(in_user_data)
        , device(device)
        , callback(in_callback)
    {
        if (info_queue) {
            const auto hr = info_queue->RegisterMessageCallback(
                DX12CallbackThunk,
                D3D12_MESSAGE_CALLBACK_FLAG_NONE,
                this,
                &cookie
            );
            // Debug layer creation failure is allowed to silently fail
            (void)hr;
        }
    }
    ~DX12DebugLayerThunk()
    {
        if (info_queue && cookie != 0) {
            info_queue->UnregisterMessageCallback(cookie);
        }
    }

private:
    static void DX12CallbackThunk(
        D3D12_MESSAGE_CATEGORY category,
        D3D12_MESSAGE_SEVERITY severity,
        D3D12_MESSAGE_ID id,
        LPCSTR pDescription,
        void* pContext
    )
    {
        auto* layer = static_cast<const DX12DebugLayerThunk*>(pContext);
        layer->DX12Callback(category, severity, id, pDescription);
    }
    void DX12Callback(
        D3D12_MESSAGE_CATEGORY category,
        D3D12_MESSAGE_SEVERITY severity,
        D3D12_MESSAGE_ID id,
        LPCSTR pDescription
    ) const
    {
        (void)category;
        (void)id;
        WisSeverity wis_severity = WisSeverityInfo;
        switch (severity) {
        case D3D12_MESSAGE_SEVERITY_CORRUPTION:
            wis_severity = WisSeverityFatal;
            break;
        case D3D12_MESSAGE_SEVERITY_ERROR:
            wis_severity = WisSeverityError;
            break;
        case D3D12_MESSAGE_SEVERITY_WARNING:
            wis_severity = WisSeverityWarning;
            break;
        case D3D12_MESSAGE_SEVERITY_INFO:
            wis_severity = WisSeverityInfo;
            break;
        case D3D12_MESSAGE_SEVERITY_MESSAGE:
            wis_severity = WisSeverityVerbose;
            break;
        default:
            wis_severity = WisSeverityInfo;
            break;
        }
        callback(wis_severity, pDescription, device, user_data);
    }

private:
    ID3D12InfoQueue1* info_queue;
    DWORD cookie = 0;
    void* user_data = nullptr;
    uint64_t device = 0;
    WisDebugCallback callback;
};

//----------------------------------------------------------------------------------------------------------------------
struct DX12RootSignatureKey {
    static constexpr GUID guid{0xf062fe85, 0x857f, 0x43a9, {0xa2, 0x66, 0x75, 0x59, 0xb8, 0x10, 0x10, 0x01}};

    // Hash of the root signature description, used for caching and identification purposes.
    std::array<uint64_t, 2> hash{};
};

//----------------------------------------------------------------------------------------------------------------------
struct DX12ShaderHeader {
    uint64_t hash[2]{}; // Hash of the shader bytecode, used for caching and identification purposes.
    std::size_t size = 0; // Size of the shader bytecode in bytes.

    // bytecode follows immediately after the header in memory.

    wis::span<const std::byte> GetBytecode() const noexcept
    {
        return wis::span<const std::byte>{reinterpret_cast<const std::byte*>(this + 1), size};
    }
    wis::span<std::byte> GetMutableBytecode() noexcept
    {
        return wis::span<std::byte>{reinterpret_cast<std::byte*>(this + 1), size};
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct DX12RenderTargetViewAuxData {
    D3D12_CPU_DESCRIPTOR_HANDLE handle;
    ID3D12Resource* resource; // Non-owning pointer.
    uint16_t base_subresource;
    uint16_t subresource_count;
    uint16_t subresource_stride;
    uint16_t format; // DXGI_FORMAT, stored as uint16_t for compactness.
    uint16_t base_stencil_subresource;
};

constexpr uint64_t DX12EncodeViewAddress(const DX12RenderTargetViewAuxData* ptr) noexcept
{
    return std::bit_cast<uint64_t>(ptr) | 0x1ull;
}

constexpr const DX12RenderTargetViewAuxData* DX12DecodeViewAddress(const uint64_t address) noexcept
{
    if ((address & 0x1ull) == 0) {
        return nullptr;
    }
    return std::bit_cast<const DX12RenderTargetViewAuxData*>(address & ~0x1ull);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Internal helper function to convert a WisDescriptorType to a D3D12_ROOT_PARAMETER_TYPE.
 * This function is used internally to determine the appropriate root parameter type for a given descriptor type when
 * creating root signatures in DirectX 12. The function maps the Wisdom descriptor types to the corresponding DirectX 12
 * root parameter types, such as constant buffer views (CBV), shader resource views (SRV), and unordered access views
 * (UAV). This is essential for correctly setting up root signatures and ensuring that the descriptors are bound
 * correctly in the shader stages.
 * @param type The WisDescriptorType to convert.
 * @return The corresponding D3D12_ROOT_PARAMETER_TYPE for the given WisDescriptorType.
 */
constexpr D3D12_ROOT_PARAMETER_TYPE DX12RootParameterType(const WisDescriptorType type) noexcept
{
    switch (type) {
    case WisDescriptorTypeConstantBuffer:
        return D3D12_ROOT_PARAMETER_TYPE_CBV;
    default:
    case WisDescriptorTypeBuffer:
    case WisDescriptorTypeAccelerationStructure:
        return D3D12_ROOT_PARAMETER_TYPE_SRV;
    case WisDescriptorTypeRWBuffer:
        return D3D12_ROOT_PARAMETER_TYPE_UAV;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Internal helper function to determine if a given WisDescriptorType can be used as a push descriptor in
 * DirectX 12. This function checks the type of the descriptor and returns true if it is one of the types that can be
 * used as a push descriptor, such as buffers and acceleration structures. This is used internally to optimize
 * descriptor updates by using push descriptors when possible.
 * @param type The WisDescriptorType to check.
 * @return True if the descriptor type can be used as a push descriptor, false otherwise.
 */
constexpr bool DX12IsPushable(const WisDescriptorType type) noexcept
{
    switch (type) {
    case WisDescriptorTypeBuffer:
    case WisDescriptorTypeConstantBuffer:
    case WisDescriptorTypeRWBuffer:
    case WisDescriptorTypeAccelerationStructure:
        return true;
    default:
        return false;
    }
}

//------------------------------------------------------------------------------------------------------
/**
 * @brief Internal helper function to determine the appropriate plane slice for a given set of barrier flags in
 * DirectX 12.
 * @param flags The WisBarrierFlags that may indicate whether the resource is a planar image or a stencil resource.
 * @param plane_slice The original plane slice index from the subresource range.
 */
inline constexpr uint32_t DX12GetCopyPlaneSlice(WisBarrierFlags flags, uint16_t plane_slice) noexcept
{
    if (flags & WisBarrierFlagsPlanarImage) {
        return plane_slice;
    }

    if (flags & WisBarrierFlagsStencilResource) {
        return 1u;
    }

    return 0u;
}
} // namespace wis::detail

#endif // WIS_DX12_DETAIL_HPP

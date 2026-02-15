#ifndef WIS_DX12_DETAIL_HPP
#define WIS_DX12_DETAIL_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/util/com_ptr.hpp>
#include <wisdom/generated/c_api.h>
#include <d3d12.h>
#include <wisdom/bridge/span.hpp>
#include <wisdom/generated/dx12_convert.hpp>

namespace wis::detail {

/**
 * @brief Internal class implementing the debug layer for DirectX 12. This class is not exposed to users and is used internally to handle debug messages from the DirectX 12 API.
 * @details The DX12DebugLayer class implements the IUnknown interface and holds a callback function and user data for handling debug messages.
 * The DX12DebugLayerThunk class is responsible for registering the callback with the DirectX 12 info queue and translating DirectX 12 debug messages into the format expected by the user's callback.
 * The thunk class also manages the lifetime of the callback registration and ensures that the callback is properly unregistered when the debug layer is destroyed.
 */
struct DX12DebugLayer final : public IUnknownImpl<IUnknown> {
private:
    ~DX12DebugLayer() = default;

public:
    WisDebugCallback callback  = nullptr;
    void*            user_data = nullptr;
};

/**
 * @brief Internal class responsible for registering the debug callback with the DirectX 12 info queue and translating messages.
 * @details The DX12DebugLayerThunk class registers a callback with the ID3D12
 * InfoQueue1 interface to receive debug messages from DirectX 12. It translates the severity of the messages into the WisSeverity format and invokes the user's callback with the appropriate parameters.
 * The class also manages the registration cookie to ensure that the callback is properly unregistered when the
 * DX12DebugLayerThunk instance is destroyed. This class is not exposed to users and is used internally by the DX12DebugLayer to handle debug messages.
 */
struct DX12DebugLayerThunk final : public IUnknownImpl<IUnknown> {
public:
    DX12DebugLayerThunk(ID3D12InfoQueue1* in_info_queue,
                        uint64_t          device,
                        WisDebugCallback  in_callback,
                        void*             in_user_data) noexcept
        : info_queue(in_info_queue)
        , user_data(in_user_data)
        , device(device)
        , callback(in_callback)
    {
        if (info_queue) {
            const auto hr = info_queue->RegisterMessageCallback(DX12CallbackThunk,
                                                                D3D12_MESSAGE_CALLBACK_FLAG_NONE,
                                                                this,
                                                                &cookie);
            // Debug layer creation failure is allowed to silently fail
            (void)hr;
        }
    }

private:
    ~DX12DebugLayerThunk()
    {
        if (info_queue && cookie != 0) {
            info_queue->UnregisterMessageCallback(cookie);
        }
    }

private:
    static void DX12CallbackThunk(D3D12_MESSAGE_CATEGORY category,
                                  D3D12_MESSAGE_SEVERITY severity,
                                  D3D12_MESSAGE_ID       id,
                                  LPCSTR                 pDescription,
                                  void*                  pContext)
    {
        auto* layer = static_cast<const DX12DebugLayerThunk*>(pContext);
        layer->DX12Callback(category, severity, id, pDescription);
    }
    void DX12Callback(D3D12_MESSAGE_CATEGORY category,
                      D3D12_MESSAGE_SEVERITY severity,
                      D3D12_MESSAGE_ID       id,
                      LPCSTR                 pDescription) const
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
    DWORD             cookie    = 0;
    void*             user_data = nullptr;
    uint64_t          device    = 0;
    WisDebugCallback  callback;
};

//-----------------------------------------------------------------------------
/**
 * @brief Internal helper function to convert a WisDescriptorType to a D3D12_ROOT_PARAMETER_TYPE.
 * This function is used internally to determine the appropriate root parameter type for a given descriptor type when creating root signatures in DirectX 12.
 * The function maps the Wisdom descriptor types to the corresponding DirectX 12 root parameter types, such as constant buffer views (CBV), shader resource views (SRV), and unordered access views (UAV).
 * This is essential for correctly setting up root signatures and ensuring that the descriptors are bound correctly in the shader stages.
 * @param type The WisDescriptorType to convert.
 * @return The corresponding D3D12_ROOT_PARAMETER_TYPE for the given WisDescriptorType.
 */
constexpr D3D12_ROOT_PARAMETER_TYPE dx12_root_parameter_type(const WisDescriptorType type) noexcept
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

//-----------------------------------------------------------------------------
/**
 * @brief Internal helper function to determine if a given WisDescriptorType can be used as a push descriptor in DirectX 12.
 * This function checks the type of the descriptor and returns true if it is one of the types that can be used as a push descriptor, such as buffers and acceleration structures.
 * This is used internally to optimize descriptor updates by using push descriptors when possible.
 * @param type The WisDescriptorType to check.
 * @return True if the descriptor type can be used as a push descriptor, false otherwise.
 */
constexpr bool dx12_is_pushable(const WisDescriptorType type) noexcept
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

//-----------------------------------------------------------------------------
/**
 * @brief Internal helper function to fill a D3D12_DESCRIPTOR_RANGE1 array based on a WisDescriptorTable. This function handles both the normal case and the overlap case where multiple descriptor ranges share the same register space.
 * @param table The WisDescriptorTable containing the descriptor entries to convert.
 * @param space The register space to use for the descriptor ranges.
 * @param mutable_range A span of D3D12_DESCRIPTOR_RANGE1 that will be filled with the converted descriptor ranges. The caller must ensure that this span has enough capacity to hold all the ranges.
 * @return The number of descriptor ranges filled in the mutable_range span.
 */
inline uint32_t dx12_fill_descriptor_range(const WisDescriptorTable&          table,
                                           uint32_t                           space,
                                           wis::span<D3D12_DESCRIPTOR_RANGE1> mutable_range) noexcept
{
    // Handle overlap case
    if (table.space_overlap != 0) {
        auto&                   src = table.entries[0];
        D3D12_DESCRIPTOR_RANGE1 range{
            .RangeType                         = convert_dx(src.type),
            .NumDescriptors                    = static_cast<UINT>(src.count == 0 ? 1 : src.count),
            .BaseShaderRegister                = static_cast<UINT>(src.bind_register),
            .RegisterSpace                     = static_cast<UINT>(space),
            .Flags                             = src.count > 1 ? D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE : D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
            .OffsetInDescriptorsFromTableStart = 0,
        };

        for (size_t i = 0; i < table.space_overlap + 1; ++i) {
            range.RegisterSpace = static_cast<UINT>(space + i);
            mutable_range[i]    = range;
        }
        return table.space_overlap + 1;
    } else {
        for (size_t i = 0; i < table.entry_count; ++i) {
            auto& src        = table.entries[i];
            mutable_range[i] = {
                .RangeType                         = convert_dx(src.type),
                .NumDescriptors                    = (src.count == 0 ? 1 : src.count),
                .BaseShaderRegister                = static_cast<UINT>(src.bind_register),
                .RegisterSpace                     = (space),
                .Flags                             = src.count > 1 ? D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE : D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
                .OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
            };
        }
        return static_cast<uint32_t>(table.entry_count);
    }
}
} // namespace wis::detail

#endif // WIS_DX12_DETAIL_HPP

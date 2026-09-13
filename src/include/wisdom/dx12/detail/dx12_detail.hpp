#ifndef WIS_DX12_DETAIL_HPP
#define WIS_DX12_DETAIL_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/bridge/span.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/generated/cpp_api.hpp>
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
//----------------------------------------------------------------------------------------------------------------------
// Barrier helper constants
constexpr static uint32_t dx12_max_barrier_size = std::max(
    {sizeof(D3D12_BUFFER_BARRIER), sizeof(D3D12_TEXTURE_BARRIER), sizeof(D3D12_GLOBAL_BARRIER)}
);
constexpr static uint32_t dx12_static_size = wis::TransientMaxBarrierCount * dx12_max_barrier_size;

template <typename Impl>
inline uint8_t* DX12AllocateScratchSpace(const Impl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
        impl.scratch_memory_size = impl.scratch_memory ? new_size : 0;
    }
    return impl.scratch_memory;
}

template <typename Impl>
inline std::array<wis::span<uint8_t>, 3> DX12AllocateBarriers(
    const Impl& impl,
    uint8_t* local_scratch,
    const WisDX12BarrierGroup& barriers
)
{
    std::array<wis::span<uint8_t>, 3> spans;
    std::size_t needed_size = barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)
                            + barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)
                            + barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER);

    if (needed_size <= dx12_static_size) {
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    }

    std::size_t sizes[] = {
        barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER),
        barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER),
        barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER),
        0,
        0,
        0
    };

    sizes[3] = sizes[0] + sizes[1];
    sizes[4] = sizes[1] + sizes[2];
    sizes[5] = sizes[0] + sizes[2];

    uint32_t closest_size = 0;
    int index = -1;
    for (int i = std::size(sizes) - 1; i >= 0; --i) {
        if (sizes[i] > dx12_static_size) {
            continue;
        }
        if (dx12_static_size - sizes[i] < dx12_static_size - closest_size) {
            closest_size = sizes[i];
            index = i;
        }
    }

    uint32_t allocated_size = needed_size - closest_size;
    auto* allocated_data = DX12AllocateScratchSpace(impl, allocated_size);

    switch (index) {
    default:
    case -1:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    case 0:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {allocated_data, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    case 1:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {local_scratch, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {spans[0].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    case 2:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {local_scratch, barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    case 3:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {allocated_data, barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    case 4:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {local_scratch, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    case 5:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER)};
        spans[1] = {allocated_data, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER)};
        spans[2] = {spans[0].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER)};
        return spans;
    }
}

inline constexpr D3D12_BARRIER_LAYOUT DX12GetOptimalBarrierLayout(
    WisCommandQueueType type,
    WisTextureState state
) noexcept
{
    switch (type) {
    case WisCommandQueueTypeGraphics:
        switch (state) {
        case WisTextureStateCommon:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COMMON;
        case WisTextureStateRead:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_GENERIC_READ;
        case WisTextureStateUnorderedAccess:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_UNORDERED_ACCESS;
        case WisTextureStateShaderResource:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_SHADER_RESOURCE;
        case WisTextureStateCopySrc:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_SOURCE;
        case WisTextureStateCopyDst:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_DEST;
        default:
            return wis::detail::DX12Convert(state);
        }
    case WisCommandQueueTypeCompute:
        switch (state) {
        case WisTextureStateCommon:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COMMON;
        case WisTextureStateRead:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_GENERIC_READ;
        case WisTextureStateUnorderedAccess:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_UNORDERED_ACCESS;
        case WisTextureStateShaderResource:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_SHADER_RESOURCE;
        case WisTextureStateCopySrc:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_SOURCE;
        case WisTextureStateCopyDst:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_DEST;
        default:
            return wis::detail::DX12Convert(state);
        }
    default:
        return wis::detail::DX12Convert(state);
    }
}

template <typename Impl, typename List>
inline void DX12InsertBarriers(
    const Impl& impl,
    List* list,
    const WisDX12BarrierGroup* barriers,
    WisCommandQueueType queue_type
)
{
    if (barriers->buffer_barrier_count + barriers->texture_barrier_count + barriers->global_barrier_count == 0) {
        return;
    }

    uint8_t local_scratch[dx12_static_size]{};

    auto [buffer_span, texture_span, global_span] = DX12AllocateBarriers(impl, local_scratch, *barriers);

    wis::span<D3D12_BUFFER_BARRIER> buffer_barriers_span{
        reinterpret_cast<D3D12_BUFFER_BARRIER*>(buffer_span.data()),
        barriers->buffer_barrier_count
    };
    uint32_t real_buffer_barrier_count = barriers->buffer_barrier_count;

    for (size_t i = 0; i < barriers->buffer_barrier_count; ++i) {
        auto& src = barriers->buffer_barriers[i];

        if (src.queue_type_after != src.queue_type_before) {
            real_buffer_barrier_count--;
            continue;
        }

        buffer_barriers_span[i] = D3D12_BUFFER_BARRIER{
            .SyncBefore = DX12Convert(src.sync_before),
            .SyncAfter = DX12Convert(src.sync_after),
            .AccessBefore = DX12Convert(src.access_before),
            .AccessAfter = DX12Convert(src.access_after),
            .pResource = std::bit_cast<ID3D12Resource*>(src.buffer),
            .Offset = src.offset,
            .Size = src.size,
        };
    }

    wis::span<D3D12_TEXTURE_BARRIER> texture_barriers_span{
        reinterpret_cast<D3D12_TEXTURE_BARRIER*>(texture_span.data()),
        barriers->texture_barrier_count
    };
    for (size_t i = 0; i < barriers->texture_barrier_count; ++i) {
        auto& src = barriers->texture_barriers[i];

        bool qfot_barrier = src.queue_type_after != src.queue_type_before;
        bool acquire_barrier = qfot_barrier && src.queue_type_after == queue_type;
        bool release_barrier = qfot_barrier && src.queue_type_before == queue_type;

        auto layout_before = DX12GetOptimalBarrierLayout(
            queue_type,
            acquire_barrier ? WisTextureStateCommon : src.state_before
        );
        auto layout_after = DX12GetOptimalBarrierLayout(
            queue_type,
            release_barrier ? WisTextureStateCommon : src.state_after
        );

        texture_barriers_span[i] = D3D12_TEXTURE_BARRIER{
            .SyncBefore = DX12Convert(src.sync_before),
            .SyncAfter = DX12Convert(src.sync_after),
            .AccessBefore = DX12Convert(src.access_before),
            .AccessAfter = DX12Convert(src.access_after),
            .LayoutBefore = layout_before,
            .LayoutAfter = layout_after,
            .pResource = std::bit_cast<ID3D12Resource*>(src.texture),
            .Subresources =
                {
                    .IndexOrFirstMipLevel = src.subresource_range.base_mip_level,
                    .NumMipLevels = src.subresource_range.mip_level_count,
                    .FirstArraySlice = src.subresource_range.base_array_layer,
                    .NumArraySlices = src.subresource_range.array_layer_count,
                    .FirstPlane = src.flags & WisBarrierFlagsPlanarImage ? src.subresource_range.plane_slice : 0u,
                    .NumPlanes = src.flags & WisBarrierFlagsPlanarImage ? src.subresource_range.plane_slice_count : 1u,
                },
            .Flags = src.state_before == WisTextureStateUndefined ? D3D12_TEXTURE_BARRIER_FLAG_DISCARD
                                                                  : D3D12_TEXTURE_BARRIER_FLAG_NONE,
        };
    }

    wis::span<D3D12_GLOBAL_BARRIER> global_barriers_span{
        reinterpret_cast<D3D12_GLOBAL_BARRIER*>(global_span.data()),
        barriers->global_barrier_count
    };
    for (size_t i = 0; i < barriers->global_barrier_count; ++i) {
        auto& src = barriers->global_barriers[i];
        global_barriers_span[i] = D3D12_GLOBAL_BARRIER{
            .SyncBefore = DX12Convert(src.sync_before),
            .SyncAfter = DX12Convert(src.sync_after),
            .AccessBefore = DX12Convert(src.access_before),
            .AccessAfter = DX12Convert(src.access_after),
        };
    }

    D3D12_BARRIER_GROUP groups[]{
        {.Type = D3D12_BARRIER_TYPE_BUFFER,
         .NumBarriers = real_buffer_barrier_count,
         .pBufferBarriers = buffer_barriers_span.data()},
        {.Type = D3D12_BARRIER_TYPE_TEXTURE,
         .NumBarriers = static_cast<uint32_t>(barriers->texture_barrier_count),
         .pTextureBarriers = texture_barriers_span.data()},
        {.Type = D3D12_BARRIER_TYPE_GLOBAL,
         .NumBarriers = static_cast<uint32_t>(barriers->global_barrier_count),
         .pGlobalBarriers = global_barriers_span.data()}
    };
    list->Barrier(std::size(groups), groups);
}
} // namespace wis::detail

#endif // WIS_DX12_DETAIL_HPP

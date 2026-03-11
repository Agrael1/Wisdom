#ifndef WIS_DX12_TYPES_HPP
#define WIS_DX12_TYPES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <dxgi1_6.h>
#include <d3d12.h>
#include <D3D12MemAlloc.h>
#include <wisdom/dx12/dx12_extensions.hpp>
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/util/com_ptr.hpp>

namespace wis {
//-----------------------------------------------------------------------------
constexpr inline wis::Result convert_result(WisResult result) noexcept
{
    return { static_cast<wis::Status>(result.status), result.platform_code, result.error };
}

//-----------------------------------------------------------------------------
namespace detail {
struct DX12DebugLayer;
struct DX12ShaderHeader;
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
    ID3D12Device10*     device;
    IDXGIAdapter4*      physical_device;
    IDXGIFactory6*      factory;
    D3D12MA::Allocator* allocator;

    uint8_t queue_priorities[WisCommandQueueTypeCount]; // store priorities for queues to be created with the device, indexed by WisCommandQueueType
};

struct DX12CommandQueueImpl {
    ID3D12CommandQueue* queue;
};

struct DX12CommandAllocatorImpl {
    ID3D12CommandAllocator* allocator;
    ID3D12Device10*         device;
    WisCommandQueueType     type;
};

struct DX12CommandListImpl {
    ID3D12GraphicsCommandList7*         list;
    ID3D12CommandAllocator*             allocator;
    mutable D3D12_GPU_DESCRIPTOR_HANDLE descriptor_handle;
    mutable D3D12_GPU_DESCRIPTOR_HANDLE sampler_handle;
    uint16_t                            descriptor_size;
    uint16_t                            sampler_size;
    WisCommandQueueType                 queue_type;
    mutable uint32_t                    scratch_memory_size;
    mutable uint8_t*                    scratch_memory;
};

struct DX12FenceImpl {
    ID3D12Fence* fence;
    HANDLE       event;
};

struct DX12ResourceAllocatorImpl {
    D3D12MA::Allocator* allocator;
};

struct DX12RootSignatureImpl {
    ID3D12RootSignature* root_signature;
};

struct DX12DescriptorHeapImpl {
    ID3D12DescriptorHeap*       descriptor_heap;
    ID3D12Device10*             device;
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle; // store GPU handle for heap start to avoid calling GetGPUDescriptorHandleForHeapStart every time we need it
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
    uint32_t                    descriptor_size; // store descriptor size for heap type to avoid calling GetDescriptorHandleIncrementSize every time we need it
    D3D12_DESCRIPTOR_HEAP_TYPE  type;
};

struct DX12BufferImpl {
    ID3D12Resource*      resource;
    D3D12MA::Allocation* allocation; // Pointer to the allocation object that manages the memory for this resource
    D3D12MA::Allocator*  allocator; // Pretty dumb, but we need this to correctly ensure release order of allocator
};

using DX12TextureImpl = DX12BufferImpl; // Textures are also resources, so we can reuse the same implementation

struct DX12PipelineCacheImpl {
    ID3D12PipelineLibrary1* library;
};

struct DX12ShaderImpl {
    detail::DX12ShaderHeader* shader;
};

struct DX12PipelineImpl {
    ID3D12PipelineState* pipeline_state;
};

} // namespace impl
} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "dx12_impl.cpp"
#include "dx12_instance.cpp"
#include "dx12_device.cpp"
#include "dx12_fence.cpp"
#include "dx12_adapter_query.cpp"
#include "dx12_command_queue.cpp"
#include "dx12_command_list.cpp"
#include "dx12_descriptor_heap.cpp"
#include "dx12_resource_allocator.cpp"
#include "dx12_command_allocator.cpp"
#include "dx12_pipeline_cache.cpp"
#endif // WISDOM_BUILD_BINARIES
#endif // WIS_DX12_TYPES_HPP

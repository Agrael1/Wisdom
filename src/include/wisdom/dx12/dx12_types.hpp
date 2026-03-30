#ifndef WIS_DX12_TYPES_HPP
#define WIS_DX12_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/util/com_ptr.hpp>

#include <D3D12MemAlloc.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace wis
{
//-----------------------------------------------------------------------------
namespace detail
{
struct DX12DebugLayer;
struct DX12ShaderHeader;
} // namespace detail

namespace impl
{

struct DX12InstanceImpl {
    IDXGIFactory6* factory;
    wis::detail::DX12DebugLayer* debug_layer;
};

struct DX12AdapterQueryImpl {
    IDXGIAdapter4** physical_devices;
    std::size_t adapter_count;
    IDXGIFactory6* factory;
    wis::detail::DX12DebugLayer* debug_layer;
};

struct DX12DeviceImpl {
    ID3D12Device10* device;
    IDXGIAdapter4* physical_device;
    IDXGIFactory6* factory;
    D3D12MA::Allocator* allocator;

    uint8_t queue_priorities[WisCommandQueueTypeCount]; // store priorities for queues to be created with the device,
                                                        // indexed by WisCommandQueueType
};

struct DX12CommandQueueImpl {
    ID3D12CommandQueue* queue;
};

struct DX12CommandAllocatorImpl {
    ID3D12CommandAllocator* allocator;
    ID3D12Device10* device;
    WisCommandQueueType type;
};

struct DX12CommandListImpl {
    ID3D12GraphicsCommandList9* list;
    ID3D12CommandAllocator* allocator;
    mutable D3D12_GPU_DESCRIPTOR_HANDLE descriptor_handle;
    mutable D3D12_GPU_DESCRIPTOR_HANDLE sampler_handle;
    uint16_t descriptor_size;
    uint16_t sampler_size;
    WisCommandQueueType queue_type;
    mutable uint32_t scratch_memory_size;
    mutable uint8_t* scratch_memory;
};

struct DX12FenceImpl {
    ID3D12Fence* fence;
    HANDLE event;
};

struct DX12ResourceAllocatorImpl {
    D3D12MA::Allocator* allocator;
};

struct DX12RootSignatureImpl {
    ID3D12RootSignature* root_signature;
};

struct DX12DescriptorHeapImpl {
    ID3D12DescriptorHeap* descriptor_heap;
    ID3D12Device10* device;
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle; // store GPU handle for heap start to avoid calling
                                            // GetGPUDescriptorHandleForHeapStart every time we need it
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
    uint32_t descriptor_size; // store descriptor size for heap type to avoid calling GetDescriptorHandleIncrementSize
                              // every time we need it
    D3D12_DESCRIPTOR_HEAP_TYPE type;
};

struct DX12ViewHeapImpl {
    ID3D12DescriptorHeap* view_heap;
    ID3D12Device10* device;
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
    uint32_t descriptor_size; // store descriptor size for heap type to avoid calling GetDescriptorHandleIncrementSize
                              // every time we need it
    D3D12_DESCRIPTOR_HEAP_TYPE type;
};

struct DX12BufferImpl {
    ID3D12Resource* resource;
    D3D12MA::Allocation* allocation; // Pointer to the allocation object that manages the memory for this resource
    D3D12MA::Allocator* allocator;   // Pretty dumb, but we need this to correctly ensure release order of allocator
};

using DX12TextureImpl = DX12BufferImpl; // Textures are also resources, so we can reuse the same implementation

struct DX12PipelineCacheImpl {
    ID3D12PipelineLibrary1* library;
    uint8_t* data; // Pointer to the pipeline data, unfortunately D3D12 is a bit awkward in this regard and doesn't
                   // provide a way to directly use the library without copying the data out of it, so we have to store
                   // it here to ensure it lives as long as the library.
};

struct DX12ShaderImpl {
    detail::DX12ShaderHeader* shader;
};

struct DX12PipelineImpl {
    ID3D12PipelineState* pipeline_state;
};

struct DX12SurfaceImpl {
    void* surface; // Store the surface as a void pointer to avoid including Windows headers in this file, it will be
                   // cast to the appropriate type in the implementation file
    bool uwp; // Whether the surface is a UWP CoreWindow, which requires special handling when creating the swapchain
};

struct DX12SwapchainImpl {
    IDXGISwapChain4* swapchain;
    mutable uint32_t flags;
    mutable uint8_t vsync;
    mutable uint8_t backbuffer_count;
    mutable uint16_t width;
    mutable uint16_t height;
    mutable uint16_t data_format; // WisDataFormat backing it. Used for compression.
};

} // namespace impl

// Manual variants of generated structures with virtual functions
struct DX12InstanceExtensionHeader {
    WisResult (*init_fptr)(DX12InstanceExtensionHeader* self, const impl::DX12InstanceImpl& instance) noexcept;
};

// Manual variants of generated structures with virtual functions
struct DX12DeviceExtensionHeader {
    WisResult (*init_fptr)(DX12DeviceExtensionHeader* self, const impl::DX12DeviceImpl& device) noexcept;
};

} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20

#    include "dx12_adapter_query.cpp"
#    include "dx12_command_allocator.cpp"
#    include "dx12_command_list.cpp"
#    include "dx12_command_queue.cpp"
#    include "dx12_descriptor_heap.cpp"
#    include "dx12_device.cpp"
#    include "dx12_fence.cpp"
#    include "dx12_impl.cpp"
#    include "dx12_instance.cpp"
#    include "dx12_pipeline_cache.cpp"
#    include "dx12_resource_allocator.cpp"
#endif // WISDOM_HEADER_ONLY
#endif // WIS_DX12_TYPES_HPP

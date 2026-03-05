// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_VK_API_HPP
#define WISDOM_CPP_VK_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/vulkan/vk_types.hpp>

namespace wis {

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
struct VKDeviceRequirements {
    wis::span<const wis::CommandQueueDesc>   queue_descs; ///< points to an array of wis::CommandQueueDesc for which queues will be created during device creation.
    wis::span<wis::VKDeviceExtensionHeader*> extensions; ///< points to an array of extensions that are to be initialized with pointers to wis::DeviceExtensionHeader.
};

struct VKTextureDeleter {
    void operator()(WisVKTexture* handle) noexcept
    {
        ::wisVKDestroyTexture(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
class VKTexture : public wis::impl::Implements<wis::impl::VKTextureImpl, WisVKTexture, wis::VKTextureDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct VKBufferDeleter {
    void operator()(WisVKBuffer* handle) noexcept
    {
        ::wisVKDestroyBuffer(handle);
    }
};
using VKBufferView = WisVKBufferView;
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
class VKBuffer : public wis::impl::Implements<wis::impl::VKBufferImpl, WisVKBuffer, wis::VKBufferDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKBufferView GetView() const noexcept
    {
        VKBufferView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKBufferView() const noexcept
    {
        return GetView();
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
     * @return void points to the pointer, which is filled with the address of the mapped memory on success.
     *
     * */
    WIS_NODISCARD inline void* Map() const noexcept
    {
        return (::wisVKBufferMap(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Unmaps the buffer memory from CPU accessible address space.
     * @return u64 Address of the buffer on GPU.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetGPUAddress() const noexcept
    {
        return (::wisVKBufferGetGPUAddress(&_impl_storage));
    }
};

struct VKDescriptorHeapDeleter {
    void operator()(WisVKDescriptorHeap* handle) noexcept
    {
        ::wisVKDestroyDescriptorHeap(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
class VKDescriptorHeap : public wis::impl::Implements<wis::impl::VKDescriptorHeapImpl, WisVKDescriptorHeap, wis::VKDescriptorHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the descriptor heap.
     * @return void CPU descriptor handle for the descriptor heap.
     *
     * */
    WIS_NODISCARD inline void* GetCPUHandle() const noexcept
    {
        return (::wisVKDescriptorHeapGetCPUHandle(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::ConstantBuffer` descriptor to the descriptor heap.
     * @param data points to wis::ConstantBufferBinding, which describes the constant buffer descriptors to write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteConstantBuffer(const wis::ConstantBufferBinding& data,
                                           std::uint32_t                     index) const noexcept
    {
        return convert_result(::wisVKDescriptorHeapWriteConstantBuffer(&_impl_storage,
                                                                       reinterpret_cast<const WisConstantBufferBinding*>(&data),
                                                                       index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::Buffer` descriptor to the descriptor heap.
     * @param buffer points to wis::Buffer to write the descriptor for.
     * @param data points to wis::BufferBinding, which describes the shader resource view descriptors to write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteStructuredBuffer(wis::VKBufferView         buffer,
                                             const wis::BufferBinding& data,
                                             std::uint32_t             index) const noexcept
    {
        return convert_result(::wisVKDescriptorHeapWriteStructuredBuffer(&_impl_storage,
                                                                         buffer,
                                                                         reinterpret_cast<const WisBufferBinding*>(&data),
                                                                         index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::RWBuffer` descriptor to the descriptor heap.
     * @param buffer points to wis::Buffer to write the descriptor for.
     * @param data points to wis::BufferBinding, which describes the shader resource view descriptors to write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteRWStructuredBuffer(wis::VKBufferView         buffer,
                                               const wis::BufferBinding& data,
                                               std::uint32_t             index) const noexcept
    {
        return convert_result(::wisVKDescriptorHeapWriteRWStructuredBuffer(&_impl_storage,
                                                                           buffer,
                                                                           reinterpret_cast<const WisBufferBinding*>(&data),
                                                                           index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::Sampler` descriptor to the descriptor heap.
     * @param sampler points to wis::SamplerDesc, which describes the sampler descriptor to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteSampler(const wis::SamplerDesc& sampler,
                                    std::uint32_t           index) const noexcept
    {
        return convert_result(::wisVKDescriptorHeapWriteSampler(&_impl_storage,
                                                                reinterpret_cast<const WisSamplerDesc*>(&sampler),
                                                                index));
    }
};

struct VKRootSignatureDeleter {
    void operator()(WisVKRootSignature* handle) noexcept
    {
        ::wisVKDestroyRootSignature(handle);
    }
};
using VKRootSignatureView = WisVKRootSignatureView;
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout and a constant data storage, which defines resource bindings for shaders.
 *
 * */
class VKRootSignature : public wis::impl::Implements<wis::impl::VKRootSignatureImpl, WisVKRootSignature, wis::VKRootSignatureDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKRootSignatureView GetView() const noexcept
    {
        VKRootSignatureView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKRootSignatureView() const noexcept
    {
        return GetView();
    }
};

struct VKResourceAllocatorDeleter {
    void operator()(WisVKResourceAllocator* handle) noexcept
    {
        ::wisVKDestroyResourceAllocator(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
class VKResourceAllocator : public wis::impl::Implements<wis::impl::VKResourceAllocatorImpl, WisVKResourceAllocator, wis::VKResourceAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
     * @param desc points to wis::BufferDesc, which describes the buffer to create.
     * @param out_result denoting the outcome of operation.
     * @return buffer points to wis::Buffer, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKBuffer CreateBuffer(const wis::BufferDesc& desc,
                                                    wis::Result&           out_result) const noexcept
    {
        wis::VKBuffer buffer;
        out_result = convert_result(::wisVKResourceAllocatorCreateBuffer(&_impl_storage,
                                                                         reinterpret_cast<const WisBufferDesc*>(&desc),
                                                                         buffer.GetStorage()));
        return buffer;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
     * @param desc points to wis::TextureDesc, which describes the texture to create.
     * @param out_result denoting the outcome of operation.
     * @return texture points to wis::Texture, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKTexture CreateTexture(const wis::TextureDesc& desc,
                                                      wis::Result&            out_result) const noexcept
    {
        wis::VKTexture texture;
        out_result = convert_result(::wisVKResourceAllocatorCreateTexture(&_impl_storage,
                                                                          reinterpret_cast<const WisTextureDesc*>(&desc),
                                                                          texture.GetStorage()));
        return texture;
    }
};

struct VKFenceDeleter {
    void operator()(WisVKFence* handle) noexcept
    {
        ::wisVKDestroyFence(handle);
    }
};
using VKFenceView = WisVKFenceView;
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
class VKFence : public wis::impl::Implements<wis::impl::VKFenceImpl, WisVKFence, wis::VKFenceDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKFenceView GetView() const noexcept
    {
        VKFenceView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKFenceView() const noexcept
    {
        return GetView();
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
     * @return u64 Value of the fence.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCompletedValue() const noexcept
    {
        return (::wisVKFenceGetCompletedValue(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
     * @param value Value to wait for.
     * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Wait(std::uint64_t value,
                            std::uint64_t wait_ns) const noexcept
    {
        return convert_result(::wisVKFenceWait(&_impl_storage,
                                               value,
                                               wait_ns));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
     * @param value Value to signal.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Signal(std::uint64_t value) const noexcept
    {
        return convert_result(::wisVKFenceSignal(&_impl_storage,
                                                 value));
    }
};

struct VKCommandListDeleter {
    void operator()(WisVKCommandList* handle) noexcept
    {
        ::wisVKDestroyCommandList(handle);
    }
};
using VKCommandListView = WisVKCommandListView;
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
class VKCommandList : public wis::impl::Implements<wis::impl::VKCommandListImpl, WisVKCommandList, wis::VKCommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKCommandListView GetView() const noexcept
    {
        VKCommandListView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKCommandListView() const noexcept
    {
        return GetView();
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Begin() const noexcept
    {
        return convert_result(::wisVKCommandListBegin(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result End() const noexcept
    {
        return convert_result(::wisVKCommandListEnd(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Binds descriptor heaps to the command list, so they can be used for resource binding.
     * @param resource_heap points to wis::DescriptorHeap with shader resource views, unordered access views and constant buffer views. If `nullptr`, no resource heap is bound.
     * @param sampler_heap points to wis::DescriptorHeap with samplers. If `nullptr`, no sampler heap is bound.
     *
     * */
    inline void SetDescriptorHeaps(const wis::VKDescriptorHeap* resource_heap,
                                   const wis::VKDescriptorHeap* sampler_heap) const noexcept
    {
        ::wisVKCommandListSetDescriptorHeaps(&_impl_storage,
                                             reinterpret_cast<const WisVKDescriptorHeap*>(resource_heap),
                                             reinterpret_cast<const WisVKDescriptorHeap*>(sampler_heap));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the root signature for the command list, so it can be used for resource binding.
     * @param signature points to wis::RootSignature to set.
     * @param pipeline defines the pipeline type to set the root signature for.
     *
     * */
    inline void SetRootSignature(wis::VKRootSignatureView signature,
                                 wis::PipelineType        pipeline) const noexcept
    {
        ::wisVKCommandListSetRootSignature(&_impl_storage,
                                           signature,
                                           static_cast<WisPipelineType>(pipeline));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the push constants for the command list, so they can be used for resource binding.
     * @param data points to , which describes the push constant data to set.
     *
     * */
    inline void SetPushConstants(const wis::PushConstantDataDesc& data) const noexcept
    {
        ::wisVKCommandListSetPushConstants(&_impl_storage,
                                           reinterpret_cast<const WisPushConstantDataDesc*>(&data));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the push descriptors for the command list, so they can be used for resource binding.
     * @param data points to , which describes the push descriptors to set.
     *
     * */
    inline void SetPushDescriptor(const wis::PushDescriptorDataDesc& data) const noexcept
    {
        ::wisVKCommandListSetPushDescriptor(&_impl_storage,
                                            reinterpret_cast<const WisPushDescriptorDataDesc*>(&data));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the descriptor table offset in descriptor heap for the command list, so it can be used for resource binding.
     * @param data defines the root parameter index to set the descriptor table for.
     *
     * */
    inline void SetDescriptorTable(const wis::DescriptorTableDataDesc& data) const noexcept
    {
        ::wisVKCommandListSetDescriptorTable(&_impl_storage,
                                             reinterpret_cast<const WisDescriptorTableDataDesc*>(&data));
    }
};

struct VKCommandAllocatorDeleter {
    void operator()(WisVKCommandAllocator* handle) noexcept
    {
        ::wisVKDestroyCommandAllocator(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pool allocator for command lists for recording GPU commands.
 *
 * */
class VKCommandAllocator : public wis::impl::Implements<wis::impl::VKCommandAllocatorImpl, WisVKCommandAllocator, wis::VKCommandAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Resets the command allocator, so it can be reused for allocating new command lists.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Reset() const noexcept
    {
        return convert_result(::wisVKCommandAllocatorReset(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
     * @param out_result denoting the outcome of operation.
     * @return list points to wis::CommandList, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandList CreateCommandList(wis::Result& out_result) const noexcept
    {
        wis::VKCommandList list;
        out_result = convert_result(::wisVKCommandAllocatorCreateCommandList(&_impl_storage, list.GetStorage()));
        return list;
    }
};

struct VKCommandQueueDeleter {
    void operator()(WisVKCommandQueue* handle) noexcept
    {
        ::wisVKDestroyCommandQueue(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
class VKCommandQueue : public wis::impl::Implements<wis::impl::VKCommandQueueImpl, WisVKCommandQueue, wis::VKCommandQueueDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Executes the command lists.
     * @param lists The command lists to execute.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Submit(wis::span<const wis::VKCommandListView> lists) const noexcept
    {
        return convert_result(::wisVKCommandQueueSubmit(&_impl_storage,
                                                        reinterpret_cast<const WisVKCommandListView*>(lists.data()),
                                                        lists.size()));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been done.
     * @param fence The fence to signal.
     * @param value The value to signal the fence with.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result SignalFence(wis::VKFenceView fence,
                                   std::uint64_t    value) const noexcept
    {
        return convert_result(::wisVKCommandQueueSignalFence(&_impl_storage,
                                                             fence,
                                                             value));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
     * Can still be enqueued after the signal.
     * @param fence The fence to wait on.
     * @param value The value to wait the fence to reach.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WaitFence(wis::VKFenceView fence,
                                 std::uint64_t    value) const noexcept
    {
        return convert_result(::wisVKCommandQueueWaitFence(&_impl_storage,
                                                           fence,
                                                           value));
    }
};

struct VKDeviceDeleter {
    void operator()(WisVKDevice* handle) noexcept
    {
        ::wisVKDestroyDevice(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
class VKDevice : public wis::impl::Implements<wis::impl::VKDeviceImpl, WisVKDevice, wis::VKDeviceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
     * @param type defines the type of the command queue to create.
     * @param out_result denoting the outcome of operation.
     * @return queue points to wis::CommandQueue, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandQueue CreateCommandQueue(wis::CommandQueueType type,
                                                                wis::Result&          out_result) const noexcept
    {
        wis::VKCommandQueue queue;
        out_result = convert_result(::wisVKDeviceCreateCommandQueue(&_impl_storage,
                                                                    static_cast<WisCommandQueueType>(type),
                                                                    queue.GetStorage()));
        return queue;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command allocator to allocate command lists with.
     * @param type defines the type of the command list this pool is able to allocate.
     * @param out_result denoting the outcome of operation.
     * @return allocator points to wis::CommandAllocator, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandAllocator CreateCommandAllocator(wis::CommandQueueType type,
                                                                        wis::Result&          out_result) const noexcept
    {
        wis::VKCommandAllocator allocator;
        out_result = convert_result(::wisVKDeviceCreateCommandAllocator(&_impl_storage,
                                                                        static_cast<WisCommandQueueType>(type),
                                                                        allocator.GetStorage()));
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
     * @param initial_value defines the initial value of the fence.
     * @param out_result denoting the outcome of operation.
     * @return fence points to wis::Fence, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKFence CreateFence(std::uint64_t initial_value,
                                                  wis::Result&  out_result) const noexcept
    {
        wis::VKFence fence;
        out_result = convert_result(::wisVKDeviceCreateFence(&_impl_storage,
                                                             initial_value,
                                                             fence.GetStorage()));
        return fence;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
     * @param out_result denoting the outcome of operation.
     * @return allocator points to wis::ResourceAllocator, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKResourceAllocator GetResourceAllocator(wis::Result& out_result) const noexcept
    {
        wis::VKResourceAllocator allocator;
        out_result = convert_result(::wisVKDeviceGetResourceAllocator(&_impl_storage, allocator.GetStorage()));
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
     * @param desc points to wis::RootSignatureDesc, which describes the pipeline layout to create.
     * @param out_result denoting the outcome of operation.
     * @return layout points to wis::RootSignature, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKRootSignature CreateRootSignature(const wis::RootSignatureDesc& desc,
                                                                  wis::Result&                  out_result) const noexcept
    {
        wis::VKRootSignature layout;
        out_result = convert_result(::wisVKDeviceCreateRootSignature(&_impl_storage,
                                                                     reinterpret_cast<const WisRootSignatureDesc*>(&desc),
                                                                     layout.GetStorage()));
        return layout;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given descriptor.
     * @param desc points to wis::DescriptorHeapDesc, which describes the descriptor heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::DescriptorHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKDescriptorHeap CreateDescriptorHeap(const wis::DescriptorHeapDesc& desc,
                                                                    wis::Result&                   out_result) const noexcept
    {
        wis::VKDescriptorHeap heap;
        out_result = convert_result(::wisVKDeviceCreateDescriptorHeap(&_impl_storage,
                                                                      reinterpret_cast<const WisDescriptorHeapDesc*>(&desc),
                                                                      heap.GetStorage()));
        return heap;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
     * @param properties points to one of the query structs, which is filled with device properties.
     *
     * */
    inline void QueryProperties(void* properties) const noexcept
    {
        ::wisVKDeviceQueryProperties(&_impl_storage,
                                     properties);
    }
};

struct VKAdapterQueryDeleter {
    void operator()(WisVKAdapterQuery* handle) noexcept
    {
        ::wisVKDestroyAdapterQuery(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
class VKAdapterQuery : public wis::impl::Implements<wis::impl::VKAdapterQueryImpl, WisVKAdapterQuery, wis::VKAdapterQueryDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
     * @return size is a number of adapters present on the system.
     *
     * */
    WIS_NODISCARD inline std::size_t GetAdapterCount() const noexcept
    {
        return (::wisVKAdapterQueryGetAdapterCount(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
     * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wis::GetAdapterCount.
     * @param out_result denoting the outcome of operation.
     * @return desc points to wis::AdapterDesc, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::AdapterDesc GetAdapterDesc(std::size_t  index,
                                                         wis::Result& out_result) const noexcept
    {
        wis::AdapterDesc desc;
        out_result = convert_result(::wisVKAdapterQueryGetAdapterDesc(&_impl_storage,
                                                                      index,
                                                                      reinterpret_cast<WisAdapterDesc*>(&desc)));
        return desc;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
     * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wis::GetAdapterCount.
     * @param requirements points to wis::DeviceRequirements, which defines required features and properties for device creation.
     * @param out_result denoting the outcome of operation.
     * @return device points to wis::Device, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKDevice CreateDevice(std::size_t                      index,
                                                    const wis::VKDeviceRequirements& requirements,
                                                    wis::Result&                     out_result) const noexcept
    {
        wis::VKDevice device;
        out_result = convert_result(::wisVKAdapterQueryCreateDevice(&_impl_storage,
                                                                    index,
                                                                    reinterpret_cast<const WisVKDeviceRequirements*>(&requirements),
                                                                    device.GetStorage()));
        return device;
    }
};

struct VKInstanceDeleter {
    void operator()(WisVKInstance* handle) noexcept
    {
        ::wisVKDestroyInstance(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
class VKInstance : public wis::impl::Implements<wis::impl::VKInstanceImpl, WisVKInstance, wis::VKInstanceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software; Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete, Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
     * @param preference defines the order in which adapters are listed.
     * @param out_result denoting the outcome of operation.
     * @return query points to wis::AdapterQuery, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKAdapterQuery QueryAdapters(wis::AdapterPreference preference,
                                                           wis::Result&           out_result) const noexcept
    {
        wis::VKAdapterQuery query;
        out_result = convert_result(::wisVKInstanceQueryAdapters(&_impl_storage,
                                                                 static_cast<WisAdapterPreference>(preference),
                                                                 query.GetStorage()));
        return query;
    }
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc points to wis::DebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::VKInstance VKCreateInstance(const wis::DebugDesc*                      debug_desc,
                                                      wis::span<wis::VKInstanceExtensionHeader*> extensions,
                                                      wis::Result&                               out_result) noexcept
{
    wis::VKInstance instance;
    out_result = convert_result(::wisVKCreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
                                                      reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
                                                      extensions.size(),
                                                      instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // WISDOM_CPP_VK_API_HPP

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
};

struct VKPipelineLayoutDeleter {
    void operator()(WisVKPipelineLayout* handle) noexcept
    {
        ::wisVKDestroyPipelineLayout(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout, which defines resource bindings for shaders.
 *
 * */
class VKPipelineLayout : public wis::impl::Implements<wis::impl::VKPipelineLayoutImpl, WisVKPipelineLayout, wis::VKPipelineLayoutDeleter>
{
public:
    using ImplType::ImplType;

public:
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
};

struct VKFenceDeleter {
    void operator()(WisVKFence* handle) noexcept
    {
        ::wisVKDestroyFence(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
class VKFence : public wis::impl::Implements<wis::impl::VKFenceImpl, WisVKFence, wis::VKFenceDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct VKCommandListDeleter {
    void operator()(WisVKCommandList* handle) noexcept
    {
        ::wisVKDestroyCommandList(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
class VKCommandList : public wis::impl::Implements<wis::impl::VKCommandListImpl, WisVKCommandList, wis::VKCommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
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
     * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
     * @param type defines the type of the command list to create.
     * @param out_result denoting the outcome of operation.
     * @return list points to wis::CommandList, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandList CreateCommandList(wis::CommandQueueType type,
                                                              wis::Result&          out_result) const noexcept
    {
        wis::VKCommandList list;
        out_result = convert_result(::wisVKDeviceCreateCommandList(&_impl_storage,
                                                                   static_cast<WisCommandQueueType>(type),
                                                                   list.GetStorage()));
        return list;
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
    WIS_NODISCARD inline wis::VKResourceAllocator CreateResourceAllocator(wis::Result& out_result) const noexcept
    {
        wis::VKResourceAllocator allocator;
        out_result = convert_result(::wisVKDeviceCreateResourceAllocator(&_impl_storage, allocator.GetStorage()));
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
     * @param desc points to wis::PipelineLayoutDesc, which describes the pipeline layout to create.
     * @param out_result denoting the outcome of operation.
     * @return layout points to wis::PipelineLayout, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipelineLayout CreatePipelineLayout(const wis::PipelineLayoutDesc& desc,
                                                                    wis::Result&                   out_result) const noexcept
    {
        wis::VKPipelineLayout layout;
        out_result = convert_result(::wisVKDeviceCreatePipelineLayout(&_impl_storage,
                                                                      reinterpret_cast<const WisPipelineLayoutDesc*>(&desc),
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

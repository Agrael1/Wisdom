// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_API_HPP
#define WISDOM_CPP_DX12_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/dx12/dx12_types.hpp>

namespace wis {

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
struct DX12DeviceRequirements {
    wis::span<const wis::CommandQueueDesc>     queue_descs; ///< points to an array of wis::CommandQueueDesc for which queues will be created during device creation.
    wis::span<wis::DX12DeviceExtensionHeader*> extensions; ///< points to an array of extensions that are to be initialized with pointers to wis::DeviceExtensionHeader.
};

struct DX12DescriptorHeapDeleter {
    void operator()(WisDX12DescriptorHeap* handle) noexcept
    {
        ::wisDX12DestroyDescriptorHeap(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
class DX12DescriptorHeap : public wis::impl::Implements<wis::impl::DX12DescriptorHeapImpl, WisDX12DescriptorHeap, wis::DX12DescriptorHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12PipelineLayoutDeleter {
    void operator()(WisDX12PipelineLayout* handle) noexcept
    {
        ::wisDX12DestroyPipelineLayout(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout, which defines resource bindings for shaders.
 *
 * */
class DX12PipelineLayout : public wis::impl::Implements<wis::impl::DX12PipelineLayoutImpl, WisDX12PipelineLayout, wis::DX12PipelineLayoutDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12ResourceAllocatorDeleter {
    void operator()(WisDX12ResourceAllocator* handle) noexcept
    {
        ::wisDX12DestroyResourceAllocator(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
class DX12ResourceAllocator : public wis::impl::Implements<wis::impl::DX12ResourceAllocatorImpl, WisDX12ResourceAllocator, wis::DX12ResourceAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12FenceDeleter {
    void operator()(WisDX12Fence* handle) noexcept
    {
        ::wisDX12DestroyFence(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
class DX12Fence : public wis::impl::Implements<wis::impl::DX12FenceImpl, WisDX12Fence, wis::DX12FenceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
     * @return u64 Value of the fence.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCompletedValue() const noexcept
    {
        return (::wisDX12FenceGetCompletedValue(&_impl_storage));
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
        return convert_result(::wisDX12FenceWait(&_impl_storage,
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
        return convert_result(::wisDX12FenceSignal(&_impl_storage,
                                                   value));
    }
};

struct DX12CommandListDeleter {
    void operator()(WisDX12CommandList* handle) noexcept
    {
        ::wisDX12DestroyCommandList(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
class DX12CommandList : public wis::impl::Implements<wis::impl::DX12CommandListImpl, WisDX12CommandList, wis::DX12CommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12CommandQueueDeleter {
    void operator()(WisDX12CommandQueue* handle) noexcept
    {
        ::wisDX12DestroyCommandQueue(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
class DX12CommandQueue : public wis::impl::Implements<wis::impl::DX12CommandQueueImpl, WisDX12CommandQueue, wis::DX12CommandQueueDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12DeviceDeleter {
    void operator()(WisDX12Device* handle) noexcept
    {
        ::wisDX12DestroyDevice(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
class DX12Device : public wis::impl::Implements<wis::impl::DX12DeviceImpl, WisDX12Device, wis::DX12DeviceDeleter>
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
    WIS_NODISCARD inline wis::DX12CommandQueue CreateCommandQueue(wis::CommandQueueType type,
                                                                  wis::Result&          out_result) const noexcept
    {
        wis::DX12CommandQueue queue;
        out_result = convert_result(::wisDX12DeviceCreateCommandQueue(&_impl_storage,
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
    WIS_NODISCARD inline wis::DX12CommandList CreateCommandList(wis::CommandQueueType type,
                                                                wis::Result&          out_result) const noexcept
    {
        wis::DX12CommandList list;
        out_result = convert_result(::wisDX12DeviceCreateCommandList(&_impl_storage,
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
    WIS_NODISCARD inline wis::DX12Fence CreateFence(std::uint64_t initial_value,
                                                    wis::Result&  out_result) const noexcept
    {
        wis::DX12Fence fence;
        out_result = convert_result(::wisDX12DeviceCreateFence(&_impl_storage,
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
    WIS_NODISCARD inline wis::DX12ResourceAllocator CreateResourceAllocator(wis::Result& out_result) const noexcept
    {
        wis::DX12ResourceAllocator allocator;
        out_result = convert_result(::wisDX12DeviceCreateResourceAllocator(&_impl_storage, allocator.GetStorage()));
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
     * @param desc points to wis::PipelineLayoutDesc, which describes the pipeline layout to create.
     * @param out_result denoting the outcome of operation.
     * @return layout points to wis::PipelineLayout, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12PipelineLayout CreatePipelineLayout(const wis::PipelineLayoutDesc& desc,
                                                                      wis::Result&                   out_result) const noexcept
    {
        wis::DX12PipelineLayout layout;
        out_result = convert_result(::wisDX12DeviceCreatePipelineLayout(&_impl_storage,
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
    WIS_NODISCARD inline wis::DX12DescriptorHeap CreateDescriptorHeap(const wis::DescriptorHeapDesc& desc,
                                                                      wis::Result&                   out_result) const noexcept
    {
        wis::DX12DescriptorHeap heap;
        out_result = convert_result(::wisDX12DeviceCreateDescriptorHeap(&_impl_storage,
                                                                        reinterpret_cast<const WisDescriptorHeapDesc*>(&desc),
                                                                        heap.GetStorage()));
        return heap;
    }
};

struct DX12AdapterQueryDeleter {
    void operator()(WisDX12AdapterQuery* handle) noexcept
    {
        ::wisDX12DestroyAdapterQuery(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
class DX12AdapterQuery : public wis::impl::Implements<wis::impl::DX12AdapterQueryImpl, WisDX12AdapterQuery, wis::DX12AdapterQueryDeleter>
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
        return (::wisDX12AdapterQueryGetAdapterCount(&_impl_storage));
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
        out_result = convert_result(::wisDX12AdapterQueryGetAdapterDesc(&_impl_storage,
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
    WIS_NODISCARD inline wis::DX12Device CreateDevice(std::size_t                        index,
                                                      const wis::DX12DeviceRequirements& requirements,
                                                      wis::Result&                       out_result) const noexcept
    {
        wis::DX12Device device;
        out_result = convert_result(::wisDX12AdapterQueryCreateDevice(&_impl_storage,
                                                                      index,
                                                                      reinterpret_cast<const WisDX12DeviceRequirements*>(&requirements),
                                                                      device.GetStorage()));
        return device;
    }
};

struct DX12InstanceDeleter {
    void operator()(WisDX12Instance* handle) noexcept
    {
        ::wisDX12DestroyInstance(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
class DX12Instance : public wis::impl::Implements<wis::impl::DX12InstanceImpl, WisDX12Instance, wis::DX12InstanceDeleter>
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
    WIS_NODISCARD inline wis::DX12AdapterQuery QueryAdapters(wis::AdapterPreference preference,
                                                             wis::Result&           out_result) const noexcept
    {
        wis::DX12AdapterQuery query;
        out_result = convert_result(::wisDX12InstanceQueryAdapters(&_impl_storage,
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
WIS_NODISCARD inline wis::DX12Instance DX12CreateInstance(const wis::DebugDesc*                        debug_desc,
                                                          wis::span<wis::DX12InstanceExtensionHeader*> extensions,
                                                          wis::Result&                                 out_result) noexcept
{
    wis::DX12Instance instance;
    out_result = convert_result(::wisDX12CreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
                                                        reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
                                                        extensions.size(),
                                                        instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // WISDOM_CPP_DX12_API_HPP

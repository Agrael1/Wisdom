// This file is generated. Do not edit directly.
#ifndef WISDOM_C_DX12_API_H
#define WISDOM_C_DX12_API_H
#include <wisdom/generated/c_api.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Texture, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Buffer, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12DescriptorHeap, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout, which defines resource bindings for shaders.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12PipelineLayout, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12ResourceAllocator, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Fence, 2);
WIS_DEFINE_HANDLE_VIEW(WisDX12Fence, 1);

static inline WisDX12FenceView wisGetDX12FenceView(const WisDX12Fence* handle)
{
    WisDX12FenceView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12CommandList, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12CommandList, 1);

static inline WisDX12CommandListView wisGetDX12CommandListView(const WisDX12CommandList* handle)
{
    WisDX12CommandListView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pool allocator for command lists for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12CommandAllocator, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12CommandQueue, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Device, 5);

/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12AdapterQuery, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Instance, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing device extension header. Used in extension development.
 *
 * */
typedef struct WisDX12DeviceExtensionHeader {
    const void* opaque; ///< defines opaque member. In @wis_mustnot be changed directly outside extension development.
} WisDX12DeviceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing instance extension header. Used in extension development.
 *
 * */
typedef struct WisDX12InstanceExtensionHeader {
    const void* opaque; ///< defines opaque member. It @wis_mustnot be changed directly outside extension development.
} WisDX12InstanceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
typedef struct WisDX12DeviceRequirements {
    const WisCommandQueueDesc*     queue_descs; ///< points to an array of WisCommandQueueDesc for which queues will be created during device creation.
    size_t                         queue_desc_count; ///< counts the number of queue descriptions in the wisAdapterQueryCreateDevice array.
    WisDX12DeviceExtensionHeader** extensions; ///< points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
    size_t                         extension_count; ///< counts the number of extensions in the wisAdapterQueryCreateDevice array.
} WisDX12DeviceRequirements;

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisTexture handle.
 * @param self is a pointer to the valid WisTexture instance.
 *
 * */
WISDOM_API void wisDX12DestroyTexture(WisDX12Texture* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisBuffer handle.
 * @param self is a pointer to the valid WisBuffer instance.
 *
 * */
WISDOM_API void wisDX12DestroyBuffer(WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDescriptorHeap handle.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 *
 * */
WISDOM_API void wisDX12DestroyDescriptorHeap(WisDX12DescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipelineLayout handle.
 * @param self is a pointer to the valid WisPipelineLayout instance.
 *
 * */
WISDOM_API void wisDX12DestroyPipelineLayout(WisDX12PipelineLayout* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisResourceAllocator handle.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 *
 * */
WISDOM_API void wisDX12DestroyResourceAllocator(WisDX12ResourceAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisFence handle.
 * @param self is a pointer to the valid WisFence instance.
 *
 * */
WISDOM_API void wisDX12DestroyFence(WisDX12Fence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandList handle.
 * @param self is a pointer to the valid WisCommandList instance.
 *
 * */
WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandAllocator handle.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 *
 * */
WISDOM_API void wisDX12DestroyCommandAllocator(WisDX12CommandAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandQueue handle.
 * @param self is a pointer to the valid WisCommandQueue instance.
 *
 * */
WISDOM_API void wisDX12DestroyCommandQueue(WisDX12CommandQueue* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDevice handle.
 * @param self is a pointer to the valid WisDevice instance.
 *
 * */
WISDOM_API void wisDX12DestroyDevice(WisDX12Device* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisAdapterQuery handle.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 *
 * */
WISDOM_API void wisDX12DestroyAdapterQuery(WisDX12AdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisInstance handle.
 * @param self is a pointer to the valid WisInstance instance.
 *
 * */
WISDOM_API void wisDX12DestroyInstance(WisDX12Instance* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the WisInstance with extensions, specified in extension array.
 * @param debug_desc points to WisDebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to WisInstanceExtensionHeader.
 * @param extension_count counts the number of extensions in the `extensions` array.
 * @param instance points to WisInstance, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CreateInstance(const WisDebugDesc*              debug_desc,
                                           WisDX12InstanceExtensionHeader** extensions,
                                           size_t                           extension_count,
                                           WisDX12Instance*                 instance);

/**
 * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software; Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete, Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
 * @param self is a pointer to the valid WisInstance instance.
 * @param preference defines the order in which adapters are listed.
 * @param query points to WisAdapterQuery, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12InstanceQueryAdapters(const WisDX12Instance* self,
                                                  WisAdapterPreference   preference,
                                                  WisDX12AdapterQuery*   query);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @return size is a number of adapters present on the system.
 *
 * */
WISDOM_API size_t wisDX12AdapterQueryGetAdapterCount(const WisDX12AdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param desc points to WisAdapterDesc, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12AdapterQueryGetAdapterDesc(const WisDX12AdapterQuery* self,
                                                       size_t                     index,
                                                       WisAdapterDesc*            desc);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param requirements points to WisDeviceRequirements, which defines required features and properties for device creation.
 * @param device points to WisDevice, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12AdapterQueryCreateDevice(const WisDX12AdapterQuery*       self,
                                                     size_t                           index,
                                                     const WisDX12DeviceRequirements* requirements,
                                                     WisDX12Device*                   device);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command queue to create.
 * @param queue points to WisCommandQueue, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateCommandQueue(const WisDX12Device* self,
                                                     WisCommandQueueType  type,
                                                     WisDX12CommandQueue* queue);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command allocator to allocate command lists with.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command list this pool is able to allocate.
 * @param allocator points to WisCommandAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateCommandAllocator(const WisDX12Device*     self,
                                                         WisCommandQueueType      type,
                                                         WisDX12CommandAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_value defines the initial value of the fence.
 * @param fence points to WisFence, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateFence(const WisDX12Device* self,
                                              uint64_t             initial_value,
                                              WisDX12Fence*        fence);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
 * @param self is a pointer to the valid WisDevice instance.
 * @param allocator points to WisResourceAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceGetResourceAllocator(const WisDX12Device*      self,
                                                       WisDX12ResourceAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisPipelineLayoutDesc, which describes the pipeline layout to create.
 * @param layout points to WisPipelineLayout, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreatePipelineLayout(const WisDX12Device*         self,
                                                       const WisPipelineLayoutDesc* desc,
                                                       WisDX12PipelineLayout*       layout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisDescriptorHeapDesc, which describes the descriptor heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateDescriptorHeap(const WisDX12Device*         self,
                                                       const WisDescriptorHeapDesc* desc,
                                                       WisDX12DescriptorHeap*       heap);

/**
 * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
 * @param self is a pointer to the valid WisDevice instance.
 * @param properties points to one of the query structs, which is filled with device properties.
 *
 * */
WISDOM_API void wisDX12DeviceQueryProperties(const WisDX12Device* self,
                                             void*                properties);

/**
 * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
 * @param self is a pointer to the valid WisFence instance.
 * @return u64 Value of the fence.
 *
 * */
WISDOM_API uint64_t wisDX12FenceGetCompletedValue(const WisDX12Fence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to wait for.
 * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12FenceWait(const WisDX12Fence* self,
                                      uint64_t            value,
                                      uint64_t            wait_ns);

/**
 * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to signal.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12FenceSignal(const WisDX12Fence* self,
                                        uint64_t            value);

/**
 * @brief Provided by Wisdom 0.7.0. Executes the command lists.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param lists The command lists to execute.
 * @param count The number of command lists to execute.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandQueueSubmit(const WisDX12CommandQueue*    self,
                                               const WisDX12CommandListView* lists,
                                               size_t                        count);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been done.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to signal.
 * @param value The value to signal the fence with.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandQueueSignalFence(const WisDX12CommandQueue* self,
                                                    WisDX12FenceView           fence,
                                                    uint64_t                   value);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
 * Can still be enqueued after the signal.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to wait on.
 * @param value The value to wait the fence to reach.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandQueueWaitFence(const WisDX12CommandQueue* self,
                                                  WisDX12FenceView           fence,
                                                  uint64_t                   value);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisBufferDesc, which describes the buffer to create.
 * @param buffer points to WisBuffer, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12ResourceAllocatorCreateBuffer(const WisDX12ResourceAllocator* self,
                                                          const WisBufferDesc*            desc,
                                                          WisDX12Buffer*                  buffer);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisTextureDesc, which describes the texture to create.
 * @param texture points to WisTexture, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12ResourceAllocatorCreateTexture(const WisDX12ResourceAllocator* self,
                                                           const WisTextureDesc*           desc,
                                                           WisDX12Texture*                 texture);

/**
 * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return void points to the pointer, which is filled with the address of the mapped memory on success.
 *
 * */
WISDOM_API void* wisDX12BufferMap(const WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Resets the command allocator, so it can be reused for allocating new command lists.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandAllocatorReset(const WisDX12CommandAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 * @param list points to WisCommandList, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandAllocatorCreateCommandList(const WisDX12CommandAllocator* self,
                                                              WisDX12CommandList*            list);

/**
 * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandListClose(const WisDX12CommandList* self);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_DX12_API_H

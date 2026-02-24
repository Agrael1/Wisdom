// This file is generated. Do not edit directly.
#ifndef WISDOM_C_VK_API_H
#define WISDOM_C_VK_API_H
#include <wisdom/generated/c_api.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
WIS_DEFINE_HANDLE(WisVKTexture, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
WIS_DEFINE_HANDLE(WisVKBuffer, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisVKDescriptorHeap, 6);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout, which defines resource bindings for shaders.
 *
 * */
WIS_DEFINE_HANDLE(WisVKPipelineLayout, 5);

/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
WIS_DEFINE_HANDLE(WisVKResourceAllocator, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
WIS_DEFINE_HANDLE(WisVKFence, 3);
WIS_DEFINE_HANDLE_VIEW(WisVKFence, 1);

static inline WisVKFenceView wisGetVKFenceView(const WisVKFence* handle)
{
    WisVKFenceView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisVKCommandList, 4);
WIS_DEFINE_HANDLE_VIEW(WisVKCommandList, 1);

static inline WisVKCommandListView wisGetVKCommandListView(const WisVKCommandList* handle)
{
    WisVKCommandListView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
WIS_DEFINE_HANDLE(WisVKCommandQueue, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
WIS_DEFINE_HANDLE(WisVKDevice, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
WIS_DEFINE_HANDLE(WisVKAdapterQuery, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
WIS_DEFINE_HANDLE(WisVKInstance, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing device extension header. Used in extension development.
 *
 * */
typedef struct WisVKDeviceExtensionHeader {
    const void* opaque; ///< defines opaque member. In @wis_mustnot be changed directly outside extension development.
} WisVKDeviceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing instance extension header. Used in extension development.
 *
 * */
typedef struct WisVKInstanceExtensionHeader {
    const void* opaque; ///< defines opaque member. It @wis_mustnot be changed directly outside extension development.
} WisVKInstanceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
typedef struct WisVKDeviceRequirements {
    const WisCommandQueueDesc*   queue_descs; ///< points to an array of WisCommandQueueDesc for which queues will be created during device creation.
    size_t                       queue_desc_count; ///< counts the number of queue descriptions in the wisAdapterQueryCreateDevice array.
    WisVKDeviceExtensionHeader** extensions; ///< points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
    size_t                       extension_count; ///< counts the number of extensions in the wisAdapterQueryCreateDevice array.
} WisVKDeviceRequirements;

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisTexture handle.
 * @param self is a pointer to the valid WisTexture instance.
 *
 * */
WISDOM_API void wisVKDestroyTexture(WisVKTexture* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisBuffer handle.
 * @param self is a pointer to the valid WisBuffer instance.
 *
 * */
WISDOM_API void wisVKDestroyBuffer(WisVKBuffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDescriptorHeap handle.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 *
 * */
WISDOM_API void wisVKDestroyDescriptorHeap(WisVKDescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipelineLayout handle.
 * @param self is a pointer to the valid WisPipelineLayout instance.
 *
 * */
WISDOM_API void wisVKDestroyPipelineLayout(WisVKPipelineLayout* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisResourceAllocator handle.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 *
 * */
WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisFence handle.
 * @param self is a pointer to the valid WisFence instance.
 *
 * */
WISDOM_API void wisVKDestroyFence(WisVKFence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandList handle.
 * @param self is a pointer to the valid WisCommandList instance.
 *
 * */
WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandQueue handle.
 * @param self is a pointer to the valid WisCommandQueue instance.
 *
 * */
WISDOM_API void wisVKDestroyCommandQueue(WisVKCommandQueue* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDevice handle.
 * @param self is a pointer to the valid WisDevice instance.
 *
 * */
WISDOM_API void wisVKDestroyDevice(WisVKDevice* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisAdapterQuery handle.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 *
 * */
WISDOM_API void wisVKDestroyAdapterQuery(WisVKAdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisInstance handle.
 * @param self is a pointer to the valid WisInstance instance.
 *
 * */
WISDOM_API void wisVKDestroyInstance(WisVKInstance* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the WisInstance with extensions, specified in extension array.
 * @param debug_desc points to WisDebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to WisInstanceExtensionHeader.
 * @param extension_count counts the number of extensions in the `extensions` array.
 * @param instance points to WisInstance, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCreateInstance(const WisDebugDesc*            debug_desc,
                                         WisVKInstanceExtensionHeader** extensions,
                                         size_t                         extension_count,
                                         WisVKInstance*                 instance);

/**
 * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software; Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete, Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
 * @param self is a pointer to the valid WisInstance instance.
 * @param preference defines the order in which adapters are listed.
 * @param query points to WisAdapterQuery, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKInstanceQueryAdapters(const WisVKInstance* self,
                                                WisAdapterPreference preference,
                                                WisVKAdapterQuery*   query);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @return size is a number of adapters present on the system.
 *
 * */
WISDOM_API size_t wisVKAdapterQueryGetAdapterCount(const WisVKAdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param desc points to WisAdapterDesc, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKAdapterQueryGetAdapterDesc(const WisVKAdapterQuery* self,
                                                     size_t                   index,
                                                     WisAdapterDesc*          desc);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param requirements points to WisDeviceRequirements, which defines required features and properties for device creation.
 * @param device points to WisDevice, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKAdapterQueryCreateDevice(const WisVKAdapterQuery*       self,
                                                   size_t                         index,
                                                   const WisVKDeviceRequirements* requirements,
                                                   WisVKDevice*                   device);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command queue to create.
 * @param queue points to WisCommandQueue, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateCommandQueue(const WisVKDevice*  self,
                                                   WisCommandQueueType type,
                                                   WisVKCommandQueue*  queue);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command list to create.
 * @param list points to WisCommandList, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateCommandList(const WisVKDevice*  self,
                                                  WisCommandQueueType type,
                                                  WisVKCommandList*   list);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_value defines the initial value of the fence.
 * @param fence points to WisFence, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateFence(const WisVKDevice* self,
                                            uint64_t           initial_value,
                                            WisVKFence*        fence);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
 * @param self is a pointer to the valid WisDevice instance.
 * @param allocator points to WisResourceAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceGetResourceAllocator(const WisVKDevice*      self,
                                                     WisVKResourceAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisPipelineLayoutDesc, which describes the pipeline layout to create.
 * @param layout points to WisPipelineLayout, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreatePipelineLayout(const WisVKDevice*           self,
                                                     const WisPipelineLayoutDesc* desc,
                                                     WisVKPipelineLayout*         layout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisDescriptorHeapDesc, which describes the descriptor heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateDescriptorHeap(const WisVKDevice*           self,
                                                     const WisDescriptorHeapDesc* desc,
                                                     WisVKDescriptorHeap*         heap);

/**
 * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
 * @param self is a pointer to the valid WisDevice instance.
 * @param properties points to one of the query structs, which is filled with device properties.
 *
 * */
WISDOM_API void wisVKDeviceQueryProperties(const WisVKDevice* self,
                                           void*              properties);

/**
 * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
 * @param self is a pointer to the valid WisFence instance.
 * @return u64 Value of the fence.
 *
 * */
WISDOM_API uint64_t wisVKFenceGetCompletedValue(const WisVKFence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to wait for.
 * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKFenceWait(const WisVKFence* self,
                                    uint64_t          value,
                                    uint64_t          wait_ns);

/**
 * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to signal.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKFenceSignal(const WisVKFence* self,
                                      uint64_t          value);

/**
 * @brief Provided by Wisdom 0.7.0. Executes the command lists.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param lists The command lists to execute.
 * @param count The number of command lists to execute.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandQueueSubmit(const WisVKCommandQueue*    self,
                                             const WisVKCommandListView* lists,
                                             size_t                      count);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been done.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to signal.
 * @param value The value to signal the fence with.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandQueueSignalFence(const WisVKCommandQueue* self,
                                                  WisVKFenceView           fence,
                                                  uint64_t                 value);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
 * Can still be enqueued after the signal.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to wait on.
 * @param value The value to wait the fence to reach.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandQueueWaitFence(const WisVKCommandQueue* self,
                                                WisVKFenceView           fence,
                                                uint64_t                 value);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisBufferDesc, which describes the buffer to create.
 * @param buffer points to WisBuffer, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKResourceAllocatorCreateBuffer(const WisVKResourceAllocator* self,
                                                        const WisBufferDesc*          desc,
                                                        WisVKBuffer*                  buffer);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisTextureDesc, which describes the texture to create.
 * @param texture points to WisTexture, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKResourceAllocatorCreateTexture(const WisVKResourceAllocator* self,
                                                         const WisTextureDesc*         desc,
                                                         WisVKTexture*                 texture);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_VK_API_H

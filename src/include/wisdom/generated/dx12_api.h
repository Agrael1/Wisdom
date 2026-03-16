// This file is generated. Do not edit directly.
#ifndef WISDOM_C_DX12_API_H
#define WISDOM_C_DX12_API_H
#include <wisdom/generated/c_api.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for resource views used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12ViewHeap, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU pipeline state object, which encapsulates the state of the GPU pipeline and allows to execute draw and dispatch calls with it.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Pipeline, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12Pipeline, 1);

static inline WisDX12PipelineView wisGetDX12PipelineView(const WisDX12Pipeline* handle)
{
    WisDX12PipelineView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU shader module, which contains shader code and allows to create pipeline state objects with it.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Shader, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12Shader, 1);

static inline WisDX12ShaderView wisGetDX12ShaderView(const WisDX12Shader* handle)
{
    WisDX12ShaderView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a cache for pipeline state objects, which allows to reuse already created pipelines and speed up pipeline creation.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12PipelineCache, 2);
WIS_DEFINE_HANDLE_VIEW(WisDX12PipelineCache, 1);

static inline WisDX12PipelineCacheView wisGetDX12PipelineCacheView(const WisDX12PipelineCache* handle)
{
    WisDX12PipelineCacheView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Texture, 3);
WIS_DEFINE_HANDLE_VIEW(WisDX12Texture, 1);

static inline WisDX12TextureView wisGetDX12TextureView(const WisDX12Texture* handle)
{
    WisDX12TextureView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Buffer, 3);
WIS_DEFINE_HANDLE_VIEW(WisDX12Buffer, 1);

static inline WisDX12BufferView wisGetDX12BufferView(const WisDX12Buffer* handle)
{
    WisDX12BufferView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12DescriptorHeap, 5);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout and a constant data storage, which defines resource bindings for shaders.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12RootSignature, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12RootSignature, 1);

static inline WisDX12RootSignatureView wisGetDX12RootSignatureView(const WisDX12RootSignature* handle)
{
    WisDX12RootSignatureView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

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
WIS_DEFINE_HANDLE(WisDX12CommandList, 7);
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
 * @brief Provided by Wisdom 0.7.0. Buffer barrier with the buffer handle.
 *
 * */
typedef struct WisDX12BufferBarrier {
    WisBarrierSync      sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync      sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess   access_before; ///< Access scope before the barrier.
    WisResourceAccess   access_after; ///< Access scope after the barrier.
    WisDX12BufferView   buffer; ///< Buffer view.
    uint64_t            offset; ///< Offset in bytes from the start of the buffer. Default is 0.
    uint64_t            size; ///< Barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
    WisCommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    WisCommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
} WisDX12BufferBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Global barrier without resource handles.
 *
 * */
typedef struct WisDX12GlobalBarrier {
    WisBarrierSync    sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync    sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess access_before; ///< Access scope before the barrier.
    WisResourceAccess access_after; ///< Access scope after the barrier.
} WisDX12GlobalBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Texture barrier with the texture handle.
 *
 * */
typedef struct WisDX12TextureBarrier {
    WisBarrierSync      sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync      sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess   access_before; ///< Access scope before the barrier.
    WisResourceAccess   access_after; ///< Access scope after the barrier.
    WisTextureState     state_before; ///< Texture state before the barrier.
    WisTextureState     state_after; ///< Texture state after the barrier.
    WisBarrierFlags     flags; ///< Barrier flags. Describe additional options for the barrier.
    WisDX12TextureView  texture; ///< Texture view.
    WisSubresourceRange subresource_range; ///< Subresource range for the barrier.
    WisCommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    WisCommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
} WisDX12TextureBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Barrier group for multiple barriers submission.
 *
 * */
typedef struct WisDX12BarrierGroup {
    const WisDX12BufferBarrier*  buffer_barriers; ///< Array of buffer barriers.
    size_t                       buffer_barrier_count; ///< Number of buffer barriers in the `WisBarrierGroup::buffer_barriers` array.
    const WisDX12TextureBarrier* texture_barriers; ///< Array of texture barriers.
    size_t                       texture_barrier_count; ///< Number of texture barriers in the `WisBarrierGroup::texture_barriers` array.
    const WisDX12GlobalBarrier*  global_barriers; ///< Array of global barriers.
    size_t                       global_barrier_count; ///< Number of global barriers in the `WisBarrierGroup::global_barriers` array.
} WisDX12BarrierGroup;

/**
 * @brief Provided by Wisdom 0.7.0. Compute pipeline description for WisPipeline creation.
 *
 * */
typedef struct WisDX12ComputePipelineDesc {
    WisDX12RootSignatureView root_signature; ///< Root signature description for the pipeline.
    WisDX12ShaderView        compute_shader; ///< Compute shader bytecode.
    WisDX12PipelineCacheView cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    WisPipelineFlags         flags; ///< Pipeline flags. Describe additional options for the pipeline.
} WisDX12ComputePipelineDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Graphics pipeline description for WisPipeline creation.
 *
 * */
typedef struct WisDX12GraphicsPipelineDesc {
    WisDX12RootSignatureView   root_signature; ///< Root signature description for the pipeline.
    WisDX12ShaderView          vertex_shader; ///< Vertex shader bytecode.
    WisDX12ShaderView          hull_shader; ///< Hull shader bytecode. If not set, the pipeline will be created without a hull shader.
    WisDX12ShaderView          domain_shader; ///< Domain shader bytecode. If not set, the pipeline will be created without a domain shader.
    WisDX12ShaderView          geometry_shader; ///< Geometry shader bytecode. If not set, the pipeline will be created without a geometry shader.
    WisDX12ShaderView          pixel_shader; ///< Pixel shader bytecode. If not set, the pipeline will be created without a pixel shader.
    WisRenderAttachmentsDesc   render_attachments; ///< Render attachments description for the pipeline. Used to create the compatible render pass for the pipeline.
    WisInputLayout             input_layout; ///< Input layout description for the pipeline. If not set, the pipeline will be created without an input layout.
    WisTopologyType            topology_type; ///< Topology type. Default is `WisTopologyTypeTriangle`.
    const WisRasterizerDesc*   rasterizer_desc; ///< Rasterizer description for the pipeline. If not set, the pipeline will be created with default rasterizer state.
    const WisSampleDesc*       sample_desc; ///< Sample description for the pipeline. If not set, the pipeline will be created with default sample state (no multisampling).
    const WisDepthStencilDesc* depth_stencil_desc; ///< Depth stencil description for the pipeline. If not set, the pipeline will be created with depth testing and stencil testing disabled.
    const WisBlendStateDesc*   blend_state_desc; ///< Blend state description for the pipeline. If not set, the pipeline will be created with blending disabled.
    WisDX12PipelineCacheView   cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    WisPipelineFlags           flags; ///< Pipeline flags. Describe additional options for the pipeline.
} WisDX12GraphicsPipelineDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisViewHeap handle.
 * @param self is a pointer to the valid WisViewHeap instance.
 *
 * */
WISDOM_API void wisDX12DestroyViewHeap(WisDX12ViewHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipeline handle.
 * @param self is a pointer to the valid WisPipeline instance.
 *
 * */
WISDOM_API void wisDX12DestroyPipeline(WisDX12Pipeline* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisShader handle.
 * @param self is a pointer to the valid WisShader instance.
 *
 * */
WISDOM_API void wisDX12DestroyShader(WisDX12Shader* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipelineCache handle.
 * @param self is a pointer to the valid WisPipelineCache instance.
 *
 * */
WISDOM_API void wisDX12DestroyPipelineCache(WisDX12PipelineCache* self);

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
 * @brief Provided by Wisdom 0.7.0. Destroys a WisRootSignature handle.
 * @param self is a pointer to the valid WisRootSignature instance.
 *
 * */
WISDOM_API void wisDX12DestroyRootSignature(WisDX12RootSignature* self);

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
 * @param desc points to WisRootSignatureDesc, which describes the pipeline layout to create.
 * @param layout points to WisRootSignature, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateRootSignature(const WisDX12Device*        self,
                                                      const WisRootSignatureDesc* desc,
                                                      WisDX12RootSignature*       layout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given description.
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
 * @brief Provided by Wisdom 0.7.0. Creates a view storage with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the view heap to create.
 * @param capacity defines the capacity in descriptors of the view heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateViewHeap(const WisDX12Device* self,
                                                 WisViewHeapType      type,
                                                 uint32_t             capacity,
                                                 WisDX12ViewHeap*     heap);

/**
 * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
 * @param self is a pointer to the valid WisDevice instance.
 * @param properties points to one of the query structs, which is filled with device properties.
 *
 * */
WISDOM_API void wisDX12DeviceQueryProperties(const WisDX12Device* self,
                                             void*                properties);

/**
 * @brief Provided by Wisdom 0.7.0. Waits on multiple fences simultaneously.
 * If wait_all is `WisMutiWaitTypeAll`, waits for all fences to be signaled.
 * Otherwise waits for any fence to be signaled.
 * @param self is a pointer to the valid WisDevice instance.
 * @param fences Array of fence views to wait on.
 * @param fence_values Fence values to wait fences to reach. Array @wis_must have fence_count values.
 * @param fence_count How many fences to wait on.
 * @param wait_for Specifies the kind of wait.
 * All - waits for all fences to be signaled.
 * Any - waits for any fence to be signaled.
 * Default is `WisMutiWaitTypeAll`
 * @param timeout The timeout in nanoseconds. If UINT64_MAX, waits indefinitely.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceWaitForMultipleFences(const WisDX12Device*    self,
                                                        const WisDX12FenceView* fences,
                                                        const uint64_t*         fence_values,
                                                        size_t                  fence_count,
                                                        WisMutiWaitType         wait_for,
                                                        uint64_t                timeout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline cache for caching pipeline state objects.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_data points to the initial cache data. If `nullptr`, the cache is created empty.
 * @param data_size defines the size of the initial cache data in bytes.
 * @param cache points to WisPipelineCache, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreatePipelineCache(const WisDX12Device*  self,
                                                      const uint8_t*        initial_data,
                                                      size_t                data_size,
                                                      WisDX12PipelineCache* cache);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a shader module from given data.
 * @param self is a pointer to the valid WisDevice instance.
 * @param data Shader bytecode.
 * @param size The size of the shader data in bytes. For SPIR-V @wis_must be multiple of 4.
 * @param shader points to WisShader, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateShader(const WisDX12Device* self,
                                               const uint8_t*       data,
                                               size_t               size,
                                               WisDX12Shader*       shader);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a compute pipeline state object with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisComputePipelineDesc, which describes the compute pipeline to create.
 * @param pipeline points to WisPipeline, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateComputePipeline(const WisDX12Device*              self,
                                                        const WisDX12ComputePipelineDesc* desc,
                                                        WisDX12Pipeline*                  pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a graphics pipeline state object with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisGraphicsPipelineDesc, which describes the graphics pipeline to create.
 * @param pipeline points to WisPipeline, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateGraphicsPipeline(const WisDX12Device*               self,
                                                         const WisDX12GraphicsPipelineDesc* desc,
                                                         WisDX12Pipeline*                   pipeline);

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
 * @param initial_state defines the initial state of the texture. State transition @wis_must be supported, query WisDeviceMemoryProperties to get if the transition is supported. If not, @wis_must be `WisTextureStateUndefined`.
 * @param texture points to WisTexture, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12ResourceAllocatorCreateTexture(const WisDX12ResourceAllocator* self,
                                                           const WisTextureDesc*           desc,
                                                           WisTextureState                 initial_state,
                                                           WisDX12Texture*                 texture);

/**
 * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return void points to the pointer, which is filled with the address of the mapped memory on success.
 *
 * */
WISDOM_API void* wisDX12BufferMap(const WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Unmaps the buffer memory from CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return u64 Address of the buffer on GPU.
 *
 * */
WISDOM_API uint64_t wisDX12BufferGetGPUAddress(const WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Writes data direct to the texture subresource. Texture @wis_must be in `WisTextureStateCommon` and @wis_must_not be a depth texture.
 * @param self is a pointer to the valid WisTexture instance.
 * @param source_data points to the data to write to the texture.
 * @param target_region points to WisTextureRegion, which describes the region of the texture to write to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12TextureWriteSubresource(const WisDX12Texture*   self,
                                                    const void*             source_data,
                                                    const WisTextureRegion* target_region);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @return void CPU descriptor handle for the descriptor heap.
 *
 * */
WISDOM_API void* wisDX12DescriptorHeapGetCPUHandle(const WisDX12DescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeConstantBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param data points to WisConstantBufferBinding, which describes the constant buffer descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteConstantBuffer(const WisDX12DescriptorHeap*    self,
                                                              const WisConstantBufferBinding* data,
                                                              uint32_t                        index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param buffer points to WisBuffer to write the descriptor for.
 * @param data points to WisBufferBinding, which describes the shader resource view descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteStructuredBuffer(const WisDX12DescriptorHeap* self,
                                                                WisDX12BufferView            buffer,
                                                                const WisBufferBinding*      data,
                                                                uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeRWBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param buffer points to WisBuffer to write the descriptor for.
 * @param data points to WisBufferBinding, which describes the shader resource view descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteRWStructuredBuffer(const WisDX12DescriptorHeap* self,
                                                                  WisDX12BufferView            buffer,
                                                                  const WisBufferBinding*      data,
                                                                  uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeSampler` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param sampler points to WisSamplerDesc, which describes the sampler descriptor to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteSampler(const WisDX12DescriptorHeap* self,
                                                       const WisSamplerDesc*        sampler,
                                                       uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param data points to , which describes the texture view to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteTexture(const WisDX12DescriptorHeap* self,
                                                       WisDX12TextureView           texture,
                                                       const WisTextureBinding*     data,
                                                       uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a texture view to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param data points to , which describes the texture view to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteRWTexture(const WisDX12DescriptorHeap* self,
                                                         WisDX12TextureView           texture,
                                                         const WisTextureBinding*     data,
                                                         uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a raytracing acceleration to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param address GPU address of a raytracing acceleration structure.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteAccelerationStructure(const WisDX12DescriptorHeap* self,
                                                                     uint64_t                     address,
                                                                     uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Copies descriptors from one heap to another.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param dst_index defines the index in the destination descriptor heap to copy descriptors to.
 * @param src_ptr points to WisDescriptorHeap to copy descriptors from. Source heap @wis_must be CPU Only heap.
 * @param src_index defines the index in the source descriptor heap to copy descriptors from.
 * @param count defines the number of descriptors to copy.
 *
 * */
WISDOM_API void wisDX12DescriptorHeapCopyDescriptors(const WisDX12DescriptorHeap* self,
                                                     uint32_t                     dst_index,
                                                     const void*                  src_ptr,
                                                     uint32_t                     src_index,
                                                     uint32_t                     count);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a render target view to the view heap and returns the CPU descriptor handle for it.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param render_target points to WisRenderTargetDesc, which describes the render target view to write.
 * @param index defines the index in the view heap to write the descriptor to.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapWriteRenderTarget(const WisDX12ViewHeap*     self,
                                                     WisDX12TextureView         texture,
                                                     const WisRenderTargetDesc* render_target,
                                                     uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a depth stencil view to the view heap and returns the CPU descriptor handle for it.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param render_target points to WisRenderTargetDesc, which describes the render target view to write.
 * @param index defines the index in the view heap to write the descriptor to.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapWriteDepthStencil(const WisDX12ViewHeap*     self,
                                                     WisDX12TextureView         texture,
                                                     const WisRenderTargetDesc* render_target,
                                                     uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param index defines the index in the view heap to get the descriptor from.
 * @return u64 Address of a view in heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapGetViewAddress(const WisDX12ViewHeap* self,
                                                  uint32_t               index);

/**
 * @brief Provided by Wisdom 0.7.0. Copies views from one heap to another.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param dst_index defines the index in the destination view heap to copy views to.
 * @param src_ptr points to WisViewHeap to copy views from.
 * @param src_index defines the index in the source view heap to copy views from.
 * @param count defines the number of views to copy.
 *
 * */
WISDOM_API void wisDX12ViewHeapCopyViews(const WisDX12ViewHeap* self,
                                         uint32_t               dst_index,
                                         uint64_t               src_ptr,
                                         uint32_t               src_index,
                                         uint32_t               count);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapGetCPUAddress(const WisDX12ViewHeap* self);

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
 * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandListBegin(const WisDX12CommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandListEnd(const WisDX12CommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Binds descriptor heaps to the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param resource_heap points to WisDescriptorHeap with shader resource views, unordered access views and constant buffer views. If `nullptr`, no resource heap is bound.
 * @param sampler_heap points to WisDescriptorHeap with samplers. If `nullptr`, no sampler heap is bound.
 *
 * */
WISDOM_API void wisDX12CommandListSetDescriptorHeaps(const WisDX12CommandList*    self,
                                                     const WisDX12DescriptorHeap* resource_heap,
                                                     const WisDX12DescriptorHeap* sampler_heap);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the root signature for the command list, so it can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param signature points to WisRootSignature to set.
 * @param pipeline defines the pipeline type to set the root signature for.
 *
 * */
WISDOM_API void wisDX12CommandListSetRootSignature(const WisDX12CommandList* self,
                                                   WisDX12RootSignatureView  signature,
                                                   WisPipelineType           pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the push constants for the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data points to , which describes the push constant data to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPushConstants(const WisDX12CommandList*      self,
                                                   const WisPushConstantDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the push descriptors for the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data points to , which describes the push descriptors to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPushDescriptor(const WisDX12CommandList*        self,
                                                    const WisPushDescriptorDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the descriptor table offset in descriptor heap for the command list, so it can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data defines the root parameter index to set the descriptor table for.
 *
 * */
WISDOM_API void wisDX12CommandListSetDescriptorTable(const WisDX12CommandList*         self,
                                                     const WisDescriptorTableDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Inserts one or more barriers on the current command list.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param barriers points to an array of barriers to insert.
 *
 * */
WISDOM_API void wisDX12CommandListInsertBarriers(const WisDX12CommandList*  self,
                                                 const WisDX12BarrierGroup* barriers);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the pipeline state object for the command list, so it can be used for draw and dispatch calls.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param pipeline points to WisPipeline to set.
 * @param type defines the pipeline type to set the pipeline for.
 *
 * */
WISDOM_API void wisDX12CommandListSetPipeline(const WisDX12CommandList* self,
                                              WisDX12PipelineView       pipeline,
                                              WisPipelineType           type);

/**
 * @brief Provided by Wisdom 0.7.0. Sets multiple viewports.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param viewports The viewports to set.
 * @param count The number of viewports to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetViewports(WisDX12CommandList* self,
                                               const WisViewport*  viewports,
                                               size_t              count);

/**
 * @brief Provided by Wisdom 0.7.0. Sets multiple scissor rects.
 * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry shader.
 * Otherwise the first is chosen.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param scissors The scissors to set.
 * @param count The number of scissors to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetScissors(WisDX12CommandList* self,
                                              const WisScissor*   scissors,
                                              size_t              count);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the primitive topology. Detemines how vertices shall be processed.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param topology The primitive topology to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPrimitiveTopology(WisDX12CommandList*  self,
                                                       WisPrimitiveTopology topology);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the depth bias. Determines how depth values are modified during rasterization.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param depth_bias The depth bias to set.
 * @param depth_bias_clamp The depth bias clamp to set.
 * @param slope_scaled_depth_bias The slope scaled depth bias to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetDepthBias(WisDX12CommandList* self,
                                               float               depth_bias,
                                               float               depth_bias_clamp,
                                               float               slope_scaled_depth_bias);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the primitive restart value. Determines the index value which is treated as a primitive restart when using indexed draw calls.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param restart_value The primitive restart value to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPrimitiveRestartValue(WisDX12CommandList*      self,
                                                           WisPrimitiveRestartValue restart_value);

/**
 * @brief Provided by Wisdom 0.7.0. Dispatches compute shader.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param group_count_x The number of groups to dispatch in X dimension.
 * @param group_count_y The number of groups to dispatch in Y dimension. Default is 1.
 * @param group_count_z The number of groups to dispatch in Z dimension. Default is 1.
 *
 * */
WISDOM_API void wisDX12CommandListDispatch(const WisDX12CommandList* self,
                                           uint32_t                  group_count_x,
                                           uint32_t                  group_count_y,
                                           uint32_t                  group_count_z);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the data from the pipeline cache.
 * @param self is a pointer to the valid WisPipelineCache instance.
 * @param data points to the data chunk, which is filled with the data of the cache data on success.
 * @param data_size defines the size of the data chunk in bytes. It @wis_must be greater or equal to the value returned by wisPipelineCacheGetSerializedSize.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12PipelineCacheSerialize(const WisDX12PipelineCache* self,
                                                   uint8_t*                    data,
                                                   size_t                      data_size);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the size of the data in the pipeline cache.
 * @param self is a pointer to the valid WisPipelineCache instance.
 * @return size Size of the data in bytes.
 *
 * */
WISDOM_API size_t wisDX12PipelineCacheGetSerializedSize(const WisDX12PipelineCache* self);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_DX12_API_H

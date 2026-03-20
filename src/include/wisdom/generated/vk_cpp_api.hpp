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
using VKSurfaceView = WisVKSurfaceView;

using VKPipelineView = WisVKPipelineView;

using VKShaderView = WisVKShaderView;

using VKPipelineCacheView = WisVKPipelineCacheView;

using VKTextureView = WisVKTextureView;

using VKBufferView = WisVKBufferView;

using VKRootSignatureView = WisVKRootSignatureView;

using VKFenceView = WisVKFenceView;

using VKCommandListView = WisVKCommandListView;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
struct VKDeviceRequirements {
    wis::span<const wis::CommandQueueDesc>   queue_descs; ///< points to an array of wis::CommandQueueDesc for which queues will be created during device creation.
    wis::span<wis::VKDeviceExtensionHeader*> extensions; ///< points to an array of extensions that are to be initialized with pointers to wis::DeviceExtensionHeader.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer barrier with the buffer handle.
 *
 * */
struct VKBufferBarrier {
    wis::BarrierSync      sync_before; ///< Synchronization scope before the barrier.
    wis::BarrierSync      sync_after; ///< Synchronization scope after the barrier.
    wis::ResourceAccess   access_before; ///< Access scope before the barrier.
    wis::ResourceAccess   access_after; ///< Access scope after the barrier.
    wis::VKBufferView     buffer; ///< Buffer view.
    std::uint64_t         offset; ///< Offset in bytes from the start of the buffer. Default is 0.
    std::uint64_t         size; ///< Barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
    wis::CommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    wis::CommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Global barrier without resource handles.
 *
 * */
struct VKGlobalBarrier {
    wis::BarrierSync    sync_before; ///< Synchronization scope before the barrier.
    wis::BarrierSync    sync_after; ///< Synchronization scope after the barrier.
    wis::ResourceAccess access_before; ///< Access scope before the barrier.
    wis::ResourceAccess access_after; ///< Access scope after the barrier.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture barrier with the texture handle.
 *
 * */
struct VKTextureBarrier {
    wis::BarrierSync      sync_before; ///< Synchronization scope before the barrier.
    wis::BarrierSync      sync_after; ///< Synchronization scope after the barrier.
    wis::ResourceAccess   access_before; ///< Access scope before the barrier.
    wis::ResourceAccess   access_after; ///< Access scope after the barrier.
    wis::TextureState     state_before; ///< Texture state before the barrier.
    wis::TextureState     state_after; ///< Texture state after the barrier.
    wis::BarrierFlags     flags; ///< Barrier flags. Describe additional options for the barrier.
    wis::VKTextureView    texture; ///< Texture view.
    wis::SubresourceRange subresource_range; ///< Subresource range for the barrier.
    wis::CommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    wis::CommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Barrier group for multiple barriers submission.
 *
 * */
struct VKBarrierGroup {
    wis::span<const wis::VKBufferBarrier>  buffer_barriers; ///< Array of buffer barriers.
    wis::span<const wis::VKTextureBarrier> texture_barriers; ///< Array of texture barriers.
    wis::span<const wis::VKGlobalBarrier>  global_barriers; ///< Array of global barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Compute pipeline description for wis::Pipeline creation.
 *
 * */
struct VKComputePipelineDesc {
    wis::VKRootSignatureView root_signature; ///< Root signature description for the pipeline.
    wis::VKShaderView        compute_shader; ///< Compute shader bytecode.
    wis::VKPipelineCacheView cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    wis::PipelineFlags       flags; ///< Pipeline flags. Describe additional options for the pipeline.
};

/**
 * @brief Provided by Wisdom 0.7.0. Graphics pipeline description for wis::Pipeline creation.
 *
 * */
struct VKGraphicsPipelineDesc {
    wis::VKRootSignatureView     root_signature; ///< Root signature description for the pipeline.
    wis::VKShaderView            vertex_shader; ///< Vertex shader bytecode.
    wis::VKShaderView            hull_shader; ///< Hull shader bytecode. If not set, the pipeline will be created without a hull shader.
    wis::VKShaderView            domain_shader; ///< Domain shader bytecode. If not set, the pipeline will be created without a domain shader.
    wis::VKShaderView            geometry_shader; ///< Geometry shader bytecode. If not set, the pipeline will be created without a geometry shader.
    wis::VKShaderView            pixel_shader; ///< Pixel shader bytecode. If not set, the pipeline will be created without a pixel shader.
    wis::RenderAttachmentsDesc   render_attachments; ///< Render attachments description for the pipeline. Used to create the compatible render pass for the pipeline.
    wis::InputLayout             input_layout; ///< Input layout description for the pipeline. If not set, the pipeline will be created without an input layout.
    wis::TopologyType            topology_type; ///< Topology type. Default is `wis::TopologyType::Triangle`.
    const wis::RasterizerDesc*   rasterizer_desc; ///< Rasterizer description for the pipeline. If not set, the pipeline will be created with default rasterizer state.
    const wis::SampleDesc*       sample_desc; ///< Sample description for the pipeline. If not set, the pipeline will be created with default sample state (no multisampling).
    const wis::DepthStencilDesc* depth_stencil_desc; ///< Depth stencil description for the pipeline. If not set, the pipeline will be created with depth testing and stencil testing disabled.
    const wis::BlendStateDesc*   blend_state_desc; ///< Blend state description for the pipeline. If not set, the pipeline will be created with blending disabled.
    wis::VKPipelineCacheView     cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    wis::PipelineFlags           flags; ///< Pipeline flags. Describe additional options for the pipeline.
};

struct VKSurfaceDeleter {
    void operator()(WisVKSurface* handle) noexcept
    {
        ::wisVKDestroySurface(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU surface, which can be used as a target for rendering and presentation.
 *
 * */
class VKSurface : public wis::impl::Implements<wis::impl::VKSurfaceImpl, WisVKSurface, wis::VKSurfaceDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKSurfaceView GetView() const noexcept
    {
        VKSurfaceView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKSurfaceView() const noexcept
    {
        return GetView();
    }
};

struct VKViewHeapDeleter {
    void operator()(WisVKViewHeap* handle) noexcept
    {
        ::wisVKDestroyViewHeap(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for resource views used in contiguous array.
 *
 * */
class VKViewHeap : public wis::impl::Implements<wis::impl::VKViewHeapImpl, WisVKViewHeap, wis::VKViewHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a render target view to the view heap and returns the CPU descriptor handle for it.
     * @param texture points to wis::Texture to write the descriptor for.
     * @param render_target points to wis::RenderTargetDesc, which describes the render target view to write.
     * @param index defines the index in the view heap to write the descriptor to.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t WriteRenderTarget(wis::VKTextureView           texture,
                                                         const wis::RenderTargetDesc& render_target,
                                                         std::uint32_t                index) const noexcept
    {
        return (::wisVKViewHeapWriteRenderTarget(&_impl_storage,
                                                 texture,
                                                 reinterpret_cast<const WisRenderTargetDesc*>(&render_target),
                                                 index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a depth stencil view to the view heap and returns the CPU descriptor handle for it.
     * @param texture points to wis::Texture to write the descriptor for.
     * @param render_target points to wis::RenderTargetDesc, which describes the render target view to write.
     * @param index defines the index in the view heap to write the descriptor to.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t WriteDepthStencil(wis::VKTextureView           texture,
                                                         const wis::RenderTargetDesc& render_target,
                                                         std::uint32_t                index) const noexcept
    {
        return (::wisVKViewHeapWriteDepthStencil(&_impl_storage,
                                                 texture,
                                                 reinterpret_cast<const WisRenderTargetDesc*>(&render_target),
                                                 index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
     * @param index defines the index in the view heap to get the descriptor from.
     * @return u64 Address of a view in heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetViewAddress(std::uint32_t index) const noexcept
    {
        return (::wisVKViewHeapGetViewAddress(&_impl_storage,
                                              index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies views from one heap to another.
     * @param dst_index defines the index in the destination view heap to copy views to.
     * @param src_ptr points to wis::ViewHeap to copy views from.
     * @param src_index defines the index in the source view heap to copy views from.
     * @param count defines the number of views to copy.
     *
     * */
    inline void CopyViews(std::uint32_t dst_index,
                          std::uint64_t src_ptr,
                          std::uint32_t src_index,
                          std::uint32_t count) const noexcept
    {
        ::wisVKViewHeapCopyViews(&_impl_storage,
                                 dst_index,
                                 src_ptr,
                                 src_index,
                                 count);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCPUHandle() const noexcept
    {
        return (::wisVKViewHeapGetCPUHandle(&_impl_storage));
    }
};

struct VKPipelineDeleter {
    void operator()(WisVKPipeline* handle) noexcept
    {
        ::wisVKDestroyPipeline(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU pipeline state object, which encapsulates the state of the GPU pipeline and allows to execute draw and dispatch calls with it.
 *
 * */
class VKPipeline : public wis::impl::Implements<wis::impl::VKPipelineImpl, WisVKPipeline, wis::VKPipelineDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKPipelineView GetView() const noexcept
    {
        VKPipelineView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKPipelineView() const noexcept
    {
        return GetView();
    }
};

struct VKShaderDeleter {
    void operator()(WisVKShader* handle) noexcept
    {
        ::wisVKDestroyShader(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU shader module, which contains shader code and allows to create pipeline state objects with it.
 *
 * */
class VKShader : public wis::impl::Implements<wis::impl::VKShaderImpl, WisVKShader, wis::VKShaderDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKShaderView GetView() const noexcept
    {
        VKShaderView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKShaderView() const noexcept
    {
        return GetView();
    }
};

struct VKPipelineCacheDeleter {
    void operator()(WisVKPipelineCache* handle) noexcept
    {
        ::wisVKDestroyPipelineCache(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a cache for pipeline state objects, which allows to reuse already created pipelines and speed up pipeline creation.
 *
 * */
class VKPipelineCache : public wis::impl::Implements<wis::impl::VKPipelineCacheImpl, WisVKPipelineCache, wis::VKPipelineCacheDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKPipelineCacheView GetView() const noexcept
    {
        VKPipelineCacheView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKPipelineCacheView() const noexcept
    {
        return GetView();
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the data from the pipeline cache.
     * @param data points to the data chunk, which is filled with the data of the cache data on success.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Serialize(wis::span<std::uint8_t> data) const noexcept
    {
        return convert_result_vk(::wisVKPipelineCacheSerialize(&_impl_storage,
                                                               reinterpret_cast<uint8_t*>(data.data()),
                                                               data.size()));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the size of the data in the pipeline cache.
     * @return size Size of the data in bytes.
     *
     * */
    WIS_NODISCARD inline std::size_t GetSerializedSize() const noexcept
    {
        return (::wisVKPipelineCacheGetSerializedSize(&_impl_storage));
    }
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
    WIS_NODISCARD VKTextureView GetView() const noexcept
    {
        VKTextureView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKTextureView() const noexcept
    {
        return GetView();
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes data direct to the texture subresource. Texture @wis_must be in `wis::TextureState::Common` and @wis_must_not be a depth texture.
     * @param source_data points to the data to write to the texture.
     * @param target_region points to wis::TextureRegion, which describes the region of the texture to write to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteSubresource(const void*               source_data,
                                        const wis::TextureRegion& target_region) const noexcept
    {
        return convert_result_vk(::wisVKTextureWriteSubresource(&_impl_storage,
                                                                source_data,
                                                                reinterpret_cast<const WisTextureRegion*>(&target_region)));
    }
};

struct VKBufferDeleter {
    void operator()(WisVKBuffer* handle) noexcept
    {
        ::wisVKDestroyBuffer(handle);
    }
};
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
        return convert_result_vk(::wisVKDescriptorHeapWriteConstantBuffer(&_impl_storage,
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
        return convert_result_vk(::wisVKDescriptorHeapWriteStructuredBuffer(&_impl_storage,
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
        return convert_result_vk(::wisVKDescriptorHeapWriteRWStructuredBuffer(&_impl_storage,
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
        return convert_result_vk(::wisVKDescriptorHeapWriteSampler(&_impl_storage,
                                                                   reinterpret_cast<const WisSamplerDesc*>(&sampler),
                                                                   index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a descriptor to the descriptor heap.
     * @param texture points to wis::Texture to write the descriptor for.
     * @param data points to , which describes the texture view to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteTexture(wis::VKTextureView         texture,
                                    const wis::TextureBinding& data,
                                    std::uint32_t              index) const noexcept
    {
        return convert_result_vk(::wisVKDescriptorHeapWriteTexture(&_impl_storage,
                                                                   texture,
                                                                   reinterpret_cast<const WisTextureBinding*>(&data),
                                                                   index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a texture view to the descriptor heap.
     * @param texture points to wis::Texture to write the descriptor for.
     * @param data points to , which describes the texture view to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteRWTexture(wis::VKTextureView         texture,
                                      const wis::TextureBinding& data,
                                      std::uint32_t              index) const noexcept
    {
        return convert_result_vk(::wisVKDescriptorHeapWriteRWTexture(&_impl_storage,
                                                                     texture,
                                                                     reinterpret_cast<const WisTextureBinding*>(&data),
                                                                     index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a raytracing acceleration to the descriptor heap.
     * @param address GPU address of a raytracing acceleration structure.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteAccelerationStructure(std::uint64_t address,
                                                  std::uint32_t index) const noexcept
    {
        return convert_result_vk(::wisVKDescriptorHeapWriteAccelerationStructure(&_impl_storage,
                                                                                 address,
                                                                                 index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies descriptors from one heap to another.
     * @param dst_index defines the index in the destination descriptor heap to copy descriptors to.
     * @param src_ptr points to wis::DescriptorHeap to copy descriptors from. Source heap @wis_must be CPU Only heap.
     * @param src_index defines the index in the source descriptor heap to copy descriptors from.
     * @param count defines the number of descriptors to copy.
     *
     * */
    inline void CopyDescriptors(std::uint32_t dst_index,
                                const void*   src_ptr,
                                std::uint32_t src_index,
                                std::uint32_t count) const noexcept
    {
        ::wisVKDescriptorHeapCopyDescriptors(&_impl_storage,
                                             dst_index,
                                             src_ptr,
                                             src_index,
                                             count);
    }
};

struct VKRootSignatureDeleter {
    void operator()(WisVKRootSignature* handle) noexcept
    {
        ::wisVKDestroyRootSignature(handle);
    }
};
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
        out_result = convert_result_vk(::wisVKResourceAllocatorCreateBuffer(&_impl_storage,
                                                                            reinterpret_cast<const WisBufferDesc*>(&desc),
                                                                            buffer.GetStorage()));
        return buffer;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
     * @param desc points to wis::TextureDesc, which describes the texture to create.
     * @param initial_state defines the initial state of the texture. State transition @wis_must be supported, query wis::DeviceMemoryProperties to get if the transition is supported. If not, @wis_must be `wis::TextureState::Undefined`.
     * @param out_result denoting the outcome of operation.
     * @return texture points to wis::Texture, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKTexture CreateTexture(const wis::TextureDesc& desc,
                                                      wis::TextureState       initial_state,
                                                      wis::Result&            out_result) const noexcept
    {
        wis::VKTexture texture;
        out_result = convert_result_vk(::wisVKResourceAllocatorCreateTexture(&_impl_storage,
                                                                             reinterpret_cast<const WisTextureDesc*>(&desc),
                                                                             static_cast<WisTextureState>(initial_state),
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
        return convert_result_vk(::wisVKFenceWait(&_impl_storage,
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
        return convert_result_vk(::wisVKFenceSignal(&_impl_storage,
                                                    value));
    }
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
        return convert_result_vk(::wisVKCommandListBegin(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result End() const noexcept
    {
        return convert_result_vk(::wisVKCommandListEnd(&_impl_storage));
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
    /**
     * @brief Provided by Wisdom 0.7.0. Inserts one or more barriers on the current command list.
     * @param barriers points to an array of barriers to insert.
     *
     * */
    inline void InsertBarriers(const wis::VKBarrierGroup& barriers) const noexcept
    {
        ::wisVKCommandListInsertBarriers(&_impl_storage,
                                         reinterpret_cast<const WisVKBarrierGroup*>(&barriers));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the pipeline state object for the command list, so it can be used for draw and dispatch calls.
     * @param pipeline points to wis::Pipeline to set.
     * @param type defines the pipeline type to set the pipeline for.
     *
     * */
    inline void SetPipeline(wis::VKPipelineView pipeline,
                            wis::PipelineType   type) const noexcept
    {
        ::wisVKCommandListSetPipeline(&_impl_storage,
                                      pipeline,
                                      static_cast<WisPipelineType>(type));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets multiple viewports.
     * @param viewports The viewports to set.
     *
     * */
    inline void SetViewports(wis::span<const wis::Viewport> viewports) noexcept
    {
        ::wisVKCommandListSetViewports(&_impl_storage,
                                       reinterpret_cast<const WisViewport*>(viewports.data()),
                                       viewports.size());
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets multiple scissor rects.
     * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry shader.
     * Otherwise the first is chosen.
     * @param scissors The scissors to set.
     *
     * */
    inline void SetScissors(wis::span<const wis::Scissor> scissors) noexcept
    {
        ::wisVKCommandListSetScissors(&_impl_storage,
                                      reinterpret_cast<const WisScissor*>(scissors.data()),
                                      scissors.size());
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the primitive topology. Detemines how vertices shall be processed.
     * @param topology The primitive topology to set.
     *
     * */
    inline void SetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
    {
        ::wisVKCommandListSetPrimitiveTopology(&_impl_storage,
                                               static_cast<WisPrimitiveTopology>(topology));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the depth bias. Determines how depth values are modified during rasterization.
     * @param depth_bias The depth bias to set.
     * @param depth_bias_clamp The depth bias clamp to set.
     * @param slope_scaled_depth_bias The slope scaled depth bias to set.
     *
     * */
    inline void SetDepthBias(float depth_bias,
                             float depth_bias_clamp,
                             float slope_scaled_depth_bias) noexcept
    {
        ::wisVKCommandListSetDepthBias(&_impl_storage,
                                       depth_bias,
                                       depth_bias_clamp,
                                       slope_scaled_depth_bias);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the primitive restart value. Determines the index value which is treated as a primitive restart when using indexed draw calls.
     * @param restart_value The primitive restart value to set.
     *
     * */
    inline void SetPrimitiveRestartValue(wis::PrimitiveRestartValue restart_value) noexcept
    {
        ::wisVKCommandListSetPrimitiveRestartValue(&_impl_storage,
                                                   static_cast<WisPrimitiveRestartValue>(restart_value));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Dispatches compute shader.
     * @param group_count_x The number of groups to dispatch in X dimension.
     * @param group_count_y The number of groups to dispatch in Y dimension. Default is 1.
     * @param group_count_z The number of groups to dispatch in Z dimension. Default is 1.
     *
     * */
    inline void Dispatch(std::uint32_t group_count_x,
                         std::uint32_t group_count_y,
                         std::uint32_t group_count_z) const noexcept
    {
        ::wisVKCommandListDispatch(&_impl_storage,
                                   group_count_x,
                                   group_count_y,
                                   group_count_z);
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
        return convert_result_vk(::wisVKCommandAllocatorReset(&_impl_storage));
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
        out_result = convert_result_vk(::wisVKCommandAllocatorCreateCommandList(&_impl_storage, list.GetStorage()));
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
        return convert_result_vk(::wisVKCommandQueueSubmit(&_impl_storage,
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
        return convert_result_vk(::wisVKCommandQueueSignalFence(&_impl_storage,
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
        return convert_result_vk(::wisVKCommandQueueWaitFence(&_impl_storage,
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
        out_result = convert_result_vk(::wisVKDeviceCreateCommandQueue(&_impl_storage,
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
        out_result = convert_result_vk(::wisVKDeviceCreateCommandAllocator(&_impl_storage,
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
        out_result = convert_result_vk(::wisVKDeviceCreateFence(&_impl_storage,
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
        out_result = convert_result_vk(::wisVKDeviceGetResourceAllocator(&_impl_storage, allocator.GetStorage()));
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
        out_result = convert_result_vk(::wisVKDeviceCreateRootSignature(&_impl_storage,
                                                                        reinterpret_cast<const WisRootSignatureDesc*>(&desc),
                                                                        layout.GetStorage()));
        return layout;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given description.
     * @param desc points to wis::DescriptorHeapDesc, which describes the descriptor heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::DescriptorHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKDescriptorHeap CreateDescriptorHeap(const wis::DescriptorHeapDesc& desc,
                                                                    wis::Result&                   out_result) const noexcept
    {
        wis::VKDescriptorHeap heap;
        out_result = convert_result_vk(::wisVKDeviceCreateDescriptorHeap(&_impl_storage,
                                                                         reinterpret_cast<const WisDescriptorHeapDesc*>(&desc),
                                                                         heap.GetStorage()));
        return heap;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a view storage with given descriptor.
     * @param type defines the type of the view heap to create.
     * @param capacity defines the capacity in descriptors of the view heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::DescriptorHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKViewHeap CreateViewHeap(wis::ViewHeapType type,
                                                        std::uint32_t     capacity,
                                                        wis::Result&      out_result) const noexcept
    {
        wis::VKViewHeap heap;
        out_result = convert_result_vk(::wisVKDeviceCreateViewHeap(&_impl_storage,
                                                                   static_cast<WisViewHeapType>(type),
                                                                   capacity,
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
    /**
     * @brief Provided by Wisdom 0.7.0. Waits on multiple fences simultaneously.
     * If wait_all is `wis::MutiWaitType::All`, waits for all fences to be signaled.
     * Otherwise waits for any fence to be signaled.
     * @param fences Array of fence views to wait on.
     * @param fence_values Fence values to wait fences to reach. Array @wis_must have fence_count values.
     * @param fence_count How many fences to wait on.
     * @param wait_for Specifies the kind of wait.
     * All - waits for all fences to be signaled.
     * Any - waits for any fence to be signaled.
     * Default is `wis::MutiWaitType::All`
     * @param timeout The timeout in nanoseconds. If UINT64_MAX, waits indefinitely.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WaitForMultipleFences(const wis::VKFenceView* fences,
                                             const std::uint64_t*    fence_values,
                                             std::size_t             fence_count,
                                             wis::MutiWaitType       wait_for,
                                             std::uint64_t           timeout) const noexcept
    {
        return convert_result_vk(::wisVKDeviceWaitForMultipleFences(&_impl_storage,
                                                                    fences,
                                                                    fence_values,
                                                                    fence_count,
                                                                    static_cast<WisMutiWaitType>(wait_for),
                                                                    timeout));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline cache for caching pipeline state objects.
     * @param initial_data points to the initial cache data. If `nullptr`, the cache is created empty.
     * @param out_result denoting the outcome of operation.
     * @return cache points to wis::PipelineCache, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipelineCache CreatePipelineCache(wis::span<const std::uint8_t> initial_data,
                                                                  wis::Result&                  out_result) const noexcept
    {
        wis::VKPipelineCache cache;
        out_result = convert_result_vk(::wisVKDeviceCreatePipelineCache(&_impl_storage,
                                                                        reinterpret_cast<const uint8_t*>(initial_data.data()),
                                                                        initial_data.size(),
                                                                        cache.GetStorage()));
        return cache;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a shader module from given data.
     * @param data Shader bytecode.
     * @param out_result denoting the outcome of operation.
     * @return shader points to wis::Shader, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKShader CreateShader(wis::span<const std::uint8_t> data,
                                                    wis::Result&                  out_result) const noexcept
    {
        wis::VKShader shader;
        out_result = convert_result_vk(::wisVKDeviceCreateShader(&_impl_storage,
                                                                 reinterpret_cast<const uint8_t*>(data.data()),
                                                                 data.size(),
                                                                 shader.GetStorage()));
        return shader;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a compute pipeline state object with given descriptor.
     * @param desc points to wis::ComputePipelineDesc, which describes the compute pipeline to create.
     * @param out_result denoting the outcome of operation.
     * @return pipeline points to wis::Pipeline, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipeline CreateComputePipeline(const wis::VKComputePipelineDesc& desc,
                                                               wis::Result&                      out_result) const noexcept
    {
        wis::VKPipeline pipeline;
        out_result = convert_result_vk(::wisVKDeviceCreateComputePipeline(&_impl_storage,
                                                                          reinterpret_cast<const WisVKComputePipelineDesc*>(&desc),
                                                                          pipeline.GetStorage()));
        return pipeline;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a graphics pipeline state object with given descriptor.
     * @param desc points to wis::GraphicsPipelineDesc, which describes the graphics pipeline to create.
     * @param out_result denoting the outcome of operation.
     * @return pipeline points to wis::Pipeline, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipeline CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc& desc,
                                                                wis::Result&                       out_result) const noexcept
    {
        wis::VKPipeline pipeline;
        out_result = convert_result_vk(::wisVKDeviceCreateGraphicsPipeline(&_impl_storage,
                                                                           reinterpret_cast<const WisVKGraphicsPipelineDesc*>(&desc),
                                                                           pipeline.GetStorage()));
        return pipeline;
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
     * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wis::AdapterQuery::GetAdapterCount.
     * @param out_result denoting the outcome of operation.
     * @return desc points to wis::AdapterDesc, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::AdapterDesc GetAdapterDesc(std::size_t  index,
                                                         wis::Result& out_result) const noexcept
    {
        wis::AdapterDesc desc;
        out_result = convert_result_vk(::wisVKAdapterQueryGetAdapterDesc(&_impl_storage,
                                                                         index,
                                                                         reinterpret_cast<WisAdapterDesc*>(&desc)));
        return desc;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Checks if the adapter at given index supports presentation to given surface.
     * @param index defines the index of the adapter to check the support for. It @wis_must be less than the value returned by wis::AdapterQuery::GetAdapterCount.
     * @param surface points to wis::Surface to check the presentation support for.
     * @return bool `true` if the adapter supports presentation to the surface, `false` otherwise.
     *
     * */
    WIS_NODISCARD inline bool GetSurfaceSupport(std::size_t        index,
                                                wis::VKSurfaceView surface) const noexcept
    {
        return (::wisVKAdapterQueryGetSurfaceSupport(&_impl_storage,
                                                     index,
                                                     surface));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
     * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wis::AdapterQuery::GetAdapterCount.
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
        out_result = convert_result_vk(::wisVKAdapterQueryCreateDevice(&_impl_storage,
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
        out_result = convert_result_vk(::wisVKInstanceQueryAdapters(&_impl_storage,
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
    out_result = convert_result_vk(::wisVKCreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
                                                         reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
                                                         extensions.size(),
                                                         instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // WISDOM_CPP_VK_API_HPP

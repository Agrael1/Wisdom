#pragma once
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/generated/api/api.hpp>
#include <wisdom/util/flags.h>
#include <D3D12MemAlloc.h>

namespace wis {
/**
 * @brief Variant of BufferBarrier with BufferView.
 * */
struct DX12BufferBarrier2 {
    wis::BufferBarrier barrier; ///< Buffer barrier.
    wis::DX12BufferView buffer; ///< Buffer view.
};

/**
 * @brief Variant of TextureBarrier with TextureView.
 * */
struct DX12TextureBarrier2 {
    wis::TextureBarrier barrier; ///< Texture barrier.
    wis::DX12TextureView texture; ///< Texture view.
};

/**
 * @brief Variant of ShaderView for all graphics stages.
 * */
struct DX12GraphicsShaderStages {
    wis::DX12ShaderView vertex; ///< Vertex shader.
    wis::DX12ShaderView hull; ///< Hull shader.
    wis::DX12ShaderView domain; ///< Domain shader.
    wis::DX12ShaderView geometry; ///< Geometry shader.
    wis::DX12ShaderView pixel; ///< Pixel shader.
};

/**
 * @brief Variant of PipelineStateDesc for graphics pipeline.
 * */
struct DX12GraphicsPipelineDesc {
    wis::DX12RootSignatureView root_signature; ///< Root signature.
    wis::InputLayout input_layout; ///< Input layout.
    wis::DX12GraphicsShaderStages shaders; ///< Shader stages.
    wis::RenderAttachmentsDesc attachments; ///< Render attachments.
    const wis::RasterizerDesc* rasterizer = nullptr; ///< Rasterizer description.
    const wis::SampleDesc* sample = nullptr; ///< Sample description.
    const wis::BlendStateDesc* blend = nullptr; ///< Blend state description.
    const wis::DepthStencilDesc* depth_stencil = nullptr; ///< Depth stencil description.
    wis::TopologyType topology_type = wis::TopologyType::Triangle; ///< Topology type. Default is wis::TopologyType::Triangle.
    /**
     * @brief View mask for Multiview feature. If multiview is not available it is ignored.
     * Default is 0. 0 means regular rendering.
     * */
    uint32_t view_mask = 0;
    wis::PipelineFlags flags; ///< Pipeline flags to add options to pipeline creation.
};

/**
 * @brief Variant of RenderPassDesc for render target.
 * */
struct DX12RenderPassRenderTargetDesc {
    wis::DX12RenderTargetView target; ///< Render target view.
    wis::LoadOperation load_op = wis::LoadOperation::Load; ///< Load operation on beginning of render pass. Default is wis::LoadOperation::Load.
    wis::StoreOperation store_op = wis::StoreOperation::Store; ///< Store operation on end of render pass. Default is wis::StoreOperation::Store.
    std::array<float, 4> clear_value{}; ///< Clear value for wis::LoadOperation::Clear.
};

/**
 * @brief Variant of RenderPassDesc for depth stencil.
 * */
struct DX12RenderPassDepthStencilDesc {
    wis::DX12RenderTargetView target; ///< Depth stencil view.
    wis::LoadOperation load_op_depth = wis::LoadOperation::Load; ///< Load operation on beginning of render pass for depth. Default is wis::LoadOperation::Load.
    wis::LoadOperation load_op_stencil = wis::LoadOperation::Load; ///< Load operation on beginning of render pass for stencil. Default is wis::LoadOperation::Load.
    wis::StoreOperation store_op_depth = wis::StoreOperation::Store; ///< Store operation on end of render pass for depth. Default is wis::StoreOperation::Store.
    wis::StoreOperation store_op_stencil = wis::StoreOperation::Store; ///< Store operation on end of render pass for stencil. Default is wis::StoreOperation::Store.
    wis::DSSelect depth_stencil_select = wis::DSSelect::None; ///< Depth stencil select. Default is wis::DSSelect::None.
    float clear_depth = 1.0f; ///< Clear depth value for wis::LoadOperation::Clear. Default is 1.0f.
    uint8_t clear_stencil = 0; ///< Clear stencil value for wis::LoadOperation::Clear. Default is 0.
};

/**
 * @brief Variant of PipelineStateDesc for render pass.
 * */
struct DX12RenderPassDesc {
    wis::RenderPassFlags flags; ///< Render pass flags.
    /**
     * @brief View mask for Multiview feature. If multiview is not available it is ignored.
     * Value must be the same as in  upon pipeline creation. Otherwise behavior is undefined.
     * Default is 0. 0 means regular rendering.
     * */
    uint32_t view_mask = 0;
    uint32_t target_count; ///< Render target count.
    const wis::DX12RenderPassRenderTargetDesc* targets = nullptr; ///< Render target descriptions.
    const wis::DX12RenderPassDepthStencilDesc* depth_stencil = nullptr; ///< Depth stencil description.
};

/**
 * @brief Variant of BufferView for vertex buffer binding.
 * */
struct DX12VertexBufferBinding {
    wis::DX12BufferView buffer; ///< Buffer view.
    uint32_t size; ///< Size of the buffer in bytes.
    uint32_t stride; ///< Stride of the buffer in bytes.
    uint32_t offset; ///< Offset in buffer in bytes. Default is 0.
};

inline constexpr DXGI_GPU_PREFERENCE convert_dx(AdapterPreference value) noexcept
{
    return static_cast<DXGI_GPU_PREFERENCE>(value);
}
inline constexpr D3D12_SHADER_VISIBILITY convert_dx(ShaderStages value) noexcept
{
    return static_cast<D3D12_SHADER_VISIBILITY>(value);
}
inline constexpr D3D12_DESCRIPTOR_RANGE_TYPE convert_dx(DescriptorType value) noexcept
{
    switch (value) {
    default:
        return {};
    case DescriptorType::Sampler:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
    case DescriptorType::ConstantBuffer:
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    case DescriptorType::Texture:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    case DescriptorType::RWTexture:
        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    case DescriptorType::RWBuffer:
        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    case DescriptorType::Buffer:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    }
}
inline constexpr DXGI_FORMAT convert_dx(DataFormat value) noexcept
{
    return static_cast<DXGI_FORMAT>(value);
}
inline constexpr D3D12_FILL_MODE convert_dx(FillMode value) noexcept
{
    return static_cast<D3D12_FILL_MODE>(value);
}
inline constexpr D3D12_CULL_MODE convert_dx(CullMode value) noexcept
{
    return static_cast<D3D12_CULL_MODE>(value);
}
inline constexpr BOOL convert_dx(WindingOrder value) noexcept
{
    return static_cast<BOOL>(value);
}
inline constexpr uint32_t convert_dx(SampleRate value) noexcept
{
    return static_cast<uint32_t>(value);
}
inline constexpr D3D12_COMPARISON_FUNC convert_dx(Compare value) noexcept
{
    return static_cast<D3D12_COMPARISON_FUNC>(value);
}
inline constexpr D3D12_STENCIL_OP convert_dx(StencilOp value) noexcept
{
    return static_cast<D3D12_STENCIL_OP>(value);
}
inline constexpr D3D12_BLEND convert_dx(BlendFactor value) noexcept
{
    return static_cast<D3D12_BLEND>(value);
}
inline constexpr D3D12_BLEND_OP convert_dx(BlendOp value) noexcept
{
    return static_cast<D3D12_BLEND_OP>(value);
}
inline constexpr D3D12_LOGIC_OP convert_dx(LogicOp value) noexcept
{
    return static_cast<D3D12_LOGIC_OP>(value);
}
inline constexpr D3D12_HEAP_TYPE convert_dx(MemoryType value) noexcept
{
    switch (value) {
    default:
        return {};
    case MemoryType::Default:
        return D3D12_HEAP_TYPE_DEFAULT;
    case MemoryType::Upload:
        return D3D12_HEAP_TYPE_UPLOAD;
    case MemoryType::Readback:
        return D3D12_HEAP_TYPE_READBACK;
    case MemoryType::GPUUpload:
        return D3D12_HEAP_TYPE_GPU_UPLOAD;
    }
}
inline constexpr D3D12_BARRIER_LAYOUT convert_dx(TextureState value) noexcept
{
    switch (value) {
    default:
        return {};
    case TextureState::Undefined:
        return D3D12_BARRIER_LAYOUT_UNDEFINED;
    case TextureState::Common:
        return D3D12_BARRIER_LAYOUT_COMMON;
    case TextureState::Read:
        return D3D12_BARRIER_LAYOUT_GENERIC_READ;
    case TextureState::RenderTarget:
        return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
    case TextureState::UnorderedAccess:
        return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
    case TextureState::DepthStencilWrite:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    case TextureState::DepthStencilRead:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
    case TextureState::ShaderResource:
        return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
    case TextureState::CopySource:
        return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
    case TextureState::CopyDest:
        return D3D12_BARRIER_LAYOUT_COPY_DEST;
    case TextureState::Present:
        return D3D12_BARRIER_LAYOUT_PRESENT;
    case TextureState::ShadingRate:
        return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
    case TextureState::VideoDecodeRead:
        return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_READ;
    case TextureState::VideoDecodeWrite:
        return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_WRITE;
    }
}
inline constexpr D3D12_DESCRIPTOR_HEAP_TYPE convert_dx(DescriptorHeapType value) noexcept
{
    return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(value);
}
inline constexpr D3D12_DESCRIPTOR_HEAP_FLAGS convert_dx(DescriptorMemory value) noexcept
{
    return static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(value);
}
inline constexpr D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE convert_dx(LoadOperation value) noexcept
{
    switch (value) {
    default:
        return {};
    case LoadOperation::Load:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
    case LoadOperation::Clear:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    case LoadOperation::DontCare:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
    }
}
inline constexpr D3D12_RENDER_PASS_ENDING_ACCESS_TYPE convert_dx(StoreOperation value) noexcept
{
    switch (value) {
    default:
        return {};
    case StoreOperation::Store:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
    case StoreOperation::DontCare:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
    case StoreOperation::Resolve:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
    }
}
inline constexpr D3D_PRIMITIVE_TOPOLOGY convert_dx(PrimitiveTopology value) noexcept
{
    switch (value) {
    default:
        return {};
    case PrimitiveTopology::PointList:
        return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    case PrimitiveTopology::LineList:
        return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case PrimitiveTopology::LineStrip:
        return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case PrimitiveTopology::TriangleList:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case PrimitiveTopology::TriangleStrip:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case PrimitiveTopology::TriangleFan:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLEFAN;
    case PrimitiveTopology::LineListAdj:
        return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
    case PrimitiveTopology::LineStripAdj:
        return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
    case PrimitiveTopology::TriangleListAdj:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
    case PrimitiveTopology::TriangleStripAdj:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
    }
}
inline constexpr D3D12_PRIMITIVE_TOPOLOGY_TYPE convert_dx(TopologyType value) noexcept
{
    switch (value) {
    default:
        return {};
    case TopologyType::Point:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
    case TopologyType::Line:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    case TopologyType::Triangle:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    case TopologyType::Patch:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
    }
}
inline constexpr D3D12_FILTER_TYPE convert_dx(Filter value) noexcept
{
    switch (value) {
    default:
        return {};
    case Filter::Point:
        return D3D12_FILTER_TYPE_POINT;
    case Filter::Linear:
        return D3D12_FILTER_TYPE_LINEAR;
    }
}
inline constexpr D3D12_TEXTURE_ADDRESS_MODE convert_dx(AddressMode value) noexcept
{
    switch (value) {
    default:
        return {};
    case AddressMode::Repeat:
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    case AddressMode::MirroredRepeat:
        return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
    case AddressMode::ClampToEdge:
        return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    case AddressMode::ClampToBorder:
        return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    case AddressMode::MirrorClampToEdge:
        return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
    }
}
inline constexpr D3D12_SRV_DIMENSION convert_dx(TextureViewType value) noexcept
{
    switch (value) {
    default:
        return {};
    case TextureViewType::Texture1D:
        return D3D12_SRV_DIMENSION_TEXTURE1D;
    case TextureViewType::Texture1DArray:
        return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
    case TextureViewType::Texture2D:
        return D3D12_SRV_DIMENSION_TEXTURE2D;
    case TextureViewType::Texture2DArray:
        return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
    case TextureViewType::Texture2DMS:
        return D3D12_SRV_DIMENSION_TEXTURE2DMS;
    case TextureViewType::Texture2DMSArray:
        return D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
    case TextureViewType::Texture3D:
        return D3D12_SRV_DIMENSION_TEXTURE3D;
    case TextureViewType::TextureCube:
        return D3D12_SRV_DIMENSION_TEXTURECUBE;
    case TextureViewType::TextureCubeArray:
        return D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
    }
}
inline constexpr D3D12_SHADER_COMPONENT_MAPPING convert_dx(ComponentSwizzle value) noexcept
{
    return static_cast<D3D12_SHADER_COMPONENT_MAPPING>(value);
}
inline constexpr DXGI_FORMAT convert_dx(IndexType value) noexcept
{
    switch (value) {
    default:
        return {};
    case IndexType::UInt16:
        return DXGI_FORMAT_R16_UINT;
    case IndexType::UInt32:
        return DXGI_FORMAT_R32_UINT;
    }
}
inline constexpr D3D12MA::ALLOCATION_FLAGS convert_dx(MemoryFlags value) noexcept
{
    D3D12MA::ALLOCATION_FLAGS output = {};
    if (value & MemoryFlags::DedicatedAllocation) {
        output |= D3D12MA::ALLOCATION_FLAG_COMMITTED;
    }
    if (value & MemoryFlags::Mapped) {
        output |= D3D12MA::ALLOCATION_FLAG_NONE;
    }
    return output;
}
inline constexpr D3D12_BARRIER_SYNC convert_dx(BarrierSync value) noexcept
{
    D3D12_BARRIER_SYNC output = {};
    if (value & BarrierSync::All) {
        output |= D3D12_BARRIER_SYNC_ALL;
    }
    if (value & BarrierSync::Draw) {
        output |= D3D12_BARRIER_SYNC_DRAW;
    }
    if (value & BarrierSync::IndexInput) {
        output |= D3D12_BARRIER_SYNC_INDEX_INPUT;
    }
    if (value & BarrierSync::VertexShading) {
        output |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
    }
    if (value & BarrierSync::PixelShading) {
        output |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
    }
    if (value & BarrierSync::DepthStencil) {
        output |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
    }
    if (value & BarrierSync::RenderTarget) {
        output |= D3D12_BARRIER_SYNC_RENDER_TARGET;
    }
    if (value & BarrierSync::Compute) {
        output |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;
    }
    if (value & BarrierSync::Raytracing) {
        output |= D3D12_BARRIER_SYNC_RAYTRACING;
    }
    if (value & BarrierSync::Copy) {
        output |= D3D12_BARRIER_SYNC_COPY;
    }
    if (value & BarrierSync::Resolve) {
        output |= D3D12_BARRIER_SYNC_RESOLVE;
    }
    if (value & BarrierSync::ExecuteIndirect) {
        output |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;
    }
    if (value & BarrierSync::AllShading) {
        output |= D3D12_BARRIER_SYNC_ALL_SHADING;
    }
    if (value & BarrierSync::NonPixelShading) {
        output |= D3D12_BARRIER_SYNC_NON_PIXEL_SHADING;
    }
    if (value & BarrierSync::ClearUAV) {
        output |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;
    }
    if (value & BarrierSync::VideoDecode) {
        output |= D3D12_BARRIER_SYNC_VIDEO_DECODE;
    }
    if (value & BarrierSync::VideoEncode) {
        output |= D3D12_BARRIER_SYNC_VIDEO_ENCODE;
    }
    if (value & BarrierSync::BuildRTAS) {
        output |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;
    }
    if (value & BarrierSync::CopyRTAS) {
        output |= D3D12_BARRIER_SYNC_COPY_RAYTRACING_ACCELERATION_STRUCTURE;
    }
    return output;
}
inline constexpr D3D12_BARRIER_ACCESS convert_dx(ResourceAccess value) noexcept
{
    D3D12_BARRIER_ACCESS output = {};
    if (value & ResourceAccess::VertexBuffer) {
        output |= D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
    }
    if (value & ResourceAccess::ConstantBuffer) {
        output |= D3D12_BARRIER_ACCESS_CONSTANT_BUFFER;
    }
    if (value & ResourceAccess::IndexBuffer) {
        output |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
    }
    if (value & ResourceAccess::RenderTarget) {
        output |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
    }
    if (value & ResourceAccess::UnorderedAccess) {
        output |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
    }
    if (value & ResourceAccess::DepthWrite) {
        output |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
    }
    if (value & ResourceAccess::DepthRead) {
        output |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
    }
    if (value & ResourceAccess::ShaderResource) {
        output |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
    }
    if (value & ResourceAccess::StreamOutput) {
        output |= D3D12_BARRIER_ACCESS_STREAM_OUTPUT;
    }
    if (value & ResourceAccess::IndirectArgument) {
        output |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
    }
    if (value & ResourceAccess::CopyDest) {
        output |= D3D12_BARRIER_ACCESS_COPY_DEST;
    }
    if (value & ResourceAccess::CopySource) {
        output |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
    }
    if (value & ResourceAccess::ConditionalRendering) {
        output |= D3D12_BARRIER_ACCESS_PREDICATION;
    }
    if (value & ResourceAccess::AccelerationStrucureRead) {
        output |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
    }
    if (value & ResourceAccess::AccelerationStrucureWrite) {
        output |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
    }
    if (value & ResourceAccess::ShadingRate) {
        output |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
    }
    if (value & ResourceAccess::VideoDecodeRead) {
        output |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_READ;
    }
    if (value & ResourceAccess::VideoDecodeWrite) {
        output |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE;
    }
    if (value & ResourceAccess::ResolveDest) {
        output |= D3D12_BARRIER_ACCESS_RESOLVE_DEST;
    }
    if (value & ResourceAccess::ResolveSource) {
        output |= D3D12_BARRIER_ACCESS_RESOLVE_SOURCE;
    }
    if (value & ResourceAccess::NoAccess) {
        output |= D3D12_BARRIER_ACCESS_NO_ACCESS;
    }
    return output;
}
inline constexpr D3D12_RENDER_PASS_FLAGS convert_dx(RenderPassFlags value) noexcept
{
    D3D12_RENDER_PASS_FLAGS output = {};
    if (value & RenderPassFlags::Suspending) {
        output |= D3D12_RENDER_PASS_FLAG_SUSPENDING_PASS;
    }
    if (value & RenderPassFlags::Resuming) {
        output |= D3D12_RENDER_PASS_FLAG_RESUMING_PASS;
    }
    return output;
}
inline constexpr D3D12_RESOURCE_FLAGS convert_dx(TextureUsage value) noexcept
{
    D3D12_RESOURCE_FLAGS output = {};
    if (value & TextureUsage::RenderTarget) {
        output |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if (value & TextureUsage::DepthStencil) {
        output |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    if (value & TextureUsage::CopySrc) {
        output |= D3D12_RESOURCE_FLAG_NONE;
    }
    if (value & TextureUsage::CopyDst) {
        output |= D3D12_RESOURCE_FLAG_NONE;
    }
    if (value & TextureUsage::ShaderResource) {
        output |= D3D12_RESOURCE_FLAG_NONE;
    }
    if (value & TextureUsage::UnorderedAccess) {
        output |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (value & TextureUsage::HostCopy) {
        output |= D3D12_RESOURCE_FLAG_NONE;
    }
    return output;
}
inline constexpr D3D12_FENCE_FLAGS convert_dx(FenceFlags value) noexcept
{
    D3D12_FENCE_FLAGS output = {};
    if (value & FenceFlags::Shared) {
        output |= D3D12_FENCE_FLAG_SHARED;
    }
    return output;
}
inline constexpr D3D12_PIPELINE_STATE_FLAGS convert_dx(PipelineFlags value) noexcept
{
    D3D12_PIPELINE_STATE_FLAGS output = {};
    return output;
}
} // namespace wis

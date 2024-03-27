#pragma once
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/util/flags.h>

namespace wis{
struct DX12BufferBarrier2{
    wis::BufferBarrier barrier;
    wis::DX12BufferView buffer;
};

struct DX12TextureBarrier2{
    wis::TextureBarrier barrier;
    wis::DX12TextureView texture;
};

struct DX12GraphicsShaderStages{
    wis::DX12ShaderView vertex;
    wis::DX12ShaderView hull;
    wis::DX12ShaderView domain;
    wis::DX12ShaderView geometry;
    wis::DX12ShaderView pixel;
};

struct DX12GraphicsPipelineDesc{
    wis::DX12RootSignatureView root_signature;
    wis::InputLayout input_layout;
    wis::DX12GraphicsShaderStages shaders;
    wis::RenderAttachmentsDesc attachments;
    wis::RasterizerDesc* rasterizer = nullptr;
    wis::SampleDesc* sample = nullptr;
    wis::BlendStateDesc* blend = nullptr;
    wis::DepthStencilDesc* depth_stencil = nullptr;
    wis::TopologyType topology_type = wis::TopologyType::Triangle;
};

struct DX12RenderPassRenderTargetDesc{
    wis::DX12RenderTargetView target;
    wis::LoadOperation load_op = wis::LoadOperation::Load;
    wis::StoreOperation store_op = wis::StoreOperation::Store;
    std::array<float, 4> clear_value {};
};

struct DX12RenderPassDesc{
    wis::DX12RenderPassRenderTargetDesc* targets;
    uint32_t target_count;
    wis::RenderPassFlags flags;
};

struct DX12VertexBufferBinding{
    wis::DX12BufferView buffer;
    uint32_t size;
    uint32_t stride;
};

inline constexpr DXGI_GPU_PREFERENCE convert_dx(AdapterPreference value) noexcept {
    return static_cast<DXGI_GPU_PREFERENCE>(value);
}
inline constexpr D3D12_SHADER_VISIBILITY convert_dx(ShaderStages value) noexcept {
    return static_cast<D3D12_SHADER_VISIBILITY>(value);
}
inline constexpr DXGI_FORMAT convert_dx(DataFormat value) noexcept {
    return static_cast<DXGI_FORMAT>(value);
}
inline constexpr D3D12_FILL_MODE convert_dx(FillMode value) noexcept {
    return static_cast<D3D12_FILL_MODE>(value);
}
inline constexpr D3D12_CULL_MODE convert_dx(CullMode value) noexcept {
    return static_cast<D3D12_CULL_MODE>(value);
}
inline constexpr BOOL convert_dx(WindingOrder value) noexcept {
    return static_cast<BOOL>(value);
}
inline constexpr uint32_t convert_dx(SampleRate value) noexcept {
    return static_cast<uint32_t>(value);
}
inline constexpr D3D12_COMPARISON_FUNC convert_dx(Compare value) noexcept {
    return static_cast<D3D12_COMPARISON_FUNC>(value);
}
inline constexpr D3D12_STENCIL_OP convert_dx(StencilOp value) noexcept {
    return static_cast<D3D12_STENCIL_OP>(value);
}
inline constexpr D3D12_BLEND convert_dx(BlendFactor value) noexcept {
    return static_cast<D3D12_BLEND>(value);
}
inline constexpr D3D12_BLEND_OP convert_dx(BlendOp value) noexcept {
    return static_cast<D3D12_BLEND_OP>(value);
}
inline constexpr D3D12_LOGIC_OP convert_dx(LogicOp value) noexcept {
    return static_cast<D3D12_LOGIC_OP>(value);
}
inline constexpr D3D12_BARRIER_SYNC convert_dx(BarrierSync value) noexcept{
    D3D12_BARRIER_SYNC output = {};
    if(value & BarrierSync::All) output |= D3D12_BARRIER_SYNC_ALL;
    if(value & BarrierSync::Draw) output |= D3D12_BARRIER_SYNC_DRAW;
    return output;
}
inline constexpr D3D12_BARRIER_ACCESS convert_dx(ResourceAccess value) noexcept{
    D3D12_BARRIER_ACCESS output = {};
    if(value & ResourceAccess::VertexBuffer) output |= D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
    if(value & ResourceAccess::ConstantBuffer) output |= D3D12_BARRIER_ACCESS_CONSTANT_BUFFER;
    if(value & ResourceAccess::IndexBuffer) output |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
    if(value & ResourceAccess::RenderTarget) output |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
    if(value & ResourceAccess::UnorderedAccess) output |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
    if(value & ResourceAccess::DepthWrite) output |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
    if(value & ResourceAccess::DepthRead) output |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
    if(value & ResourceAccess::ShaderResource) output |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
    if(value & ResourceAccess::StreamOutput) output |= D3D12_BARRIER_ACCESS_STREAM_OUTPUT;
    if(value & ResourceAccess::IndirectArgument) output |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
    if(value & ResourceAccess::CopyDest) output |= D3D12_BARRIER_ACCESS_COPY_DEST;
    if(value & ResourceAccess::CopySource) output |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
    if(value & ResourceAccess::ConditionalRendering) output |= D3D12_BARRIER_ACCESS_PREDICATION;
    if(value & ResourceAccess::AccelerationStrucureRead) output |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
    if(value & ResourceAccess::AccelerationStrucureWrite) output |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
    if(value & ResourceAccess::ShadingRate) output |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
    if(value & ResourceAccess::VideoDecodeRead) output |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_READ;
    if(value & ResourceAccess::VideoDecodeWrite) output |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE;
    if(value & ResourceAccess::Present) output |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE;
    if(value & ResourceAccess::NoAccess) output |= D3D12_BARRIER_ACCESS_NO_ACCESS;
    return output;
}
inline constexpr D3D12_BARRIER_LAYOUT convert_dx(TextureState value) noexcept{
    switch(value){
    default: return {};
    case TextureState::Undefined: return D3D12_BARRIER_LAYOUT_UNDEFINED;
    case TextureState::Common: return D3D12_BARRIER_LAYOUT_COMMON;
    case TextureState::Read: return D3D12_BARRIER_LAYOUT_GENERIC_READ;
    case TextureState::RenderTarget: return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
    case TextureState::UnorderedAccess: return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
    case TextureState::DepthStencilWrite: return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    case TextureState::DepthStencilRead: return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
    case TextureState::ShaderResource: return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
    case TextureState::CopySource: return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
    case TextureState::CopyDest: return D3D12_BARRIER_LAYOUT_COPY_DEST;
    case TextureState::Present: return D3D12_BARRIER_LAYOUT_PRESENT;
    case TextureState::ShadingRate: return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
    case TextureState::VideoDecodeRead: return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_READ;
    case TextureState::VideoDecodeWrite: return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_WRITE;
    }
}
inline constexpr D3D12_RENDER_PASS_FLAGS convert_dx(RenderPassFlags value) noexcept{
    D3D12_RENDER_PASS_FLAGS output = {};
    if(value & RenderPassFlags::Suspending) output |= D3D12_RENDER_PASS_FLAG_SUSPENDING_PASS;
    if(value & RenderPassFlags::Resuming) output |= D3D12_RENDER_PASS_FLAG_RESUMING_PASS;
    return output;
}
inline constexpr D3D12_DESCRIPTOR_HEAP_TYPE convert_dx(DescriptorHeapType value) noexcept {
    return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(value);
}
inline constexpr D3D12_DESCRIPTOR_HEAP_FLAGS convert_dx(DescriptorMemory value) noexcept {
    return static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(value);
}
inline constexpr D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE convert_dx(LoadOperation value) noexcept{
    switch(value){
    default: return {};
    case LoadOperation::Load: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
    case LoadOperation::Clear: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    case LoadOperation::DontCare: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
    }
}
inline constexpr D3D12_RENDER_PASS_ENDING_ACCESS_TYPE convert_dx(StoreOperation value) noexcept{
    switch(value){
    default: return {};
    case StoreOperation::Store: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
    case StoreOperation::DontCare: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
    case StoreOperation::Resolve: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
    }
}
inline constexpr D3D_PRIMITIVE_TOPOLOGY convert_dx(PrimitiveTopology value) noexcept{
    switch(value){
    default: return {};
    case PrimitiveTopology::PointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    case PrimitiveTopology::LineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case PrimitiveTopology::LineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case PrimitiveTopology::TriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case PrimitiveTopology::TriangleStrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case PrimitiveTopology::TriangleFan: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLEFAN;
    case PrimitiveTopology::LineListAdj: return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
    case PrimitiveTopology::LineStripAdj: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
    case PrimitiveTopology::TriangleListAdj: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
    case PrimitiveTopology::TriangleStripAdj: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
    }
}
inline constexpr D3D12_PRIMITIVE_TOPOLOGY_TYPE convert_dx(TopologyType value) noexcept{
    switch(value){
    default: return {};
    case TopologyType::Point: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
    case TopologyType::Line: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    case TopologyType::Triangle: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    case TopologyType::Patch: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
    }
}
inline constexpr uint32_t convert_dx(SampleCount value) noexcept {
    return static_cast<uint32_t>(value);
}
inline constexpr D3D12_RESOURCE_FLAGS convert_dx(TextureUsage value) noexcept{
    D3D12_RESOURCE_FLAGS output = {};
    if(value & TextureUsage::RenderTarget) output |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    if(value & TextureUsage::DepthStencil) output |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    if(value & TextureUsage::CopySrc) output |= D3D12_RESOURCE_FLAG_NONE;
    if(value & TextureUsage::CopyDst) output |= D3D12_RESOURCE_FLAG_NONE;
    if(value & TextureUsage::ShaderResource) output |= D3D12_RESOURCE_FLAG_NONE;
    if(value & TextureUsage::UnorderedAccess) output |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    return output;
}
}

#pragma once
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/util/flags.h>

namespace wis {
/**
 * @brief Variant of BufferBarrier with BufferView.
 * */
struct VKBufferBarrier2 {
    wis::BufferBarrier barrier; ///< Buffer barrier.
    wis::VKBufferView buffer; ///< Buffer view.
};

/**
 * @brief Variant of TextureBarrier with TextureView.
 * */
struct VKTextureBarrier2 {
    wis::TextureBarrier barrier; ///< Texture barrier.
    wis::VKTextureView texture; ///< Texture view.
};

/**
 * @brief Variant of ShaderView for all graphics stages.
 * */
struct VKGraphicsShaderStages {
    wis::VKShaderView vertex; ///< Vertex shader.
    wis::VKShaderView hull; ///< Hull shader.
    wis::VKShaderView domain; ///< Domain shader.
    wis::VKShaderView geometry; ///< Geometry shader.
    wis::VKShaderView pixel; ///< Pixel shader.
};

/**
 * @brief Variant of PipelineStateDesc for graphics pipeline.
 * */
struct VKGraphicsPipelineDesc {
    wis::VKRootSignatureView root_signature; ///< Root signature.
    wis::InputLayout input_layout; ///< Input layout.
    wis::VKGraphicsShaderStages shaders; ///< Shader stages.
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
struct VKRenderPassRenderTargetDesc {
    wis::VKRenderTargetView target; ///< Render target view.
    wis::LoadOperation load_op = wis::LoadOperation::Load; ///< Load operation on beginning of render pass. Default is wis::LoadOperation::Load.
    wis::StoreOperation store_op = wis::StoreOperation::Store; ///< Store operation on end of render pass. Default is wis::StoreOperation::Store.
    std::array<float, 4> clear_value{}; ///< Clear value for wis::LoadOperation::Clear.
};

/**
 * @brief Variant of RenderPassDesc for depth stencil.
 * */
struct VKRenderPassDepthStencilDesc {
    wis::VKRenderTargetView target; ///< Depth stencil view.
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
struct VKRenderPassDesc {
    wis::RenderPassFlags flags; ///< Render pass flags.
    /**
     * @brief View mask for Multiview feature. If multiview is not available it is ignored.
     * Value must be the same as in  upon pipeline creation. Otherwise behavior is undefined.
     * Default is 0. 0 means regular rendering.
     * */
    uint32_t view_mask = 0;
    uint32_t target_count; ///< Render target count.
    const wis::VKRenderPassRenderTargetDesc* targets = nullptr; ///< Render target descriptions.
    const wis::VKRenderPassDepthStencilDesc* depth_stencil = nullptr; ///< Depth stencil description.
};

/**
 * @brief Variant of BufferView for vertex buffer binding.
 * */
struct VKVertexBufferBinding {
    wis::VKBufferView buffer; ///< Buffer view.
    uint32_t size; ///< Size of the buffer in bytes.
    uint32_t stride; ///< Stride of the buffer in bytes.
    uint32_t offset; ///< Offset in buffer in bytes. Default is 0.
};

inline constexpr VkShaderStageFlagBits convert_vk(ShaderStages value) noexcept
{
    switch (value) {
    default:
        return VK_SHADER_STAGE_ALL;
    case ShaderStages::All:
        return VK_SHADER_STAGE_ALL;
    case ShaderStages::Vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderStages::Hull:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case ShaderStages::Domain:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case ShaderStages::Geometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case ShaderStages::Pixel:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderStages::Amplification:
        return VK_SHADER_STAGE_TASK_BIT_NV;
    case ShaderStages::Mesh:
        return VK_SHADER_STAGE_MESH_BIT_NV;
    }
}
inline constexpr VkDescriptorType convert_vk(DescriptorType value) noexcept
{
    switch (value) {
    default:
        return {};
    case DescriptorType::Sampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case DescriptorType::ConstantBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::Texture:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case DescriptorType::RWTexture:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case DescriptorType::RWBuffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case DescriptorType::Buffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }
}
inline constexpr VkFormat convert_vk(DataFormat value) noexcept
{
    switch (value) {
    default:
        return {};
    case DataFormat::RGBA32Float:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case DataFormat::RGBA32Uint:
        return VK_FORMAT_R32G32B32A32_UINT;
    case DataFormat::RGBA32Sint:
        return VK_FORMAT_R32G32B32A32_SINT;
    case DataFormat::RGB32Float:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case DataFormat::RGB32Uint:
        return VK_FORMAT_R32G32B32_UINT;
    case DataFormat::RGB32Sint:
        return VK_FORMAT_R32G32B32_SINT;
    case DataFormat::RGBA16Float:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    case DataFormat::RGBA16Unorm:
        return VK_FORMAT_R16G16B16A16_UNORM;
    case DataFormat::RGBA16Uint:
        return VK_FORMAT_R16G16B16A16_UINT;
    case DataFormat::RGBA16Snorm:
        return VK_FORMAT_R16G16B16A16_SNORM;
    case DataFormat::RGBA16Sint:
        return VK_FORMAT_R16G16B16A16_SINT;
    case DataFormat::RG32Float:
        return VK_FORMAT_R32G32_SFLOAT;
    case DataFormat::RG32Uint:
        return VK_FORMAT_R32G32_UINT;
    case DataFormat::RG32Sint:
        return VK_FORMAT_R32G32_SINT;
    case DataFormat::D32FloatS8Uint:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    case DataFormat::RGB10A2Unorm:
        return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    case DataFormat::RGB10A2Uint:
        return VK_FORMAT_A2B10G10R10_UINT_PACK32;
    case DataFormat::RG11B10Float:
        return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
    case DataFormat::RGBA8Unorm:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case DataFormat::RGBA8UnormSrgb:
        return VK_FORMAT_R8G8B8A8_SRGB;
    case DataFormat::RGBA8Uint:
        return VK_FORMAT_R8G8B8A8_UINT;
    case DataFormat::RGBA8Snorm:
        return VK_FORMAT_R8G8B8A8_SNORM;
    case DataFormat::RGBA8Sint:
        return VK_FORMAT_R8G8B8A8_SINT;
    case DataFormat::RG16Float:
        return VK_FORMAT_R16G16_SFLOAT;
    case DataFormat::RG16Unorm:
        return VK_FORMAT_R16G16_UNORM;
    case DataFormat::RG16Uint:
        return VK_FORMAT_R16G16_UINT;
    case DataFormat::RG16Snorm:
        return VK_FORMAT_R16G16_SNORM;
    case DataFormat::RG16Sint:
        return VK_FORMAT_R16G16_SINT;
    case DataFormat::D32Float:
        return VK_FORMAT_D32_SFLOAT;
    case DataFormat::R32Float:
        return VK_FORMAT_R32_SFLOAT;
    case DataFormat::R32Uint:
        return VK_FORMAT_R32_UINT;
    case DataFormat::R32Sint:
        return VK_FORMAT_R32_SINT;
    case DataFormat::D24UnormS8Uint:
        return VK_FORMAT_D24_UNORM_S8_UINT;
    case DataFormat::RG8Unorm:
        return VK_FORMAT_R8G8_UNORM;
    case DataFormat::RG8Uint:
        return VK_FORMAT_R8G8_UINT;
    case DataFormat::RG8Snorm:
        return VK_FORMAT_R8G8_SNORM;
    case DataFormat::RG8Sint:
        return VK_FORMAT_R8G8_SINT;
    case DataFormat::R16Float:
        return VK_FORMAT_R16_SFLOAT;
    case DataFormat::D16Unorm:
        return VK_FORMAT_D16_UNORM;
    case DataFormat::R16Unorm:
        return VK_FORMAT_R16_UNORM;
    case DataFormat::R16Uint:
        return VK_FORMAT_R16_UINT;
    case DataFormat::R16Snorm:
        return VK_FORMAT_R16_SNORM;
    case DataFormat::R16Sint:
        return VK_FORMAT_R16_SINT;
    case DataFormat::R8Unorm:
        return VK_FORMAT_R8_UNORM;
    case DataFormat::R8Uint:
        return VK_FORMAT_R8_UINT;
    case DataFormat::R8Snorm:
        return VK_FORMAT_R8_SNORM;
    case DataFormat::R8Sint:
        return VK_FORMAT_R8_SINT;
    case DataFormat::RGB9E5UFloat:
        return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
    case DataFormat::BC1RGBAUnorm:
        return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case DataFormat::BC1RGBAUnormSrgb:
        return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
    case DataFormat::BC2RGBAUnorm:
        return VK_FORMAT_BC2_UNORM_BLOCK;
    case DataFormat::BC2RGBAUnormSrgb:
        return VK_FORMAT_BC2_SRGB_BLOCK;
    case DataFormat::BC3RGBAUnorm:
        return VK_FORMAT_BC3_UNORM_BLOCK;
    case DataFormat::BC3RGBAUnormSrgb:
        return VK_FORMAT_BC3_SRGB_BLOCK;
    case DataFormat::BC4RUnorm:
        return VK_FORMAT_BC4_UNORM_BLOCK;
    case DataFormat::BC4RSnorm:
        return VK_FORMAT_BC4_SNORM_BLOCK;
    case DataFormat::BC5RGUnorm:
        return VK_FORMAT_BC5_UNORM_BLOCK;
    case DataFormat::BC5RGSnorm:
        return VK_FORMAT_BC5_SNORM_BLOCK;
    case DataFormat::B5G6R5Unorm:
        return VK_FORMAT_R5G6B5_UNORM_PACK16;
    case DataFormat::B5G5R5A1Unorm:
        return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
    case DataFormat::BGRA8Unorm:
        return VK_FORMAT_B8G8R8A8_UNORM;
    case DataFormat::BGRA8UnormSrgb:
        return VK_FORMAT_B8G8R8A8_SRGB;
    case DataFormat::BC6HUfloat16:
        return VK_FORMAT_BC6H_UFLOAT_BLOCK;
    case DataFormat::BC6HSfloat16:
        return VK_FORMAT_BC6H_SFLOAT_BLOCK;
    case DataFormat::BC7RGBAUnorm:
        return VK_FORMAT_BC7_UNORM_BLOCK;
    case DataFormat::BC7RGBAUnormSrgb:
        return VK_FORMAT_BC7_SRGB_BLOCK;
    case DataFormat::BGRA4Unorm:
        return VK_FORMAT_A4R4G4B4_UNORM_PACK16;
    }
}
inline constexpr VkPolygonMode convert_vk(FillMode value) noexcept
{
    switch (value) {
    default:
        return {};
    case FillMode::Lines:
        return VK_POLYGON_MODE_LINE;
    case FillMode::Solid:
        return VK_POLYGON_MODE_FILL;
    }
}
inline constexpr VkCullModeFlags convert_vk(CullMode value) noexcept
{
    switch (value) {
    default:
        return {};
    case CullMode::None:
        return VK_CULL_MODE_NONE;
    case CullMode::Front:
        return VK_CULL_MODE_FRONT_BIT;
    case CullMode::Back:
        return VK_CULL_MODE_BACK_BIT;
    }
}
inline constexpr VkFrontFace convert_vk(WindingOrder value) noexcept
{
    switch (value) {
    default:
        return {};
    case WindingOrder::Clockwise:
        return VK_FRONT_FACE_CLOCKWISE;
    case WindingOrder::CounterClockwise:
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }
}
inline constexpr VkSampleCountFlagBits convert_vk(SampleRate value) noexcept
{
    switch (value) {
    default:
        return {};
    case SampleRate::S1:
        return VK_SAMPLE_COUNT_1_BIT;
    case SampleRate::S2:
        return VK_SAMPLE_COUNT_2_BIT;
    case SampleRate::S4:
        return VK_SAMPLE_COUNT_4_BIT;
    case SampleRate::S8:
        return VK_SAMPLE_COUNT_8_BIT;
    case SampleRate::S16:
        return VK_SAMPLE_COUNT_16_BIT;
    }
}
inline constexpr VkCompareOp convert_vk(Compare value) noexcept
{
    switch (value) {
    default:
        return {};
    case Compare::None:
        return VK_COMPARE_OP_NEVER;
    case Compare::Never:
        return VK_COMPARE_OP_NEVER;
    case Compare::Less:
        return VK_COMPARE_OP_LESS;
    case Compare::Equal:
        return VK_COMPARE_OP_EQUAL;
    case Compare::LessEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case Compare::Greater:
        return VK_COMPARE_OP_GREATER;
    case Compare::NotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case Compare::GreaterEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case Compare::Always:
        return VK_COMPARE_OP_ALWAYS;
    }
}
inline constexpr VkStencilOp convert_vk(StencilOp value) noexcept
{
    switch (value) {
    default:
        return {};
    case StencilOp::Keep:
        return VK_STENCIL_OP_KEEP;
    case StencilOp::Zero:
        return VK_STENCIL_OP_ZERO;
    case StencilOp::Replace:
        return VK_STENCIL_OP_REPLACE;
    case StencilOp::IncClamp:
        return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case StencilOp::DecClamp:
        return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    case StencilOp::Invert:
        return VK_STENCIL_OP_INVERT;
    case StencilOp::IncWrap:
        return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case StencilOp::DecWrap:
        return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    }
}
inline constexpr VkBlendFactor convert_vk(BlendFactor value) noexcept
{
    switch (value) {
    default:
        return {};
    case BlendFactor::Zero:
        return VK_BLEND_FACTOR_ZERO;
    case BlendFactor::One:
        return VK_BLEND_FACTOR_ONE;
    case BlendFactor::SrcColor:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case BlendFactor::InvSrcColor:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case BlendFactor::SrcAlpha:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case BlendFactor::InvSrcAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DestAlpha:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case BlendFactor::InvDestAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case BlendFactor::DestColor:
        return VK_BLEND_FACTOR_DST_COLOR;
    case BlendFactor::InvDestColor:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlphaSat:
        return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    case BlendFactor::ConstantColor:
        return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case BlendFactor::InvBlendFactor:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::Src1Color:
        return VK_BLEND_FACTOR_SRC1_COLOR;
    case BlendFactor::InvSrc1Color:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
    case BlendFactor::Src1Alpha:
        return VK_BLEND_FACTOR_SRC1_ALPHA;
    case BlendFactor::InvSrc1Alpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
    }
}
inline constexpr VkBlendOp convert_vk(BlendOp value) noexcept
{
    switch (value) {
    default:
        return {};
    case BlendOp::Add:
        return VK_BLEND_OP_ADD;
    case BlendOp::Subtract:
        return VK_BLEND_OP_SUBTRACT;
    case BlendOp::RevSubtract:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    case BlendOp::Min:
        return VK_BLEND_OP_MIN;
    case BlendOp::Max:
        return VK_BLEND_OP_MAX;
    }
}
inline constexpr VkLogicOp convert_vk(LogicOp value) noexcept
{
    switch (value) {
    default:
        return {};
    case LogicOp::Clear:
        return VK_LOGIC_OP_CLEAR;
    case LogicOp::Set:
        return VK_LOGIC_OP_SET;
    case LogicOp::Copy:
        return VK_LOGIC_OP_COPY;
    case LogicOp::CopyInverted:
        return VK_LOGIC_OP_COPY_INVERTED;
    case LogicOp::Noop:
        return VK_LOGIC_OP_NO_OP;
    case LogicOp::Invert:
        return VK_LOGIC_OP_INVERT;
    case LogicOp::And:
        return VK_LOGIC_OP_AND;
    case LogicOp::Nand:
        return VK_LOGIC_OP_NAND;
    case LogicOp::Or:
        return VK_LOGIC_OP_OR;
    case LogicOp::Nor:
        return VK_LOGIC_OP_NOR;
    case LogicOp::Xor:
        return VK_LOGIC_OP_XOR;
    case LogicOp::Equiv:
        return VK_LOGIC_OP_EQUIVALENT;
    case LogicOp::AndReverse:
        return VK_LOGIC_OP_AND_REVERSE;
    case LogicOp::AndInverted:
        return VK_LOGIC_OP_AND_INVERTED;
    case LogicOp::OrReverse:
        return VK_LOGIC_OP_OR_REVERSE;
    case LogicOp::OrInverted:
        return VK_LOGIC_OP_OR_INVERTED;
    }
}
inline constexpr VkMemoryPropertyFlags convert_vk(MemoryType value) noexcept
{
    switch (value) {
    default:
        return {};
    case MemoryType::Default:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    case MemoryType::Upload:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    case MemoryType::Readback:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    case MemoryType::GPUUpload:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
}
inline constexpr VkImageLayout convert_vk(TextureState value) noexcept
{
    switch (value) {
    default:
        return {};
    case TextureState::Undefined:
        return VK_IMAGE_LAYOUT_UNDEFINED;
    case TextureState::Common:
        return VK_IMAGE_LAYOUT_GENERAL;
    case TextureState::Read:
        return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    case TextureState::RenderTarget:
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case TextureState::UnorderedAccess:
        return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    case TextureState::DepthStencilWrite:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case TextureState::DepthStencilRead:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case TextureState::ShaderResource:
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case TextureState::CopySource:
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case TextureState::CopyDest:
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case TextureState::Present:
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    case TextureState::ShadingRate:
        return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
    case TextureState::VideoDecodeRead:
        return VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
    case TextureState::VideoDecodeWrite:
        return VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
    }
}
inline constexpr VkAttachmentLoadOp convert_vk(LoadOperation value) noexcept
{
    switch (value) {
    default:
        return {};
    case LoadOperation::Load:
        return VK_ATTACHMENT_LOAD_OP_LOAD;
    case LoadOperation::Clear:
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    case LoadOperation::DontCare:
        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
}
inline constexpr VkAttachmentStoreOp convert_vk(StoreOperation value) noexcept
{
    switch (value) {
    default:
        return {};
    case StoreOperation::Store:
        return VK_ATTACHMENT_STORE_OP_STORE;
    case StoreOperation::DontCare:
        return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    case StoreOperation::Resolve:
        return VK_ATTACHMENT_STORE_OP_STORE;
    }
}
inline constexpr VkPrimitiveTopology convert_vk(PrimitiveTopology value) noexcept
{
    switch (value) {
    default:
        return {};
    case PrimitiveTopology::PointList:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveTopology::LineList:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveTopology::LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveTopology::TriangleList:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveTopology::TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    case PrimitiveTopology::TriangleFan:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    case PrimitiveTopology::LineListAdj:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
    case PrimitiveTopology::LineStripAdj:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
    case PrimitiveTopology::TriangleListAdj:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
    case PrimitiveTopology::TriangleStripAdj:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
    }
}
inline constexpr VkPrimitiveTopology convert_vk(TopologyType value) noexcept
{
    switch (value) {
    default:
        return {};
    case TopologyType::Point:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case TopologyType::Line:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case TopologyType::Triangle:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case TopologyType::Patch:
        return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
    }
}
inline constexpr VkFilter convert_vk(Filter value) noexcept
{
    switch (value) {
    default:
        return {};
    case Filter::Point:
        return VK_FILTER_NEAREST;
    case Filter::Linear:
        return VK_FILTER_LINEAR;
    }
}
inline constexpr VkSamplerAddressMode convert_vk(AddressMode value) noexcept
{
    switch (value) {
    default:
        return {};
    case AddressMode::Repeat:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case AddressMode::MirroredRepeat:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case AddressMode::ClampToEdge:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case AddressMode::ClampToBorder:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case AddressMode::MirrorClampToEdge:
        return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    }
}
inline constexpr VkImageViewType convert_vk(TextureViewType value) noexcept
{
    switch (value) {
    default:
        return {};
    case TextureViewType::Texture1D:
        return VK_IMAGE_VIEW_TYPE_1D;
    case TextureViewType::Texture1DArray:
        return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
    case TextureViewType::Texture2D:
        return VK_IMAGE_VIEW_TYPE_2D;
    case TextureViewType::Texture2DArray:
        return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    case TextureViewType::Texture2DMS:
        return VK_IMAGE_VIEW_TYPE_2D;
    case TextureViewType::Texture2DMSArray:
        return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    case TextureViewType::Texture3D:
        return VK_IMAGE_VIEW_TYPE_3D;
    case TextureViewType::TextureCube:
        return VK_IMAGE_VIEW_TYPE_CUBE;
    case TextureViewType::TextureCubeArray:
        return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
    }
}
inline constexpr VkComponentSwizzle convert_vk(ComponentSwizzle value) noexcept
{
    switch (value) {
    default:
        return {};
    case ComponentSwizzle::Red:
        return VK_COMPONENT_SWIZZLE_R;
    case ComponentSwizzle::Green:
        return VK_COMPONENT_SWIZZLE_G;
    case ComponentSwizzle::Blue:
        return VK_COMPONENT_SWIZZLE_B;
    case ComponentSwizzle::Alpha:
        return VK_COMPONENT_SWIZZLE_A;
    case ComponentSwizzle::Zero:
        return VK_COMPONENT_SWIZZLE_ZERO;
    case ComponentSwizzle::One:
        return VK_COMPONENT_SWIZZLE_ONE;
    }
}
inline constexpr VkIndexType convert_vk(IndexType value) noexcept
{
    switch (value) {
    default:
        return {};
    case IndexType::UInt16:
        return VK_INDEX_TYPE_UINT16;
    case IndexType::UInt32:
        return VK_INDEX_TYPE_UINT32;
    }
}
inline constexpr VmaAllocationCreateFlags convert_vk(MemoryFlags value) noexcept
{
    VmaAllocationCreateFlags output = {};
    if (value & MemoryFlags::DedicatedAllocation) {
        output |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    }
    if (value & MemoryFlags::Mapped) {
        output |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }
    return output;
}
inline constexpr VkPipelineStageFlags2 convert_vk(BarrierSync value) noexcept
{
    VkPipelineStageFlags2 output = {};
    if (value & BarrierSync::All) {
        output |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    }
    if (value & BarrierSync::Draw) {
        output |= VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT | VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT | VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (value & BarrierSync::IndexInput) {
        output |= VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT;
    }
    if (value & BarrierSync::VertexShading) {
        output |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
    }
    if (value & BarrierSync::PixelShading) {
        output |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    }
    if (value & BarrierSync::DepthStencil) {
        output |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
    }
    if (value & BarrierSync::RenderTarget) {
        output |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (value & BarrierSync::Compute) {
        output |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & BarrierSync::Raytracing) {
        output |= VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR;
    }
    if (value & BarrierSync::Copy) {
        output |= VK_PIPELINE_STAGE_2_COPY_BIT;
    }
    if (value & BarrierSync::Resolve) {
        output |= VK_PIPELINE_STAGE_2_COPY_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_RESOLVE_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & BarrierSync::ExecuteIndirect) {
        output |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & BarrierSync::AllShading) {
        output |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & BarrierSync::NonPixelShading) {
        output |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & BarrierSync::ClearUAV) {
        output |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & BarrierSync::VideoDecode) {
        output |= VK_PIPELINE_STAGE_2_VIDEO_DECODE_BIT_KHR;
    }
    if (value & BarrierSync::VideoEncode) {
        output |= VK_PIPELINE_STAGE_2_VIDEO_ENCODE_BIT_KHR;
    }
    if (value & BarrierSync::BuildRTAS) {
        output |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
    }
    if (value & BarrierSync::CopyRTAS) {
        output |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_COPY_BIT_KHR;
    }
    return output;
}
inline constexpr VkAccessFlags2 convert_vk(ResourceAccess value) noexcept
{
    VkAccessFlags2 output = {};
    if (value & ResourceAccess::VertexBuffer) {
        output |= VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
    }
    if (value & ResourceAccess::ConstantBuffer) {
        output |= VK_ACCESS_2_UNIFORM_READ_BIT;
    }
    if (value & ResourceAccess::IndexBuffer) {
        output |= VK_ACCESS_2_INDEX_READ_BIT;
    }
    if (value & ResourceAccess::RenderTarget) {
        output |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (value & ResourceAccess::UnorderedAccess) {
        output |= VK_ACCESS_2_SHADER_WRITE_BIT | VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & ResourceAccess::DepthWrite) {
        output |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    if (value & ResourceAccess::DepthRead) {
        output |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }
    if (value & ResourceAccess::ShaderResource) {
        output |= VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & ResourceAccess::StreamOutput) {
        output |= VK_ACCESS_2_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
    }
    if (value & ResourceAccess::IndirectArgument) {
        output |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & ResourceAccess::CopyDest) {
        output |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
    }
    if (value & ResourceAccess::CopySource) {
        output |= VK_ACCESS_2_TRANSFER_READ_BIT;
    }
    if (value & ResourceAccess::ConditionalRendering) {
        output |= VK_ACCESS_2_CONDITIONAL_RENDERING_READ_BIT_EXT;
    }
    if (value & ResourceAccess::AccelerationStrucureRead) {
        output |= VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR;
    }
    if (value & ResourceAccess::AccelerationStrucureWrite) {
        output |= VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
    }
    if (value & ResourceAccess::ShadingRate) {
        output |= VK_ACCESS_2_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
    }
    if (value & ResourceAccess::VideoDecodeRead) {
        output |= VK_ACCESS_2_VIDEO_DECODE_READ_BIT_KHR;
    }
    if (value & ResourceAccess::VideoDecodeWrite) {
        output |= VK_ACCESS_2_VIDEO_DECODE_WRITE_BIT_KHR;
    }
    if (value & ResourceAccess::ResolveDest) {
        output |= VK_ACCESS_2_TRANSFER_WRITE_BIT | VK_ACCESS_2_SHADER_WRITE_BIT;
    }
    if (value & ResourceAccess::ResolveSource) {
        output |= VK_ACCESS_2_TRANSFER_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & ResourceAccess::NoAccess) {
        output |= VK_ACCESS_2_NONE;
    }
    return output;
}
inline constexpr VkRenderingFlags convert_vk(RenderPassFlags value) noexcept
{
    VkRenderingFlags output = {};
    if (value & RenderPassFlags::Suspending) {
        output |= VK_RENDERING_SUSPENDING_BIT;
    }
    if (value & RenderPassFlags::Resuming) {
        output |= VK_RENDERING_RESUMING_BIT;
    }
    return output;
}
inline constexpr VkImageUsageFlags convert_vk(TextureUsage value) noexcept
{
    VkImageUsageFlags output = {};
    if (value & TextureUsage::RenderTarget) {
        output |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if (value & TextureUsage::DepthStencil) {
        output |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (value & TextureUsage::CopySrc) {
        output |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if (value & TextureUsage::CopyDst) {
        output |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if (value & TextureUsage::ShaderResource) {
        output |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (value & TextureUsage::UnorderedAccess) {
        output |= VK_IMAGE_USAGE_STORAGE_BIT;
    }
    if (value & TextureUsage::HostCopy) {
        output |= VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT;
    }
    return output;
}
inline constexpr VkPipelineCreateFlags convert_vk(PipelineFlags value) noexcept
{
    VkPipelineCreateFlags output = {};
    if (value & PipelineFlags::DescriptorBuffer) {
        output |= VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
    }
    return output;
}
} // namespace wis

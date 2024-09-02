// GENERATED
#pragma once
#include <array>
#include <cstdint>

namespace wis {
struct Result;
struct Size2D;
struct Size3D;
struct BufferRegion;
struct AdapterDesc;
struct InputSlotDesc;
struct InputAttribute;
struct InputLayout;
struct RasterizerDesc;
struct SampleDesc;
struct StencilDesc;
struct DepthStencilDesc;
struct BlendAttachmentDesc;
struct BlendStateDesc;
struct RenderAttachmentsDesc;
struct RootConstant;
struct SwapchainDesc;
struct TextureDesc;
struct AllocationInfo;
struct TextureRegion;
struct BufferTextureCopyRegion;
struct PushDescriptor;
struct SubresourceRange;
struct RenderTargetDesc;
struct Viewport;
struct Scissor;
struct BufferBarrier;
struct TextureBarrier;
struct DescriptorTableEntry;
struct DescriptorTable;
struct SamplerDesc;
struct ComponentMapping;
struct ShaderResourceDesc;

enum class ShaderStages {
    All = 0,
    Vertex = 1,
    Hull = 2,
    Domain = 3,
    Geometry = 4,
    Pixel = 5,
    Amplification = 6,
    Mesh = 7,
    Count = 8,
};

/**
 * @brief Represents main error communication source for the API.
 * Compare against wis::Status::Ok for success.
 *
 * */
enum class Status : int32_t {
    Ok = 0, ///< Operation succeded.
    /**
     * @brief Operation timed out.
     * Counts as a success value in some cases.
     * */
    Timeout = 1,
    /**
     * @brief Operation partially succeeded.
     * Some times it means that core value is initialized,
     * but some functionality may be missing and may require
     * you to check if demanded functionality is in place
     * */
    Partial = 2,
    Error = -1,
    InvalidArgument = -2,
    OutOfMemory = -3,
    DeviceLost = -4,
    Occluded = -5,
};

enum class QueuePriority {
    Common = 0,
    High = 100,
    Realtime = 10000,
};

enum class MutiWaitFlags : uint32_t {
    All = 0,
    Any = 1,
};

enum class DescriptorType {
    ShaderResource = 0,
    ConstantBuffer = 1,
    UnorderedAccess = 2,
    Sampler = 3,
};

enum class QueueType : uint32_t {
    Graphics = 0,
    DX12Bundle = 1,
    Compute = 2,
    Copy = 3,
    VideoDecode = 4,
    DX12VideoProcess = 5,
    DX12VideoEncode = 6,
};

enum class AdapterPreference {
    None = 0,
    MinConsumption = 1,
    Performance = 2,
};

enum class Severity {
    Debug = 0,
    Trace = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5,
};

enum class InputClass {
    PerVertex = 0,
    PerInstance = 1,
};

enum class CullMode {
    None = 1,
    Front = 2,
    Back = 3,
};

enum class DataFormat {
    Unknown = 0,
    RGBA32Float = 2,
    RGBA32Uint = 3,
    RGBA32Sint = 4,
    RGB32Float = 6,
    RGB32Uint = 7,
    RGB32Sint = 8,
    RGBA16Float = 10,
    RGBA16Unorm = 11,
    RGBA16Uint = 12,
    RGBA16Snorm = 13,
    RGBA16Sint = 14,
    RG32Float = 16,
    RG32Uint = 17,
    RG32Sint = 18,
    D32FloatS8Uint = 20,
    RGB10A2Unorm = 24,
    RGB10A2Uint = 25,
    RG11B10Float = 26,
    RGBA8Unorm = 28,
    RGBA8UnormSrgb = 29,
    RGBA8Uint = 30,
    RGBA8Snorm = 31,
    RGBA8Sint = 32,
    RG16Float = 34,
    RG16Unorm = 35,
    RG16Uint = 36,
    RG16Snorm = 37,
    RG16Sint = 38,
    D32Float = 40,
    R32Float = 41,
    R32Uint = 42,
    R32Sint = 43,
    D24UnormS8Uint = 45,
    RG8Unorm = 49,
    RG8Uint = 50,
    RG8Snorm = 51,
    RG8Sint = 52,
    R16Float = 54,
    D16Unorm = 55,
    R16Unorm = 56,
    R16Uint = 57,
    R16Snorm = 58,
    R16Sint = 59,
    R8Unorm = 61,
    R8Uint = 62,
    R8Snorm = 63,
    R8Sint = 64,
    RGB9E5UFloat = 67,
    RG8BG8Unorm = 68,
    GR8GB8Unorm = 69,
    BC1RGBAUnorm = 71,
    BC1RGBAUnormSrgb = 72,
    BC2RGBAUnorm = 74,
    BC2RGBAUnormSrgb = 75,
    BC3RGBAUnorm = 77,
    BC3RGBAUnormSrgb = 78,
    BC4RUnorm = 80,
    BC4RSnorm = 81,
    BC5RGUnorm = 83,
    BC5RGSnorm = 84,
    B5R6G5Unorm = 85,
    B5G5R5A1Unorm = 86,
    BGRA8Unorm = 87,
    BGRA8UnormSrgb = 91,
    BC6HUfloat16 = 95,
    BC6HSfloat16 = 96,
    BC7RGBAUnorm = 98,
    BC7RGBAUnormSrgb = 99,
};

enum class FillMode {
    Lines = 2,
    Solid = 3,
};

enum class DescriptorMemory {
    CpuOnly = 0,
    ShaderVisible = 1,
};

enum class WindingOrder {
    Clockwise = 0,
    CounterClockwise = 1,
};

enum class SampleRate {
    S1 = 1,
    S2 = 2,
    S4 = 4,
    S8 = 8,
    S16 = 16,
};

enum class Compare {
    None = 0,
    Never = 1,
    Less = 2,
    Equal = 3,
    LessEqual = 4,
    Greater = 5,
    NotEqual = 6,
    GreaterEqual = 7,
    Always = 8,
};

enum class StencilOp {
    Keep = 1,
    Zero = 2,
    Replace = 3,
    IncClamp = 4,
    DecClamp = 5,
    Invert = 6,
    IncWrap = 7,
    DecWrap = 8,
};

enum class BlendFactor {
    Zero = 1,
    One = 2,
    SrcColor = 3,
    InvSrcColor = 4,
    SrcAlpha = 5,
    InvSrcAlpha = 6,
    DestAlpha = 7,
    InvDestAlpha = 8,
    DestColor = 9,
    InvDestColor = 10,
    SrcAlphaSat = 11,
    BlendFactor = 14,
    InvBlendFactor = 15,
    Src1Color = 16,
    InvSrc1Color = 17,
    Src1Alpha = 18,
    InvSrc1Alpha = 19,
};

enum class BlendOp {
    Add = 1,
    Subtract = 2,
    RevSubtract = 3,
    Min = 4,
    Max = 5,
};

enum class SampleCount {
    S1 = 1,
    S2 = 2,
    S4 = 4,
    S8 = 8,
    S16 = 16,
    S32 = 32,
    S64 = 64,
};

enum class LogicOp {
    Clear = 0,
    Set = 1,
    Copy = 2,
    CopyInverted = 3,
    Noop = 4,
    Invert = 5,
    And = 6,
    Nand = 7,
    Or = 8,
    Nor = 9,
    Xor = 10,
    Equiv = 11,
    AndReverse = 12,
    AndInverted = 13,
    OrReverse = 14,
    OrInverted = 15,
};

enum class MemoryType {
    Default = 0,
    Upload = 1,
    Readback = 2,
    GPUUpload = 3,
};

enum class ShaderIntermediate {
    DXIL = 0,
    SPIRV = 1,
};

enum class TextureState : uint32_t {
    Undefined = 4294967295,
    Common = 0,
    Read = 1,
    RenderTarget = 2,
    UnorderedAccess = 3,
    DepthStencilWrite = 4,
    DepthStencilRead = 5,
    ShaderResource = 6,
    CopySource = 7,
    CopyDest = 8,
    Present = 9,
    ShadingRate = 10,
    VideoDecodeRead = 11,
    VideoDecodeWrite = 12,
};

enum class LoadOperation {
    Load = 0,
    Clear = 1,
    DontCare = 2,
};

enum class TextureLayout {
    Texture1D = 2,
    Texture1DArray = 3,
    Texture2D = 4,
    Texture2DArray = 5,
    Texture2DMS = 6,
    Texture2DMSArray = 7,
    Texture3D = 8,
};

enum class DescriptorHeapType {
    Descriptor = 0,
    Sampler = 1,
};

enum class StoreOperation {
    Store = 0,
    DontCare = 1,
    Resolve = 2,
};

enum class PrimitiveTopology {
    PointList = 1,
    LineList = 2,
    LineStrip = 3,
    TriangleList = 4,
    TriangleStrip = 5,
    TriangleFan = 6,
    LineListAdj = 10,
    LineStripAdj = 11,
    TriangleListAdj = 12,
    TriangleStripAdj = 13,
};

enum class TopologyType {
    Point = 1,
    Line = 2,
    Triangle = 3,
    Patch = 4,
};

enum class Filter {
    Point = 0,
    Linear = 1,
};

enum class AddressMode {
    Repeat = 0,
    MirroredRepeat = 1,
    ClampToEdge = 2,
    ClampToBorder = 3,
    MirrorClampToEdge = 4,
};

enum class TextureViewType {
    Texture1D = 0,
    Texture1DArray = 1,
    Texture2D = 2,
    Texture2DArray = 3,
    Texture2DMS = 4,
    Texture2DMSArray = 5,
    Texture3D = 6,
    TextureCube = 7,
    TextureCubeArray = 8,
};

enum class ComponentSwizzle : uint8_t {
    Red = 0,
    Green = 1,
    Blue = 2,
    Alpha = 3,
    Zero = 4,
    One = 5,
};

enum class AdapterFlags {
    None = 0x0,
    Remote = 1 << 0,
    Software = 1 << 1,
    DX12ACGCompatible = 1 << 2,
    DX12SupportsMonitoredFences = 1 << 3,
    DX12SupportsNonMonitoredFences = 1 << 4,
    DX12KeyedMutexConformance = 1 << 5,
};

enum class DSSelect {
    None = 0x0,
    Depth = 1 << 0,
    Stencil = 1 << 1,
    DepthStencil = 0x3,
};

enum class ColorComponents {
    None = 0x0,
    R = 1 << 0,
    G = 1 << 1,
    B = 1 << 2,
    A = 1 << 3,
    All = 0xF,
};

enum class BufferUsage {
    None = 0x0,
    CopySrc = 1 << 0,
    CopyDst = 1 << 1,
    ConstantBuffer = 1 << 4,
    IndexBuffer = 1 << 6,
    VertexBuffer = 1 << 7,
};

enum class MemoryFlags {
    None = 0x0,
    DedicatedAllocation = 1 << 0,
    Mapped = 1 << 1,
};

enum class RenderPassFlags {
    None = 0x0,
    Suspending = 1 << 1,
    Resuming = 1 << 2,
};

enum class BarrierSync {
    None = 0x0,
    All = 1 << 0,
    Draw = 1 << 1,
    IndexInput = 1 << 2,
    VertexShading = 1 << 3,
    PixelShading = 1 << 4,
    DepthStencil = 1 << 5,
    RenderTarget = 1 << 6,
    Compute = 1 << 7,
    Raytracing = 1 << 8,
    Copy = 1 << 9,
    Resolve = 1 << 10,
    ExecuteIndirect = 1 << 11,
    AllShading = 1 << 12,
    NonPixelShading = 1 << 13,
    ClearUAV = 1 << 14,
    VideoDecode = 1 << 15,
    VideoEncode = 1 << 16,
    BuildRTAS = 1 << 17,
    CopyRTAS = 1 << 18,
};

enum class ResourceAccess {
    Common = 0x0,
    VertexBuffer = 1 << 0,
    ConstantBuffer = 1 << 1,
    IndexBuffer = 1 << 2,
    RenderTarget = 1 << 3,
    UnorderedAccess = 1 << 4,
    DepthWrite = 1 << 5,
    DepthRead = 1 << 6,
    ShaderResource = 1 << 7,
    StreamOutput = 1 << 8,
    IndirectArgument = 1 << 9,
    CopyDest = 1 << 10,
    CopySource = 1 << 11,
    ConditionalRendering = 1 << 12,
    AccelerationStrucureRead = 1 << 13,
    AccelerationStrucureWrite = 1 << 14,
    ShadingRate = 1 << 15,
    VideoDecodeRead = 1 << 16,
    VideoDecodeWrite = 1 << 17,
    Present = 1 << 18,
    ResolveDest = 1 << 19,
    ResolveSource = 1 << 20,
    NoAccess = 1 << 31,
};

enum class TextureUsage {
    None = 0x0,
    RenderTarget = 1 << 0,
    DepthStencil = 1 << 1,
    CopySrc = 1 << 2,
    CopyDst = 1 << 3,
    ShaderResource = 1 << 4,
    UnorderedAccess = 1 << 5,
    HostCopy = 1 << 7,
};

struct Result {
    wis::Status status = wis::Status::Ok;
    const char* error = nullptr;
};

struct Size2D {
    uint32_t width;
    uint32_t height;
};

struct Size3D {
    uint32_t width;
    uint32_t height;
    uint32_t depth_or_layers;
};

struct BufferRegion {
    uint64_t src_offset = 0;
    uint64_t dst_offset = 0;
    uint64_t size_bytes;
};

struct AdapterDesc {
    std::array<const char, 256> description{};
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision;
    uint64_t dedicated_video_memory;
    uint64_t dedicated_system_memory;
    uint64_t shared_system_memory;
    uint64_t adapter_id;
    wis::AdapterFlags flags;
};

struct InputSlotDesc {
    uint32_t slot;
    uint32_t stride_bytes;
    wis::InputClass input_class;
};

struct InputAttribute {
    uint32_t input_slot;
    const char* semantic_name;
    uint32_t semantic_index;
    uint32_t location;
    wis::DataFormat format;
    uint32_t offset_bytes;
};

struct InputLayout {
    wis::InputSlotDesc* slots;
    uint32_t slot_count;
    wis::InputAttribute* attributes;
    uint32_t attribute_count;
};

struct RasterizerDesc {
    wis::FillMode fill_mode = wis::FillMode::Solid;
    wis::CullMode cull_mode = wis::CullMode::Back;
    wis::WindingOrder front_face = wis::WindingOrder::Clockwise;
    bool depth_bias_enable = false;
    float depth_bias = 0.0f;
    float depth_bias_clamp = 0.0f;
    float depth_bias_slope_factor = 0.0f;
    bool depth_clip_enable = true;
};

struct SampleDesc {
    wis::SampleRate rate = wis::SampleRate::S1;
    float quality = 0.0f;
    uint32_t sample_mask = 0xffffffff;
};

struct StencilDesc {
    wis::StencilOp fail_op = wis::StencilOp::Keep;
    wis::StencilOp depth_fail_op = wis::StencilOp::Keep;
    wis::StencilOp pass_op = wis::StencilOp::Keep;
    wis::Compare comparison = wis::Compare::Always;
    uint8_t read_mask = 0xff;
    uint8_t write_mask = 0xff;
};

struct DepthStencilDesc {
    bool depth_enable = false;
    bool depth_write_enable = false;
    wis::Compare depth_comp = wis::Compare::Less;
    bool stencil_enable = false;
    wis::StencilDesc stencil_front = {};
    wis::StencilDesc stencil_back = {};
    bool depth_bound_test = false;
};

struct BlendAttachmentDesc {
    bool blend_enable = false;
    wis::BlendFactor src_color_blend = wis::BlendFactor::One;
    wis::BlendFactor dst_color_blend = wis::BlendFactor::Zero;
    wis::BlendOp color_blend_op = wis::BlendOp::Add;
    wis::BlendFactor src_alpha_blend = wis::BlendFactor::One;
    wis::BlendFactor dst_alpha_blend = wis::BlendFactor::Zero;
    wis::BlendOp alpha_blend_op = wis::BlendOp::Add;
    wis::ColorComponents color_write_mask = wis::ColorComponents::All;
};

struct BlendStateDesc {
    bool logic_op_enable = false;
    wis::LogicOp logic_op = wis::LogicOp::Noop;
    std::array<wis::BlendAttachmentDesc, 8> attachments{};
    uint32_t attachment_count;
};

struct RenderAttachmentsDesc {
    wis::DataFormat* attachment_formats;
    uint32_t attachments_count;
    wis::DataFormat depth_attachment;
};

struct RootConstant {
    wis::ShaderStages stage;
    uint32_t size_bytes;
};

struct SwapchainDesc {
    wis::Size2D size;
    wis::DataFormat format;
    uint32_t buffer_count;
    bool stereo;
    bool vsync;
};

struct TextureDesc {
    wis::DataFormat format;
    wis::Size3D size;
    uint32_t mip_levels;
    wis::TextureLayout layout = wis::TextureLayout::Texture2D;
    wis::SampleCount sample_count = wis::SampleCount::S1;
    wis::TextureUsage usage = wis::TextureUsage::None;
};

struct AllocationInfo {
    uint64_t size_bytes;
    uint64_t alignment_bytes;
};

struct TextureRegion {
    wis::Size3D offset;
    wis::Size3D size;
    uint32_t mip;
    uint32_t array_layer;
    wis::DataFormat format;
};

struct BufferTextureCopyRegion {
    uint64_t buffer_offset;
    wis::TextureRegion texture;
};

struct PushDescriptor {
    wis::ShaderStages stage;
    uint32_t bind_register;
    wis::DescriptorType type;
    uint32_t reserved;
};

struct SubresourceRange {
    uint32_t base_mip_level;
    uint32_t level_count;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct RenderTargetDesc {
    wis::DataFormat format;
    wis::TextureLayout layout;
    uint32_t mip;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct Viewport {
    float top_leftx;
    float top_lefty;
    float width;
    float height;
    float min_depth;
    float max_depth;
};

struct Scissor {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct BufferBarrier {
    wis::BarrierSync sync_before;
    wis::BarrierSync sync_after;
    wis::ResourceAccess access_before;
    wis::ResourceAccess access_after;
    uint64_t offset = 0;
    uint64_t size = UINT64_MAX;
};

struct TextureBarrier {
    wis::BarrierSync sync_before;
    wis::BarrierSync sync_after;
    wis::ResourceAccess access_before;
    wis::ResourceAccess access_after;
    wis::TextureState state_before;
    wis::TextureState state_after;
    wis::SubresourceRange subresource_range;
};

struct DescriptorTableEntry {
    wis::DescriptorType type;
    uint32_t bind_register;
    uint32_t binding;
    uint32_t count;
};

struct DescriptorTable {
    wis::DescriptorHeapType type;
    wis::DescriptorTableEntry* entries;
    uint32_t entry_count;
    wis::ShaderStages stage;
};

struct SamplerDesc {
    wis::Filter min_filter;
    wis::Filter mag_filter;
    wis::Filter mip_filter;
    bool anisotropic;
    uint32_t max_anisotropy;
    wis::AddressMode address_u;
    wis::AddressMode address_v;
    wis::AddressMode address_w;
    float min_lod;
    float max_lod;
    float mip_lod_bias;
    wis::Compare comparison_op;
    std::array<float, 4> border_color{};
};

struct ComponentMapping {
    wis::ComponentSwizzle r = wis::ComponentSwizzle::Red;
    wis::ComponentSwizzle g = wis::ComponentSwizzle::Green;
    wis::ComponentSwizzle b = wis::ComponentSwizzle::Blue;
    wis::ComponentSwizzle a = wis::ComponentSwizzle::Alpha;
};

struct ShaderResourceDesc {
    wis::DataFormat format;
    wis::TextureViewType view_type;
    wis::ComponentMapping component_mapping;
    wis::SubresourceRange subresource_range;
};

//=================================DELEGATES=================================

typedef void (*DebugCallback)(wis::Severity severity, const char* message, void* user_data);
//==============================TYPE TRAITS==============================

template<typename T>
struct is_flag_enum : public std::false_type {
};
template<>
struct is_flag_enum<wis::AdapterFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::DSSelect> : public std::true_type {
};
template<>
struct is_flag_enum<wis::ColorComponents> : public std::true_type {
};
template<>
struct is_flag_enum<wis::BufferUsage> : public std::true_type {
};
template<>
struct is_flag_enum<wis::MemoryFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::RenderPassFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::BarrierSync> : public std::true_type {
};
template<>
struct is_flag_enum<wis::ResourceAccess> : public std::true_type {
};
template<>
struct is_flag_enum<wis::TextureUsage> : public std::true_type {
};
//============================== CONSTS ==============================

static inline constexpr Result success{
    wis::Status::Ok, "Operation succeeded"
};

template<typename RetTy>
struct ResultValue {
    Result status;
    RetTy value;

    constexpr ResultValue(RetTy value) noexcept
        : status(success), value(std::move(value)) { }
    constexpr ResultValue(Result status) noexcept
        : status(status) { }
    constexpr ResultValue(Result status, RetTy value) noexcept
        : status(status), value(std::move(value)) { }
};

template<uint32_t s, typename RetTy>
constexpr decltype(auto) get(ResultValue<RetTy>& rv) noexcept
{
    if constexpr (s == 0)
        return std::forward<Result>(rv.status);
    else
        return std::forward<RetTy>(rv.value);
}

} // namespace wis

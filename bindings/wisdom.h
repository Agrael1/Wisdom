// GENERATED
#pragma once
#include "wisdom_exports.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum WisShaderStages {
    ShaderStagesAll = 0,
    ShaderStagesVertex = 1,
    ShaderStagesHull = 2,
    ShaderStagesDomain = 3,
    ShaderStagesGeometry = 4,
    ShaderStagesPixel = 5,
    ShaderStagesAmplification = 6,
    ShaderStagesMesh = 7,
    ShaderStagesCount = 8,
};

enum WisStatus {
    StatusOk = 0,
    StatusTimeout = 1,
    StatusError = -1,
    StatusInvalidArgument = -2,
    StatusOutOfMemory = -3,
    StatusDeviceLost = -4,
    StatusOccluded = -5,
};

enum WisQueuePriority {
    QueuePriorityCommon = 0,
    QueuePriorityHigh = 100,
    QueuePriorityRealtime = 10000,
};

enum WisMutiWaitFlags {
    MutiWaitFlagsAll = 0,
    MutiWaitFlagsAny = 1,
};

enum WisDescriptorType {
    DescriptorTypeShaderResource = 0,
    DescriptorTypeConstantBuffer = 1,
    DescriptorTypeUnorderedAccess = 2,
    DescriptorTypeSampler = 3,
};

enum WisQueueType {
    QueueTypeGraphics = 0,
    QueueTypeDX12Bundle = 1,
    QueueTypeCompute = 2,
    QueueTypeCopy = 3,
    QueueTypeVideoDecode = 4,
    QueueTypeDX12VideoProcess = 5,
    QueueTypeDX12VideoEncode = 6,
};

enum WisAdapterPreference {
    AdapterPreferenceNone = 0,
    AdapterPreferenceMinConsumption = 1,
    AdapterPreferencePerformance = 2,
};

enum WisSeverity {
    SeverityDebug = 0,
    SeverityTrace = 1,
    SeverityInfo = 2,
    SeverityWarning = 3,
    SeverityError = 4,
    SeverityCritical = 5,
};

enum WisInputClass {
    InputClassPerVertex = 0,
    InputClassPerInstance = 1,
};

enum WisCullMode {
    CullModeNone = 1,
    CullModeFront = 2,
    CullModeBack = 3,
};

enum WisDataFormat {
    DataFormatUnknown = 0,
    DataFormatRGBA32Float = 2,
    DataFormatRGBA32Uint = 3,
    DataFormatRGBA32Sint = 4,
    DataFormatRGB32Float = 6,
    DataFormatRGB32Uint = 7,
    DataFormatRGB32Sint = 8,
    DataFormatRGBA16Float = 10,
    DataFormatRGBA16Unorm = 11,
    DataFormatRGBA16Uint = 12,
    DataFormatRGBA16Snorm = 13,
    DataFormatRGBA16Sint = 14,
    DataFormatRG32Float = 16,
    DataFormatRG32Uint = 17,
    DataFormatRG32Sint = 18,
    DataFormatD32FloatS8Uint = 20,
    DataFormatRGB10A2Unorm = 24,
    DataFormatRGB10A2Uint = 25,
    DataFormatRG11B10Float = 26,
    DataFormatRGBA8Unorm = 28,
    DataFormatRGBA8UnormSrgb = 29,
    DataFormatRGBA8Uint = 30,
    DataFormatRGBA8Snorm = 31,
    DataFormatRGBA8Sint = 32,
    DataFormatRG16Float = 34,
    DataFormatRG16Unorm = 35,
    DataFormatRG16Uint = 36,
    DataFormatRG16Snorm = 37,
    DataFormatRG16Sint = 38,
    DataFormatD32Float = 40,
    DataFormatR32Float = 41,
    DataFormatR32Uint = 42,
    DataFormatR32Sint = 43,
    DataFormatD24UnormS8Uint = 45,
    DataFormatRG8Unorm = 49,
    DataFormatRG8Uint = 50,
    DataFormatRG8Snorm = 51,
    DataFormatRG8Sint = 52,
    DataFormatR16Float = 54,
    DataFormatD16Unorm = 55,
    DataFormatR16Unorm = 56,
    DataFormatR16Uint = 57,
    DataFormatR16Snorm = 58,
    DataFormatR16Sint = 59,
    DataFormatR8Unorm = 61,
    DataFormatR8Uint = 62,
    DataFormatR8Snorm = 63,
    DataFormatR8Sint = 64,
    DataFormatRGB9E5UFloat = 67,
    DataFormatRG8BG8Unorm = 68,
    DataFormatGR8GB8Unorm = 69,
    DataFormatBC1RGBAUnorm = 71,
    DataFormatBC1RGBAUnormSrgb = 72,
    DataFormatBC2RGBAUnorm = 74,
    DataFormatBC2RGBAUnormSrgb = 75,
    DataFormatBC3RGBAUnorm = 77,
    DataFormatBC3RGBAUnormSrgb = 78,
    DataFormatBC4RUnorm = 80,
    DataFormatBC4RSnorm = 81,
    DataFormatBC5RGUnorm = 83,
    DataFormatBC5RGSnorm = 84,
    DataFormatB5R6G5Unorm = 85,
    DataFormatB5G5R5A1Unorm = 86,
    DataFormatBGRA8Unorm = 87,
    DataFormatBGRA8UnormSrgb = 91,
    DataFormatBC6HUfloat16 = 95,
    DataFormatBC6HSfloat16 = 96,
    DataFormatBC7RGBAUnorm = 98,
    DataFormatBC7RGBAUnormSrgb = 99,
};

enum WisFillMode {
    FillModeLines = 2,
    FillModeSolid = 3,
};

enum WisDescriptorMemory {
    DescriptorMemoryCpuOnly = 0,
    DescriptorMemoryShaderVisible = 1,
};

enum WisWindingOrder {
    WindingOrderClockwise = 0,
    WindingOrderCounterClockwise = 1,
};

enum WisSampleRate {
    SampleRateS1 = 1,
    SampleRateS2 = 2,
    SampleRateS4 = 4,
    SampleRateS8 = 8,
    SampleRateS16 = 16,
};

enum WisCompare {
    CompareNone = 0,
    CompareNever = 1,
    CompareLess = 2,
    CompareEqual = 3,
    CompareLessEqual = 4,
    CompareGreater = 5,
    CompareNotEqual = 6,
    CompareGreaterEqual = 7,
    CompareAlways = 8,
};

enum WisStencilOp {
    StencilOpKeep = 1,
    StencilOpZero = 2,
    StencilOpReplace = 3,
    StencilOpIncClamp = 4,
    StencilOpDecClamp = 5,
    StencilOpInvert = 6,
    StencilOpIncWrap = 7,
    StencilOpDecWrap = 8,
};

enum WisBlendFactor {
    BlendFactorZero = 1,
    BlendFactorOne = 2,
    BlendFactorSrcColor = 3,
    BlendFactorInvSrcColor = 4,
    BlendFactorSrcAlpha = 5,
    BlendFactorInvSrcAlpha = 6,
    BlendFactorDestAlpha = 7,
    BlendFactorInvDestAlpha = 8,
    BlendFactorDestColor = 9,
    BlendFactorInvDestColor = 10,
    BlendFactorSrcAlphaSat = 11,
    BlendFactorBlendFactor = 14,
    BlendFactorInvBlendFactor = 15,
    BlendFactorSrc1Color = 16,
    BlendFactorInvSrc1Color = 17,
    BlendFactorSrc1Alpha = 18,
    BlendFactorInvSrc1Alpha = 19,
};

enum WisBlendOp {
    BlendOpAdd = 1,
    BlendOpSubtract = 2,
    BlendOpRevSubtract = 3,
    BlendOpMin = 4,
    BlendOpMax = 5,
};

enum WisSampleCount {
    SampleCountS1 = 1,
    SampleCountS2 = 2,
    SampleCountS4 = 4,
    SampleCountS8 = 8,
    SampleCountS16 = 16,
    SampleCountS32 = 32,
    SampleCountS64 = 64,
};

enum WisLogicOp {
    LogicOpClear = 0,
    LogicOpSet = 1,
    LogicOpCopy = 2,
    LogicOpCopyInverted = 3,
    LogicOpNoop = 4,
    LogicOpInvert = 5,
    LogicOpAnd = 6,
    LogicOpNand = 7,
    LogicOpOr = 8,
    LogicOpNor = 9,
    LogicOpXor = 10,
    LogicOpEquiv = 11,
    LogicOpAndReverse = 12,
    LogicOpAndInverted = 13,
    LogicOpOrReverse = 14,
    LogicOpOrInverted = 15,
};

enum WisMemoryType {
    MemoryTypeDefault = 0,
    MemoryTypeUpload = 1,
    MemoryTypeReadback = 2,
    MemoryTypeGPUUpload = 3,
};

enum WisShaderIntermediate {
    ShaderIntermediateDXIL = 0,
    ShaderIntermediateSPIRV = 1,
};

enum WisTextureState {
    TextureStateUndefined = 4294967295,
    TextureStateCommon = 0,
    TextureStateRead = 1,
    TextureStateRenderTarget = 2,
    TextureStateUnorderedAccess = 3,
    TextureStateDepthStencilWrite = 4,
    TextureStateDepthStencilRead = 5,
    TextureStateShaderResource = 6,
    TextureStateCopySource = 7,
    TextureStateCopyDest = 8,
    TextureStatePresent = 9,
    TextureStateShadingRate = 10,
    TextureStateVideoDecodeRead = 11,
    TextureStateVideoDecodeWrite = 12,
};

enum WisLoadOperation {
    LoadOperationLoad = 0,
    LoadOperationClear = 1,
    LoadOperationDontCare = 2,
};

enum WisTextureLayout {
    TextureLayoutTexture1D = 2,
    TextureLayoutTexture1DArray = 3,
    TextureLayoutTexture2D = 4,
    TextureLayoutTexture2DArray = 5,
    TextureLayoutTexture2DMS = 6,
    TextureLayoutTexture2DMSArray = 7,
    TextureLayoutTexture3D = 8,
};

enum WisDescriptorHeapType {
    DescriptorHeapTypeDescriptor = 0,
    DescriptorHeapTypeSampler = 1,
};

enum WisStoreOperation {
    StoreOperationStore = 0,
    StoreOperationDontCare = 1,
    StoreOperationResolve = 2,
};

enum WisPrimitiveTopology {
    PrimitiveTopologyPointList = 1,
    PrimitiveTopologyLineList = 2,
    PrimitiveTopologyLineStrip = 3,
    PrimitiveTopologyTriangleList = 4,
    PrimitiveTopologyTriangleStrip = 5,
    PrimitiveTopologyTriangleFan = 6,
    PrimitiveTopologyLineListAdj = 10,
    PrimitiveTopologyLineStripAdj = 11,
    PrimitiveTopologyTriangleListAdj = 12,
    PrimitiveTopologyTriangleStripAdj = 13,
};

enum WisTopologyType {
    TopologyTypePoint = 1,
    TopologyTypeLine = 2,
    TopologyTypeTriangle = 3,
    TopologyTypePatch = 4,
};

enum WisFilter {
    FilterPoint = 0,
    FilterLinear = 1,
};

enum WisAddressMode {
    AddressModeRepeat = 0,
    AddressModeMirroredRepeat = 1,
    AddressModeClampToEdge = 2,
    AddressModeClampToBorder = 3,
    AddressModeMirrorClampToEdge = 4,
};

enum WisTextureViewType {
    TextureViewTypeTexture1D = 0,
    TextureViewTypeTexture1DArray = 1,
    TextureViewTypeTexture2D = 2,
    TextureViewTypeTexture2DArray = 3,
    TextureViewTypeTexture2DMS = 4,
    TextureViewTypeTexture2DMSArray = 5,
    TextureViewTypeTexture3D = 6,
    TextureViewTypeTextureCube = 7,
    TextureViewTypeTextureCubeArray = 8,
};

enum WisComponentSwizzle {
    ComponentSwizzleRed = 0,
    ComponentSwizzleGreen = 1,
    ComponentSwizzleBlue = 2,
    ComponentSwizzleAlpha = 3,
    ComponentSwizzleZero = 4,
    ComponentSwizzleOne = 5,
};

//-------------------------------------------------------------------------

enum WisAdapterFlagsBits {
    AdapterFlagsNone = 0x0,
    AdapterFlagsRemote = 1 << 0,
    AdapterFlagsSoftware = 1 << 1,
    AdapterFlagsDX12ACGCompatible = 1 << 2,
    AdapterFlagsDX12SupportsMonitoredFences = 1 << 3,
    AdapterFlagsDX12SupportsNonMonitoredFences = 1 << 4,
    AdapterFlagsDX12KeyedMutexConformance = 1 << 5,
};

enum WisDSSelectBits {
    DSSelectNone = 0x0,
    DSSelectDepth = 1 << 0,
    DSSelectStencil = 1 << 1,
    DSSelectDepthStencil = 0x3,
};

enum WisColorComponentsBits {
    ColorComponentsNone = 0x0,
    ColorComponentsR = 1 << 0,
    ColorComponentsG = 1 << 1,
    ColorComponentsB = 1 << 2,
    ColorComponentsA = 1 << 3,
    ColorComponentsAll = 0xF,
};

enum WisBufferFlagsBits {
    BufferFlagsNone = 0x0,
    BufferFlagsConstantBuffer = 1 << 4,
    BufferFlagsIndexBuffer = 1 << 6,
    BufferFlagsVertexBuffer = 1 << 7,
};

enum WisMemoryFlagsBits {
    MemoryFlagsNone = 0x0,
    MemoryFlagsDedicatedAllocation = 1 << 0,
    MemoryFlagsMapped = 1 << 1,
};

enum WisRenderPassFlagsBits {
    RenderPassFlagsNone = 0x0,
    RenderPassFlagsSuspending = 1 << 1,
    RenderPassFlagsResuming = 1 << 2,
};

enum WisBarrierSyncBits {
    BarrierSyncNone = 0x0,
    BarrierSyncAll = 1 << 0,
    BarrierSyncDraw = 1 << 1,
    BarrierSyncIndexInput = 1 << 2,
    BarrierSyncVertexShading = 1 << 3,
    BarrierSyncPixelShading = 1 << 4,
    BarrierSyncDepthStencil = 1 << 5,
    BarrierSyncRenderTarget = 1 << 6,
    BarrierSyncCompute = 1 << 7,
    BarrierSyncRaytracing = 1 << 8,
    BarrierSyncCopy = 1 << 9,
    BarrierSyncResolve = 1 << 10,
    BarrierSyncExecuteIndirect = 1 << 11,
    BarrierSyncAllShading = 1 << 12,
    BarrierSyncNonPixelShading = 1 << 13,
    BarrierSyncClearUAV = 1 << 14,
    BarrierSyncVideoDecode = 1 << 15,
    BarrierSyncVideoEncode = 1 << 16,
    BarrierSyncBuildRTAS = 1 << 17,
    BarrierSyncCopyRTAS = 1 << 18,
};

enum WisResourceAccessBits {
    ResourceAccessCommon = 0x0,
    ResourceAccessVertexBuffer = 1 << 0,
    ResourceAccessConstantBuffer = 1 << 1,
    ResourceAccessIndexBuffer = 1 << 2,
    ResourceAccessRenderTarget = 1 << 3,
    ResourceAccessUnorderedAccess = 1 << 4,
    ResourceAccessDepthWrite = 1 << 5,
    ResourceAccessDepthRead = 1 << 6,
    ResourceAccessShaderResource = 1 << 7,
    ResourceAccessStreamOutput = 1 << 8,
    ResourceAccessIndirectArgument = 1 << 9,
    ResourceAccessCopyDest = 1 << 10,
    ResourceAccessCopySource = 1 << 11,
    ResourceAccessConditionalRendering = 1 << 12,
    ResourceAccessAccelerationStrucureRead = 1 << 13,
    ResourceAccessAccelerationStrucureWrite = 1 << 14,
    ResourceAccessShadingRate = 1 << 15,
    ResourceAccessVideoDecodeRead = 1 << 16,
    ResourceAccessVideoDecodeWrite = 1 << 17,
    ResourceAccessPresent = 1 << 18,
    ResourceAccessResolveDest = 1 << 19,
    ResourceAccessResolveSource = 1 << 20,
    ResourceAccessNoAccess = 1 << 31,
};

enum WisTextureUsageBits {
    TextureUsageNone = 0x0,
    TextureUsageRenderTarget = 1 << 0,
    TextureUsageDepthStencil = 1 << 1,
    TextureUsageCopySrc = 1 << 2,
    TextureUsageCopyDst = 1 << 3,
    TextureUsageShaderResource = 1 << 4,
    TextureUsageUnorderedAccess = 1 << 5,
    TextureUsageHostCopy = 1 << 7,
};

//-------------------------------------------------------------------------

typedef struct WisResult WisResult;
typedef struct WisSize2D WisSize2D;
typedef struct WisSize3D WisSize3D;
typedef struct WisBufferRegion WisBufferRegion;
typedef struct WisAdapterDesc WisAdapterDesc;
typedef struct WisInputSlotDesc WisInputSlotDesc;
typedef struct WisInputAttribute WisInputAttribute;
typedef struct WisInputLayout WisInputLayout;
typedef struct WisRasterizerDesc WisRasterizerDesc;
typedef struct WisSampleDesc WisSampleDesc;
typedef struct WisStencilDesc WisStencilDesc;
typedef struct WisDepthStencilDesc WisDepthStencilDesc;
typedef struct WisBlendAttachmentDesc WisBlendAttachmentDesc;
typedef struct WisBlendStateDesc WisBlendStateDesc;
typedef struct WisRenderAttachmentsDesc WisRenderAttachmentsDesc;
typedef struct WisRootConstant WisRootConstant;
typedef struct WisSwapchainDesc WisSwapchainDesc;
typedef struct WisTextureDesc WisTextureDesc;
typedef struct WisTextureRegion WisTextureRegion;
typedef struct WisBufferTextureCopyRegion WisBufferTextureCopyRegion;
typedef struct WisPushDescriptor WisPushDescriptor;
typedef struct WisSubresourceRange WisSubresourceRange;
typedef struct WisRenderTargetDesc WisRenderTargetDesc;
typedef struct WisViewport WisViewport;
typedef struct WisScissor WisScissor;
typedef struct WisBufferBarrier WisBufferBarrier;
typedef struct WisTextureBarrier WisTextureBarrier;
typedef struct WisDescriptorTableEntry WisDescriptorTableEntry;
typedef struct WisDescriptorTable WisDescriptorTable;
typedef struct WisSamplerDesc WisSamplerDesc;
typedef struct WisComponentMapping WisComponentMapping;
typedef struct WisShaderResourceDesc WisShaderResourceDesc;
typedef enum WisShaderStages WisShaderStages;
typedef enum WisStatus WisStatus;
typedef enum WisQueuePriority WisQueuePriority;
typedef enum WisMutiWaitFlags WisMutiWaitFlags;
typedef enum WisDescriptorType WisDescriptorType;
typedef enum WisQueueType WisQueueType;
typedef enum WisAdapterPreference WisAdapterPreference;
typedef enum WisSeverity WisSeverity;
typedef enum WisInputClass WisInputClass;
typedef enum WisCullMode WisCullMode;
typedef enum WisDataFormat WisDataFormat;
typedef enum WisFillMode WisFillMode;
typedef enum WisDescriptorMemory WisDescriptorMemory;
typedef enum WisWindingOrder WisWindingOrder;
typedef enum WisSampleRate WisSampleRate;
typedef enum WisCompare WisCompare;
typedef enum WisStencilOp WisStencilOp;
typedef enum WisBlendFactor WisBlendFactor;
typedef enum WisBlendOp WisBlendOp;
typedef enum WisSampleCount WisSampleCount;
typedef enum WisLogicOp WisLogicOp;
typedef enum WisMemoryType WisMemoryType;
typedef enum WisShaderIntermediate WisShaderIntermediate;
typedef enum WisTextureState WisTextureState;
typedef enum WisLoadOperation WisLoadOperation;
typedef enum WisTextureLayout WisTextureLayout;
typedef enum WisDescriptorHeapType WisDescriptorHeapType;
typedef enum WisStoreOperation WisStoreOperation;
typedef enum WisPrimitiveTopology WisPrimitiveTopology;
typedef enum WisTopologyType WisTopologyType;
typedef enum WisFilter WisFilter;
typedef enum WisAddressMode WisAddressMode;
typedef enum WisTextureViewType WisTextureViewType;
typedef enum WisComponentSwizzle WisComponentSwizzle;
typedef enum WisAdapterFlagsBits WisAdapterFlagsBits;
typedef uint32_t WisAdapterFlags;
typedef enum WisDSSelectBits WisDSSelectBits;
typedef uint32_t WisDSSelect;
typedef enum WisColorComponentsBits WisColorComponentsBits;
typedef uint32_t WisColorComponents;
typedef enum WisBufferFlagsBits WisBufferFlagsBits;
typedef uint32_t WisBufferFlags;
typedef enum WisMemoryFlagsBits WisMemoryFlagsBits;
typedef uint32_t WisMemoryFlags;
typedef enum WisRenderPassFlagsBits WisRenderPassFlagsBits;
typedef uint32_t WisRenderPassFlags;
typedef enum WisBarrierSyncBits WisBarrierSyncBits;
typedef uint32_t WisBarrierSync;
typedef enum WisResourceAccessBits WisResourceAccessBits;
typedef uint32_t WisResourceAccess;
typedef enum WisTextureUsageBits WisTextureUsageBits;
typedef uint32_t WisTextureUsage;

//-------------------------------------------------------------------------

struct WisResult {
    WisStatus status;
    const char* error;
};

struct WisSize2D {
    uint32_t width;
    uint32_t height;
};

struct WisSize3D {
    uint32_t width;
    uint32_t height;
    uint32_t depth_or_layers;
};

struct WisBufferRegion {
    uint64_t src_offset;
    uint64_t dst_offset;
    uint64_t size_bytes;
};

struct WisAdapterDesc {
    const char description[256];
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision;
    uint64_t dedicated_video_memory;
    uint64_t dedicated_system_memory;
    uint64_t shared_system_memory;
    uint64_t adapter_id;
    WisAdapterFlags flags;
};

struct WisInputSlotDesc {
    uint32_t slot;
    uint32_t stride_bytes;
    WisInputClass input_class;
};

struct WisInputAttribute {
    uint32_t input_slot;
    const char* semantic_name;
    uint32_t semantic_index;
    uint32_t location;
    WisDataFormat format;
    uint32_t offset_bytes;
};

struct WisInputLayout {
    WisInputSlotDesc* slots;
    uint32_t slot_count;
    WisInputAttribute* attributes;
    uint32_t attribute_count;
};

struct WisRasterizerDesc {
    WisFillMode fill_mode;
    WisCullMode cull_mode;
    WisWindingOrder front_face;
    bool depth_bias_enable;
    float depth_bias;
    float depth_bias_clamp;
    float depth_bias_slope_factor;
    bool depth_clip_enable;
};

struct WisSampleDesc {
    WisSampleRate rate;
    float quality;
    uint32_t sample_mask;
};

struct WisStencilDesc {
    WisStencilOp fail_op;
    WisStencilOp depth_fail_op;
    WisStencilOp pass_op;
    WisCompare comparison;
    uint8_t read_mask;
    uint8_t write_mask;
};

struct WisDepthStencilDesc {
    bool depth_enable;
    bool depth_write_enable;
    WisCompare depth_comp;
    bool stencil_enable;
    WisStencilDesc stencil_front;
    WisStencilDesc stencil_back;
    bool depth_bound_test;
};

struct WisBlendAttachmentDesc {
    bool blend_enable;
    WisBlendFactor src_color_blend;
    WisBlendFactor dst_color_blend;
    WisBlendOp color_blend_op;
    WisBlendFactor src_alpha_blend;
    WisBlendFactor dst_alpha_blend;
    WisBlendOp alpha_blend_op;
    WisColorComponents color_write_mask;
};

struct WisBlendStateDesc {
    bool logic_op_enable;
    WisLogicOp logic_op;
    WisBlendAttachmentDesc attachments[8];
    uint32_t attachment_count;
};

struct WisRenderAttachmentsDesc {
    WisDataFormat* attachment_formats;
    uint32_t attachments_count;
    WisDataFormat depth_attachment;
};

struct WisRootConstant {
    WisShaderStages stage;
    uint32_t size_bytes;
};

struct WisSwapchainDesc {
    WisSize2D size;
    WisDataFormat format;
    uint32_t buffer_count;
    bool stereo;
    bool vsync;
};

struct WisTextureDesc {
    WisDataFormat format;
    WisSize3D size;
    uint32_t mip_levels;
    WisTextureLayout layout;
    WisSampleCount sample_count;
    WisTextureUsage usage;
};

struct WisTextureRegion {
    WisSize3D offset;
    WisSize3D size;
    uint32_t mip;
    uint32_t array_layer;
    WisDataFormat format;
};

struct WisBufferTextureCopyRegion {
    uint64_t buffer_offset;
    WisTextureRegion texture;
};

struct WisPushDescriptor {
    WisShaderStages stage;
    uint32_t bind_register;
    WisDescriptorType type;
    uint32_t reserved;
};

struct WisSubresourceRange {
    uint32_t base_mip_level;
    uint32_t level_count;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct WisRenderTargetDesc {
    WisDataFormat format;
    WisTextureLayout layout;
    uint32_t mip;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct WisViewport {
    float top_leftx;
    float top_lefty;
    float width;
    float height;
    float min_depth;
    float max_depth;
};

struct WisScissor {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct WisBufferBarrier {
    WisBarrierSync sync_before;
    WisBarrierSync sync_after;
    WisResourceAccess access_before;
    WisResourceAccess access_after;
    uint64_t offset;
    uint64_t size;
};

struct WisTextureBarrier {
    WisBarrierSync sync_before;
    WisBarrierSync sync_after;
    WisResourceAccess access_before;
    WisResourceAccess access_after;
    WisTextureState state_before;
    WisTextureState state_after;
    WisSubresourceRange subresource_range;
};

struct WisDescriptorTableEntry {
    WisDescriptorType type;
    uint32_t bind_register;
    uint32_t binding;
    uint32_t count;
};

struct WisDescriptorTable {
    WisDescriptorHeapType type;
    WisDescriptorTableEntry* entries;
    uint32_t entry_count;
    WisShaderStages stage;
};

struct WisSamplerDesc {
    WisFilter min_filter;
    WisFilter mag_filter;
    WisFilter mip_filter;
    bool anisotropic;
    uint32_t max_anisotropy;
    WisAddressMode address_u;
    WisAddressMode address_v;
    WisAddressMode address_w;
    float min_lod;
    float max_lod;
    float mip_lod_bias;
    WisCompare comparison_op;
    float border_color[4];
};

struct WisComponentMapping {
    WisComponentSwizzle r;
    WisComponentSwizzle g;
    WisComponentSwizzle b;
    WisComponentSwizzle a;
};

struct WisShaderResourceDesc {
    WisDataFormat format;
    WisTextureViewType view_type;
    WisComponentMapping component_mapping;
    WisSubresourceRange subresource_range;
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

//=================================DELEGATES=================================

typedef void (*DebugCallback)(WisSeverity severity, const char* message, void* user_data);

//-------------------------------------------------------------------------

#ifdef WISDOM_VULKAN
typedef struct VKCommandListView VKCommandListView;
typedef struct VKGraphicsPipelineDesc VKGraphicsPipelineDesc;
typedef struct VKFenceView VKFenceView;
typedef struct VKRenderPassDepthStencilDesc VKRenderPassDepthStencilDesc;
typedef struct VKBufferView VKBufferView;
typedef struct VKTextureView VKTextureView;
typedef struct VKShaderView VKShaderView;
typedef struct VKRenderTargetView VKRenderTargetView;
typedef struct VKRootSignatureView VKRootSignatureView;
typedef struct VKDescriptorBufferView VKDescriptorBufferView;
typedef struct VKBufferBarrier2 VKBufferBarrier2;
typedef struct VKTextureBarrier2 VKTextureBarrier2;
typedef struct VKGraphicsShaderStages VKGraphicsShaderStages;
typedef struct VKRenderPassRenderTargetDesc VKRenderPassRenderTargetDesc;
typedef struct VKRenderPassDesc VKRenderPassDesc;
typedef struct VKVertexBufferBinding VKVertexBufferBinding;

//-------------------------------------------------------------------------

struct VKFenceView {
    void* value;
};

struct VKBufferView {
    void* value;
};

struct VKTextureView {
    void* value;
    uint32_t value2;
    WisSize2D value3;
};

struct VKRenderTargetView {
    void* value;
};

struct VKCommandListView {
    void* value;
};

struct VKShaderView {
    void* value;
};

struct VKRootSignatureView {
    void* value;
};

struct VKDescriptorBufferView {
    uint64_t value1;
    uint32_t value2;
};

struct VKBufferBarrier2 {
    WisBufferBarrier barrier;
    VKBufferView buffer;
};

struct VKTextureBarrier2 {
    WisTextureBarrier barrier;
    VKTextureView texture;
};

struct VKGraphicsShaderStages {
    VKShaderView vertex;
    VKShaderView hull;
    VKShaderView domain;
    VKShaderView geometry;
    VKShaderView pixel;
};

struct VKGraphicsPipelineDesc {
    VKRootSignatureView root_signature;
    WisInputLayout input_layout;
    VKGraphicsShaderStages shaders;
    WisRenderAttachmentsDesc attachments;
    WisRasterizerDesc* rasterizer;
    WisSampleDesc* sample;
    WisBlendStateDesc* blend;
    WisDepthStencilDesc* depth_stencil;
    WisTopologyType topology_type;
};

struct VKRenderPassRenderTargetDesc {
    VKRenderTargetView target;
    WisLoadOperation load_op;
    WisStoreOperation store_op;
    float clear_value[4];
};

struct VKRenderPassDepthStencilDesc {
    VKRenderTargetView target;
    WisLoadOperation load_op_depth;
    WisLoadOperation load_op_stencil;
    WisStoreOperation store_op_depth;
    WisStoreOperation store_op_stencil;
    WisDSSelect depth_stencil_select;
    float clear_depth;
    uint8_t clear_stencil;
};

struct VKRenderPassDesc {
    WisRenderPassFlags flags;
    uint32_t target_count;
    VKRenderPassRenderTargetDesc* targets;
    VKRenderPassDepthStencilDesc* depth_stencil;
};

struct VKVertexBufferBinding {
    VKBufferView buffer;
    uint32_t size;
    uint32_t stride;
};

//-------------------------------------------------------------------------

typedef struct VKCommandQueue_t* VKCommandQueue;
typedef struct VKRootSignature_t* VKRootSignature;
typedef struct VKFactory_t* VKFactory;
typedef struct VKDeviceExtension_t* VKDeviceExtension;
typedef struct VKPipelineState_t* VKPipelineState;
typedef struct VKAdapter_t* VKAdapter;
typedef struct VKDevice_t* VKDevice;
typedef struct VKFence_t* VKFence;
typedef struct VKFactoryExtension_t* VKFactoryExtension;
typedef struct VKResourceAllocator_t* VKResourceAllocator;
typedef struct VKShader_t* VKShader;
typedef struct VKCommandList_t* VKCommandList;
typedef struct VKSwapChain_t* VKSwapChain;
typedef struct VKBuffer_t* VKBuffer;
typedef struct VKTexture_t* VKTexture;
typedef struct VKDebugMessenger_t* VKDebugMessenger;
typedef struct VKRenderTarget_t* VKRenderTarget;
typedef struct VKDescriptorBuffer_t* VKDescriptorBuffer;
typedef struct VKSampler_t* VKSampler;
typedef struct VKShaderResource_t* VKShaderResource;

//-------------------------------------------------------------------------

WISDOM_API void VKFactoryDestroy(VKFactory self);
WISDOM_API void VKAdapterDestroy(VKAdapter self);
WISDOM_API void VKDeviceDestroy(VKDevice self);
WISDOM_API void VKFenceDestroy(VKFence self);
WISDOM_API void VKResourceAllocatorDestroy(VKResourceAllocator self);
WISDOM_API void VKRootSignatureDestroy(VKRootSignature self);
WISDOM_API void VKCommandQueueDestroy(VKCommandQueue self);
WISDOM_API void VKShaderDestroy(VKShader self);
WISDOM_API void VKPipelineStateDestroy(VKPipelineState self);
WISDOM_API void VKCommandListDestroy(VKCommandList self);
WISDOM_API void VKBufferDestroy(VKBuffer self);
WISDOM_API void VKTextureDestroy(VKTexture self);
WISDOM_API void VKSwapChainDestroy(VKSwapChain self);
WISDOM_API void VKDebugMessengerDestroy(VKDebugMessenger self);
WISDOM_API void VKRenderTargetDestroy(VKRenderTarget self);
WISDOM_API void VKDescriptorBufferDestroy(VKDescriptorBuffer self);
WISDOM_API void VKSamplerDestroy(VKSampler self);
WISDOM_API void VKShaderResourceDestroy(VKShaderResource self);
WISDOM_API WisResult VKCreateFactory(bool debug_layer, VKFactory* out_factory);
WISDOM_API WisResult VKCreateDevice(VKAdapter adapter, VKDevice* out_device);

//-------------------------------------------------------------------------

WISDOM_API VKFenceView AsVKFenceView(VKFence self);
WISDOM_API VKBufferView AsVKBufferView(VKBuffer self);
WISDOM_API VKTextureView AsVKTextureView(VKTexture self);
WISDOM_API VKRenderTargetView AsVKRenderTargetView(VKRenderTarget self);
WISDOM_API VKCommandListView AsVKCommandListView(VKCommandList self);
WISDOM_API VKShaderView AsVKShaderView(VKShader self);
WISDOM_API VKRootSignatureView AsVKRootSignatureView(VKRootSignature self);
WISDOM_API VKDescriptorBufferView AsVKDescriptorBufferView(VKDescriptorBuffer self);
#endif

#ifdef WISDOM_DX12
typedef struct DX12CommandListView DX12CommandListView;
typedef struct DX12GraphicsPipelineDesc DX12GraphicsPipelineDesc;
typedef struct DX12FenceView DX12FenceView;
typedef struct DX12RenderPassDepthStencilDesc DX12RenderPassDepthStencilDesc;
typedef struct DX12BufferView DX12BufferView;
typedef struct DX12TextureView DX12TextureView;
typedef struct DX12ShaderView DX12ShaderView;
typedef struct DX12RenderTargetView DX12RenderTargetView;
typedef struct DX12RootSignatureView DX12RootSignatureView;
typedef struct DX12DescriptorBufferView DX12DescriptorBufferView;
typedef struct DX12BufferBarrier2 DX12BufferBarrier2;
typedef struct DX12TextureBarrier2 DX12TextureBarrier2;
typedef struct DX12GraphicsShaderStages DX12GraphicsShaderStages;
typedef struct DX12RenderPassRenderTargetDesc DX12RenderPassRenderTargetDesc;
typedef struct DX12RenderPassDesc DX12RenderPassDesc;
typedef struct DX12VertexBufferBinding DX12VertexBufferBinding;

//-------------------------------------------------------------------------

struct DX12FenceView {
    void* value;
};

struct DX12BufferView {
    void* value;
};

struct DX12TextureView {
    void* value;
};

struct DX12RenderTargetView {
    void* value;
};

struct DX12CommandListView {
    void* value;
};

struct DX12ShaderView {
    void* bytecode;
    uint32_t size_bytes;
};

struct DX12RootSignatureView {
    void* value;
};

struct DX12DescriptorBufferView {
    void* value;
};

struct DX12BufferBarrier2 {
    WisBufferBarrier barrier;
    DX12BufferView buffer;
};

struct DX12TextureBarrier2 {
    WisTextureBarrier barrier;
    DX12TextureView texture;
};

struct DX12GraphicsShaderStages {
    DX12ShaderView vertex;
    DX12ShaderView hull;
    DX12ShaderView domain;
    DX12ShaderView geometry;
    DX12ShaderView pixel;
};

struct DX12GraphicsPipelineDesc {
    DX12RootSignatureView root_signature;
    WisInputLayout input_layout;
    DX12GraphicsShaderStages shaders;
    WisRenderAttachmentsDesc attachments;
    WisRasterizerDesc* rasterizer;
    WisSampleDesc* sample;
    WisBlendStateDesc* blend;
    WisDepthStencilDesc* depth_stencil;
    WisTopologyType topology_type;
};

struct DX12RenderPassRenderTargetDesc {
    DX12RenderTargetView target;
    WisLoadOperation load_op;
    WisStoreOperation store_op;
    float clear_value[4];
};

struct DX12RenderPassDepthStencilDesc {
    DX12RenderTargetView target;
    WisLoadOperation load_op_depth;
    WisLoadOperation load_op_stencil;
    WisStoreOperation store_op_depth;
    WisStoreOperation store_op_stencil;
    WisDSSelect depth_stencil_select;
    float clear_depth;
    uint8_t clear_stencil;
};

struct DX12RenderPassDesc {
    WisRenderPassFlags flags;
    uint32_t target_count;
    DX12RenderPassRenderTargetDesc* targets;
    DX12RenderPassDepthStencilDesc* depth_stencil;
};

struct DX12VertexBufferBinding {
    DX12BufferView buffer;
    uint32_t size;
    uint32_t stride;
};

//-------------------------------------------------------------------------

typedef struct DX12CommandQueue_t* DX12CommandQueue;
typedef struct DX12RootSignature_t* DX12RootSignature;
typedef struct DX12Factory_t* DX12Factory;
typedef struct DX12DeviceExtension_t* DX12DeviceExtension;
typedef struct DX12PipelineState_t* DX12PipelineState;
typedef struct DX12Adapter_t* DX12Adapter;
typedef struct DX12Device_t* DX12Device;
typedef struct DX12Fence_t* DX12Fence;
typedef struct DX12FactoryExtension_t* DX12FactoryExtension;
typedef struct DX12ResourceAllocator_t* DX12ResourceAllocator;
typedef struct DX12Shader_t* DX12Shader;
typedef struct DX12CommandList_t* DX12CommandList;
typedef struct DX12SwapChain_t* DX12SwapChain;
typedef struct DX12Buffer_t* DX12Buffer;
typedef struct DX12Texture_t* DX12Texture;
typedef struct DX12DebugMessenger_t* DX12DebugMessenger;
typedef struct DX12RenderTarget_t* DX12RenderTarget;
typedef struct DX12DescriptorBuffer_t* DX12DescriptorBuffer;
typedef struct DX12Sampler_t* DX12Sampler;
typedef struct DX12ShaderResource_t* DX12ShaderResource;

//-------------------------------------------------------------------------

WISDOM_API void DX12FactoryDestroy(DX12Factory self);
WISDOM_API void DX12AdapterDestroy(DX12Adapter self);
WISDOM_API void DX12DeviceDestroy(DX12Device self);
WISDOM_API void DX12FenceDestroy(DX12Fence self);
WISDOM_API void DX12ResourceAllocatorDestroy(DX12ResourceAllocator self);
WISDOM_API void DX12RootSignatureDestroy(DX12RootSignature self);
WISDOM_API void DX12CommandQueueDestroy(DX12CommandQueue self);
WISDOM_API void DX12ShaderDestroy(DX12Shader self);
WISDOM_API void DX12PipelineStateDestroy(DX12PipelineState self);
WISDOM_API void DX12CommandListDestroy(DX12CommandList self);
WISDOM_API void DX12BufferDestroy(DX12Buffer self);
WISDOM_API void DX12TextureDestroy(DX12Texture self);
WISDOM_API void DX12SwapChainDestroy(DX12SwapChain self);
WISDOM_API void DX12DebugMessengerDestroy(DX12DebugMessenger self);
WISDOM_API void DX12RenderTargetDestroy(DX12RenderTarget self);
WISDOM_API void DX12DescriptorBufferDestroy(DX12DescriptorBuffer self);
WISDOM_API void DX12SamplerDestroy(DX12Sampler self);
WISDOM_API void DX12ShaderResourceDestroy(DX12ShaderResource self);
WISDOM_API WisResult DX12CreateFactory(bool debug_layer, DX12Factory* out_factory);
WISDOM_API WisResult DX12CreateDevice(DX12Adapter adapter, DX12Device* out_device);

//-------------------------------------------------------------------------

WISDOM_API DX12FenceView AsDX12FenceView(DX12Fence self);
WISDOM_API DX12BufferView AsDX12BufferView(DX12Buffer self);
WISDOM_API DX12TextureView AsDX12TextureView(DX12Texture self);
WISDOM_API DX12RenderTargetView AsDX12RenderTargetView(DX12RenderTarget self);
WISDOM_API DX12CommandListView AsDX12CommandListView(DX12CommandList self);
WISDOM_API DX12ShaderView AsDX12ShaderView(DX12Shader self);
WISDOM_API DX12RootSignatureView AsDX12RootSignatureView(DX12RootSignature self);
WISDOM_API DX12DescriptorBufferView AsDX12DescriptorBufferView(DX12DescriptorBuffer self);
#endif

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------

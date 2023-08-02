#pragma once
#ifndef WISDOM_MODULES
#include <string_view>
#include <array>
#endif

WIS_EXPORT namespace wis
{
    /// @brief Enum class for the different types of data formats that can be used in a texture, input layout, etc.
    /// @note This enum class is based on the DXGI_FORMAT enum class from the DirectX API.
    enum class DataFormat : uint32_t {
        unknown = 0,
        r32g32b32a32_typeless = 1,
        r32g32b32a32_float = 2,
        r32g32b32a32_uint = 3,
        r32g32b32a32_sint = 4,
        r32g32b32_typeless = 5,
        r32g32b32_float = 6,
        r32g32b32_uint = 7,
        r32g32b32_sint = 8,
        r16g16b16a16_typeless = 9,
        r16g16b16a16_float = 10,
        r16g16b16a16_unorm = 11,
        r16g16b16a16_uint = 12,
        r16g16b16a16_snorm = 13,
        r16g16b16a16_sint = 14,
        r32g32_typeless = 15,
        r32g32_float = 16,
        r32g32_uint = 17,
        r32g32_sint = 18,
        r32g8x24_typeless = 19,
        d32_float_s8x24_uint = 20,
        r32_float_x8x24_typeless = 21,
        x32_typeless_g8x24_uint = 22,
        r10g10b10a2_typeless = 23,
        r10g10b10a2_unorm = 24,
        r10g10b10a2_uint = 25,
        r11g11b10_float = 26,
        r8g8b8a8_typeless = 27,
        r8g8b8a8_unorm = 28,
        r8g8b8a8_unorm_srgb = 29,
        r8g8b8a8_uint = 30,
        r8g8b8a8_snorm = 31,
        r8g8b8a8_sint = 32,
        r16g16_typeless = 33,
        r16g16_float = 34,
        r16g16_unorm = 35,
        r16g16_uint = 36,
        r16g16_snorm = 37,
        r16g16_sint = 38,
        r32_typeless = 39,
        d32_float = 40,
        r32_float = 41,
        r32_uint = 42,
        r32_sint = 43,
        r24g8_typeless = 44,
        d24_unorm_s8_uint = 45,
        r24_unorm_x8_typeless = 46,
        x24_typeless_g8_uint = 47,
        r8g8_typeless = 48,
        r8g8_unorm = 49,
        r8g8_uint = 50,
        r8g8_snorm = 51,
        r8g8_sint = 52,
        r16_typeless = 53,
        r16_float = 54,
        d16_unorm = 55,
        r16_unorm = 56,
        r16_uint = 57,
        r16_snorm = 58,
        r16_sint = 59,
        r8_typeless = 60,
        r8_unorm = 61,
        r8_uint = 62,
        r8_snorm = 63,
        r8_sint = 64,
        a8_unorm = 65,
        r1_unorm = 66,
        r9g9b9e5_sharedexp = 67,
        r8g8_b8g8_unorm = 68,
        g8r8_g8b8_unorm = 69,
        bc1_typeless = 70,
        bc1_unorm = 71,
        bc1_unorm_srgb = 72,
        bc2_typeless = 73,
        bc2_unorm = 74,
        bc2_unorm_srgb = 75,
        bc3_typeless = 76,
        bc3_unorm = 77,
        bc3_unorm_srgb = 78,
        bc4_typeless = 79,
        bc4_unorm = 80,
        bc4_snorm = 81,
        bc5_typeless = 82,
        bc5_unorm = 83,
        bc5_snorm = 84,
        b5g6r5_unorm = 85,
        b5g5r5a1_unorm = 86,
        b8g8r8a8_unorm = 87,
        b8g8r8x8_unorm = 88,
        r10g10b10_xr_bias_a2_unorm = 89,
        b8g8r8a8_typeless = 90,
        b8g8r8a8_unorm_srgb = 91,
        b8g8r8x8_typeless = 92,
        b8g8r8x8_unorm_srgb = 93,
        bc6h_typeless = 94,
        bc6h_uf16 = 95,
        bc6h_sf16 = 96,
        bc7_typeless = 97,
        bc7_unorm = 98,
        bc7_unorm_srgb = 99,
        ayuv = 100,
        y410 = 101,
        y416 = 102,
        nv12 = 103,
        p010 = 104,
        p016 = 105,
        opaque_420 = 106,
        yuy2 = 107,
        y210 = 108,
        y216 = 109,
        nv11 = 110,
        ai44 = 111,
        ia44 = 112,
        p8 = 113,
        a8p8 = 114,
        b4g4r4a4_unorm = 115,

        p208 = 130,
        v208 = 131,
        v408 = 132,

        sampler_feedback_min_mip_opaque = 189,
        sampler_feedback_mip_region_used_opaque = 190,
    };

    /// @brief An enum describing the type of an index.
    enum class IndexType {
        uint16 = 0,
        uint32 = 1
    };

    /// @brief A string representation of the data format enum.
    constexpr inline std::string_view data_format_strings[]{
        "unknown = 0",
        "r32g32b32a32_typeless = 1",
        "r32g32b32a32_float = 2",
        "r32g32b32a32_uint = 3",
        "r32g32b32a32_sint = 4",
        "r32g32b32_typeless = 5",
        "r32g32b32_float = 6",
        "r32g32b32_uint = 7",
        "r32g32b32_sint = 8",
        "r16g16b16a16_typeless = 9",
        "r16g16b16a16_float = 10",
        "r16g16b16a16_unorm = 11",
        "r16g16b16a16_uint = 12",
        "r16g16b16a16_snorm = 13",
        "r16g16b16a16_sint = 14",
        "r32g32_typeless = 15",
        "r32g32_float = 16",
        "r32g32_uint = 17",
        "r32g32_sint = 18",
        "r32g8x24_typeless = 19",
        "d32_float_s8x24_uint = 20",
        "r32_float_x8x24_typeless = 21",
        "x32_typeless_g8x24_uint = 22",
        "r10g10b10a2_typeless = 23",
        "r10g10b10a2_unorm = 24",
        "r10g10b10a2_uint = 25",
        "r11g11b10_float = 26",
        "r8g8b8a8_typeless = 27",
        "r8g8b8a8_unorm = 28",
        "r8g8b8a8_unorm_srgb = 29",
        "r8g8b8a8_uint = 30",
        "r8g8b8a8_snorm = 31",
        "r8g8b8a8_sint = 32",
        "r16g16_typeless = 33",
        "r16g16_float = 34",
        "r16g16_unorm = 35",
        "r16g16_uint = 36",
        "r16g16_snorm = 37",
        "r16g16_sint = 38",
        "r32_typeless = 39",
        "d32_float = 40",
        "r32_float = 41",
        "r32_uint = 42",
        "r32_sint = 43",
        "r24g8_typeless = 44",
        "d24_unorm_s8_uint = 45",
        "r24_unorm_x8_typeless = 46",
        "x24_typeless_g8_uint = 47",
        "r8g8_typeless = 48",
        "r8g8_unorm = 49",
        "r8g8_uint = 50",
        "r8g8_snorm = 51",
        "r8g8_sint = 52",
        "r16_typeless = 53",
        "r16_float = 54",
        "d16_unorm = 55",
        "r16_unorm = 56",
        "r16_uint = 57",
        "r16_snorm = 58",
        "r16_sint = 59",
        "r8_typeless = 60",
        "r8_unorm = 61",
        "r8_uint = 62",
        "r8_snorm = 63",
        "r8_sint = 64",
        "a8_unorm = 65",
        "r1_unorm = 66",
        "r9g9b9e5_sharedexp = 67",
        "r8g8_b8g8_unorm = 68",
        "g8r8_g8b8_unorm = 69",
        "bc1_typeless = 70",
        "bc1_unorm = 71",
        "bc1_unorm_srgb = 72",
        "bc2_typeless = 73",
        "bc2_unorm = 74",
        "bc2_unorm_srgb = 75",
        "bc3_typeless = 76",
        "bc3_unorm = 77",
        "bc3_unorm_srgb = 78",
        "bc4_typeless = 79",
        "bc4_unorm = 80",
        "bc4_snorm = 81",
        "bc5_typeless = 82",
        "bc5_unorm = 83",
        "bc5_snorm = 84",
        "b5g6r5_unorm = 85",
        "b5g5r5a1_unorm = 86",
        "b8g8r8a8_unorm = 87",
        "b8g8r8x8_unorm = 88",
        "r10g10b10_xr_bias_a2_unorm = 89",
        "b8g8r8a8_typeless = 90",
        "b8g8r8a8_unorm_srgb = 91",
        "b8g8r8x8_typeless = 92",
        "b8g8r8x8_unorm_srgb = 93",
        "bc6h_typeless = 94",
        "bc6h_uf16 = 95",
        "bc6h_sf16 = 96",
        "bc7_typeless = 97",
        "bc7_unorm = 98",
        "bc7_unorm_srgb = 99",
        "ayuv = 100",
        "y410 = 101",
        "y416 = 102",
        "nv12 = 103",
        "p010 = 104",
        "p016 = 105",
        "opaque_420 = 106",
        "yuy2 = 107",
        "y210 = 108",
        "y216 = 109",
        "nv11 = 110",
        "ai44 = 111",
        "ia44 = 112",
        "p8 = 113",
        "a8p8 = 114",
        "b4g4r4a4_unorm = 115",

        "p208 = 130",
        "v208 = 131",
        "v408 = 132",

        "sampler_feedback_min_mip_opaque = 189",
        "sampler_feedback_mip_region_used_opaque = 190",
    };

    /// @brief Queue type for a command list and command queue queries
    /// @note  The order of the enum values is important, do not change it
    enum class QueueType {
        direct = 0,
        bundle = 1, //< Bundle queues are used for bundles in D3D12, but are not supported in Vulkan yet
        compute = 2,
        copy = 3,
        video_decode = 4, //< Video decode queues are used for video decode in D3D12, but are not supported in Vulkan yet
        video_process = 5, //< Video process queues are used for video process in D3D12, but are not supported in Vulkan yet
        video_encode = 6, //< Video encode queues are used for video encode in D3D12, but are not supported in Vulkan yet
    };

    /// @brief Basic Viewport structure
    struct Viewport {
        /// @brief Create a viewport with the given width, height and depth range
        /// @param width Width of the viewport
        /// @param height Height of the viewport
        /// @param min_depth Minimum depth of the viewport
        /// @param max_depth Maximum depth of the viewport
        /// @param top_leftx Top left x coordinate of the viewport
        /// @param top_lefty Top left y coordinate of the viewport
        Viewport(float width, float height, float min_depth = 0.0f, float max_depth = 1.0f, float top_leftx = 0.0f, float top_lefty = 0.0f)
            : top_leftx(top_leftx), top_lefty(top_lefty), width(width), height(height), min_depth(min_depth), max_depth(max_depth) { }

        float top_leftx;
        float top_lefty;
        float width;
        float height;
        float min_depth;
        float max_depth;
    };

    /// @brief Basic Rect structure for scissor rects
    /// @note  The order of the members is important, do not change it or the structure will not match the D3D12 structure
    struct ScissorRect {
        /// @brief Create a scissor rect with the given width and height and top left corner at top,left
        /// @param right Right coordinate of the scissor rect
        /// @param bottom Bottom coordinate of the scissor rect
        /// @param left Left coordinate of the scissor rect
        /// @param top Top coordinate of the scissor rect
        ScissorRect(long right, long bottom, long left = 0, long top = 0)
            : left(left), top(top), right(right), bottom(bottom) { }

        long left;
        long top;
        long right;
        long bottom;
    };

    /// @brief Enum for primitive topologies
    /// Matches the D3D12_PRIMITIVE_TOPOLOGY enum
    enum class PrimitiveTopology {
        undefined = 0,
        pointlist = 1,
        linelist = 2,
        linestrip = 3,
        trianglelist = 4,
        trianglestrip = 5,
        trianglefan = 6,
        linelist_adj = 10,
        linestrip_adj = 11,
        trianglelist_adj = 12,
        trianglestrip_adj = 13,
        control_point_patchlist_1 = 33,
        control_point_patchlist_2 = 34,
        control_point_patchlist_3 = 35,
        control_point_patchlist_4 = 36,
        control_point_patchlist_5 = 37,
        control_point_patchlist_6 = 38,
        control_point_patchlist_7 = 39,
        control_point_patchlist_8 = 40,
        control_point_patchlist_9 = 41,
        control_point_patchlist_10 = 42,
        control_point_patchlist_11 = 43,
        control_point_patchlist_12 = 44,
        control_point_patchlist_13 = 45,
        control_point_patchlist_14 = 46,
        control_point_patchlist_15 = 47,
        control_point_patchlist_16 = 48,
        control_point_patchlist_17 = 49,
        control_point_patchlist_18 = 50,
        control_point_patchlist_19 = 51,
        control_point_patchlist_20 = 52,
        control_point_patchlist_21 = 53,
        control_point_patchlist_22 = 54,
        control_point_patchlist_23 = 55,
        control_point_patchlist_24 = 56,
        control_point_patchlist_25 = 57,
        control_point_patchlist_26 = 58,
        control_point_patchlist_27 = 59,
        control_point_patchlist_28 = 60,
        control_point_patchlist_29 = 61,
        control_point_patchlist_30 = 62,
        control_point_patchlist_31 = 63,
        control_point_patchlist_32 = 64,
    };

    /// @brief Queue options for a command queue creation
    /// @note  Matches the D3D12_COMMAND_QUEUE_DESC structure
    struct QueueOptions {
        enum class Priority {
            normal = 0,
            high = 100,
            global_realtime = 10000
        };
        enum class Flags {
            none = 0,
        };

        QueueOptions() = default;
        QueueOptions(QueueType type, Priority priority = Priority::normal, Flags flags = Flags::none, uint32_t node_mask = 0)
            : type(type), priority(priority), flags(flags), node_mask(node_mask) { }

        QueueType type = QueueType::direct; //< Type of the queue (not all types are supported on all backends)
        Priority priority = Priority::normal; //< Priority of the queue (D3D12 only, but changes Vulkan queue search algorithm)
        Flags flags = Flags::none; //< Flags for the queue creation (unused)
        uint32_t node_mask = 0; //< Node mask for multi GPU systems (default to 0 for single GPU systems)
    };

    /// @brief Structure for Subresource ranges selection on textures for barriers
    struct SubresourceRange {
        /// @brief Denotes the whole dimension of the texture (all mips, all layers)
        static constexpr inline uint32_t whole = ~0u;

        /// @brief Creates a full texture subresource range
        SubresourceRange() = default;

        /// @brief Creates a texture subresource range with the given parameters
        /// @param base_mip Base mip level
        /// @param extent_mips Number of mip levels to include in the range
        /// @param base_layer Base array layer
        /// @param extent_layers Number of array layers to include in the range
        SubresourceRange(uint32_t base_mip, uint32_t extent_mips, uint32_t base_layer, uint32_t extent_layers)
            : base_mip(base_mip), extent_mips(extent_mips), base_layer(base_layer), extent_layers(extent_layers) { }

        /// @brief Creates a texture subresource range with mips only selection
        /// @param base_mip Base mip level
        /// @param extent_mips Number of mip levels to include in the range
        /// @note  The range will include all array layers
        SubresourceRange(uint32_t base_mip, uint32_t extent_mips)
            : base_mip(base_mip), extent_mips(extent_mips) { }

        // TODO: Is there mipped array textures?
        uint32_t base_mip = 0;
        uint32_t extent_mips = whole;
        uint32_t base_layer = 0;
        uint32_t extent_layers = whole;
    };

    /// @brief Structure for selecting a texture type
    enum class TextureType {
        T1D = 2,
        T1DARRAY = 3,
        T2D = 4,
        T2DARRAY = 5,
        T2DMS = 6,
        T2DMSARRAY = 7,
        T3D = 8
    };

    /// @brief Structure for selecting a DescriptorHeap type
    enum class PoolType {
        CBV_SRV_UAV = 0,
        SAMPLER = 1,
    };

    enum class ShaderStage {
        vertex = 1, // VK_SHADER_STAGE_VERTEX_BIT,
        hull = 2, // VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
        domain = 4, // VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
        geometry = 8, // VK_SHADER_STAGE_GEOMETRY_BIT,
        pixel = 16, // VK_SHADER_STAGE_FRAGMENT_BIT,
        compute = 32, // VK_SHADER_STAGE_COMPUTE_BIT,
        all_graphics = 31, // VK_SHADER_STAGE_ALL_GRAPHICS,
        all = 0x7FFFFFFF, // VK_SHADER_STAGE_ALL,
    };

    enum class BindingType {
        SRV = 0,
        UAV,
        CBV,
        SAMPLER,
    };

    struct BindingDescriptor {
        uint32_t binding = 0;
        uint32_t count = 1;
        ShaderStage stages = ShaderStage::all;
        BindingType type = BindingType::SRV;
    };

    // TODO: Add support for cubemaps
    // TODO: Better selector for texture types
    struct RenderSelector {
        TextureType type = TextureType::T2DARRAY;
        uint32_t mip = 0;
        uint32_t base_layer = 0;
        uint32_t extent_layers = 1;
    };

    /// @brief Size of a texture in pixels
    struct Size2D {
        Size2D(uint32_t w, uint32_t h)
            : width(w), height(h) { }
        uint32_t width; //< Width of the texture in pixels
        uint32_t height; //< Height of the texture in pixels
    };

    /// @brief Color clear value for a render target
    using ColorClear = std::array<float, 4>;

    /// @brief Depth clear value for a depth stencil
    using DepthClear = float;
}

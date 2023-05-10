#pragma once
#include <string_view>

namespace wis
{
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

	enum class QueueType 
	{
		direct = 0,
		bundle = 1,
		compute = 2,
		copy = 3,
		video_decode = 4,
		video_process = 5,
		video_encode
	};
	struct Viewport
	{
		float top_leftx = 0;
		float top_lefty = 0;
		float width;
		float height;
		float min_depth = 0.0f;
		float max_depth = 1.0f;
	};
	struct ScissorRect
	{
		long left = 0;
		long top = 0;
		long right;
		long bottom;
	};
	enum class PrimitiveTopology
	{
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


	struct QueueOptions
	{
		enum class Priority {
			normal = 0,
			high = 100,
			global_realtime = 10000
		};
		enum class Flags {
			none = 0,
		};

		QueueType type = QueueType::direct;
		Priority priority = Priority::normal;
		Flags flags = Flags::none;
		uint32_t node_mask = 0;
	};

	struct SubresourceRange
	{
		static constexpr inline uint32_t whole = ~0u;
		uint32_t base_mip = 0;
		uint32_t extent_mips = 1;
		uint32_t base_layer = 0;
		uint32_t extent_layers = 1;
	};
	constexpr inline SubresourceRange EntireTexture{0, SubresourceRange::whole, 0, SubresourceRange::whole };


	enum class TextureType
	{
		T1D = 2,
		T1DARRAY = 3,
		T2D = 4,
		T2DARRAY = 5,
		T2DMS = 6,
		T2DMSARRAY = 7,
		T3D = 8
	};
	struct RenderSelector
	{
		TextureType type = TextureType::T2DARRAY;
		uint32_t mip = 0;
		uint32_t base_layer = 0;
		uint32_t extent_layers = 1;
	};


	/// Size of a texture in pixels
	struct Size2D
	{
		Size2D(uint32_t w, uint32_t h) : width(w), height(h) {}
		uint32_t width; //< Width of the texture in pixels
		uint32_t height; //< Height of the texture in pixels
	};

	using ColorClear = std::array<float, 4>;
	using DepthClear = float;
}
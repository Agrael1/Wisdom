#pragma once
#include <wisdom/api/api_common.h>
#include <wisdom/util/flags.h>
#include <vulkan/vulkan.hpp>

namespace wis
{
	constexpr inline vk::Format vk_format_map[]
	{
		vk::Format::eUndefined,//unknown = 0,
		vk::Format::eR32G32B32A32Sfloat,//r32g32b32a32_typeless = 1,
		vk::Format::eR32G32B32A32Sfloat,//r32g32b32a32_float = 1,
		vk::Format::eR32G32B32A32Uint,//r32g32b32a32_uint = 3,
		vk::Format::eR32G32B32A32Sint,//r32g32b32a32_sint = 4,
		vk::Format::eR32G32B32Sfloat,//r32g32b32_typeless = 5,
		vk::Format::eR32G32B32Sfloat,//r32g32b32_float = 6,
		vk::Format::eR32G32B32Uint,//r32g32b32_uint = 7,
		vk::Format::eR32G32B32Sint,//r32g32b32_sint = 8,

		vk::Format::eR16G16B16A16Sfloat,//r16g16b16a16_typeless = 9,
		vk::Format::eR16G16B16A16Sfloat,//r16g16b16a16_float = 10,
		vk::Format::eR16G16B16A16Unorm,//r16g16b16a16_unorm = 11,
		vk::Format::eR16G16B16A16Uint,//r16g16b16a16_uint = 12,
		vk::Format::eR16G16B16A16Snorm,//r16g16b16a16_snorm = 13,
		vk::Format::eR16G16B16A16Sint,//r16g16b16a16_sint = 14,

		vk::Format::eR32G32Sfloat,//r32g32_typeless = 15,
		vk::Format::eR32G32Sfloat,//r32g32_float = 16,
		vk::Format::eR32G32Uint,//r32g32_uint = 17,
		vk::Format::eR32G32Sint,//r32g32_sint = 18,

		vk::Format::eUndefined,//r32g8x24_typeless = 19,
		vk::Format::eUndefined,//d32_float_s8x24_uint = 20,
		vk::Format::eUndefined,//r32_float_x8x24_typeless = 21,
		vk::Format::eUndefined,//x32_typeless_g8x24_uint = 22,

		vk::Format::eUndefined,//r10g10b10a2_typeless = 23,
		vk::Format::eA2R10G10B10UnormPack32,//r10g10b10a2_unorm = 24,
		vk::Format::eA2R10G10B10UintPack32,//r10g10b10a2_uint = 25,
		vk::Format::eB10G11R11UfloatPack32,//r11g11b10_float = 26,

		vk::Format::eUndefined,//r8g8b8a8_typeless = 27,
		vk::Format::eR8G8B8A8Unorm,//r8g8b8a8_unorm = 28,
		vk::Format::eR8G8B8A8Srgb,//r8g8b8a8_unorm_srgb = 29,
		vk::Format::eR8G8B8A8Uint,//r8g8b8a8_uint = 30,
		vk::Format::eR8G8B8A8Snorm,//r8g8b8a8_snorm = 31,
		vk::Format::eR8G8B8A8Sint,//r8g8b8a8_sint = 32,

		vk::Format::eUndefined,//r16g16_typeless = 33,
		vk::Format::eR16G16Sfloat,//r16g16_float = 34,
		vk::Format::eR16G16Unorm,//r16g16_unorm = 35,
		vk::Format::eR16G16Uint,//r16g16_uint = 36,
		vk::Format::eR16G16Snorm,//r16g16_snorm = 37,
		vk::Format::eR16G16Sint,//r16g16_sint = 38,

		vk::Format::eUndefined,//r32_typeless = 39,
		vk::Format::eD32Sfloat,//d32_float = 40,
		vk::Format::eR32Sfloat,//r32_float = 41,
		vk::Format::eR32Uint,//r32_uint = 42,
		vk::Format::eR32Sint,//r32_sint = 43,

		vk::Format::eUndefined,//r24g8_typeless = 44,
		vk::Format::eD24UnormS8Uint,//d24_unorm_s8_uint = 45,
		vk::Format::eX8D24UnormPack32,//r24_unorm_x8_typeless = 46,
		vk::Format::eUndefined,//x24_typeless_g8_uint = 47,

		vk::Format::eUndefined,//r8g8_typeless = 48,
		vk::Format::eR8G8Unorm,//r8g8_unorm = 49,
		vk::Format::eR8G8Uint,//r8g8_uint = 50,
		vk::Format::eR8G8Snorm,//r8g8_snorm = 51,
		vk::Format::eR8G8Sint,//r8g8_sint = 52,

		//,//r16_typeless = 53,
		//,//r16_float = 54,
		//,//d16_unorm = 55,
		//,//r16_unorm = 56,
		//,//r16_uint = 57,
		//,//r16_snorm = 58,
		//,//r16_sint = 59,
		//,//r8_typeless = 60,
		//,//r8_unorm = 61,
		//,//r8_uint = 62,
		//,//r8_snorm = 63,
		//,//r8_sint = 64,
		//,//a8_unorm = 65,
		//,//r1_unorm = 66,
		//,//r9g9b9e5_sharedexp = 67,
		//,//r8g8_b8g8_unorm = 68,
		//,//g8r8_g8b8_unorm = 69,
		//,//bc1_typeless = 70,
		//,//bc1_unorm = 71,
		//,//bc1_unorm_srgb = 72,
		//,//bc2_typeless = 73,
		//,//bc2_unorm = 74,
		//,//bc2_unorm_srgb = 75,
		//,//bc3_typeless = 76,
		//,//bc3_unorm = 77,
		//,//bc3_unorm_srgb = 78,
		//,//bc4_typeless = 79,
		//,//bc4_unorm = 80,
		//,//bc4_snorm = 81,
		//,//bc5_typeless = 82,
		//,//bc5_unorm = 83,
		//,//bc5_snorm = 84,
		//,//b5g6r5_unorm = 85,
		//,//b5g5r5a1_unorm = 86,
		//,//b8g8r8a8_unorm = 87,
		//,//b8g8r8x8_unorm = 88,
		//,//r10g10b10_xr_bias_a2_unorm = 89,
		//,//b8g8r8a8_typeless = 90,
		//,//b8g8r8a8_unorm_srgb = 91,
		//,//b8g8r8x8_typeless = 92,
		//,//b8g8r8x8_unorm_srgb = 93,
		//,//bc6h_typeless = 94,
		//,//bc6h_uf16 = 95,
		//,//bc6h_sf16 = 96,
		//,//bc7_typeless = 97,
		//,//bc7_unorm = 98,
		//,//bc7_unorm_srgb = 99,
		//,//ayuv = 100,
		//,//y410 = 101,
		//,//y416 = 102,
		//,//nv12 = 103,
		//,//p010 = 104,
		//,//p016 = 105,
		//,//opaque_420 = 106,
		//,//yuy2 = 107,
		//,//y210 = 108,
		//,//y216 = 109,
		//,//nv11 = 110,
		//,//ai44 = 111,
		//,//ia44 = 112,
		//,//p8 = 113,
		//,//a8p8 = 114,
		//,//b4g4r4a4_unorm = 115,
		//,//
		//,//p208 = 130,
		//,//v208 = 131,
		//,//v408 = 132,
		//,//
		//,//
		//,//sampler_feedback_min_mip_opaque = 189,
		//,//sampler_feedback_mip_region_used_opaque = 190,
	};

	inline constexpr vk::Format map_format(wis::DataFormat df)
	{
		using namespace river::flags;
		if (+df > 52) return vk::Format::eUndefined;
		return vk_format_map[+df];
	}
}
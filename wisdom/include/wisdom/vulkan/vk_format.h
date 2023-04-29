#pragma once
#include <wisdom/api/api_common.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/api/api_render_pass.h>
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

		vk::Format::eR16Sfloat,//r16_typeless = 53,
		vk::Format::eR16Sfloat,//r16_float = 54,
		vk::Format::eD16Unorm,//d16_unorm = 55,
		vk::Format::eR16Unorm,//r16_unorm = 56,
		vk::Format::eR16Uint,//r16_uint = 57,
		vk::Format::eR16Snorm,//r16_snorm = 58,
		vk::Format::eR16Sint,//r16_sint = 59,

		vk::Format::eUndefined,//r8_typeless = 60,
		vk::Format::eR8Unorm,//r8_unorm = 61,
		vk::Format::eR8Uint,//r8_uint = 62,
		vk::Format::eR8Snorm,//r8_snorm = 63,
		vk::Format::eR8Sint,//r8_sint = 64,

		vk::Format::eUndefined,//a8_unorm = 65,
		vk::Format::eUndefined,//r1_unorm = 66,
		vk::Format::eUndefined,//r9g9b9e5_sharedexp = 67,
		vk::Format::eUndefined,//r8g8_b8g8_unorm = 68,
		vk::Format::eUndefined,//g8r8_g8b8_unorm = 69,
		vk::Format::eUndefined,//bc1_typeless = 70,
		vk::Format::eUndefined,//bc1_unorm = 71,
		vk::Format::eUndefined,//bc1_unorm_srgb = 72,
		vk::Format::eUndefined,//bc2_typeless = 73,
		vk::Format::eUndefined,//bc2_unorm = 74,
		vk::Format::eUndefined,//bc2_unorm_srgb = 75,
		vk::Format::eUndefined,//bc3_typeless = 76,
		vk::Format::eUndefined,//bc3_unorm = 77,
		vk::Format::eUndefined,//bc3_unorm_srgb = 78,
		vk::Format::eUndefined,//bc4_typeless = 79,
		vk::Format::eUndefined,//bc4_unorm = 80,
		vk::Format::eUndefined,//bc4_snorm = 81,
		vk::Format::eUndefined,//bc5_typeless = 82,
		vk::Format::eUndefined,//bc5_unorm = 83,
		vk::Format::eUndefined,//bc5_snorm = 84,
		vk::Format::eUndefined,//b5g6r5_unorm = 85,
		vk::Format::eUndefined,//b5g5r5a1_unorm = 86,

		vk::Format::eB8G8R8A8Unorm,//b8g8r8a8_unorm = 87,
		vk::Format::eB8G8R8A8Unorm,//b8g8r8x8_unorm = 88,
		vk::Format::eUndefined,//r10g10b10_xr_bias_a2_unorm = 89,
		vk::Format::eUndefined,//b8g8r8a8_typeless = 90,
		vk::Format::eB8G8R8A8Srgb,//b8g8r8a8_unorm_srgb = 91,
		vk::Format::eUndefined,//b8g8r8x8_typeless = 92,
		vk::Format::eB8G8R8A8Srgb,//b8g8r8x8_unorm_srgb = 93,

		vk::Format::eUndefined,//bc6h_typeless = 94,
		vk::Format::eUndefined,//bc6h_uf16 = 95,
		vk::Format::eUndefined,//bc6h_sf16 = 96,
		vk::Format::eUndefined,//bc7_typeless = 97,
		vk::Format::eUndefined,//bc7_unorm = 98,
		vk::Format::eUndefined,//bc7_unorm_srgb = 99,
		vk::Format::eUndefined,//ayuv = 100,
		vk::Format::eUndefined,//y410 = 101,
		vk::Format::eUndefined,//y416 = 102,
		vk::Format::eUndefined,//nv12 = 103,
		vk::Format::eUndefined,//p010 = 104,
		vk::Format::eUndefined,//p016 = 105,
		vk::Format::eUndefined,//opaque_420 = 106,
		vk::Format::eUndefined,//yuy2 = 107,
		vk::Format::eUndefined,//y210 = 108,
		vk::Format::eUndefined,//y216 = 109,
		vk::Format::eUndefined,//nv11 = 110,
		vk::Format::eUndefined,//ai44 = 111,
		vk::Format::eUndefined,//ia44 = 112,
		vk::Format::eUndefined,//p8 = 113,
		vk::Format::eUndefined,//a8p8 = 114,
		vk::Format::eUndefined,//b4g4r4a4_unorm = 115,

		vk::Format::eUndefined,//p208 = 130,
		vk::Format::eUndefined,//v208 = 131,
		vk::Format::eUndefined,//v408 = 132,

		vk::Format::eUndefined,//sampler_feedback_min_mip_opaque = 189,
		vk::Format::eUndefined,//sampler_feedback_mip_region_used_opaque = 190,
	};

	inline constexpr vk::Format vk_format(wis::DataFormat df)
	{
		using namespace river::flags;
		return vk_format_map[+df];
	}

	inline constexpr vk::ImageAspectFlags aspect_flags(vk::Format format)
	{
		switch (format)
		{
		case vk::Format::eD32SfloatS8Uint:
		case vk::Format::eD24UnormS8Uint:
		case vk::Format::eD16UnormS8Uint:
			return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
		case vk::Format::eD16Unorm:
		case vk::Format::eD32Sfloat:
		case vk::Format::eX8D24UnormPack32:
			return vk::ImageAspectFlagBits::eDepth;
		case vk::Format::eS8Uint:
			return vk::ImageAspectFlagBits::eStencil;
		default:
			return vk::ImageAspectFlagBits::eColor;
		}
	}

	inline constexpr vk::ImageLayout convert_state(ResourceState state)
	{
		switch (state) {
		default:
		case wis::ResourceState::common: return vk::ImageLayout::eGeneral;
		case wis::ResourceState::render_target: return vk::ImageLayout::eColorAttachmentOptimal;
		case wis::ResourceState::unordered_access: return vk::ImageLayout::eGeneral;
		case wis::ResourceState::depthstencil_write: return vk::ImageLayout::eDepthStencilAttachmentOptimal;
		case wis::ResourceState::depthstencil_read: return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
		case wis::ResourceState::non_pixel_shader_resource: return vk::ImageLayout::eShaderReadOnlyOptimal;
		case wis::ResourceState::pixel_shader_resource: return vk::ImageLayout::eShaderReadOnlyOptimal;
		case wis::ResourceState::copy_dest: return vk::ImageLayout::eTransferDstOptimal;
		case wis::ResourceState::copy_source: return vk::ImageLayout::eTransferSrcOptimal;
		case wis::ResourceState::shading_rate_source: return vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;
		case wis::ResourceState::present: return vk::ImageLayout::ePresentSrcKHR;
		case wis::ResourceState::undefined: return vk::ImageLayout::eUndefined;
		}
	}
	inline constexpr vk::ImageLayout convert(ResourceState state)
	{
		return convert_state(state);
	}
	inline constexpr vk::AttachmentLoadOp convert(PassLoadOperation state)
	{
		switch (state)
		{
		case PassLoadOperation::load:
			return vk::AttachmentLoadOp::eLoad;
		case PassLoadOperation::clear:
			return vk::AttachmentLoadOp::eClear;
		case PassLoadOperation::discard:
			return vk::AttachmentLoadOp::eDontCare;
		}
		assert(false);
		return vk::AttachmentLoadOp::eLoad;
	}
	inline constexpr vk::AttachmentStoreOp convert(PassStoreOperation state)
	{
		switch (state)
		{
		case PassStoreOperation::store:
			return vk::AttachmentStoreOp::eStore;
		case PassStoreOperation::discard:
			return vk::AttachmentStoreOp::eDontCare;
		}
		assert(false);
		return vk::AttachmentStoreOp::eStore;
	}




}
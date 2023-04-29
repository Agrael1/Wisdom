#pragma once
#include <cstdint>

namespace wis
{
	enum class BarrierType
	{
		transition,
		aliasing,
		uav
	};
	enum class BarrierFlags : uint32_t
	{
		none,
		begin_only,
		end_only
	};

	enum ResourceState : uint32_t
	{
		unknown = 0,
		common = 1 << 0,
		vertex_and_constant_buffer = 1 << 1,
		index_buffer = 1 << 2,
		render_target = 1 << 3,
		unordered_access = 1 << 4,
		depthstencil_write = 1 << 5,
		depthstencil_read = 1 << 6,
		non_pixel_shader_resource = 1 << 7,
		pixel_shader_resource = 1 << 8,
		indirect_argument = 1 << 9,
		copy_dest = 1 << 10,
		copy_source = 1 << 11,
		raytracing_acceleration_structure = 1 << 12,
		shading_rate_source = 1 << 13,
		present = 1 << 14,

		generic_read =
		vertex_and_constant_buffer |
		index_buffer |
		copy_source |
		non_pixel_shader_resource |
		pixel_shader_resource |
		indirect_argument,

		undefined = 1 << 15,
	};

	//TODO: Access
	struct ResourceBarrier
	{
		BarrierType type;
		BarrierFlags flags = BarrierFlags::none;
		ResourceState before;
		ResourceState after;
		uint32_t base_mip_level = 0;
		uint32_t level_count = 1;
		uint32_t base_array_layer = 0;
		uint32_t layer_count = 1;
	};
}
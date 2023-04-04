#pragma once

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
	enum class ResourceState //TODO: move to resource
	{
		common = 0,
		vertex_and_constant_buffer = 0x1,
		index_buffer = 0x2,
		render_target = 0x4,
		unordered_access = 0x8,
		depth_write = 0x10,
		depth_read = 0x20,
		non_pixel_shader_resource = 0x40,
		pixel_shader_resource = 0x80,
		stream_out = 0x100,
		indirect_argument = 0x200,
		copy_dest = 0x400,
		copy_source = 0x800,
		resolve_dest = 0x1000,
		resolve_source = 0x2000,
		raytracing_acceleration_structure = 0x400000,
		shading_rate_source = 0x1000000,
		generic_read = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
		all_shader_resource = (0x40 | 0x80),
		present = 0,
		predication = 0x200,
		video_decode_read = 0x10000,
		video_decode_write = 0x20000,
		video_process_read = 0x40000,
		video_process_write = 0x80000,
		video_encode_read = 0x200000,
		video_encode_write = 0x800000
	};



	template<BarrierType type, class Resource>
	struct ResourceBarrier;

	template<class Resource>
	struct ResourceBarrier<BarrierType::transition, Resource>
	{
		BarrierFlags flags = BarrierFlags::none;
		uint32_t subresource = std::numeric_limits<uint32_t>::max();
		const Resource& resource;

		ResourceState before;
		ResourceState after;
	};

	template<class Resource>
	using BasicTransitionBarrier = ResourceBarrier<BarrierType::transition, Resource>;
}
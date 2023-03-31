#pragma once

namespace wis
{
	struct QueueOptions
	{
		enum class Type {
			direct = 0,
			bundle = 1,
			compute = 2,
			copy = 3,
			video_decode = 4,
			video_process = 5,
			video_encode
		};
		enum class Priority {
			normal = 0,
			high = 100,
			global_realtime = 10000
		};
		enum class Flags {
			none = 0,
		};

		Type type = Type::direct;
		Priority priority = Priority::normal;
		Flags flags = Flags::none;
		uint32_t node_mask = 0;
	};
}
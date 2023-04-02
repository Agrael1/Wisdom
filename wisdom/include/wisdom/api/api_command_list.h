#pragma once

namespace wis
{
	enum class CommandListType
	{
        direct = 0,
        bundle = 1,
        compute = 2,
        copy = 3,
        video_decode = 4,
        video_process = 5,
        video_encode = 6,
        none = -1
	};
}
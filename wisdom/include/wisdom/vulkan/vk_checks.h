#pragma once
#include <vulkan/vulkan.hpp>

namespace wis
{
	inline bool succeded(vk::Result res)
	{
		return res == vk::Result::eSuccess;
	}
}
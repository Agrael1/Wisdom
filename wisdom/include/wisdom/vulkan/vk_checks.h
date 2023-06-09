#pragma once
#ifndef WISDOM_MODULES
#include <vulkan/vulkan.hpp>
#endif

namespace wis
{
	/// @brief Check if a vulkan result is a success
	/// @param res Result to check
	/// @return True if the result is a success
	inline bool succeded(vk::Result res)
	{
		return res == vk::Result::eSuccess;
	}
}
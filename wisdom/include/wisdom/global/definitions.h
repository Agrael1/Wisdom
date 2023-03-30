#pragma once

#define noxnd noexcept(!DEBUG_MODE)

namespace wis
{
	inline constexpr const bool debug_mode = DEBUG_MODE;
}
#pragma once

#define noxnd noexcept(!DEBUG_MODE)

namespace wis
{
	inline constexpr const bool debug_mode = DEBUG_MODE;
	inline constexpr const bool runtime_asserts = RUNTIME_ASSERTS;
}
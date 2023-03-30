#pragma once

namespace wis
{
	/// @brief Enumeration specifying Adapter query ordering
	enum class AdapterPreference
	{
		None, //< Default order (defined by system)
		MinConsumption, //< Order:Integrated, Discrete
		Performance //< Order: Discrete, Integrated
	};
}
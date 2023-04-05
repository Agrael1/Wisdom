#pragma once
#include <cstdint>

namespace wis
{
	/// @brief Enumeration specifying Adapter query ordering
	enum class AdapterPreference
	{
		None, //< Default order (defined by system)
		MinConsumption, //< Order:Integrated, Discrete
		Performance //< Order: Discrete, Integrated
	};

	//used only for vulkan
	// TODO: add layers and extensions
	struct ApplicationInfo
	{
	public:
		struct Version {
			uint32_t major{1};
			uint32_t minor{0};
			uint32_t patch{0};
		};
	public:
		const char* application_name = "";
		const char* engine_name = "";
		Version app_version{};
		Version engine_version{};
	};
}
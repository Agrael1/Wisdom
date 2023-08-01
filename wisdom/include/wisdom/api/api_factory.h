#pragma once
#ifndef WISDOM_MODULES
#include <cstdint>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
/// @brief Enumeration specifying Adapter query ordering
enum class AdapterPreference {
    None, //< Default order (defined by system)
    MinConsumption, //< Order:Integrated, Discrete
    Performance //< Order: Discrete, Integrated
};

/// @brief Application info, used only for debug purposes in Vulkan
struct ApplicationInfo {
    struct Version {
        uint32_t major{ 1 };
        uint32_t minor{ 0 };
        uint32_t patch{ 0 };
    };

    const char* application_name = "";
    const char* engine_name = "";
    Version app_version{};
    Version engine_version{};
};
}

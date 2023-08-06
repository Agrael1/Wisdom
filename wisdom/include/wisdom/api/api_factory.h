#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/bindings/types.h>
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
    struct ApplicationInfo : wisApplicationInfo {

    };
}

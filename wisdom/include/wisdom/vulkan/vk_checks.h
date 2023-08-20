#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/util/log_layer.h>
#include <vulkan/vulkan.hpp>
#include <wisdom/bridge/format.h>
#endif

namespace wis {
/// @brief Check if a vulkan result is a success
/// @param res Result to check
/// @return True if the result is a success
inline bool succeeded(vk::Result res, wis::Severity severity = wis::Severity::error, wis::source_location sl = wis::source_location::current()) noexcept
{
    if constexpr (debug_mode || runtime_asserts) {
        if (res != vk::Result::eSuccess) {
            wis::lib_log(severity, wis::format("Vulkan Error:\n[Error Code]: 0x{:08X}({})\n"
                                               "[Description]: {}\n",
                                               size_t(res), size_t(res), vk::to_string(res)),
                         sl);
        }
    }
    return res == vk::Result::eSuccess;
}
} // namespace wis
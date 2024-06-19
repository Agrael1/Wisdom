#ifndef VK_DEVICE_EXT_H
#define VK_DEVICE_EXT_H
#include <wisdom/generated/api/api.h>
#include <wisdom/util/misc.h>
#include <tuple>
#include <span>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <vulkan/vulkan_core.h>

namespace wis {
class VKDevice;
}

namespace wis {
struct VKDeviceExtension {
    virtual ~VKDeviceExtension() = default;

    // Unfortunate that we have to pass all the maps and sets here, but it's the only way to get the information
    virtual bool GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash>& available_extensions,
                                  std::unordered_set<std::string_view>& ext_name_set,
                                  std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                  std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        return true;
    }
    virtual bool Supported() const noexcept
    {
        return false;
    }

    virtual wis::Result Init(const wis::VKDevice& instance,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        return {};
    }
};
} // namespace wis

#endif // VK_DEVICE_EXT_H

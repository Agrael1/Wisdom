#ifndef WIS_VK_TYPES_H
#define WIS_VK_TYPES_H
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/impl/vulkan/vk_loader.hpp>
#include <vulkan/vulkan.h>
#include <unordered_set>
#include <cstring>
#include <array>
#include <atomic>

namespace wis {
namespace detail {
struct CStringHash {
    std::size_t operator()(const char* s) const
    {
        // Implementation of FNV-1a hash algorithm (fast and simple for strings)
        std::size_t hash = 2166136261u;
        while (*s) {
            hash ^= static_cast<std::size_t>(*s++);
            hash *= 16777619u;
        }
        return hash;
    }
};
struct CStringEqual {
    bool operator()(const char* a, const char* b) const
    {
        return std::strcmp(a, b) == 0;
    }
};
using CStringSet = std::unordered_set<const char*, CStringHash, CStringEqual>;

//-----------------------------------------------------------------------------
struct control_block_base {
public:
    size_t add_ref() noexcept
    {
        // Relaxed memory order is sufficient since this does not impose any ordering on other operations
        return m_ref_cnt.fetch_add(1, std::memory_order_relaxed);
    }

    size_t release() noexcept
    {
        // A release memory order to ensure that all releases are ordered
        return m_ref_cnt.fetch_sub(1, std::memory_order_release);
    }

public:
    std::atomic<size_t> m_ref_cnt{ 1 };
};

//-----------------------------------------------------------------------------
struct empty_type {
};

//-----------------------------------------------------------------------------
template<typename HeaderType>
struct control_block : public control_block_base {
    HeaderType header;
};
template<>
struct control_block<empty_type> : public control_block_base {
};
} // namespace detail

//-----------------------------------------------------------------------------
struct InstanceExtensionCollector {
    constexpr static const char* instance_extensions[]{
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
        VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    };
    constexpr static std::size_t instance_layer_initial_size = 4;
    constexpr static std::size_t instance_ext_initial_size   = 4;

    InstanceExtensionCollector() noexcept
    {
        enabled_extension_names_set.reserve(std::size(instance_extensions) + instance_ext_initial_size);
        enabled_layer_names_set.reserve(instance_layer_initial_size); // typical number of layers is small
        for (const auto& ext : instance_extensions) {
            enabled_extension_names_set.insert(ext);
        }
    }

    void AddEnabledExtension(const char* name) noexcept
    {
        enabled_extension_names_set.insert(name);
    }
    void AddEnabledLayer(const char* name) noexcept
    {
        enabled_layer_names_set.insert(name);
    }
    bool IsExtensionEnabled(const char* name) const noexcept
    {
        return enabled_extension_names_set.find(name) != enabled_extension_names_set.end();
    }
    bool IsLayerEnabled(const char* name) const noexcept
    {
        return enabled_layer_names_set.find(name) != enabled_layer_names_set.end();
    }

public:
    std::size_t GetEnabledExtensionCount() const noexcept
    {
        return enabled_extension_names_set.size();
    }
    std::size_t GetEnabledLayerCount() const noexcept
    {
        return enabled_layer_names_set.size();
    }

    const detail::CStringSet& GetEnabledExtensionNamesSet() const noexcept
    {
        return enabled_extension_names_set;
    }
    const detail::CStringSet& GetEnabledLayerNamesSet() const noexcept
    {
        return enabled_layer_names_set;
    }

private:
    detail::CStringSet enabled_extension_names_set;
    detail::CStringSet enabled_layer_names_set;
};

//-----------------------------------------------------------------------------
struct VKDeviceExtensionHeader {
    int dummy;
};

namespace impl {
//-----------------------------------------------------------------------------
struct VKMainGlobal {
    PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr                    vkGetDeviceProcAddr;
    PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkCreateInstance                       vkCreateInstance;
    PFN_vkEnumerateInstanceVersion             vkEnumerateInstanceVersion;

public:
    bool Init(void* library) noexcept
    {
        ASSIGN_PROC_ADDRESS_CHECK(library, vkGetInstanceProcAddr);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkGetDeviceProcAddr);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkEnumerateInstanceLayerProperties);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkEnumerateInstanceExtensionProperties);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkCreateInstance);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkEnumerateInstanceVersion);
        return true;
    }
};

//-----------------------------------------------------------------------------
struct VKMainAdapter {
    PFN_vkGetPhysicalDeviceMemoryProperties        vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceProperties              vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties   vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkEnumerateDeviceExtensionProperties       vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceFeatures2               vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceProperties2             vkGetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties2       vkGetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR       vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR  vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR       vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR  vkGetPhysicalDeviceSurfacePresentModesKHR;

public:
    bool Init(VkInstance instance, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceMemoryProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceQueueFamilyProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkEnumerateDeviceExtensionProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceFeatures2, "vkGetPhysicalDeviceFeatures2KHR");
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceProperties2, "vkGetPhysicalDeviceProperties2KHR");
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceMemoryProperties2, "vkGetPhysicalDeviceMemoryProperties2KHR");
        ASSIGN_PROC_ADDRESS_OPTIONAL(instance, vkGetPhysicalDeviceSurfaceCapabilities2KHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceSupportKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceFormatsKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfacePresentModesKHR);
        return true;
    }
};
struct VKMainInstance {
    PFN_vkDestroyInstance          vkDestroyInstance;
    PFN_vkCreateDevice             vkCreateDevice;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkDestroySurfaceKHR        vkDestroySurfaceKHR;

public:
    bool Init(VkInstance instance, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkDestroyInstance);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkCreateDevice);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkEnumeratePhysicalDevices);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkDestroySurfaceKHR);
        return true;
    }
};

//-----------------------------------------------------------------------------
struct VKInstanceHeader {
    VKMainGlobal                global_table;
    VKMainInstance              instance_table;
    VKMainAdapter               adapter_table;
    wis::detail::unique_library library;
};

struct VKInstanceImpl {
    VkInstance                               instance;
    detail::control_block<VKInstanceHeader>* shared_header;
    uint32_t                                 api_version;
    bool                                     debug_layer;
};
} // namespace impl

struct VKInstanceExtensionHeader {
    virtual void      CollectInfo(InstanceExtensionCollector& collector)         = 0;
    virtual WisResult Init(struct impl::VKInstanceImpl& instance_impl,
                           wis::span<const char* const> enabled_extensions,
                           wis::span<const char* const> enabled_layers) noexcept = 0;
};
} // namespace wis

#endif // DX12_FACTORY_H

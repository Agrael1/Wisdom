#ifndef WIS_VK_TYPES_H
#define WIS_VK_TYPES_H
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/bridge/span.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/impl/vulkan/vk_tables.hpp>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include <array>
#include <atomic>

namespace wis {
//-----------------------------------------------------------------------------
constexpr inline wis::Result convert_result(WisResult result) noexcept
{
    return { static_cast<wis::Status>(result.status), result.platform_code, result.error };
}

//-----------------------------------------------------------------------------
namespace detail {
// Hash helpers
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
//-----------------------------------------------------------------------------
// hash for VkExtensionProperties
struct VkExtensionPropertiesHash {
    using is_transparent = void;
    std::size_t operator()(const VkExtensionProperties& ext) const noexcept
    {
        return CStringHash{}(ext.extensionName);
    }
    std::size_t operator()(const char* name) const noexcept
    {
        return CStringHash{}(name);
    }
};

//-----------------------------------------------------------------------------
struct VkLayerPropertiesHash {
    using is_transparent = void;
    std::size_t operator()(const VkLayerProperties& layer) const noexcept
    {
        return CStringHash{}(layer.layerName);
    }
    std::size_t operator()(const char* name) const noexcept
    {
        return CStringHash{}(name);
    }
};

// Equality helpers
//-----------------------------------------------------------------------------
struct CStringEqual {
    bool operator()(const char* a, const char* b) const
    {
        return std::strcmp(a, b) == 0;
    }
};

//-----------------------------------------------------------------------------
struct VkExtensionPropertiesEqual {
    using is_transparent = void;
    bool operator()(const VkExtensionProperties& ext, const char* name) const noexcept
    {
        return std::strcmp(ext.extensionName, name) == 0;
    }
    bool operator()(const char* name, const VkExtensionProperties& ext) const noexcept
    {
        return std::strcmp(name, ext.extensionName) == 0;
    }
    bool operator()(const VkExtensionProperties& a, const VkExtensionProperties& b) const noexcept
    {
        return std::strcmp(a.extensionName, b.extensionName) == 0;
    }
};

//-----------------------------------------------------------------------------
struct VkLayerPropertiesEqual {
    using is_transparent = void;
    bool operator()(const VkLayerProperties& layer, const char* name) const noexcept
    {
        return std::strcmp(layer.layerName, name) == 0;
    }
    bool operator()(const char* name, const VkLayerProperties& layer) const noexcept
    {
        return std::strcmp(name, layer.layerName) == 0;
    }
    bool operator()(const VkLayerProperties& a, const VkLayerProperties& b) const noexcept
    {
        return std::strcmp(a.layerName, b.layerName) == 0;
    }
};

using CStringSet               = std::unordered_set<const char*, CStringHash, CStringEqual>;
using VkExtensionPropertiesSet = std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual>;
using VkLayerPropertiesSet     = std::unordered_set<VkLayerProperties, VkLayerPropertiesHash, VkLayerPropertiesEqual>;

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

//-----------------------------------------------------------------------------
struct VKDebugCallbackThunk {
public:
    static VkBool32 VKAPI_PTR wisDebugUtilsMessengerCallbackThunk(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void*                                       pUserData)
    {
        auto* thunk = reinterpret_cast<const VKDebugCallbackThunk*>(pUserData);
        thunk->wisDebugUtilsMessengerCallback(messageSeverity, messageTypes, pCallbackData);
        return false;
    }
    void wisDebugUtilsMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const
    {
        WisSeverity wis_severity = WisSeverityInfo;
        switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            wis_severity = WisSeverityVerbose;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            wis_severity = WisSeverityInfo;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            wis_severity = WisSeverityWarning;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            wis_severity = WisSeverityError;
            break;
        default:
            wis_severity = WisSeverityInfo;
            break;
        }

        // Get device handle if possible
        uint64_t device = 0;
        for (auto&& obj : wis::span<const VkDebugUtilsObjectNameInfoEXT>{ pCallbackData->pObjects, pCallbackData->objectCount }) {
            if (obj.objectType == VK_OBJECT_TYPE_DEVICE) {
                device = obj.objectHandle;
                break;
            }
        }

        callback(wis_severity, pCallbackData->pMessage, device, user_data);
    }

public:
    WisDebugCallback callback  = nullptr;
    void*            user_data = nullptr;
};
} // namespace detail

namespace impl {
//-----------------------------------------------------------------------------
struct VKInstanceHeader {
    VKMainGlobal                global_table;
    VKMainInstance              instance_table;
    VKMainAdapter               adapter_table;
    VkDebugUtilsMessengerEXT    debug_messenger;
    wis::detail::unique_library library;

    std::unique_ptr<wis::detail::VKDebugCallbackThunk> debug_callback_thunk;
};

struct VKDeviceHeader {
    VKMainDevice                               device_table;
    VKMainCommandQueue                         command_queue_table;
    VKMainCommandList                          command_list_table;
    std::unique_ptr<VkQueueFamilyProperties[]> queue_family_properties;
    uint8_t*                                   queue_semaphores;
    std::size_t                                queue_family_count;
    detail::control_block<VKInstanceHeader>*   shared_header;
    VkInstance                                 instance;
};

struct VKInstanceImpl {
    VkInstance                               instance;
    detail::control_block<VKInstanceHeader>* shared_header;
    uint32_t                                 api_version;
};

struct VKAdapterQueryImpl {
    VkPhysicalDevice*                        physical_devices;
    std::size_t                              adapter_count;
    VkInstance                               instance;
    detail::control_block<VKInstanceHeader>* shared_header;
};
struct VKDeviceImpl {
    VkDevice                               device;
    VkPhysicalDevice                       physical_device;
    detail::control_block<VKDeviceHeader>* device_header;
};
struct VKCommandQueueImpl {
    VkQueue                                queue;
    uint8_t*                               semaphore_ptr;
    VkDevice                               device;
    detail::control_block<VKDeviceHeader>* device_header;
};
} // namespace impl

//-----------------------------------------------------------------------------
struct WISDOM_API VKInstanceExtensionCollector {
    constexpr static const char* instance_extensions[]{
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
        VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    };
    constexpr static std::size_t instance_layer_initial_size = 4;
    constexpr static std::size_t instance_ext_initial_size   = 4;

public:
    VKInstanceExtensionCollector(const impl::VKMainGlobal& table, WisResult& out_result) noexcept;

public:
    void EnableExtension(const char* name) noexcept
    {
        if (IsExtensionPresent(name)) {
            enabled_extension_names_set.insert(name);
        }
    }
    void EnableLayer(const char* name) noexcept
    {
        if (IsLayerPresent(name)) {
            enabled_layer_names_set.insert(name);
        }
    }

    WIS_INLINE bool                         IsExtensionPresent(const char* name) const noexcept;
    WIS_INLINE bool                         IsLayerPresent(const char* name) const noexcept;
    WIS_INLINE const VkLayerProperties*     GetLayerProperties(const char* name) const noexcept;
    WIS_INLINE const VkExtensionProperties* GetExtensionProperties(const char* name) const noexcept;

public:
    struct ExtReturn {
        std::unique_ptr<const char*[]> names_array;
        std::size_t                    count_exts;
        std::size_t                    count_layers;
    };
    WIS_NODISCARD ExtReturn GetExtensionsAndLayers(WisResult& out_res) const noexcept;

private:
    detail::CStringSet               enabled_extension_names_set;
    detail::CStringSet               enabled_layer_names_set;
    detail::VkExtensionPropertiesSet available_extensions_set;
    detail::VkLayerPropertiesSet     available_layers_set;
};

//-----------------------------------------------------------------------------
struct WISDOM_API VKDeviceExtensionCollector {
    struct ExtensionInfo {
        const char*     name;
        VkStructureType feature_struct       = VK_STRUCTURE_TYPE_MAX_ENUM;
        std::size_t     feature_struct_size  = 0;
        VkStructureType property_struct      = VK_STRUCTURE_TYPE_MAX_ENUM;
        std::size_t     property_struct_size = 0;
    };

public:
    VKDeviceExtensionCollector(const impl::VKMainAdapter& adapter_table,
                               VkPhysicalDevice           adapter,
                               WisResult&                 res) noexcept;

    // Phase 1: enable extensions
public:
    WIS_INLINE void                         EnableExtension(const ExtensionInfo& extension) noexcept;
    WIS_INLINE bool                         IsExtensionPresent(const char* name) const noexcept;
    WIS_INLINE const VkExtensionProperties* GetExtensionProperties(const char* name) const noexcept;

    struct InitBuffer {
        std::unique_ptr<uint8_t[]> buffer;
        const char**               extension_names  = nullptr;
        std::size_t                extension_count  = 0;
        VkBaseOutStructure*        feature_structs  = nullptr;
        VkBaseOutStructure*        property_structs = nullptr;
    };
    WIS_INLINE InitBuffer GetInitBuffer(WisResult& out_res) const noexcept;

    // Used by device initialization to force bind feature structures
    template<typename VKFeatureStruct>
    void ForceBindFeatureStruct(VKFeatureStruct* feature_struct) noexcept
    {
        feature_map[feature_struct->sType] = uintptr_t(feature_struct);
    }
    template<typename VKPropertyStruct>
    void ForceBindPropertyStruct(VKPropertyStruct* property_struct) noexcept
    {
        property_map[property_struct->sType] = uintptr_t(property_struct);
    }

    // Phase 2: get enabled extensions, features and properties
public:
    template<typename VKFeatureStruct>
    VKFeatureStruct* GetEnabledFeatureStruct(VkStructureType type) const noexcept
    {
        auto it = feature_map.find(type);
        if (it != feature_map.end()) {
            return reinterpret_cast<VKFeatureStruct*>(it->second);
        }
        return nullptr;
    }
    template<typename VKPropertyStruct>
    VKPropertyStruct* GetEnabledPropertyStruct(VkStructureType type) const noexcept
    {
        auto it = property_map.find(type);
        if (it != property_map.end()) {
            return reinterpret_cast<VKPropertyStruct*>(it->second);
        }
        return nullptr;
    }

private:
    detail::CStringSet                                     enabled_extension_names_set;
    detail::VkExtensionPropertiesSet                       available_extensions_set;
    mutable std::unordered_map<VkStructureType, uintptr_t> feature_map;
    mutable std::unordered_map<VkStructureType, uintptr_t> property_map;
};

//-----------------------------------------------------------------------------
struct VKInstanceExtensionHeader {
    virtual void      CollectInfo(VKInstanceExtensionCollector& collector)         = 0;
    virtual WisResult Init(struct impl::VKInstanceImpl&        instance_impl,
                           const VKInstanceExtensionCollector& collector) noexcept = 0;
};
struct VKDeviceExtensionHeader {
    virtual void      CollectInfo(VKDeviceExtensionCollector& collector)         = 0;
    virtual WisResult Init(struct impl::VKDeviceImpl&        device_impl,
                           const VKDeviceExtensionCollector& collector) noexcept = 0;
};
} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "vk_instance.cpp"
#include "vk_types.cpp"
#endif // WISDOM_BUILD_BINARIES
#endif // DX12_FACTORY_H

#ifndef WIS_VK_EXTENSIONS_HPP
#define WIS_VK_EXTENSIONS_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <wisdom/global/definitions.h>
#include <wisdom/vulkan/vk_tables.hpp>
#include <wisdom/vulkan/vk_types.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <unordered_set>

namespace wis {
namespace impl {
struct VKInstanceImpl;
struct VKDeviceImpl;
} // namespace impl

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
//----------------------------------------------------------------------------------------------------------------------
// hash for VkExtensionProperties
struct VkExtensionPropertiesHash {
    using is_transparent = void;
    std::size_t operator()(const VkExtensionProperties& ext) const noexcept { return CStringHash{}(ext.extensionName); }
    std::size_t operator()(const char* name) const noexcept { return CStringHash{}(name); }
};

//----------------------------------------------------------------------------------------------------------------------
struct VkLayerPropertiesHash {
    using is_transparent = void;
    std::size_t operator()(const VkLayerProperties& layer) const noexcept { return CStringHash{}(layer.layerName); }
    std::size_t operator()(const char* name) const noexcept { return CStringHash{}(name); }
};

// Equality helpers
//----------------------------------------------------------------------------------------------------------------------
struct CStringEqual {
    bool operator()(const char* a, const char* b) const { return std::strncmp(a, b, VK_MAX_EXTENSION_NAME_SIZE) == 0; }
};

//----------------------------------------------------------------------------------------------------------------------
struct VkExtensionPropertiesEqual {
    using is_transparent = void;
    bool operator()(const VkExtensionProperties& ext, const char* name) const noexcept
    {
        return std::strncmp(ext.extensionName, name, VK_MAX_EXTENSION_NAME_SIZE) == 0;
    }
    bool operator()(const char* name, const VkExtensionProperties& ext) const noexcept
    {
        return std::strncmp(name, ext.extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0;
    }
    bool operator()(const VkExtensionProperties& a, const VkExtensionProperties& b) const noexcept
    {
        return std::strncmp(a.extensionName, b.extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0;
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct VkLayerPropertiesEqual {
    using is_transparent = void;
    bool operator()(const VkLayerProperties& layer, const char* name) const noexcept
    {
        return std::strncmp(layer.layerName, name, VK_MAX_EXTENSION_NAME_SIZE) == 0;
    }
    bool operator()(const char* name, const VkLayerProperties& layer) const noexcept
    {
        return std::strncmp(name, layer.layerName, VK_MAX_EXTENSION_NAME_SIZE) == 0;
    }
    bool operator()(const VkLayerProperties& a, const VkLayerProperties& b) const noexcept
    {
        return std::strncmp(a.layerName, b.layerName, VK_MAX_EXTENSION_NAME_SIZE) == 0;
    }
};

using CStringSet = std::unordered_set<const char*, CStringHash, CStringEqual>;
using VkExtensionPropertiesSet = std::
    unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual>;
using VkLayerPropertiesSet = std::unordered_set<VkLayerProperties, VkLayerPropertiesHash, VkLayerPropertiesEqual>;
} // namespace detail

//----------------------------------------------------------------------------------------------------------------------
struct WISDOM_API VKInstanceExtensionCollector {
    constexpr static const char* instance_extensions[]{
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
        VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    };
    constexpr static std::size_t instance_layer_initial_size = 4;
    constexpr static std::size_t instance_ext_initial_size = 4;

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

    bool IsExtensionPresent(const char* name) const noexcept
    {
        return available_extensions_set.find(name) != available_extensions_set.end();
    }

    bool IsLayerPresent(const char* name) const noexcept
    {
        return available_layers_set.find(name) != available_layers_set.end();
    }
    WIS_INLINE const VkLayerProperties* GetLayerProperties(const char* name) const noexcept;
    WIS_INLINE const VkExtensionProperties* GetExtensionProperties(const char* name) const noexcept;

public:
    struct ExtReturn {
        std::unique_ptr<const char*[]> names_array;
        std::size_t count_exts;
        std::size_t count_layers;
    };
    WIS_NODISCARD ExtReturn GetExtensionsAndLayers(WisResult& out_res) const noexcept;

private:
    detail::CStringSet enabled_extension_names_set;
    detail::CStringSet enabled_layer_names_set;
    detail::VkExtensionPropertiesSet available_extensions_set;
    detail::VkLayerPropertiesSet available_layers_set;
};

//----------------------------------------------------------------------------------------------------------------------
struct WISDOM_API VKDeviceExtensionCollector {
    struct ExtensionInfo {
        const char* name;
        VkStructureType feature_struct = VK_STRUCTURE_TYPE_MAX_ENUM;
        std::size_t feature_struct_size = 0;
        VkStructureType property_struct = VK_STRUCTURE_TYPE_MAX_ENUM;
        std::size_t property_struct_size = 0;
    };

public:
    VKDeviceExtensionCollector(
        const impl::VKMainAdapter& adapter_table,
        VkPhysicalDevice adapter,
        WisResult& res
    ) noexcept;

    // Phase 1: enable extensions
public:
    WIS_INLINE void EnableExtension(const ExtensionInfo& extension) noexcept;
    WIS_INLINE bool IsExtensionPresent(const char* name) const noexcept;
    WIS_INLINE const VkExtensionProperties* GetExtensionProperties(const char* name) const noexcept;

    struct InitBuffer {
        std::unique_ptr<uint64_t[]> buffer;
        const char** extension_names = nullptr;
        std::size_t extension_count = 0;
        VkBaseOutStructure* feature_structs = nullptr;
        VkBaseOutStructure* property_structs = nullptr;
    };
    WIS_INLINE InitBuffer GetInitBuffer(WisResult& out_res) const noexcept;

    // Used by device initialization to force bind feature structures
    template <typename VKFeatureStruct>
    void ForceBindFeatureStruct(VKFeatureStruct* feature_struct) noexcept
    {
        feature_map[feature_struct->sType] = uintptr_t(feature_struct);
    }
    template <typename VKPropertyStruct>
    void ForceBindPropertyStruct(VKPropertyStruct* property_struct) noexcept
    {
        property_map[property_struct->sType] = uintptr_t(property_struct);
    }

    // Phase 2: get enabled extensions, features and properties
public:
    template <typename VKFeatureStruct>
    VKFeatureStruct* GetEnabledFeatureStruct(VkStructureType type) const noexcept
    {
        auto it = feature_map.find(type);
        if (it != feature_map.end()) {
            return reinterpret_cast<VKFeatureStruct*>(it->second);
        }
        return nullptr;
    }
    template <typename VKPropertyStruct>
    VKPropertyStruct* GetEnabledPropertyStruct(VkStructureType type) const noexcept
    {
        auto it = property_map.find(type);
        if (it != property_map.end()) {
            return reinterpret_cast<VKPropertyStruct*>(it->second);
        }
        return nullptr;
    }

private:
    detail::CStringSet enabled_extension_names_set;
    detail::VkExtensionPropertiesSet available_extensions_set;
    mutable std::unordered_map<VkStructureType, uintptr_t> feature_map;
    mutable std::unordered_map<VkStructureType, uintptr_t> property_map;
};

template <typename T>
struct VKInstanceExtensionImpl : public VKInstanceExtensionHeader {
    VKInstanceExtensionImpl() noexcept
        : VKInstanceExtensionHeader{&VKInstanceExtensionImpl<T>::InitThunk}
    {
        assert(
            std::uintptr_t(static_cast<T*>(this)) == std::uintptr_t(static_cast<VKInstanceExtensionHeader*>(this))
            && "VKInstanceExtensionImpl must be the first base class!"
        );
    }

private:
    static WisResult InitThunk(
        VKInstanceExtensionHeader* self,
        impl::VKInstanceImpl* instance_impl,
        VKInstanceExtensionCollector* collector
    ) noexcept
    {
        if (!instance_impl) {
            return reinterpret_cast<T*>(self)->CollectInfo(*collector);
        }
        return reinterpret_cast<T*>(self)->Init(
            const_cast<const impl::VKInstanceImpl&>(*instance_impl),
            const_cast<const VKInstanceExtensionCollector&>(*collector)
        );
    }

public:
    WisResult Init(const impl::VKInstanceImpl& instance_impl, const VKInstanceExtensionCollector& collector) noexcept
    {
        (void)instance_impl;
        (void)collector;
        return {};
    }
    WisResult CollectInfo(VKInstanceExtensionCollector& collector) noexcept
    {
        (void)collector;
        return {};
    }
};

template <typename T>
struct VKDeviceExtensionImpl : public VKDeviceExtensionHeader {
    VKDeviceExtensionImpl() noexcept
        : VKDeviceExtensionHeader{&VKDeviceExtensionImpl<T>::InitThunk}
    {
        assert(
            std::uintptr_t(static_cast<T*>(this)) == std::uintptr_t(static_cast<VKDeviceExtensionHeader*>(this))
            && "VKDeviceExtensionImpl must be the first base class!"
        );
    }

private:
    static WisResult InitThunk(
        VKDeviceExtensionHeader* self,
        impl::VKDeviceImpl* device_impl,
        VKDeviceExtensionCollector* collector
    ) noexcept
    {
        if (!device_impl) {
            return reinterpret_cast<T*>(self)->CollectInfo(*collector);
        }
        return reinterpret_cast<T*>(self)->Init(
            const_cast<const impl::VKDeviceImpl&>(*device_impl),
            const_cast<const VKDeviceExtensionCollector&>(*collector)
        );
    }

public:
    WisResult Init(const impl::VKDeviceImpl& device_impl, const VKDeviceExtensionCollector& collector) noexcept
    {
        (void)device_impl;
        (void)collector;
        return {};
    }
    WisResult CollectInfo(VKDeviceExtensionCollector& collector) noexcept
    {
        (void)collector;
        return {};
    }
};
} // namespace wis

#endif // !WIS_VK_DEVICE_EXT_HPP

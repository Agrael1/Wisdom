#ifndef WIS_VK_TYPES_CPP
#define WIS_VK_TYPES_CPP

#include <wisdom/impl/vulkan/vk_types.hpp>
#include <wisdom/impl/vulkan/vk_utils.hpp>
#include <wisdom/util/allocation.hpp>

using namespace wis;
using namespace wis::detail;
using namespace wis::impl;

namespace wis::detail {
inline std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual>
GetInstanceExtensions(WisResult& result, const VKMainGlobal& table) noexcept
{
    std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual> exts;
    // Get available extensions
    uint32_t ext_count = 0;
    VkResult vr        = table.vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
    if (!succeeded(vr)) {
        result = make_result<Func(), "Failed to enumerate Vulkan instance extension properties">(vr);
        return exts;
    }
    std::unique_ptr<VkExtensionProperties[]> ext_props_raw = make_unique<VkExtensionProperties[]>(ext_count);
    if (!ext_props_raw) {
        result = make_result<Func(), "Not enough memory for extension properties">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return exts;
    }
    vr = table.vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, ext_props_raw.get());

    // Unique set of extensions
    try {
        exts.reserve(ext_count);
    } catch (const std::bad_alloc&) {
        result = make_result<Func(), "Not enough memory for extensions">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return exts;
    }
    for (const auto& i : wis::span{ ext_props_raw.get(), ext_count }) {
        exts.insert(i);
    }
    return exts;
}

inline std::unordered_set<VkLayerProperties, VkLayerPropertiesHash, VkLayerPropertiesEqual>
GetInstanceLayers(WisResult& result, const VKMainGlobal& table) noexcept
{
    std::unordered_set<VkLayerProperties, VkLayerPropertiesHash, VkLayerPropertiesEqual> layers;
    if (result.status != WisStatusOk) {
        return layers;
    }

    // Get available layers
    uint32_t layer_count = 0;
    VkResult vr          = table.vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (!succeeded(vr)) {
        result = make_result<Func(), "Failed to enumerate Vulkan instance layer properties">(vr);
        return layers;
    }
    std::unique_ptr<VkLayerProperties[]> layer_props_raw = make_unique<VkLayerProperties[]>(layer_count);
    if (!layer_props_raw) {
        result = make_result<Func(), "Not enough memory for layer properties">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return layers;
    }
    vr = table.vkEnumerateInstanceLayerProperties(&layer_count, layer_props_raw.get());
    // Unique set of layers
    try {
        layers.reserve(layer_count);
    } catch (const std::bad_alloc&) {
        result = make_result<Func(), "Not enough memory for layers">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return layers;
    }
    for (const auto& i : wis::span{ layer_props_raw.get(), layer_count }) {
        layers.insert(i);
    }
    return layers;
}

inline std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual>
GetDeviceExtensions(WisResult&           result,
                    const VKMainAdapter& adapter_table,
                    VkPhysicalDevice     physical_device) noexcept
{
    std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual> exts;

    // Get available extensions
    uint32_t ext_count = 0;
    VkResult vr        = adapter_table.vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &ext_count, nullptr);
    if (!succeeded(vr)) {
        result = make_result<Func(), "Failed to enumerate Vulkan device extension properties">(vr);
        return exts;
    }
    std::unique_ptr<VkExtensionProperties[]> ext_props_raw = make_unique<VkExtensionProperties[]>(ext_count);
    if (!ext_props_raw) {
        result = make_result<Func(), "Not enough memory for extension properties">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return exts;
    }
    vr = adapter_table.vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &ext_count, ext_props_raw.get());
    // Unique set of extensions
    try {
        exts.reserve(ext_count);
    } catch (const std::bad_alloc&) {
        result = make_result<Func(), "Not enough memory for extensions">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return exts;
    }
    for (const auto& i : wis::span{ ext_props_raw.get(), ext_count }) {
        exts.insert(i);
    }
    return exts;
}
} // namespace wis::detail

wis::VKInstanceExtensionCollector::VKInstanceExtensionCollector(const VKMainGlobal& table, WisResult& out_result) noexcept
    : available_extensions_set(GetInstanceExtensions(out_result, table))
    , available_layers_set(GetInstanceLayers(out_result, table))
{
    if (out_result.status != WisStatusOk) {
        return;
    }

    try {
        enabled_extension_names_set.reserve(wis::detail::size(available_extensions_set));
        enabled_layer_names_set.reserve(wis::detail::size(available_layers_set));
    } catch (const std::bad_alloc&) {
        out_result = make_result<Func(), "Not enough memory for extension and layer sets">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return;
    }
    for (const auto& ext : instance_extensions) {
        enabled_extension_names_set.insert(ext);
    }
    out_result = vk_success;
}

wis::VKInstanceExtensionCollector::ExtReturn
wis::VKInstanceExtensionCollector::GetExtensionsAndLayers(WisResult& out_res) const noexcept
{
    ExtReturn   result;
    std::size_t ext_count   = enabled_extension_names_set.size();
    std::size_t layer_count = enabled_layer_names_set.size();

    auto names_array = make_unique<const char*[]>(ext_count + layer_count);
    if (!names_array) {
        out_res = make_result<Func(), "Not enough memory for extension and layer names">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return result;
    }
    std::size_t index = 0;
    for (const auto& name : enabled_extension_names_set) {
        names_array[index++] = name;
    }
    for (const auto& name : enabled_layer_names_set) {
        names_array[index++] = name;
    }

    result.names_array  = std::move(names_array);
    result.count_exts   = ext_count;
    result.count_layers = layer_count;
    return result;
}

bool wis::VKInstanceExtensionCollector::IsExtensionPresent(const char* name) const noexcept
{
    return available_extensions_set.find(name) != available_extensions_set.end();
}
bool wis::VKInstanceExtensionCollector::IsLayerPresent(const char* name) const noexcept
{
    return available_layers_set.find(name) != available_layers_set.end();
}
const VkLayerProperties*
wis::VKInstanceExtensionCollector::GetLayerProperties(const char* name) const noexcept
{
    auto it = available_layers_set.find(name);
    if (it != available_layers_set.end()) {
        return &(*it);
    }
    return nullptr;
}
const VkExtensionProperties*
wis::VKInstanceExtensionCollector::GetExtensionProperties(const char* name) const noexcept
{
    auto it = available_extensions_set.find(name);
    if (it != available_extensions_set.end()) {
        return &(*it);
    }
    return nullptr;
}

wis::VKDeviceExtensionCollector::VKDeviceExtensionCollector(const VKMainAdapter& adapter_table,
                                                            VkPhysicalDevice     adapter,
                                                            WisResult&           res) noexcept
    : available_extensions_set(GetDeviceExtensions(res, adapter_table, adapter))
{
    if (res.status != WisStatusOk) {
        return;
    }

    try {
        enabled_extension_names_set.reserve(wis::detail::size(available_extensions_set));
    } catch (const std::bad_alloc&) {
        res = make_result<Func(), "Not enough memory for extension set">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return;
    }
    res = vk_success;
}

void wis::VKDeviceExtensionCollector::EnableExtension(const ExtensionInfo& extension) noexcept
{
    if (IsExtensionPresent(extension.name)) {
        enabled_extension_names_set.insert(extension.name);
        if (extension.feature_struct != VK_STRUCTURE_TYPE_MAX_ENUM) {
            feature_map[extension.feature_struct] = uintptr_t(extension.feature_struct_size);
        }
        if (extension.property_struct != VK_STRUCTURE_TYPE_MAX_ENUM) {
            property_map[extension.property_struct] = uintptr_t(extension.property_struct_size);
        }
    }
}

bool wis::VKDeviceExtensionCollector::IsExtensionPresent(const char* name) const noexcept
{
    return available_extensions_set.find(name) != available_extensions_set.end();
}
const VkExtensionProperties* wis::VKDeviceExtensionCollector::GetExtensionProperties(const char* name) const noexcept
{
    auto it = available_extensions_set.find(name);
    if (it != available_extensions_set.end()) {
        return &(*it);
    }
    return nullptr;
}

wis::VKDeviceExtensionCollector::InitBuffer
wis::VKDeviceExtensionCollector::GetInitBuffer(WisResult& out_res) const noexcept
{
    InitBuffer result;

    // Allocate the buffer for:
    // - enabled extension names
    // - enabled feature structures
    // - enabled property structures
    std::size_t extension_count = enabled_extension_names_set.size();
    std::size_t feature_size    = 0;
    std::size_t property_size   = 0;
    for (const auto& [stype, size] : feature_map) {
        feature_size += size;
    }
    for (const auto& [stype, size] : property_map) {
        property_size += size;
    }

    // Assume alignment of 8 for all structures
    std::size_t total_size = extension_count * sizeof(const char*) + feature_size + property_size;
    if (total_size == 0) {
        return result;
    }

    std::unique_ptr<std::uint64_t[]> buffer(new (std::nothrow) std::uint64_t[total_size / sizeof(std::uint64_t) + 1]);
    if (!buffer) {
        out_res = make_result<Func(), "Not enough memory for device init buffer">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return result;
    }

    // Zero initialize the buffer
    std::memset(buffer.get(), 0, total_size);
    auto* ptr = reinterpret_cast<std::uint8_t*>(buffer.get());

    // Fill extension names
    wis::span<const char*> extension_names_span(reinterpret_cast<const char**>(ptr), extension_count);
    ptr += extension_count * sizeof(const char*);
    std::size_t index = 0;
    for (const auto& name : enabled_extension_names_set) {
        extension_names_span[index++] = name;
    }

    // Fill feature structures
    VkBaseOutStructure* feature_struct_head = nullptr;
    for (auto& [stype, size] : feature_map) {
        VkBaseOutStructure* struct_ptr = reinterpret_cast<VkBaseOutStructure*>(ptr);
        struct_ptr->sType              = stype;
        struct_ptr->pNext              = feature_struct_head;
        feature_struct_head            = struct_ptr;

        // Rewrite size to be pointer to this structure
        ptr += size;
        size = uintptr_t(struct_ptr);
    }

    // Fill property structures
    VkBaseOutStructure* property_struct_head = nullptr;
    for (auto& [stype, size] : property_map) {
        VkBaseOutStructure* struct_ptr = reinterpret_cast<VkBaseOutStructure*>(ptr);
        struct_ptr->sType              = stype;
        struct_ptr->pNext              = property_struct_head;
        property_struct_head           = struct_ptr;

        // Rewrite size to be pointer to this structure
        ptr += size;
        size = uintptr_t(struct_ptr);
    }

    assert(ptr <= reinterpret_cast<std::uint8_t*>(buffer.get()) + total_size);

    result.buffer           = std::move(buffer);
    result.extension_names  = extension_names_span.data();
    result.extension_count  = extension_count;
    result.feature_structs  = feature_struct_head;
    result.property_structs = property_struct_head;
    return result;
}

std::pair<VkDescriptorSet, VkDescriptorPool>
detail::VKStaticSamplerPoolAllocator::AllocateSet(VkDevice device, VKMainDevice& table, VkDescriptorSetLayout dsl) noexcept
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    // Step 1: Try current pool
    if (current_pool_index < pool_count) {
        VkDescriptorSetAllocateInfo alloc_info{
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = pools[current_pool_index],
            .descriptorSetCount = 1,
            .pSetLayouts        = &dsl,
        };
        VkDescriptorSet set = VK_NULL_HANDLE;
        VkResult        res = table.vkAllocateDescriptorSets(device, &alloc_info, &set);
        if (res == VK_SUCCESS) {
            return { set, pools[current_pool_index] };
        }
    }
    // Step 2: Try other pools
    for (std::uint32_t i = 0; i < pool_count; ++i) {
        VkDescriptorSetAllocateInfo alloc_info{
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = pools[i],
            .descriptorSetCount = 1,
            .pSetLayouts        = &dsl,
        };
        VkDescriptorSet set = VK_NULL_HANDLE;
        VkResult        res = table.vkAllocateDescriptorSets(device, &alloc_info, &set);
        if (res == VK_SUCCESS) {
            current_pool_index = i;
            return { set, pools[current_pool_index] };
        }
    }
    // Step 3: Create new pool
    if (current_pool_index + 1 < static_sampler_max_pools) {
        VkDescriptorPoolSize pool_size{
            .type            = VK_DESCRIPTOR_TYPE_SAMPLER,
            .descriptorCount = static_sampler_chunk_size,
        };
        VkDescriptorPoolCreateInfo pool_info{
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext         = nullptr,
            .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets       = static_sampler_chunk_size,
            .poolSizeCount = 1,
            .pPoolSizes    = &pool_size,
        };
        VkDescriptorPool new_pool = VK_NULL_HANDLE;
        VkResult         res      = table.vkCreateDescriptorPool(device, &pool_info, nullptr, &new_pool);
        if (res == VK_SUCCESS) {
            pools[pool_count++] = new_pool;
            current_pool_index  = pool_count - 1;
            // Allocate from new pool
            VkDescriptorSetAllocateInfo alloc_info{
                .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext              = nullptr,
                .descriptorPool     = pools[current_pool_index],
                .descriptorSetCount = 1,
                .pSetLayouts        = &dsl,
            };
            VkDescriptorSet set = VK_NULL_HANDLE;
            res                 = table.vkAllocateDescriptorSets(device, &alloc_info, &set);
            if (res == VK_SUCCESS) {
                return { set, pools[current_pool_index] };
            }
        }
    }
    // Failed to allocate
    return { VK_NULL_HANDLE, VK_NULL_HANDLE };
}

#endif // WIS_VK_TYPES_CPP
#ifndef VK_CREATE_FACTORY_CPP
#define VK_CREATE_FACTORY_CPP
#include <wisdom/vulkan/vk_factory.h>
#include <unordered_map>
#include <array>

wis::ResultValue<wis::detail::fixed_allocation<VkExtensionProperties>>
        wis::VKFactory::EnumerateExtensions() noexcept
{
    auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;
    uint32_t count = 0;
    gt.vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    auto extensions = wis::detail::make_fixed_allocation<VkExtensionProperties>(count);
    if (!extensions)
        return wis::make_result<FUNC, "Not enough memory">(VK_ERROR_OUT_OF_HOST_MEMORY);

    auto vr = gt.vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.get());
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to enumerate extensions">(vr);
    return std::move(extensions);
}

wis::ResultValue<wis::detail::fixed_allocation<VkLayerProperties>>
        wis::VKFactory::EnumerateLayers() noexcept
{
    auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;
    uint32_t count = 0;
    gt.vkEnumerateInstanceLayerProperties(&count, nullptr);
    auto layers = wis::detail::make_fixed_allocation<VkLayerProperties>(count);
    if (!layers)
        return wis::make_result<FUNC, "Not enough memory">(VK_ERROR_OUT_OF_HOST_MEMORY);

    auto vr = gt.vkEnumerateInstanceLayerProperties(&count, layers.get());
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to enumerate layers">(vr);
    return std::move(layers);
}

wis::ResultValue<wis::detail::fixed_allocation<const char*>>
        wis::VKFactory::FoundExtensions(std::span<const char*> in_extensions) noexcept
{
    auto ext_string = [](const auto& ext) {
        std::string str = "All Extensions:\n";
        for (const auto& i : ext)
            wis::format_to(std::back_inserter(str), "{},\n", i.extensionName);
        return str;
    };

    auto [res, all_exts] = EnumerateExtensions();
    if (res.status != wis::Status::Ok)
        return res;

    if constexpr (wis::debug_mode)
        wis::lib_info(ext_string(std::span{ all_exts }));

    // allocate a bit more than needed
    auto found_extension = wis::detail::make_fixed_allocation<const char*>(in_extensions.size());
    if (!found_extension)
        return wis::make_result<FUNC, "Not enough memory for extensions">(VK_ERROR_OUT_OF_HOST_MEMORY);

    size_t index = 0;

    // low cost search (may throw)
    std::unordered_map<std::string_view, const VkExtensionProperties*> ext_map;
    ext_map.reserve(all_exts.size);
    for (const auto& i : all_exts)
        ext_map[i.extensionName] = &i;

    // O(n)
    for (const auto* extension : in_extensions) {
        if (ext_map.contains(extension))
            found_extension[index++] = extension;
        else
            wis::lib_warn(wis::format("Extension {} not found", extension));
    }

    found_extension.size = index;

    if constexpr (wis::debug_mode) {
        std::string debug_str{ "Used Extensions:\n" };
        for (const auto* i : found_extension)
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        wis::lib_info(std::move(debug_str));
    }

    return std::move(found_extension);
}

wis::ResultValue<wis::detail::fixed_allocation<const char*>>
        wis::VKFactory::FoundLayers(std::span<const char*> in_layers) noexcept
{
    auto ext_string = [](const auto& ext) {
        std::string str = "All Layers:\n";
        for (const auto& i : ext)
            wis::format_to(std::back_inserter(str), "{},\n", i.layerName);
        return str;
    };

    auto [res, all_layers] = EnumerateLayers();
    if (res.status != wis::Status::Ok)
        return res;

    if constexpr (wis::debug_mode)
        wis::lib_info(ext_string(std::span{ all_layers }));

    // allocate a bit more than needed
    auto found_layers = wis::detail::make_fixed_allocation<const char*>(in_layers.size());
    if (!found_layers)
        return wis::make_result<FUNC, "Not enough memory for layers">(VK_ERROR_OUT_OF_HOST_MEMORY);

    size_t index = 0;

    // low cost search (may throw)
    std::unordered_map<std::string_view, const VkLayerProperties*> ext_map;
    ext_map.reserve(all_layers.size);
    for (const auto& i : all_layers)
        ext_map[i.layerName] = &i;

    // O(n)
    for (const auto* layer : in_layers) {
        if (ext_map.contains(layer))
            found_layers[index++] = layer;
        else
            wis::lib_warn(wis::format("Layer {} not found", layer));
    }

    found_layers.size = index;

    if constexpr (wis::debug_mode) {
        std::string debug_str{ "Used Layers:\n" };
        for (const auto* i : found_layers)
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        wis::lib_info(std::move(debug_str));
    }

    return std::move(found_layers);
}

//--------------------------------------------------------------------------------------------------

wis::ResultValue<wis::VKFactory>
wis::VKCreateFactory(bool debug_layer) noexcept
{
    detail::VKFactoryGlobals::Instance().InitializeGlobalTable();
    auto& gt = detail::VKFactoryGlobals::Instance().global_table;
    VkResult vr{};
    uint32_t version = 0;
    if (gt.vkEnumerateInstanceVersion) {
        vr = gt.vkEnumerateInstanceVersion(&version);
        if (!wis::succeeded(vr))
            return wis::make_result<FUNC, "Failed to enumerate instance version">(vr);
    } else {
        version = VK_API_VERSION_1_0;
    }

    wis::lib_info(wis::format("Vulkan version: {}.{}.{}", VK_API_VERSION_MAJOR(version),
                              VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

    VkApplicationInfo info{
        VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "", VK_MAKE_API_VERSION(0, 1, 0, 0), "",
        VK_MAKE_API_VERSION(0, 1, 0, 0), version
    };

    return detail::VKCreateFactoryWithExtensions(debug_layer, (const char**)wis::detail::instance_extensions.data(), wis::detail::instance_extensions.size(), (const char**)wis::detail::instance_layers.data(), wis::detail::instance_layers.size());
}

wis::ResultValue<wis::VKFactory>
wis::VKCreateFactoryWithExtensions(bool debug_layer, VKFactoryExtension** extensions, size_t extension_count) noexcept
{
    size_t ext_alloc_size = detail::instance_extensions.size();
    size_t layer_alloc_size = detail::instance_layers.size();
    for (size_t i = 0; i < extension_count; i++) {
        ext_alloc_size += extensions[i]->RequiredExtensionsSize();
        layer_alloc_size += extensions[i]->RequiredLayersSize();
    }

    const char** ext_alloc_raw = nullptr;
    std::unique_ptr<const char*[]> ext_alloc;

    if (ext_alloc_size > detail::instance_extensions.size()) {
        ext_alloc = wis::detail::make_unique_for_overwrite<const char*[]>(ext_alloc_size);
        ext_alloc_raw = ext_alloc.get();
        std::copy(detail::instance_extensions.begin(), detail::instance_extensions.end(), ext_alloc_raw);
    } else {
        ext_alloc_raw = const_cast<const char**>(detail::instance_extensions.data());
    }

    const char** layer_alloc_raw = nullptr;
    std::unique_ptr<const char*[]> layer_alloc;

    if (layer_alloc_size > detail::instance_layers.size()) {
        layer_alloc = wis::detail::make_unique_for_overwrite<const char*[]>(layer_alloc_size);
        layer_alloc_raw = ext_alloc.get();
        std::copy(detail::instance_layers.begin(), detail::instance_layers.end(), layer_alloc_raw);
    } else {
        layer_alloc_raw = const_cast<const char**>(detail::instance_layers.data());
    }

    size_t index_ext = detail::instance_extensions.size();
    size_t index_layer = detail::instance_layers.size();

    for (size_t i = 0; i < extension_count; i++) {
        auto ext = extensions[i]->GetRequiredExtensions();
        auto layer = extensions[i]->GetRequiredLayers();

        if (ext.size() > 0)
            std::copy(ext.begin(), ext.end(), ext_alloc_raw + index_ext);
        if (layer.size() > 0)
            std::copy(layer.begin(), layer.end(), layer_alloc_raw + index_layer);

        index_ext += ext.size();
        index_layer += layer.size();
    }

    auto [resx, factory] = detail::VKCreateFactoryWithExtensions(debug_layer, ext_alloc_raw, ext_alloc_size, layer_alloc_raw, layer_alloc_size);
    if (resx.status != wis::Status::Ok)
        return resx;

    for (size_t i = 0; i < extension_count; i++) {
        auto res = extensions[i]->Init(factory);
        if (res.status != wis::Status::Ok)
            return res;
    }

    return std::move(factory);
}

wis::ResultValue<wis::VKFactory>
wis::detail::VKCreateFactoryWithExtensions(bool debug_layer, const char** exts, size_t extension_count, const char** layers, size_t layer_count) noexcept
{
    detail::VKFactoryGlobals::Instance().InitializeGlobalTable();
    auto& gt = detail::VKFactoryGlobals::Instance().global_table;

    VkResult vr{};
    uint32_t version = 0;
    if (gt.vkEnumerateInstanceVersion) {
        vr = gt.vkEnumerateInstanceVersion(&version);
        if (!wis::succeeded(vr))
            return wis::make_result<FUNC, "Failed to enumerate instance version">(vr);
    } else {
        version = VK_API_VERSION_1_0;
    }

    wis::lib_info(wis::format("Vulkan version: {}.{}.{}", VK_API_VERSION_MAJOR(version),
                              VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

    VkApplicationInfo info{
        VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "", VK_MAKE_API_VERSION(0, 1, 0, 0), "",
        VK_MAKE_API_VERSION(0, 1, 0, 0), version
    };

    auto [res1, found_extensions] = VKFactory::FoundExtensions({ exts, exts + extension_count });
    if (res1.status != wis::Status::Ok)
        return res1;

    auto [res2, found_layers] = VKFactory::FoundLayers({ layers, layers + layer_count });
    if (res2.status != wis::Status::Ok)
        return res2;

    VkInstanceCreateInfo create_info{ .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                      .pApplicationInfo = &info,
                                      .enabledLayerCount = uint32_t(found_layers.size),
                                      .ppEnabledLayerNames = found_layers.get_data(),
                                      .enabledExtensionCount = uint32_t(found_extensions.size),
                                      .ppEnabledExtensionNames = found_extensions.get_data() };

    VkInstance unsafe_instance;
    vr = gt.vkCreateInstance(&create_info, nullptr, &unsafe_instance);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to create instance">(vr);

    return VKCreateFactoryEx(unsafe_instance, version, debug_layer);
}

wis::ResultValue<wis::VKFactory>
wis::detail::VKCreateFactoryEx(VkInstance instance, uint32_t version, bool debug_layer) noexcept
{
    wis::detail::VKFactoryGlobals::Instance().InitializeGlobalTable();
    auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;

    auto destroy_instance = (PFN_vkDestroyInstance)gt.vkGetInstanceProcAddr(instance, "vkDestroyInstance");
    wis::managed_handle<VkInstance> safe_instance{ instance, destroy_instance };
    auto table = wis::detail::make_unique<wis::VKMainInstance>();
    if (!table)
        return wis::make_result<FUNC, "Failed to create instance table">(VK_ERROR_OUT_OF_HOST_MEMORY);

    if (!table->Init(safe_instance.get(), gt.vkGetInstanceProcAddr))
        return wis::make_result<FUNC, "Failed to initialize instance table">(VK_ERROR_UNKNOWN);

    auto factory = wis::VKFactory{ wis::SharedInstance{ safe_instance.release(), destroy_instance, std::move(table) }, version, debug_layer };
    auto result = factory.VKEnumeratePhysicalDevices();
    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Failed to enumerate physical devices">(result);

    return std::move(factory);
}
#endif // !VK_CREATE_FACTORY_CPP

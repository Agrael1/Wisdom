#ifndef WIS_VK_CREATE_FACTORY_CPP
#define WIS_VK_CREATE_FACTORY_CPP
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/global/constants.h>
#include <unordered_map>
#include <array>
#include <ranges>
#endif

wis::detail::fixed_allocation<const char*>
wis::ImplVKFactory::FoundExtensions(wis::Result& result, std::span<const char*> in_extensions) noexcept
{
    auto& exts = detail::VKFactoryGlobals::Instance().instance_extensions;
    auto ext_string = [](const auto& ext) {
        std::string str = "All Extensions:\n";
        for (const auto& i : ext) {
            wis::format_to(std::back_inserter(str), "{},\n", i);
        }
        return str;
    };

    if constexpr (wis::debug_mode) {
        wis::lib_info(ext_string(exts));
    }

    // Unique set of extensions
    std::unordered_set<std::string_view, wis::string_hash, std::equal_to<>> exts_set;
    exts_set.reserve(in_extensions.size());
    for (const auto& i : in_extensions) {
        exts_set.insert(i);
    }

    // allocate a bit more than needed
    wis::detail::fixed_allocation<const char*> found_extension = wis::detail::make_fixed_allocation<const char*>(exts_set.size());
    if (!found_extension) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Not enough memory for extensions">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return found_extension;
    }

    size_t index = 0;

    // O(n)
    for (auto extension : exts_set) {
        if (exts.find(extension) != exts.end()) {
            found_extension[index++] = extension.data();
        } else {
            wis::lib_warn(wis::format("Extension {} not found", extension));
        }
    }

    found_extension.size = index;

    if constexpr (wis::debug_mode) {
        std::string debug_str{ "Used Extensions:\n" };
        for (const auto* i : found_extension) {
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        }
        wis::lib_info(std::move(debug_str));
    }

    return found_extension;
}

wis::detail::fixed_allocation<const char*>
wis::ImplVKFactory::FoundLayers(wis::Result& result, std::span<const char*> in_layers) noexcept
{
    auto& exts = detail::VKFactoryGlobals::Instance().instance_layers;

    auto ext_string = [](const auto& ext) {
        std::string str = "All Layers:\n";
        for (const auto& i : ext) {
            wis::format_to(std::back_inserter(str), "{},\n", i);
        }
        return str;
    };

    if constexpr (wis::debug_mode) {
        wis::lib_info(ext_string(exts));
    }

    std::unordered_set<std::string_view, wis::string_hash, std::equal_to<>> layer_set;
    layer_set.reserve(in_layers.size());
    for (const auto& i : in_layers) {
        layer_set.insert(i);
    }

    // allocate a bit more than needed
    auto found_layers = wis::detail::make_fixed_allocation<const char*>(layer_set.size());
    if (!found_layers) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Not enough memory for layers">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return found_layers;
    }
    size_t index = 0;

    // O(n)
    for (const auto layer : layer_set) {
        if (exts.contains(layer)) {
            found_layers[index++] = layer.data();
        } else {
            wis::lib_warn(wis::format("Layer {} not found", layer));
        }
    }

    found_layers.size = index;

    if constexpr (wis::debug_mode) {
        std::string debug_str{ "Used Layers:\n" };
        for (const auto* i : found_layers) {
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        }
        wis::lib_info(std::move(debug_str));
    }

    return std::move(found_layers);
}

//--------------------------------------------------------------------------------------------------
namespace wis::detail {
inline wis::VKFactory
VKCreateFactoryWithExtensions(wis::Result& result, bool debug_layer, const char** exts, size_t extension_count, const char** layers, size_t layer_count) noexcept
{
    VKFactory out_factory;
    auto& internal = out_factory.GetMutableInternal();

    auto xr = detail::VKFactoryGlobals::Instance().InitializeFactoryGlobals();
    if (xr.status != wis::Status::Ok) {
        result = xr;
        return out_factory;
    }
    auto& gt = detail::VKFactoryGlobals::GetGlobalTable();

    VkResult vr{};
    uint32_t version = 0;
    if (gt.vkEnumerateInstanceVersion) {
        vr = gt.vkEnumerateInstanceVersion(&version);
        if (!wis::succeeded(vr)) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to enumerate instance version">(vr);
            return out_factory;
        }
    } else {
        version = VK_API_VERSION_1_0;
    }

    wis::lib_info(wis::format("Vulkan version: {}.{}.{}", VK_API_VERSION_MAJOR(version),
                              VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

    VkApplicationInfo info{
        VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "", VK_MAKE_API_VERSION(0, 1, 0, 0), "",
        VK_MAKE_API_VERSION(0, 1, 0, 0), version
    };

    auto found_extensions = VKFactory::FoundExtensions(result, { exts, exts + extension_count });
    if (result.status != wis::Status::Ok) {
        return out_factory;
    }

    auto found_layers = VKFactory::FoundLayers(result, { layers, layers + layer_count });
    if (result.status != wis::Status::Ok) {
        return out_factory;
    }
    VkInstanceCreateInfo create_info{ .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                      .pApplicationInfo = &info,
                                      .enabledLayerCount = uint32_t(found_layers.size),
                                      .ppEnabledLayerNames = found_layers.get_data(),
                                      .enabledExtensionCount = uint32_t(found_extensions.size),
                                      .ppEnabledExtensionNames = found_extensions.get_data() };

    VkInstance unsafe_instance;
    vr = gt.vkCreateInstance(&create_info, nullptr, &unsafe_instance);
    if (!wis::succeeded(vr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to create instance">(vr);
        return out_factory;
    }
    auto destroy_instance = (PFN_vkDestroyInstance)gt.vkGetInstanceProcAddr(unsafe_instance, "vkDestroyInstance");
    wis::managed_handle<VkInstance> safe_instance{ unsafe_instance, destroy_instance };

    auto table = wis::detail::make_unique<wis::VKMainInstance>();
    if (!table) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to create instance table">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_factory;
    }
    if (!table->Init(safe_instance.get(), gt.vkGetInstanceProcAddr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to initialize instance table">(VK_ERROR_UNKNOWN);
        return out_factory;
    }

    internal.api_version = version;
    internal.factory = wis::SharedInstance{ safe_instance.release(), destroy_instance, std::move(table), &gt };
    internal.debug_layer = debug_layer;

    vr = out_factory.VKEnumeratePhysicalDevices();
    if (!wis::succeeded(vr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to enumerate physical devices">(vr);
    }
    return out_factory;
}
} // namespace wis::detail

//--------------------------------------------------------------------------------------------------
wis::VKFactory
wis::ImplVKCreateFactory(wis::Result& result, bool debug_layer, VKFactoryExtension** extensions, size_t extension_count) noexcept
{
    size_t ext_alloc_size = std::size(detail::instance_extensions);
    size_t layer_alloc_size = std::size(detail::instance_layers);
    for (size_t i = 0; i < extension_count; i++) {
        ext_alloc_size += extensions[i]->RequiredExtensionsSize();
        layer_alloc_size += extensions[i]->RequiredLayersSize();
    }

    const char** ext_alloc_raw = nullptr;
    std::unique_ptr<const char*[]> ext_alloc;

    if (ext_alloc_size > std::size(detail::instance_extensions)) {
        ext_alloc = wis::detail::make_unique_for_overwrite<const char*[]>(ext_alloc_size);
        ext_alloc_raw = ext_alloc.get();
        std::copy(std::begin(detail::instance_extensions), std::end(detail::instance_extensions), ext_alloc_raw);
    } else {
        ext_alloc_raw = const_cast<const char**>(detail::instance_extensions.data());
    }

    const char** layer_alloc_raw = nullptr;
    std::unique_ptr<const char*[]> layer_alloc;

    if (layer_alloc_size > std::size(detail::instance_layers)) {
        layer_alloc = wis::detail::make_unique_for_overwrite<const char*[]>(layer_alloc_size);
        layer_alloc_raw = layer_alloc.get();
        std::copy(std::begin(detail::instance_layers), std::end(detail::instance_layers), layer_alloc_raw);
    } else {
        layer_alloc_raw = const_cast<const char**>(detail::instance_layers.data());
    }

    size_t index_ext = std::size(detail::instance_extensions);
    size_t index_layer = std::size(detail::instance_layers);

    for (size_t i = 0; i < extension_count; i++) {
        auto ext = extensions[i]->GetRequiredExtensions();
        auto layer = extensions[i]->GetRequiredLayers();

        if (ext.size() > 0) {
            std::copy(ext.begin(), ext.end(), ext_alloc_raw + index_ext);
        }
        if (layer.size() > 0) {
            std::copy(layer.begin(), layer.end(), layer_alloc_raw + index_layer);
        }

        index_ext += ext.size();
        index_layer += layer.size();
    }

    auto factory = detail::VKCreateFactoryWithExtensions(result, debug_layer, ext_alloc_raw, ext_alloc_size, layer_alloc_raw, layer_alloc_size);
    if (result.status != wis::Status::Ok) {
        return factory;
    }

    for (size_t i = 0; i < extension_count; i++) {
        auto res = extensions[i]->Init(factory);
        if (res.status != wis::Status::Ok) {
            result = res;
            return factory;
        }
    }

    return factory;
}

#endif // !VK_CREATE_FACTORY_CPP

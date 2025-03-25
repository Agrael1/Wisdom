#include <catch2/catch_test_macros.hpp>
#include <wisdom/wisdom_extended_allocation.hpp>
#include <wisdom/wisdom_platform.hpp>
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.hpp>
#include <iostream>

struct LogProvider2 : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override {
        // std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

static void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    std::cout << message << std::endl;
    if (severity >= wis::Severity::Error) {
        auto& error = *static_cast<bool*>(user_data);
        error = true;
    }
}

struct A : public wis::VKDeviceExtension {
    virtual bool GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                                  std::unordered_set<std::string_view>& ext_name_set,
                                  std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                  std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        if (available_extensions.find(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME) == available_extensions.end()) {
            return false;
        }
        ext_name_set.insert(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME);
        structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT] = sizeof(VkPhysicalDeviceCustomBorderColorFeaturesEXT);
        property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceCustomBorderColorPropertiesEXT);
        return true;
    }
    virtual bool Supported() const noexcept
    {
        return true;
    }
    virtual wis::Result Init(const wis::VKDevice& instance,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        VkPhysicalDeviceCustomBorderColorFeaturesEXT& features = *reinterpret_cast<VkPhysicalDeviceCustomBorderColorFeaturesEXT*>(structure_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT));
        VkPhysicalDeviceCustomBorderColorPropertiesEXT& properties = *reinterpret_cast<VkPhysicalDeviceCustomBorderColorPropertiesEXT*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT));

        if (!features.customBorderColors) {
            return wis::make_result<wis::Func<wis::FuncD()>(), "Custom border colors not supported">(VkResult::VK_ERROR_UNKNOWN);
        }

        // enabled -> features.customBorderColors = true;
        REQUIRE(features.customBorderColors);

        return {};
    }
};

TEST_CASE("device_with_extensions")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider2>());

    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    auto [res, factory] = wis::CreateFactory(true, exts.data(), 1);
    auto [res1, info] = ext.CreateDebugMessenger(&DebugCallback, &error);

    wis::Device device;
    A a;

    wis::VKDeviceExtension* exts2[] = { &a };

    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            auto [res, hdevice] = wis::VKCreateDevice(std::move(adapter), exts2, 1);
            if (res.status == wis::Status::Ok) {
                device = std::move(hdevice);
                break;
            };

        } else {
            break;
        }
    }
}

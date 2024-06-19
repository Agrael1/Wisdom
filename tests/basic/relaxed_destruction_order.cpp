#include <catch2/catch_test_macros.hpp>
#include <wisdom/wisdom_platform.h>
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.h>
#include <iostream>

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override{
        // std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    std::cout << message << std::endl;
    if (severity >= wis::Severity::Error) {
        auto& error = *static_cast<bool*>(user_data);
        error = true;
    }
}

TEST_CASE("basic_factory")
{
    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    auto [res, factory] = wis::CreateFactoryWithExtensions(true, exts.data(), 1);
    auto [res1, info] = ext.CreateDebugMessenger(&DebugCallback, &error);

    factory = {};
    REQUIRE_FALSE(factory);
    REQUIRE_FALSE(error);
}

TEST_CASE("basic_device")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    auto [res, factory] = wis::CreateFactoryWithExtensions(true, exts.data(), 1);
    auto [res1, info] = ext.CreateDebugMessenger(&DebugCallback, &error);

    wis::Device device;

    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            auto [res, hdevice] = wis::CreateDevice(std::move(adapter));
            if (res.status == wis::Status::Ok) {
                device = std::move(hdevice);
                break;
            };

        } else {
            break;
        }
    }

    auto [res2, queue] = device.CreateCommandQueue(wis::QueueType::Graphics);
    auto [res3, fence] = device.CreateFence();
    auto [res4, cmd_list] = device.CreateCommandList(wis::QueueType::Graphics);
    auto [res5, allocator] = device.CreateAllocator();

    REQUIRE(device);
    REQUIRE(queue);
    REQUIRE(fence);
    REQUIRE(cmd_list);

    SECTION("fence destruction")
    {
        fence = {};
        REQUIRE_FALSE(fence);
        REQUIRE_FALSE(error);
    }

    SECTION("reverse order destruction")
    {
        auto [res5, fence2] = device.CreateFence();

        factory = {};
        REQUIRE_FALSE(factory);
        REQUIRE_FALSE(error);

        device = {};
        REQUIRE_FALSE(device);
        REQUIRE_FALSE(error);

        queue = {};
        REQUIRE_FALSE(queue);
        REQUIRE_FALSE(error);

        fence2 = {};
        REQUIRE_FALSE(fence2);
        REQUIRE_FALSE(error);

        allocator = {};
        REQUIRE_FALSE(allocator);
        REQUIRE_FALSE(error);
    }

    SECTION("command list replacement")
    {
        wis::CommandList a;
        wis::CommandList cmd_list2 = std::move(a);
        REQUIRE_FALSE(cmd_list2);
        REQUIRE_FALSE(error);

        REQUIRE_FALSE(a);
        a = {};
        REQUIRE_FALSE(error);
        REQUIRE_FALSE(a);
    }
}

struct A : public wis::VKDeviceExtension {
    static constexpr std::array<std::pair<VkStructureType, uint32_t>, 1> required_extensions = {
        std::pair<VkStructureType, uint32_t>{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT, sizeof(VkPhysicalDeviceCustomBorderColorFeaturesEXT) }
    };

    virtual bool GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash>& available_extensions,
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
        return false;
    }
    virtual wis::Result Init(const wis::VKDevice& instance,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        VkPhysicalDeviceCustomBorderColorFeaturesEXT& features = *reinterpret_cast<VkPhysicalDeviceCustomBorderColorFeaturesEXT*>(structure_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT));
        VkPhysicalDeviceCustomBorderColorPropertiesEXT& properties = *reinterpret_cast<VkPhysicalDeviceCustomBorderColorPropertiesEXT*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT));

        if (!features.customBorderColors) {
            return wis::make_result<FUNC, "Custom border colors not supported">(VkResult::VK_ERROR_UNKNOWN);
        }

        return {};
    }
};

TEST_CASE("device_with_extensions")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    auto [res, factory] = wis::CreateFactoryWithExtensions(true, exts.data(), 1);
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

            auto [res, hdevice] = wis::VKCreateDeviceWithExtensions(std::move(adapter), exts2, 1);
            if (res.status == wis::Status::Ok) {
                device = std::move(hdevice);
                break;
            };

        } else {
            break;
        }
    }
}

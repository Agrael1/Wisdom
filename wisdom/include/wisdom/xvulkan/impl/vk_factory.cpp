#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_factory.h>
#endif // !WISDOM_HEADER_ONLY

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <wisdom/global/definitions.h>
#include <wisdom/util/misc.h>

#define VKGT wis::Internal<wis::VKFactory>::global_table

namespace {
struct FactoryData {
public:
  static const FactoryData& instance() noexcept {
    static FactoryData d;
    return d;
  }
  static auto& GetExtensions() noexcept { return instance().extensions; }
  static auto& GetLayers() noexcept { return instance().layers; }
  [[nodiscard]] static std::string ExtensionsString() noexcept {
    std::string debug_str1{"Available Extensions:\n"};
    for (const auto& i : instance().extensions) {
      wis::format_to(std::back_inserter(debug_str1), "{}\n", i.first);
    }
    return debug_str1;
  }
  [[nodiscard]] static std::string LayersString() noexcept {
    std::string debug_str1{"Available Layers:\n"};
    for (const auto& i : instance().layers) {
      wis::format_to(std::back_inserter(debug_str1), "\t{}\n", i.first);
    }
    return debug_str1;
  }

private:
  FactoryData(bool blayers = true, bool bextensions = true) {
    if (bextensions)
      LoadExtensions();
    if (blayers)
      LoadLayers();
  }

  void LoadExtensions() noexcept {
    std::vector<VkExtensionProperties> vextensions;
    uint32_t count = 0;
    auto vr = VKGT.vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    do
      vextensions.resize(count);
    while ((vr = VKGT.vkEnumerateInstanceExtensionProperties(nullptr, &count,
                                                             vextensions.data())) == VK_INCOMPLETE);

    if (!wis::succeeded(vr))
      return;

    for (auto& i : vextensions)
      extensions.emplace(i.extensionName, i);
  }
  void LoadLayers() noexcept {
    std::vector<VkLayerProperties> vlayers;
    uint32_t count = 0;
    auto vr = VKGT.vkEnumerateInstanceLayerProperties(&count, nullptr);
    do
      vlayers.resize(count);
    while ((vr = VKGT.vkEnumerateInstanceLayerProperties(&count, vlayers.data())) == VK_INCOMPLETE);

    if (!wis::succeeded(vr))
      return;

    for (auto& i : vlayers)
      layers.emplace(i.layerName, i);
  }

  std::unordered_map<std::string, VkExtensionProperties, wis::string_hash> extensions;
  std::unordered_map<std::string, VkLayerProperties, wis::string_hash> layers;
};
inline constexpr uint32_t order_performance(VkPhysicalDeviceType t) {
  switch (t) {
  default:
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    return 3;
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    return 4;
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
    return 2;
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
    return 1;
  }
}
inline constexpr uint32_t order_power(VkPhysicalDeviceType t) {
  switch (t) {
  default:
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    return 4;
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    return 3;
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
    return 2;
  case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
    return 1;
  }
}
} // namespace

constexpr inline std::array req_extensions {
  VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
      VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_METAL_EXT)
      VK_EXT_METAL_SURFACE_EXTENSION_NAME, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
      VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
      VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#endif
#if DEBUG_MODE
      VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

std::vector<const char*> wis::VKFactory::FoundExtensions() noexcept {
  const auto& extensions = FactoryData::GetExtensions();

  if constexpr (wis::debug_mode)
    wis::lib_info(FactoryData::ExtensionsString());

  std::vector<const char*> found_extension;
  for (const auto* extension : req_extensions) {
    if (extensions.contains(extension))
      found_extension.push_back(extension);
  }

  if constexpr (wis::debug_mode) {
    std::string debug_str{"Used Extensions:\n"};
    for (const auto* i : found_extension)
      wis::format_to(std::back_inserter(debug_str), "{},\n", i);
    wis::lib_info(std::move(debug_str));
  }

  return found_extension;
}
std::vector<const char*> wis::VKFactory::FoundLayers() noexcept {
  std::vector<const char*> out;
  if constexpr (wis::debug_mode) {
    const auto& layers = FactoryData::GetLayers();
    if constexpr (wis::debug_mode)
      wis::lib_info(FactoryData::LayersString());

    if (layers.contains("VK_LAYER_KHRONOS_validation"))
      out.push_back("VK_LAYER_KHRONOS_validation");

    std::string debug_str{"Used Layers:\n"};
    for (const auto* i : out)
      wis::format_to(std::back_inserter(debug_str), "{},\n", i);
    wis::lib_info(std::move(debug_str));
  }

  return out;
}

[[nodiscard]] std::pair<wis::Result, wis::VKFactory>
wis::VKCreateFactory(bool debug_layer, wis::DebugCallback callback, void* user_data) noexcept {
  VKFactory::InitializeGlobalTable();
  auto& gt = VKFactory::global_table;
  VkResult vr{};
  uint32_t version = 0;
  if (gt.vkEnumerateInstanceVersion) {
    vr = gt.vkEnumerateInstanceVersion(&version);
    if (!wis::succeeded(vr))
      return std::pair{wis::make_result<FUNC, "Failed to enumerate instance version">(vr),
                       wis::VKFactory{}};
  } else {
    version = VK_API_VERSION_1_0;
  }

  wis::lib_info(wis::format("Vulkan version: {}.{}.{}", VK_API_VERSION_MAJOR(version),
                            VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

  VkApplicationInfo info{
      VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "", VK_MAKE_API_VERSION(0, 1, 0, 0), "",
      VK_MAKE_API_VERSION(0, 1, 0, 0),    version};

  auto found_extension = VKFactory::FoundExtensions();
  auto found_layers = VKFactory::FoundLayers();

  VkInstanceCreateInfo create_info{.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                   .pApplicationInfo = &info,
                                   .enabledLayerCount = static_cast<uint32_t>(found_layers.size()),
                                   .ppEnabledLayerNames = found_layers.data(),
                                   .enabledExtensionCount =
                                       static_cast<uint32_t>(found_extension.size()),
                                   .ppEnabledExtensionNames = found_extension.data()};

  std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback =
      std::make_unique<std::pair<wis::DebugCallback, void*>>(callback, user_data);

  VkDebugUtilsMessengerCreateInfoEXT create_instance_debug{
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      .pfnUserCallback = VKFactory::DebugCallbackThunk,
      .pUserData = debug_callback.get()};

  if (debug_layer) {
    create_info.pNext = &create_instance_debug;
  }

  wis::shared_handle<VkInstance> instance;
  if (!wis::succeeded(vr = gt.vkCreateInstance(&create_info, nullptr,
                                               instance.put_unsafe(gt.vkDestroyInstance))))
    return std::pair{wis::make_result<FUNC, "Failed to create instance">(vr), wis::VKFactory{}};

  auto factory =
      wis::VKFactory{std::move(instance), version, debug_layer, std::move(debug_callback)};
  return !wis::succeeded(vr = factory.EnumeratePhysicalDevices())
             ? std::pair{wis::make_result<FUNC, "Failed to enumerate physical devices">(vr),
                         wis::VKFactory{}}
             : std::pair{wis::success, std::move(factory)};
}

VKAPI_ATTR VkBool32 VKAPI_CALL wis::VKFactory::DebugCallbackThunk(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) noexcept {
  auto& [callback, user_data] = *reinterpret_cast<std::pair<wis::DebugCallback, void*>*>(pUserData);
  callback(convert_vk(messageSeverity),
           wis::format("\n[Validation layer]: {}\n [Message]:{}",
                       pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "",
                       pCallbackData->pMessage)
               .c_str(),
           user_data);
  return false;
}

wis::VKFactory::VKFactory(
    wis::shared_handle<VkInstance> instance, uint32_t api_ver, bool debug_layer,
    std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback) noexcept
    : QueryInternal(std::move(instance)) {
  instance_table = std::make_unique<wis::VkInstanceTable>();
  api_version = api_ver;
  instance_table->Init(factory.get(), global_table);
  if constexpr (wis::debug_layer) {
    if (debug_layer && debug_callback) {
      EnableDebugLayer(std::move(debug_callback));
    }
  }
}

wis::VKFactory::~VKFactory() noexcept {
  if (debug_callback) {
    instance_table->vkDestroyDebugUtilsMessengerEXT(factory.get(), messenger, nullptr);
    factory.reset();
  }
}

[[nodiscard]] std::pair<wis::Result, wis::VKAdapter>
wis::VKFactory::GetAdapter(uint32_t index, AdapterPreference preference) const noexcept {
  if (index >= adapters.size()) {
    return std::pair{wis::make_result<FUNC, "Index out of range">(VK_ERROR_UNKNOWN), VKAdapter{}};
  }
  auto& adapter = adapters[index];
  switch (preference) {
  default:
    return std::pair{wis::success, adapter.adapter};
  case AdapterPreference::MinConsumption:
    return std::pair{wis::success, adapters[adapter.index_consumption].adapter};
  case AdapterPreference::Performance:
    return std::pair{wis::success, adapters[adapter.index_performance].adapter};
  }
}

VkResult wis::VKFactory::EnumeratePhysicalDevices() noexcept {
  std::vector<VkPhysicalDevice> phys_adapters;
  uint32_t count = 0;
  auto vr = instance_table->vkEnumeratePhysicalDevices(factory.get(), &count, nullptr);
  do
    phys_adapters.resize(count);
  while ((vr = instance_table->vkEnumeratePhysicalDevices(factory.get(), &count,
                                                          phys_adapters.data())) == VK_INCOMPLETE);
  if (!wis::succeeded(vr))
    return vr;

  adapters.resize(count);

  if (phys_adapters.size() > 1) {
    auto indices = std::views::iota(0u, count);
    std::vector<uint32_t> indices_cons(indices.begin(), indices.end());
    std::vector<uint32_t> indices_perf(indices.begin(), indices.end());

    auto less_consumption = [this](VkPhysicalDevice a, VkPhysicalDevice b) {
      VkPhysicalDeviceProperties a_properties{};
      VkPhysicalDeviceProperties b_properties{};
      instance_table->vkGetPhysicalDeviceProperties(a, &a_properties);
      instance_table->vkGetPhysicalDeviceProperties(b, &b_properties);

      return order_power(a_properties.deviceType) > order_power(b_properties.deviceType)
                 ? true
                 : a_properties.limits.maxMemoryAllocationCount >
                       b_properties.limits.maxMemoryAllocationCount;
    };
    auto less_performance = [this](VkPhysicalDevice a, VkPhysicalDevice b) {
      VkPhysicalDeviceProperties a_properties{};
      VkPhysicalDeviceProperties b_properties{};
      instance_table->vkGetPhysicalDeviceProperties(a, &a_properties);
      instance_table->vkGetPhysicalDeviceProperties(b, &b_properties);

      return order_performance(a_properties.deviceType) > order_performance(b_properties.deviceType)
                 ? true
                 : a_properties.limits.maxMemoryAllocationCount >
                       b_properties.limits.maxMemoryAllocationCount;
    };

    std::ranges::sort(indices_cons,
                      [this, &phys_adapters, less_consumption](uint32_t a, uint32_t b) {
                        return less_consumption(phys_adapters[a], phys_adapters[b]);
                      });
    std::ranges::sort(indices_perf,
                      [this, &phys_adapters, less_consumption](uint32_t a, uint32_t b) {
                        return less_consumption(phys_adapters[a], phys_adapters[b]);
                      });

    for (size_t i = 0; i < count; i++) {
      auto& adapter = adapters[i];
      adapter.adapter = VKAdapter{phys_adapters[i], instance_table.get()};
      adapter.index_performance = indices_perf[i];
      adapter.index_consumption = indices_cons[i];
    }
  } else {
    for (size_t i = 0; i < count; i++) {
      auto& adapter = adapters[i];
      adapter.adapter = VKAdapter{phys_adapters[i], instance_table.get()};
      adapter.index_performance = i;
      adapter.index_consumption = i;
    }
  }
  return VK_SUCCESS;
}

void wis::VKFactory::EnableDebugLayer(
    std::unique_ptr<std::pair<wis::DebugCallback, void*>> xdebug_callback) noexcept {
  debug_callback = std::move(xdebug_callback);
  VkDebugUtilsMessengerCreateInfoEXT create_info{
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      .pfnUserCallback = VKFactory::DebugCallbackThunk,
      .pUserData = debug_callback.get()};

  auto vr = instance_table->vkCreateDebugUtilsMessengerEXT(factory.get(), &create_info, nullptr,
                                                           &messenger);
  if (!wis::succeeded(vr)) {
    wis::lib_error("Failed to create debug messenger");
  }
}

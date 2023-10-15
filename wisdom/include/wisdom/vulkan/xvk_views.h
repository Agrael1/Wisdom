#pragma once
#include <wisdom/vulkan/xvk_handles.h>
#include <wisvk/vk_loader.hpp>
#include <tuple>

namespace wis {
    using VKAdapterHandle = std::tuple<VkPhysicalDevice, VkInstanceTable*>;
    using VKFactoryHandle = std::tuple<wis::shared_handle<VkInstance>, VkInstanceTable*>;
    using VKDeviceHandle = std::tuple<wis::SharedDevice>;

    using VKFenceView = std::tuple<VkSemaphore>;
}
#pragma once
#include <wisvk/vk_managed_handles.hpp>
#include <wisvk/vk_loader.hpp>
#include <tuple>

namespace wis {
    using VKAdapterHandle = std::tuple<VkPhysicalDevice, VkInstanceTable*>;
    using VKFactoryHandle = std::tuple<wis::shared_handle<VkInstance>, VkInstanceTable*>;
}
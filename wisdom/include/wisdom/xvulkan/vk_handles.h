#pragma once
#include <memory>
#include <span>
#include <vector>
#include <vk_mem_alloc.h>
#include <wisvk/vk_loader.hpp>
#include <wisvk/vk_movable.hpp>
#include <wisvk/vk_managed_handles.hpp>

namespace wis {
struct SharedDeviceHeader {
    deleter_of_t<VkDevice> deleter;
    std::unique_ptr<VkDeviceTable> device_table;
};
struct SharedInstanceHeader {
    deleter_of_t<VkInstance> deleter;
    std::unique_ptr<VkInstanceTable> instance_table;
};

class SharedDevice : public shared_handle_base<VkDevice, SharedDeviceHeader, SharedDevice>
{
public:
    SharedDevice() noexcept = default;
    explicit SharedDevice(VkDevice device, std::unique_ptr<VkDeviceTable> device_table) noexcept
        : shared_handle_base(device, nullptr, std::move(device_table)), m_device_table(m_control->m_header.device_table.get())
    {
        m_control->m_header.deleter.m_pfn = m_control->m_header.device_table->vkDestroyDevice;
    }

public:
    auto& table() const noexcept { return *m_device_table; }

protected:
    VkDeviceTable* m_device_table = nullptr;
};
class SharedInstance : public shared_handle_base<VkInstance, SharedInstanceHeader, SharedInstance>
{
public:
    SharedInstance() noexcept = default;
    explicit SharedInstance(VkInstance device, PFN_vkDestroyInstance deleter, std::unique_ptr<VkInstanceTable> instance_table) noexcept
        : shared_handle_base(device, deleter, std::move(instance_table)), m_device_table(m_control->m_header.instance_table.get())
    {
    }

public:
    auto& table() const noexcept { return *m_device_table; }

protected:
    VkInstanceTable* m_device_table = nullptr;
};

template<typename HandleType>
struct managed_header_ex : public managed_header<HandleType> {
};

template<typename HandleType>
    requires std::same_as<parent_of_t<HandleType>, VkDevice>
struct managed_header_ex<HandleType> {
    SharedDevice parent;
    deleter_of_t<HandleType> deleter;
};
template<typename HandleType>
    requires std::same_as<parent_of_t<HandleType>, VkInstance>
struct managed_header_ex<HandleType> {
    SharedInstance parent;
    deleter_of_t<HandleType> deleter;
};

template<typename HandleType>
class managed_handle_ex : public managed_handle_base<HandleType, managed_header_ex<HandleType>,
                                                     managed_handle_ex<HandleType>>
{
public:
    using managed_handle_base<HandleType, managed_header_ex<HandleType>,
                              managed_handle_ex<HandleType>>::managed_handle_base;
};

/// Specializations

template<>
class shared_handle<VmaAllocator>
    : public wis::shared_handle_base<VmaAllocator, wis::SharedDevice, shared_handle<VmaAllocator>>
{
    using base =
            wis::shared_handle_base<VmaAllocator, wis::SharedDevice, shared_handle<VmaAllocator>>;
    friend base;

public:
    shared_handle() = default;
    explicit shared_handle(wis::SharedDevice device, VmaAllocator handle) noexcept
        : wis::shared_handle_base<VmaAllocator, wis::SharedDevice, shared_handle<VmaAllocator>>(
                  handle, std::move(device)) { }

    [[nodiscard]] const auto& parent() const noexcept { return header(); }

protected:
    void internal_destroy() noexcept { vmaDestroyAllocator(get()); }
};


class SharedSurface : public wis::shared_handle_base<VkSurfaceKHR, wis::managed_header_ex<VkSurfaceKHR>, SharedSurface>
{
    using wis::shared_handle_base<VkSurfaceKHR, wis::managed_header_ex<VkSurfaceKHR>,  SharedSurface>::shared_handle_base;
};
} // namespace wis
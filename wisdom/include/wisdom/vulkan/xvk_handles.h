#pragma once
#include <wisvk/vk_loader.hpp>
#include <wisvk/vk_managed_handles.hpp>
#include <memory>

namespace wis {
struct SharedDeviceHeader : public managed_header<VkDevice> {
    std::unique_ptr<VkDeviceTable> device_table;
};

class SharedDevice : public shared_handle_base<VkDevice, SharedDeviceHeader, SharedDevice>
{
public:
    SharedDevice() noexcept = default;
    explicit SharedDevice(VkDevice device, std::unique_ptr<VkDeviceTable> device_table) noexcept
        : shared_handle_base(device,
                             nullptr,
                             std::move(device_table))
        , m_device_table(m_control->m_header.device_table.get())
    {
        m_control->m_header.deleter.m_pfn = m_control->m_header.device_table->vkDestroyDevice;
    }

public:
    auto* table() const noexcept
    {
        return m_device_table;
    }

protected:
    VkDeviceTable* m_device_table;
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
class managed_handle_ex : public managed_handle_base<HandleType, managed_header_ex<HandleType>, managed_handle_ex<HandleType>> {
public:
    using managed_handle_base<HandleType, managed_header_ex<HandleType>, managed_handle_ex<HandleType>>::managed_handle_base;
};

struct SharedInstanceHeader : public managed_header<VkInstance> {
    std::unique_ptr<VkInstanceTable> instance_table;
};

// class SharedInstance : public shared_handle_base<VkInstance, SharedInstanceHeader, SharedInstance>
//{
// public:
//     SharedInstance() noexcept = default;
//     explicit SharedInstance(VkInstance device, std::unique_ptr<VkInstanceTable> device_table) noexcept
//         : shared_handle_base(device,
//                              nullptr,
//                              std::move(device_table))
//         , m_instance_table(m_control->m_header.instance_table.get())
//     {
//         m_control->m_header.deleter.m_pfn = m_control->m_header.instance_table->vkDestroyInstance;
//     }
//
// public:
//     auto* table() const noexcept
//     {
//         return m_instance_table;
//     }
//
// protected:
//     VkInstanceTable* m_instance_table;
// };
} // namespace wis
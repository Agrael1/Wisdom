#ifndef VK_HANDLES_H
#define VK_HANDLES_H
#include <memory>
#include <span>
#include <vector>
#include <vk_mem_alloc.h>
#include <wisdom/generated/vulkan/vk_functions.hpp>
#include <wisvk/vk_movable.hpp>
#include <wisvk/vk_managed_handles.hpp>

namespace wis {
struct SharedDeviceHeader {
    deleter_of_t<VkDevice> deleter;
    std::unique_ptr<wis::VKMainDevice> device_table;
};
struct SharedInstanceHeader {
    deleter_of_t<VkInstance> deleter;
    std::unique_ptr<wis::VKMainInstance> instance_table;
};

class SharedDevice : public shared_handle_base<VkDevice, SharedDeviceHeader, SharedDevice>
{
public:
    SharedDevice() noexcept = default;
    explicit SharedDevice(VkDevice device, std::unique_ptr<VKMainDevice> device_table, VKMainGlobal* global_table) noexcept
        : shared_handle_base(device, nullptr, std::move(device_table)), m_device_table(m_control->m_header.device_table.get()), m_global_table(global_table)
    {
        m_control->m_header.deleter.m_pfn = m_control->m_header.device_table->vkDestroyDevice;
    }

public:
    auto& table() const noexcept
    {
        return *m_device_table;
    }
    auto& gtable() const noexcept
    {
        return *m_global_table;
    }

    template<typename PFN>
    [[nodiscard]] PFN GetInstanceProcAddr(const char* name) const noexcept
    {
        return reinterpret_cast<PFN>(m_global_table->vkGetInstanceProcAddr(get(), name));
    }
    template<typename PFN>
    [[nodiscard]] PFN GetDeviceProcAddr(const char* name) const noexcept
    {
        return reinterpret_cast<PFN>(m_global_table->vkGetDeviceProcAddr(get(), name));
    }

protected:
    VKMainDevice* m_device_table = nullptr;
    VKMainGlobal* m_global_table = nullptr;
};
class SharedInstance : public shared_handle_base<VkInstance, SharedInstanceHeader, SharedInstance>
{
public:
    SharedInstance() noexcept = default;
    explicit SharedInstance(VkInstance device, PFN_vkDestroyInstance deleter, std::unique_ptr<VKMainInstance> instance_table, VKMainGlobal* global_table) noexcept
        : shared_handle_base(device, deleter, std::move(instance_table)), m_instance_table(m_control->m_header.instance_table.get()), m_global_table(global_table)
    {
    }

public:
    auto& table() const noexcept
    {
        return *m_instance_table;
    }
    auto& gtable() const noexcept
    {
        return *m_global_table;
    }
    template<typename PFN>
    [[nodiscard]] PFN GetInstanceProcAddr(const char* name) const noexcept
    {
        return reinterpret_cast<PFN>(m_global_table->vkGetInstanceProcAddr(get(), name));
    }

protected:
    VKMainInstance* m_instance_table = nullptr;
    VKMainGlobal* m_global_table = nullptr;
};

struct SharedPipelineHeader {
    SharedDevice parent;
    deleter_of_t<VkPipeline> deleter;
};
class SharedPipeline : public shared_handle_base<VkPipeline, SharedPipelineHeader, SharedPipeline>
{
public:
    SharedPipeline() noexcept = default;
    explicit SharedPipeline(VkPipeline pipeline, SharedDevice device, PFN_vkDestroyPipeline deleter) noexcept
        : shared_handle_base(pipeline, device, deleter)
    {
    }
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
    managed_handle_ex() = default;
    using managed_handle_base<HandleType, managed_header_ex<HandleType>,
          managed_handle_ex<HandleType>>::managed_handle_base;
};

template<typename HandleType>
struct scoped_header {
};

template<typename HandleType>
requires std::same_as<parent_of_t<HandleType>, nullptr_t>
struct scoped_header<HandleType> {
    deleter_of_t<HandleType> deleter;
};
template<typename HandleType>
requires requires { !std::is_same_v<parent_of_t<HandleType>, nullptr_t>; }
struct scoped_header<HandleType> {
    parent_of_t<HandleType> parent;
    deleter_of_t<HandleType> deleter;
};

template<typename HandleType>
class scoped_handle : public managed_handle_base<HandleType, scoped_header<HandleType>,
    scoped_handle<HandleType>>
{
public:
    scoped_handle() = default;
    using managed_handle_base<HandleType, scoped_header<HandleType>,
          scoped_handle<HandleType>>::managed_handle_base;
    friend managed_handle_base<HandleType, scoped_header<HandleType>,
           scoped_handle<HandleType>>;

protected:
    void internal_destroy() noexcept
    requires(!has_header_v<HandleType>)
    {
    }
    void internal_destroy() noexcept
    requires(!has_parent_v<HandleType> && !has_pool_v<HandleType>)
    {
        this->m_header.deleter(this->m_handle);
    }
    void internal_destroy() noexcept
    requires(has_parent_v<HandleType> && !has_pool_v<HandleType>)
    {
        this->m_header.deleter(this->m_header.parent, this->m_handle);
    }
    void internal_destroy() noexcept
    requires(has_pool_v<HandleType>)
    {
        this->m_header.deleter(this->m_header.parent, this->m_header.pool.get(), this->m_handle);
    }
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

    [[nodiscard]] const auto& parent() const noexcept
    {
        return header();
    }

    [[nodiscard]] const auto& device() const noexcept
    {
        return header();
    }

protected:
    void internal_destroy() noexcept
    {
        vmaDestroyAllocator(get());
    }
};

class SharedSurface : public wis::shared_handle_base<VkSurfaceKHR, wis::managed_header_ex<VkSurfaceKHR>, SharedSurface>
{
    using wis::shared_handle_base<VkSurfaceKHR, wis::managed_header_ex<VkSurfaceKHR>, SharedSurface>::shared_handle_base;
};
} // namespace wis

#endif // VK_HANDLES_H

#pragma once
#include <wisvk/vk_loader.hpp>
#include <wisvk/vk_managed_handles.hpp>
#include <vk_mem_alloc.h>
#include <memory>
#include <vector>
#include <span>

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
class managed_handle_ex : public managed_handle_base<HandleType, managed_header_ex<HandleType>, managed_handle_ex<HandleType>>
{
public:
    using managed_handle_base<HandleType, managed_header_ex<HandleType>, managed_handle_ex<HandleType>>::managed_handle_base;
};

template<>
class managed_handle_ex<VmaAllocator> : public wis::managed_handle_base<VmaAllocator, wis::SharedDevice, managed_handle_ex<VmaAllocator>>
{
    using base = wis::managed_handle_base<VmaAllocator, wis::SharedDevice, managed_handle_ex<VmaAllocator>>;
    friend base;

public:
    managed_handle_ex() = default;
    explicit managed_handle_ex(wis::SharedDevice device, VmaAllocator handle) noexcept
        : wis::managed_handle_base<VmaAllocator, wis::SharedDevice, managed_handle_ex<VmaAllocator>>(handle, std::move(device))
    {
    }

    [[nodiscard]] const auto& parent() const noexcept
    {
        return header();
    }

protected:
    void internal_destroy() noexcept
    {
        vmaDestroyAllocator(get());
    }
};

//template<typename HandleType>
//class managed_vector_ex
//{
//public:
//    managed_vector_ex() noexcept = default;
//    template<typename... Args>
//    managed_vector_ex(size_t size, Args&&... args) noexcept
//        : header(std::forward<Args>(args)...)
//    {
//        reserve(size);
//    }
//    managed_vector_ex(const managed_vector_ex&) = delete;
//    managed_vector_ex(managed_vector_ex&&) noexcept = default;
//    ~managed_vector_ex()
//    {
//        destroy();
//    }
//
//    managed_vector_ex& operator=(const managed_vector_ex&) = delete;
//    managed_vector_ex& operator=(managed_vector_ex&&) noexcept = default;
//
//    operator std::span<HandleType*>() noexcept
//    {
//        return data;
//    }
//    operator std::span<const HandleType*>() const noexcept
//    {
//        return data;
//    }
//    operator bool() const noexcept
//    {
//        return !data.empty();
//    }
//
//public:
//    bool reserve(size_t size) noexcept
//    {
//        data.reserve(size);
//        return true; // TODO: check if allocation succeeded for allocators
//    }
//    void push_back(HandleType* handle) noexcept
//    {
//        data.push_back(handle);
//    }
//    void push_front(HandleType* handle) noexcept
//    {
//        data.insert(data.begin(), handle);
//    }
//    uint32_t size() const noexcept
//    {
//        return static_cast<uint32_t>(data.size());
//    }
//
//private:
//    void destroy()const noexcept
//    {
//        for (auto* handle : data) {
//            header.deleter(header.parent.get(), handle);
//        }
//    }
//
//private:
//    managed_header_ex<HandleType> header;
//    std::vector<HandleType> data;
//};

} // namespace wis
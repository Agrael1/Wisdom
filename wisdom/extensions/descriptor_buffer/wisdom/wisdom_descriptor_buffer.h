#ifndef WISDOM_DESCRIPTOR_BUFFER_H
#define WISDOM_DESCRIPTOR_BUFFER_H
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12_descriptor_buffer.h>
#include <d3d12.h>

namespace wis {
class DX12DescriptorBufferExtension;

template<>
struct Internal<DX12DescriptorBufferExtension> {
    wis::com_ptr<ID3D12Device10> device;
};

class ImplDX12DescriptorBufferExtension : public QueryInternalExtension<DX12DescriptorBufferExtension, DX12DeviceExtension>
{
public:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept
    {
        device = instance.GetInternal().device;
        return wis::success;
    }
    virtual bool Supported() const noexcept override
    {
        return true;
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12RootSignature>
    CreateRootSignature(const RootConstant* root_constants = nullptr,
                        uint32_t constants_size = 0,
                        const wis::DescriptorTable* tables = nullptr,
                        uint32_t tables_count = 0) const noexcept;

    [[nodiscard]] uint32_t
    GetDescriptorTableAlignment(wis::DescriptorHeapType heap) const noexcept
    {
        return device->GetDescriptorHandleIncrementSize(wis::convert_dx(heap));
    }
    [[nodiscard]] uint32_t
    GetDescriptorSize(wis::DescriptorHeapType heap) const noexcept
    {
        return device->GetDescriptorHandleIncrementSize(wis::convert_dx(heap));
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12DescriptorBuffer>
    CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t memory_bytes) const noexcept;

public: // Command List
    WIS_INLINE void SetDescriptorBuffers(wis::DX12CommandListView cmd_list,
                                         wis::DX12DescriptorBufferView resource_desc_buffer,
                                         wis::DX12DescriptorBufferView sampler_desc_buffer) const noexcept;

    WIS_INLINE void SetDescriptorTableOffset(wis::DX12CommandListView cmd_list,
            wis::DX12RootSignatureView root_signature,
            uint32_t root_table_index,
            wis::DX12DescriptorBufferGPUView buffer,
            uint32_t table_aligned_byte_offset) const noexcept;
};

#pragma region DX12DescriptorBufferExtension
class DX12DescriptorBufferExtension : public wis::ImplDX12DescriptorBufferExtension
{
public:
    using wis::ImplDX12DescriptorBufferExtension::ImplDX12DescriptorBufferExtension;

public:
    /**
     * @brief Creates a root signature object.
     * @param root_constants The root constants to create the root signature with.
     * @param constants_size The number of root constants.
     * @param tables The descriptor tables to create the root signature with.
     * @param tables_count The number of descriptor tables.
     * @return wis::DX12RootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12RootSignature> CreateRootSignature(const wis::RootConstant* root_constants = nullptr, uint32_t constants_size = 0, const wis::DescriptorTable* tables = nullptr, uint32_t tables_count = 0) const noexcept
    {
        return wis::ImplDX12DescriptorBufferExtension::CreateRootSignature(root_constants, constants_size, tables, tables_count);
    }
    /**
     * @brief Creates a descriptor buffer object.
     * @param type The type of the descriptor buffer to create.
     * @param memory Memory location of the buffer (CPU or GPU).
     * @param memory_bytes The size of the descriptor buffer in bytes.
     * @return wis::DX12DescriptorBuffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12DescriptorBuffer> CreateDescriptorBuffer(wis::DescriptorHeapType type, wis::DescriptorMemory memory, uint64_t memory_bytes) const noexcept
    {
        return wis::ImplDX12DescriptorBufferExtension::CreateDescriptorBuffer(type, memory, memory_bytes);
    }
    /**
     * @brief Returns the alignment of the descriptor table in bytes.
     * @param type The type of the descriptor buffer to get the alignment for.
     * @return The alignment of the descriptor table in bytes.
     * */
    inline uint64_t GetDescriptorTableAlignment(wis::DescriptorHeapType type) const noexcept
    {
        return wis::ImplDX12DescriptorBufferExtension::GetDescriptorTableAlignment(type);
    }
    /**
     * @brief Returns the size of single descriptor in bytes.
     * @param type The type of the descriptor buffer to get the size for.
     * @return The size of the descriptor table in bytes.
     * */
    inline uint64_t GetDescriptorSize(wis::DescriptorHeapType type) const noexcept
    {
        return wis::ImplDX12DescriptorBufferExtension::GetDescriptorSize(type);
    }
    /**
     * @brief Sets the descriptor buffers to the command list.
     * @param list The command list to set the descriptor buffers to.
     * @param resource_buffer The resource descriptor buffer to set.
     * @param sampler_buffer The sampler descriptor buffer to set.
     * */
    inline void SetDescriptorBuffers(wis::DX12CommandListView list, wis::DX12DescriptorBufferView resource_buffer, wis::DX12DescriptorBufferView sampler_buffer) const noexcept
    {
        wis::ImplDX12DescriptorBufferExtension::SetDescriptorBuffers(std::move(list), std::move(resource_buffer), std::move(sampler_buffer));
    }
    /**
     * @brief Sets the offset into the descriptor buffer
     * for selected descriptor table. Pipeline will get the bindings from that offset.
     * @param list The command list to set the descriptor buffer offset to.
     * @param root_signature The root signature to get binding table from.
     * @param table_index Index of the descriptor table in the root signature.
     * @param buffer The descriptor buffer to set the offset for.
     * @param table_aligned_byte_offset Byte offset from the buffer beginning in table alignment sizes.
     * */
    inline void SetDescriptorTableOffset(wis::DX12CommandListView list, wis::DX12RootSignatureView root_signature, uint32_t table_index, wis::DX12DescriptorBufferGPUView buffer, uint32_t table_aligned_byte_offset) const noexcept
    {
        wis::ImplDX12DescriptorBufferExtension::SetDescriptorTableOffset(std::move(list), std::move(root_signature), table_index, std::move(buffer), table_aligned_byte_offset);
    }
};
#pragma endregion DX12DescriptorBufferExtension

} // namespace wis

#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vk_descriptor_buffer.h>

namespace wis {
class VKDescriptorBufferExtension;

template<>
struct Internal<VKDescriptorBufferExtension> {
    wis::SharedDevice device;
    wis::shared_handle<VmaAllocator> allocator;
    VKDescriptorBufferProperties descriptor_buffer_props;
    VKDescBufferExtDevice ftable;
};

class ImplVKDescriptorBufferExtension : public QueryInternalExtension<VKDescriptorBufferExtension, wis::VKDeviceExtension>
{
protected:
    virtual WIS_INLINE bool
    GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                     std::unordered_set<std::string_view>& ext_name_set,
                     std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                     std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

    virtual WIS_INLINE wis::Result
    Init(const wis::VKDevice& instance,
         const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
         const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

public:
    virtual bool Supported() const noexcept override
    {
        return ftable.vkGetDescriptorEXT;
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRootSignature>
    CreateRootSignature(const RootConstant* constants = nullptr,
                        uint32_t constants_size = 0,
                        const wis::DescriptorTable* tables = nullptr,
                        uint32_t tables_count = 0) const noexcept;

    [[nodiscard]] uint32_t
    GetDescriptorTableAlignment([[maybe_unused]] wis::DescriptorHeapType heap) const noexcept
    {
        return descriptor_buffer_props.offset_alignment;
    }

    [[nodiscard]] uint32_t
    GetDescriptorSize(wis::DescriptorHeapType heap) const noexcept
    {
        auto& heap_features = descriptor_buffer_props;
        return heap == wis::DescriptorHeapType::Descriptor
               ? heap_features.mutable_descriptor_size
               : heap_features.sampler_size;
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKDescriptorBuffer>
    CreateDescriptorBuffer(wis::DescriptorHeapType heap_type,
                           wis::DescriptorMemory memory_type,
                           uint64_t memory_bytes) const noexcept;

public: // Command List
    WIS_INLINE void SetDescriptorBuffers(wis::VKCommandListView cmd_list,
                                         wis::VKDescriptorBufferView resource_desc_buffer,
                                         wis::VKDescriptorBufferView sampler_desc_buffer) const noexcept;

    WIS_INLINE void SetDescriptorTableOffset(wis::VKCommandListView cmd_list,
            wis::VKRootSignatureView root_signature,
            uint32_t root_table_index,
            wis::VKDescriptorBufferGPUView buffer,
            uint32_t table_aligned_byte_offset) const noexcept;

protected:
    [[nodiscard]] wis::ResultValue<VkDescriptorSetLayout>
    CreateDescriptorSetLayout(const wis::DescriptorTable* table) const noexcept
    {
        return table->type == wis::DescriptorHeapType::Descriptor
               ? VKCreateDescriptorSetDescriptorLayout(table)
               : VKCreateDescriptorSetSamplerLayout(table);
    }
    WIS_INLINE wis::ResultValue<VkDescriptorSetLayout> VKCreateDescriptorSetDescriptorLayout(const wis::DescriptorTable* table) const noexcept;
    WIS_INLINE wis::ResultValue<VkDescriptorSetLayout> VKCreateDescriptorSetSamplerLayout(const wis::DescriptorTable* table) const noexcept;
};

#pragma region VKDescriptorBufferExtension
class VKDescriptorBufferExtension : public wis::ImplVKDescriptorBufferExtension
{
public:
    using wis::ImplVKDescriptorBufferExtension::ImplVKDescriptorBufferExtension;

public:
    /**
     * @brief Creates a root signature object.
     * @param root_constants The root constants to create the root signature with.
     * @param constants_size The number of root constants.
     * @param tables The descriptor tables to create the root signature with.
     * @param tables_count The number of descriptor tables.
     * @return wis::VKRootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRootSignature> CreateRootSignature(const wis::RootConstant* root_constants = nullptr, uint32_t constants_size = 0, const wis::DescriptorTable* tables = nullptr, uint32_t tables_count = 0) const noexcept
    {
        return wis::ImplVKDescriptorBufferExtension::CreateRootSignature(root_constants, constants_size, tables, tables_count);
    }
    /**
     * @brief Creates a descriptor buffer object.
     * @param type The type of the descriptor buffer to create.
     * @param memory Memory location of the buffer (CPU or GPU).
     * @param memory_bytes The size of the descriptor buffer in bytes.
     * @return wis::VKDescriptorBuffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKDescriptorBuffer> CreateDescriptorBuffer(wis::DescriptorHeapType type, wis::DescriptorMemory memory, uint64_t memory_bytes) const noexcept
    {
        return wis::ImplVKDescriptorBufferExtension::CreateDescriptorBuffer(type, memory, memory_bytes);
    }
    /**
     * @brief Returns the alignment of the descriptor table in bytes.
     * @param type The type of the descriptor buffer to get the alignment for.
     * @return The alignment of the descriptor table in bytes.
     * */
    inline uint64_t GetDescriptorTableAlignment(wis::DescriptorHeapType type) const noexcept
    {
        return wis::ImplVKDescriptorBufferExtension::GetDescriptorTableAlignment(type);
    }
    /**
     * @brief Returns the size of single descriptor in bytes.
     * @param type The type of the descriptor buffer to get the size for.
     * @return The size of the descriptor table in bytes.
     * */
    inline uint64_t GetDescriptorSize(wis::DescriptorHeapType type) const noexcept
    {
        return wis::ImplVKDescriptorBufferExtension::GetDescriptorSize(type);
    }
    /**
     * @brief Sets the descriptor buffers to the command list.
     * @param list The command list to set the descriptor buffers to.
     * @param resource_buffer The resource descriptor buffer to set.
     * @param sampler_buffer The sampler descriptor buffer to set.
     * */
    inline void SetDescriptorBuffers(wis::VKCommandListView list, wis::VKDescriptorBufferView resource_buffer, wis::VKDescriptorBufferView sampler_buffer) const noexcept
    {
        wis::ImplVKDescriptorBufferExtension::SetDescriptorBuffers(std::move(list), std::move(resource_buffer), std::move(sampler_buffer));
    }
    /**
     * @brief Sets the offset into the descriptor buffer
     * for selected descriptor table. Pipeline will get the bindings from that offset.
     * @param list The command list to set the descriptor buffer offset to.
     * @param root_signature The root signature to get binding table from.
     * @param table_index Index of the descriptor table in the root signature.
     * @param buffer The descriptor buffer to set the offset for.
     * @param table_aligned_byte_offset Byte offset from the buffer beginning in table alignment sizes.
     * */
    inline void SetDescriptorTableOffset(wis::VKCommandListView list, wis::VKRootSignatureView root_signature, uint32_t table_index, wis::VKDescriptorBufferGPUView buffer, uint32_t table_aligned_byte_offset) const noexcept
    {
        wis::ImplVKDescriptorBufferExtension::SetDescriptorTableOffset(std::move(list), std::move(root_signature), table_index, std::move(buffer), table_aligned_byte_offset);
    }
};
#pragma endregion VKDescriptorBufferExtension
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using DescriptorBufferExtension = DX12DescriptorBufferExtension;
using DescriptorBuffer = DX12DescriptorBuffer;
#elif defined(WISDOM_VULKAN)
using DescriptorBufferExtension = VKDescriptorBufferExtension;
using DescriptorBuffer = VKDescriptorBuffer;
#endif // WISDOM_DX12
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/impl.dx.cpp"
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_EXTENDED_ALLOCATION_H

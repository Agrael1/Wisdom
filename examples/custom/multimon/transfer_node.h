#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_extended_allocation.hpp>
#include <wisdom/wisdom_descriptor_buffer.hpp>
#include <wisdom/wisdom_platform.hpp>
#include <span>
#include <array>
#include <string_view>
#include <expected>

struct ExternalBuffer {
    ExternalBuffer() = default;
    ExternalBuffer(wis::SharedDevice device, VkBuffer buffer, VkDeviceMemory memory)
        : device(std::move(device)), buffer(buffer), memory(memory)
    {
    }
    ExternalBuffer(ExternalBuffer&& other) noexcept
        : device(std::move(other.device)), buffer(std::move(other.buffer)), memory(std::move(other.memory))
    {
    }
    ExternalBuffer& operator=(ExternalBuffer&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        Destroy();
        device = std::move(other.device);
        buffer = std::move(other.buffer);
        memory = std::move(other.memory);
        return *this;
    }
    void Destroy() noexcept
    {
        if (buffer) {
            device.table().vkDestroyBuffer(device.get(), buffer, nullptr);
        }
        if (memory) {
            device.table().vkFreeMemory(device.get(), memory, nullptr);
        }
    }
    ~ExternalBuffer()
    {
        Destroy();
    }

public:
    operator wis::VKBufferView() const noexcept
    {
        return wis::VKBufferView{ buffer };
    }

    wis::SharedDevice device;
    wis::h::VkBuffer buffer;
    wis::h::VkDeviceMemory memory;
};

struct ExtMemoryHost : public wis::DeviceExtension {
    PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT = nullptr;
    wis::SharedDevice shared_device;
    uint64_t size_alignment = 0;

protected:
    virtual WIS_INLINE bool
    GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                     std::unordered_set<std::string_view>& ext_name_set,
                     std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                     std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        if (available_extensions.find(VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME) == available_extensions.end()) {
            return false;
        }

        ext_name_set.insert(VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME);
        property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceExternalMemoryHostPropertiesEXT);
        return true;
    }

    virtual WIS_INLINE wis::Result
    Init(const wis::VKDevice& instance,
         const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
         const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        shared_device = instance.GetInternal().device;
        vkGetMemoryHostPointerPropertiesEXT = shared_device.GetDeviceProcAddr<PFN_vkGetMemoryHostPointerPropertiesEXT>("vkGetMemoryHostPointerPropertiesEXT");

        auto& props = *(VkPhysicalDeviceExternalMemoryHostPropertiesEXT*)(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT));
        size_alignment = props.minImportedHostPointerAlignment;
        return {};
    }

public:
    virtual bool Supported() const noexcept override
    {
        return vkGetMemoryHostPointerPropertiesEXT;
    }

    ExternalBuffer CreateExternalBuffer(wis::Result& result, wis::ResourceAllocator allocator, void* mapping, uint64_t size) const noexcept
    {
        ExternalBuffer buffer;

        uint32_t mem_idx = 0;
        VkMemoryHostPointerPropertiesEXT props{
            .sType = VK_STRUCTURE_TYPE_MEMORY_HOST_POINTER_PROPERTIES_EXT,
        };
        auto res = vkGetMemoryHostPointerPropertiesEXT(shared_device.get(),
                   VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT,
                   mapping,
                   &props);
        if (res != VK_SUCCESS) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "vkGetMemoryHostPointerPropertiesEXT failed: ">(res);
            return buffer;
        }

        VmaAllocationCreateInfo xalloc_info{
            //.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
            .usage = VMA_MEMORY_USAGE_GPU_TO_CPU,
            .requiredFlags = 0,
            .preferredFlags = 0,
            .memoryTypeBits = props.memoryTypeBits,
            .pool = nullptr,
            .pUserData = nullptr,
            .priority = 0.0f,
        };
        vmaFindMemoryTypeIndex(allocator.GetInternal().allocator.get(), props.memoryTypeBits, &xalloc_info, &mem_idx);

        VkExternalMemoryBufferCreateInfo ext_info{
            .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO,
            .handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT
        };
        VkBufferCreateInfo buffer_info{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = &ext_info,
            .size = size,
            .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VkImportMemoryHostPointerInfoEXT import_info{
            .sType = VK_STRUCTURE_TYPE_IMPORT_MEMORY_HOST_POINTER_INFO_EXT,
            .handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT,
            .pHostPointer = mapping,
        };
        VkMemoryAllocateInfo alloc_info{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = &import_info,
            .allocationSize = wis::detail::aligned_size(size, size_alignment),
            .memoryTypeIndex = mem_idx,
        };
        res = shared_device.table().vkAllocateMemory(shared_device.get(), &alloc_info, nullptr, &buffer.memory);
        if (res != VK_SUCCESS) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "vkAllocateMemory failed: ">(res);
            return buffer;
        }
        res = shared_device.table().vkCreateBuffer(shared_device.get(), &buffer_info, nullptr, &buffer.buffer);
        if (res != VK_SUCCESS) {
            shared_device.table().vkFreeMemory(shared_device.get(), buffer.memory, nullptr);
            buffer.memory = nullptr;

            result = wis::make_result<wis::Func<wis::FuncD()>(), "vkCreateBuffer failed: ">(res);
            return buffer;
        }

        res = shared_device.table().vkBindBufferMemory(shared_device.get(), buffer.buffer, buffer.memory, 0);
        if (res != VK_SUCCESS) {
            shared_device.table().vkDestroyBuffer(shared_device.get(), buffer.buffer, nullptr);
            shared_device.table().vkFreeMemory(shared_device.get(), buffer.memory, nullptr);
            buffer.buffer = nullptr;
            buffer.memory = nullptr;
            result = wis::make_result<wis::Func<wis::FuncD()>(), "vkBindBufferMemory failed: ">(res);
        }
        return buffer;
    }
};

struct TransferNode {
    static constexpr uint32_t kFrameCount = 2;

public:
    void InitSwapchain(wis::SwapChain&& swap, wis::SwapChain&& swap2);
    void Resize(uint32_t width, uint32_t height, uint32_t swapn);
    void VKImportFrame(wis::Size2D frame, void* mapping);
    void WaitForGPU() noexcept;

    void Frame();
    void BlitFrame(uint32_t swapn);

public:
    wis::Size2D frame_size[2];

    wis::ExtendedAllocation ext_alloc;
    wis::DescriptorBufferExtension desc_buffer_ext;
    wis::Device transfer_device;
    wis::ResourceAllocator allocator;
    wis::SwapChain swap[2];
    std::span<const wis::Texture> back_buffers[2];
    std::array<wis::RenderTarget, 2> render_targets[2];
    wis::CommandQueue queue;
    wis::CommandList cmd_list;
    wis::CommandList copy_cmd_list;

    wis::Size2D input_size;
    ExtMemoryHost ext_mem_host;
    ExternalBuffer input_buffer;
    wis::Texture texture;
    wis::ShaderResource texture_srv;

    wis::Texture debug_texture;
    wis::Buffer debug_buffer;

    wis::Fence fence;
    uint64_t fence_value = 1;

    wis::Shader vs;
    wis::Shader ps;

    wis::RootSignature root_signature;
    wis::PipelineState pipeline_state;

    wis::DescriptorBuffer desc_buffer;
    wis::DescriptorBuffer sampler_buffer;
    wis::Sampler sampler;
};

std::expected<TransferNode, std::string_view>
CreateTransferNode(wis::Adapter&& adapter);

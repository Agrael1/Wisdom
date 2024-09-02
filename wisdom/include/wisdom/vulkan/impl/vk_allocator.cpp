#ifndef VK_ALLOCATOR_CPP
#define VK_ALLOCATOR_CPP
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>
#include <wisdom/util/misc.h>
#include <wisdom/vulkan/vk_external.h>

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory, wis::MemoryFlags mem_flags) const noexcept
{
    VkBufferCreateInfo desc;
    VKFillBufferDesc(size, usage, desc);

    VmaAllocationCreateInfo alloc{
        .flags = wis::convert_vk(mem_flags),
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = wis::convert_vk(memory)
    };
    return VKCreateBuffer(desc, alloc);
}

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory, wis::MemoryFlags mem_flags) const noexcept
{
    VkImageCreateInfo img_desc;
    VKFillImageDesc(desc, img_desc);

    VmaAllocationCreateInfo alloc{
        .flags = wis::convert_vk(mem_flags),
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = wis::convert_vk(memory)
    };
    return VKCreateTexture(img_desc, alloc);
}

wis::AllocationInfo
wis::VKResourceAllocator::GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept
{
    VkMemoryRequirements2 memReq{
        .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2
    };
    VKFillTextureAllocationInfo(desc, memReq);
    return {
        .size_bytes = memReq.memoryRequirements.size,
        .alignment_bytes = memReq.memoryRequirements.alignment,
    };
}

wis::AllocationInfo
wis::VKResourceAllocator::GetBufferAllocationInfo(uint64_t size, BufferUsage flags) const noexcept
{
    VkMemoryRequirements2 memReq{
        .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2
    };
    VKFillBufferAllocationInfo(size, flags, memReq);
    return {
        .size_bytes = memReq.memoryRequirements.size,
        .alignment_bytes = memReq.memoryRequirements.alignment,
    };
}

wis::ResultValue<wis::VKMemory>
wis::VKResourceAllocator::AllocateImageMemory(uint64_t size, wis::TextureUsage usage,
                                              wis::MemoryType memory,
                                              wis::MemoryFlags mem_flags) const noexcept
{
    VkMemoryRequirements2 req{};
    VKFillTextureAllocationInfo({
                                        .format = wis::DataFormat::RGBA8Unorm,
                                        .size = { 1, 1 },
                                        .usage = usage,
                                },
                                req);

    req.memoryRequirements.size = wis::detail::aligned_size(size, req.memoryRequirements.alignment);

    VmaMemoryUsage vma_usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
    switch (memory) {
    case wis::MemoryType::Default:
        break;
    case wis::MemoryType::Upload:
    case wis::MemoryType::GPUUpload:
        vma_usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
        break;
    case wis::MemoryType::Readback:
        vma_usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
        break;
    default:
        break;
    }

    VmaAllocationCreateInfo alloc_desc{
        .flags = convert_vk(mem_flags),
        .usage = vma_usage,
        .requiredFlags = wis::convert_vk(memory),
    };
    VmaAllocation allocation;
    auto result = vmaAllocateMemory(allocator.get(), &req.memoryRequirements, &alloc_desc, &allocation, nullptr);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Image memory allocation failed">(result);

    return VKMemory{ allocator, allocation };
}
wis::ResultValue<wis::VKMemory>
wis::VKResourceAllocator::AllocateBufferMemory(uint64_t size, wis::BufferUsage usage,
                                               wis::MemoryType memory,
                                               wis::MemoryFlags mem_flags) const noexcept
{
    VkMemoryRequirements2 req{};
    VKFillBufferAllocationInfo(size, usage, req);

    req.memoryRequirements.size = wis::detail::aligned_size(size, req.memoryRequirements.alignment);

    VmaMemoryUsage vma_usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
    switch (memory) {
    case wis::MemoryType::Default:
        break;
    case wis::MemoryType::Upload:
    case wis::MemoryType::GPUUpload:
        vma_usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
        break;
    case wis::MemoryType::Readback:
        vma_usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
        break;
    default:
        break;
    }

    VmaAllocationCreateInfo alloc_desc{
        .flags = convert_vk(mem_flags),
        .usage = vma_usage,
        .requiredFlags = wis::convert_vk(memory),
    };
    VmaAllocation allocation;
    auto result = vmaAllocateMemory(allocator.get(), &req.memoryRequirements, &alloc_desc, &allocation, nullptr);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Buffer memory allocation failed">(result);

    return VKMemory{ allocator, allocation };
}

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::PlaceBuffer(wis::VKMemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept
{
    if (std::get<0>(memory) != allocator.get())
        return wis::make_result<FUNC, "Memory allocator mismatch">(VK_ERROR_UNKNOWN);

    VkBufferCreateInfo desc;
    VKFillBufferDesc(size, usage, desc);
    return VKCreateAliasingBuffer(desc, memory_offset, std::get<1>(memory));
}

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::PlaceTexture(wis::VKMemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept
{
    if (std::get<0>(memory) != allocator.get())
        return wis::make_result<FUNC, "Memory allocator mismatch">(VK_ERROR_UNKNOWN);

    VkImageCreateInfo img_desc;
    VKFillImageDesc(desc, img_desc);
    return VKCreateAliasingTexture(img_desc, memory_offset, std::get<1>(memory));
}

// =========================================================================================

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::VKCreateTexture(VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
{
    constexpr static VkExternalMemoryImageCreateInfoKHR external_info{
        .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .handleTypes = detail::memory_handle_type
    };

    desc.pNext = interop ? &external_info : nullptr;

    VmaAllocation allocation;
    VkImage buffer;

    auto result = vmaCreateImage(
            allocator.get(),
            reinterpret_cast<const VkImageCreateInfo*>(&desc),
            &alloc_desc,
            &buffer,
            &allocation,
            nullptr);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Texture allocation failed">(result);

    return VKTexture{ desc.format, buffer, { desc.extent.width, desc.extent.height }, allocator, allocation };
}

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::VKCreateBuffer(VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
{
    constexpr static VkExternalMemoryBufferCreateInfoKHR external_info{
        .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO_KHR,
        .pNext = nullptr,
        .handleTypes = detail::memory_handle_type
    };

    desc.pNext = interop ? &external_info : nullptr;

    VmaAllocation allocation;
    VkBuffer buffer;
    VkResult result = vmaCreateBuffer(
            allocator.get(),
            &desc,
            &alloc_desc,
            &buffer,
            &allocation,
            nullptr);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Buffer allocation failed">(result);

    return VKBuffer{ allocator, buffer, allocation };
}

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::VKCreateAliasingBuffer(VkBufferCreateInfo& desc, VkDeviceSize offset, VmaAllocation alloc) const noexcept
{
    constexpr static VkExternalMemoryBufferCreateInfoKHR external_info{
        .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO_KHR,
        .pNext = nullptr,
        .handleTypes = detail::memory_handle_type
    };

    desc.pNext = interop ? &external_info : nullptr;

    VkBuffer buffer;
    auto res = vmaCreateAliasingBuffer2(allocator.get(), alloc, offset, &desc, &buffer);
    if (!wis::succeeded(res)) {
        return wis::make_result<FUNC, "Aliasing buffer creation failed">(res);
    }

    return VKBuffer{ allocator, buffer, nullptr };
}

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::VKCreateAliasingTexture(VkImageCreateInfo& desc, VkDeviceSize offset, VmaAllocation alloc) const noexcept
{
    constexpr static VkExternalMemoryImageCreateInfoKHR external_info{
        .sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .handleTypes = detail::memory_handle_type
    };

    desc.pNext = interop ? &external_info : nullptr;

    VkImage buffer;
    auto res = vmaCreateAliasingImage2(allocator.get(), alloc, offset, &desc, &buffer);
    if (!wis::succeeded(res)) {
        return wis::make_result<FUNC, "Aliasing buffer creation failed">(res);
    }

    return VKTexture{ desc.format, buffer, { desc.extent.width, desc.extent.height }, allocator, nullptr };
}

void wis::VKResourceAllocator::VKFillBufferDesc(uint64_t size, wis::BufferUsage flags, VkBufferCreateInfo& info) noexcept
{
    info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VkBufferUsageFlags(VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VkBufferUsageFlagBits(flags)),
    };
}
void wis::VKResourceAllocator::VKFillImageDesc(const wis::TextureDesc& desc, VkImageCreateInfo& info) noexcept
{
    info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .format = convert_vk(desc.format),
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .usage = convert_vk(desc.usage),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    switch (desc.layout) {
    case wis::TextureLayout::Texture1D:
        info.imageType = VK_IMAGE_TYPE_1D;
        info.extent = { desc.size.width, 1, 1 };
        info.mipLevels = desc.mip_levels;
        info.arrayLayers = 1;
        break;
    case wis::TextureLayout::Texture2D:
        info.imageType = VK_IMAGE_TYPE_2D;
        info.extent = { desc.size.width, desc.size.height, 1 };
        info.mipLevels = desc.mip_levels;
        info.arrayLayers = 1;
        break;
    case wis::TextureLayout::Texture1DArray:
        info.imageType = VK_IMAGE_TYPE_1D;
        info.extent = { desc.size.width, 1, 1 };
        info.mipLevels = desc.mip_levels;
        info.arrayLayers = desc.size.depth_or_layers;
        break;
    default:
    case wis::TextureLayout::Texture2DArray:
        info.imageType = VK_IMAGE_TYPE_2D;
        info.extent = { desc.size.width, desc.size.height, 1 };
        info.mipLevels = desc.mip_levels;
        info.arrayLayers = desc.size.depth_or_layers;
        break;
    case wis::TextureLayout::Texture3D:
        info.imageType = VK_IMAGE_TYPE_3D;
        info.flags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
        info.extent = { desc.size.width, desc.size.height, desc.size.depth_or_layers };
        info.mipLevels = desc.mip_levels;
        info.arrayLayers = 1;
        break;
    case wis::TextureLayout::Texture2DMS:
        info.imageType = VK_IMAGE_TYPE_2D;
        info.extent = { desc.size.width, desc.size.height, 1 };
        info.mipLevels = 1;
        info.arrayLayers = 1;
        info.samples = convert_vk(desc.sample_count);
        break;
    case wis::TextureLayout::Texture2DMSArray:
        info.imageType = VK_IMAGE_TYPE_2D;
        info.extent = { desc.size.width, desc.size.height, 1 };
        info.mipLevels = 1;
        info.arrayLayers = desc.size.depth_or_layers;
        info.samples = convert_vk(desc.sample_count);
        break;
    }
}

void wis::VKResourceAllocator::VKFillTextureAllocationInfo(const wis::TextureDesc& desc, VkMemoryRequirements2& out_info) const noexcept
{
    VkImageCreateInfo imageInfo;
    VKFillImageDesc(desc, imageInfo);

    VkDeviceImageMemoryRequirementsKHR devImgMemReq{
        .sType = VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS_KHR,
        .pCreateInfo = &imageInfo
    };
    VkMemoryRequirements2 memReq{
        .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2
    };
    auto& dtable = allocator.header().table();
    dtable.vkGetDeviceImageMemoryRequirements(
            allocator.header().get(), &devImgMemReq, &memReq);
    out_info = memReq;
}
void wis::VKResourceAllocator::VKFillBufferAllocationInfo(uint64_t size, wis::BufferUsage flags, VkMemoryRequirements2& out_info) const noexcept
{
    VkBufferCreateInfo buf_info;
    VKFillBufferDesc(size, flags, buf_info);

    VkDeviceBufferMemoryRequirementsKHR devImgMemReq{
        .sType = VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS_KHR,
        .pCreateInfo = &buf_info
    };
    VkMemoryRequirements2 memReq{
        .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2
    };
    auto& dtable = allocator.header().table();
    dtable.vkGetDeviceBufferMemoryRequirements(
            allocator.header().get(), &devImgMemReq, &memReq);
    out_info = memReq;
}

#endif // !VK_ALLOCATOR_CPP

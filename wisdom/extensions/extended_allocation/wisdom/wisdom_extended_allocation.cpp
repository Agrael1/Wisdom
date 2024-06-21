#ifndef WISDOM_EXTENDED_ALLOCATION_CPP
#define WISDOM_EXTENDED_ALLOCATION_CPP
#include <wisdom/wisdom_extended_allocation.h>
#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_property_format_table.h>

#if defined(WISDOM_DX12)
wis::ResultValue<wis::DX12Texture>
wis::DX12ExtendedAllocation::CreateTexture(const wis::DX12ResourceAllocator& allocator,
                                           wis::TextureDesc desc,
                                           wis::MemoryType memory,
                                           wis::MemoryFlags flags) const noexcept
{
    auto tex_desc = DX12ResourceAllocator::DX12CreateTextureDesc(desc);

    memory = memory == wis::MemoryType::GPUUpload && supports_gpu_upload ? wis::MemoryType::GPUUpload : wis::MemoryType::Upload;
    D3D12MA::ALLOCATION_FLAGS all_flags = D3D12MA::ALLOCATION_FLAG_NONE;
    if (flags & wis::MemoryFlags::DedicatedAllocation)
        all_flags |= D3D12MA::ALLOCATION_FLAG_COMMITTED;

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = all_flags,
        .HeapType = convert_dx(memory),
    };
    return allocator.DX12CreateResource(all_desc, tex_desc, D3D12_RESOURCE_STATE_COMMON);
}

wis::Result
wis::DX12ExtendedAllocation::WriteMemoryToSubresource(const void* host_data,
                                                      wis::DX12TextureView dst_texture,
                                                      wis::TextureState initial_state,
                                                      wis::TextureRegion region) const noexcept
{
    auto resource = std::get<0>(dst_texture);
    auto texture_desc = resource->GetDesc();
    UINT dest_subresource = D3D12CalcSubresource(region.mip, region.array_layer, 0u, texture_desc.MipLevels, texture_desc.DepthOrArraySize);
    

    auto hr = resource->Map(dest_subresource, nullptr, nullptr);
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to map resource">(hr);

    UINT row_pitch = 0;
    UINT slice_pitch = 0;
    hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorRowPitch(convert_dx(region.format), region.size.width, row_pitch);
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to calculate row pitch">(hr);


    hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorSlicePitch(convert_dx(region.format), row_pitch, region.size.height, slice_pitch);
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to calculate slice pitch">(hr);

    D3D12_BOX box{
        .left = region.offset.width,
        .top = region.offset.height,
        .front = region.offset.depth_or_layers,
        .right = region.offset.width + region.size.width,
        .bottom = region.offset.height + region.size.height,
        .back = region.offset.depth_or_layers + region.size.depth_or_layers,
    };
    hr = resource->WriteToSubresource(dest_subresource, &box, host_data, row_pitch, slice_pitch);
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to write to subresource">(hr);

    resource->Unmap(dest_subresource, nullptr);
    return wis::success;
}
#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
wis::ResultValue<wis::VKTexture>
wis::VKExtendedAllocation::CreateTexture(const wis::VKResourceAllocator& allocator,
                                         wis::TextureDesc desc,
                                         wis::MemoryType memory,
                                         wis::MemoryFlags flags) const noexcept
{
    VkImageCreateInfo img_desc = VKResourceAllocator::VKCreateTextureDesc(desc);

    VmaAllocationCreateFlags xflags = 0;
    if (flags & wis::MemoryFlags::Mapped)
        xflags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    if (flags & wis::MemoryFlags::DedicatedAllocation)
        xflags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

    VmaAllocationCreateInfo alloc{
        .flags = xflags,
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = wis::convert_vk(memory),
    };

    auto [res, texture] = allocator.VKCreateTexture(img_desc, alloc);
    if (res.status != wis::Status::Ok)
        return res;

    if (!(img_desc.usage & VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT)) {
        return std::move(texture);
    }

    // Transition image layout to general for host copy layouts
    auto& tex_i = texture.GetInternal();
    VkHostImageLayoutTransitionInfoEXT transition{
        .sType = VK_STRUCTURE_TYPE_HOST_IMAGE_LAYOUT_TRANSITION_INFO_EXT,
        .pNext = nullptr,
        .image = texture.GetInternal().buffer,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_GENERAL,
        .subresourceRange = {
                .aspectMask = aspect_flags(tex_i.format),
                .baseMipLevel = 0,
                .levelCount = img_desc.mipLevels,
                .baseArrayLayer = 0,
                .layerCount = img_desc.arrayLayers,
        },
    };

    auto vr = vkTransitionImageLayoutEXT(device.get(), 1, &transition);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to transition image layout">(vr);
    return std::move(texture);
}

wis::Result
wis::VKExtendedAllocation::WriteMemoryToSubresource(const void* host_data,
                                                    wis::VKTextureView dst_texture,
                                                    wis::TextureState initial_state,
                                                    const wis::BufferTextureCopyRegion* regions,
                                                    uint32_t region_count) const noexcept
{
    wis::detail::limited_allocator<VkMemoryToImageCopyEXT, 8> allocator(region_count, true);
    auto* copies = allocator.data();

    auto aspects = aspect_flags(std::get<1>(dst_texture));

    for (size_t i = 0; i < region_count; i++) {
        auto& region = regions[i];
        copies[i] = VkMemoryToImageCopyEXT{
            .sType = VK_STRUCTURE_TYPE_MEMORY_TO_IMAGE_COPY_EXT,
            .pNext = nullptr,
            .pHostPointer = reinterpret_cast<const uint8_t*>(host_data) + region.buffer_offset,
            .memoryRowLength = {},
            .memoryImageHeight = {},
            .imageSubresource = {
                    .aspectMask = aspects,
                    .mipLevel = region.texture.mip,
                    .baseArrayLayer = region.texture.array_layer,
                    .layerCount = 1u,
            },
            .imageOffset = { int(region.texture.offset.width), int(region.texture.offset.height), int(region.texture.offset.depth_or_layers) },
            .imageExtent = { region.texture.size.width, region.texture.size.height, region.texture.size.depth_or_layers },
        };
    }

    VkCopyMemoryToImageInfoEXT copy_info{
        .sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_IMAGE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .dstImage = std::get<0>(dst_texture),
        .dstImageLayout = convert_vk(initial_state),
        .regionCount = region_count,
        .pRegions = copies,
    };

    auto vr = vkCopyMemoryToImageEXT(device.get(), &copy_info);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to copy memory to image">(vr);
    return wis::success;
}

#endif // WISDOM_VULKAN
#endif // WISDOM_EXTENDED_ALLOCATION_CPP

#ifndef WISDOM_DESCRIPTOR_BUFFER_VK_CPP
#define WISDOM_DESCRIPTOR_BUFFER_VK_CPP
#include <wisdom/wisdom_descriptor_buffer.hpp>

#if defined(WISDOM_VULKAN)
bool wis::ImplVKDescriptorBufferExtension::GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
        std::unordered_set<std::string_view>& ext_name_set,
        std::unordered_map<VkStructureType, uintptr_t>& structure_map,
        std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    if (available_extensions.contains(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME)) {
        ext_name_set.insert(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME);
        structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT] = sizeof(VkPhysicalDeviceDescriptorBufferFeaturesEXT);
        property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceDescriptorBufferPropertiesEXT);
    } else
        return false;

    if (available_extensions.contains(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)) {
        ext_name_set.insert(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME);
        structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_EXT] = sizeof(VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT);
    } else if (available_extensions.contains(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)) {
        ext_name_set.insert(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME);
        structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE] = sizeof(VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE);
    } else
        return false;

    return true;
}

wis::Result
wis::ImplVKDescriptorBufferExtension::Init(const wis::VKDevice& instance,
        const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
        const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    if (!property_map.contains(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT)) {
        return wis::success;
    }

    device = instance.GetInternal().device;
    allocator = instance.GetInternal().allocator; // Get the allocator from the device

    auto& vk_descriptor_buffer_properties = *reinterpret_cast<VkPhysicalDeviceDescriptorBufferPropertiesEXT*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT));

    descriptor_buffer_props.constant_buffer_size = uint16_t(vk_descriptor_buffer_properties.uniformBufferDescriptorSize);
    descriptor_buffer_props.storage_buffer_size = uint16_t(vk_descriptor_buffer_properties.storageBufferDescriptorSize);

    descriptor_buffer_props.sampled_image_size = uint16_t(vk_descriptor_buffer_properties.sampledImageDescriptorSize);
    descriptor_buffer_props.storage_image_size = uint16_t(vk_descriptor_buffer_properties.storageImageDescriptorSize);

    descriptor_buffer_props.storage_texel_buffer_size = uint16_t(vk_descriptor_buffer_properties.storageTexelBufferDescriptorSize);
    descriptor_buffer_props.uniform_texel_buffer_size = uint16_t(vk_descriptor_buffer_properties.uniformTexelBufferDescriptorSize);

    auto max1 = std::max(vk_descriptor_buffer_properties.uniformBufferDescriptorSize, vk_descriptor_buffer_properties.storageBufferDescriptorSize);
    auto max2 = std::max(vk_descriptor_buffer_properties.sampledImageDescriptorSize, vk_descriptor_buffer_properties.storageImageDescriptorSize);
    auto max3 = std::max(vk_descriptor_buffer_properties.storageTexelBufferDescriptorSize, vk_descriptor_buffer_properties.uniformTexelBufferDescriptorSize);

    auto max12 = std::max(max1, max2);

    descriptor_buffer_props.mutable_descriptor_size = uint16_t(std::max(max12, max3));
    descriptor_buffer_props.sampler_size = uint16_t(vk_descriptor_buffer_properties.samplerDescriptorSize);
    descriptor_buffer_props.offset_alignment = uint16_t(vk_descriptor_buffer_properties.descriptorBufferOffsetAlignment);

    ftable.Init(device.get(), device.gtable().vkGetDeviceProcAddr);
    return wis::success;
}

wis::VKRootSignature
wis::ImplVKDescriptorBufferExtension::CreateRootSignature(wis::Result& result, const PushConstant* constants,
        uint32_t constants_size,
        const PushDescriptor* push_descriptors,
        uint32_t push_descriptors_size,
        const wis::DescriptorTable* tables,
        uint32_t tables_count) const noexcept
{
    VKRootSignature out_signature;
    auto& internal = out_signature.GetMutableInternal();

    if (constants_size > wis::max_push_constants) {
        result = wis::make_result<FUNC, "constants_size exceeds max_push_constants">(VkResult::VK_ERROR_UNKNOWN);
        return out_signature;
    }
    if (push_descriptors_size > wis::max_push_descriptors) {
        result = wis::make_result<FUNC, "push_descriptors_size exceeds max_push_descriptors">(VkResult::VK_ERROR_UNKNOWN);
        return out_signature;
    }

    uint32_t table_offset = push_descriptors_size > 0;
    if (tables_count > 0 || push_descriptors_size > 0) {
        if (internal.vk_dsls = wis::detail::make_unique_for_overwrite<VkDescriptorSetLayout[]>(tables_count + table_offset); !internal.vk_dsls) {
            result = wis::make_result<FUNC, "Failed to allocate descriptor set layout array">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
            return out_signature;
        }
    }


    // Create push descriptor set layout
    if (table_offset)
    {
        uint32_t push_binding_count = constants_size;
        VkDescriptorSetLayoutBinding push_bindings[wis::max_push_descriptors] {};
        for (uint32_t i = 0; i < push_binding_count; i++) {
            auto& r = push_descriptors[i];
            auto& b = push_bindings[i];
            b.binding = 0; // Push descriptors always have binding 0
            b.descriptorType = convert_vk(r.type);
            b.descriptorCount = 1; // Push descriptors are always single
            b.stageFlags = convert_vk(r.stage);
        }
        VkDescriptorSetLayoutCreateInfo push_desc_info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR | VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
            .bindingCount = push_binding_count,
            .pBindings = push_bindings,
        };
        auto res = device.table().vkCreateDescriptorSetLayout(device.get(), &push_desc_info, nullptr, &internal.vk_dsls[0]);
        if (!succeeded(res)) {
            result = wis::make_result<FUNC, "Failed to create a push descriptor set layout">(res);
            return out_signature;
        }
    }

    VkPushConstantRange push_constants[wis::max_push_constants] {}; // max push constants
    for (uint32_t i = 0; i < constants_size; i++) {
        auto& c = push_constants[i];
        auto& r = constants[i];
        c.stageFlags = convert_vk(r.stage);
        c.offset = 0;
        c.size = r.size_bytes;
    }

    for (size_t i = 0; i < tables_count; i++) {
        auto h = CreateDescriptorSetLayout(result, &tables[i]);
        if (result.status != wis::Status::Ok) {
            for (size_t j = 0; j < i + table_offset; j++) {
                device.table().vkDestroyDescriptorSetLayout(device.get(), internal.vk_dsls[j], nullptr);
            }

            return out_signature;
        }
        internal.vk_dsls[i + table_offset] = h;
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = tables_count + table_offset, // push descriptor set layout
        .pSetLayouts = internal.vk_dsls.get(),
        .pushConstantRangeCount = constants_size,
        .pPushConstantRanges = push_constants,
    };
    auto vr = device.table().vkCreatePipelineLayout(device.get(), &pipeline_layout_info, nullptr, internal.root.put(device, device.table().vkDestroyPipelineLayout));
    if (!succeeded(vr)) {
        for (uint32_t i = 0; i < tables_count; i++) {
            device.table().vkDestroyDescriptorSetLayout(device.get(), internal.vk_dsls[i], nullptr);
        }
        result = wis::make_result<FUNC, "Failed to create a pipeline layout">(vr);
        return out_signature;
    }
    internal.dsl_count = tables_count + table_offset; // number of descriptor set layouts to destroy
    return out_signature;
}

wis::VKDescriptorBuffer
wis::ImplVKDescriptorBufferExtension::CreateDescriptorBuffer(wis::Result& result, wis::DescriptorHeapType heap_type,
        wis::DescriptorMemory memory_type,
        uint64_t memory_bytes) const noexcept
{
    VKDescriptorBuffer out_buffer;
    auto& internal = out_buffer.GetMutableInternal();

    uint32_t descriptor_size = heap_type == wis::DescriptorHeapType::Descriptor
                               ? descriptor_buffer_props.mutable_descriptor_size
                               : descriptor_buffer_props.sampler_size;

    VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
                               (heap_type == wis::DescriptorHeapType::Descriptor
                                ? VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT
                                : VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT);

    VkBufferCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = memory_bytes,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
    };

    VmaAllocationCreateInfo alloc_info{
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .preferredFlags = 0,
    };
    auto vr = vmaCreateBuffer(allocator.get(), &info, &alloc_info, &internal.buffer, &internal.allocation, nullptr);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a descriptor heap buffer">(vr);
        return out_buffer;
    }
    internal.allocator = allocator;
    internal.type = heap_type;
    internal.properties = descriptor_buffer_props;
    internal.vkGetDescriptorEXT = ftable.vkGetDescriptorEXT;

    auto& device = this->allocator.header();
    VkBufferDeviceAddressInfo address_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = internal.buffer
    };
    internal.address = device.table().vkGetBufferDeviceAddress(device.get(), &address_info);
    vmaMapMemory(this->allocator.get(), internal.allocation, reinterpret_cast<void**>(&internal.data));
    internal.descriptor_size = internal.type == wis::DescriptorHeapType::Descriptor
                               ? descriptor_buffer_props.mutable_descriptor_size
                               : descriptor_buffer_props.sampler_size;

    return out_buffer;
}
void wis::ImplVKDescriptorBufferExtension::SetDescriptorBuffers(wis::VKCommandListView cmd_list,
        wis::VKDescriptorBufferView resource_desc_buffer,
        wis::VKDescriptorBufferView sampler_desc_buffer) const noexcept
{
    auto b1 = std::get<0>(resource_desc_buffer);
    auto b2 = std::get<0>(sampler_desc_buffer);

    VkDescriptorBufferBindingInfoEXT infos[2] = {
        {   .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT,
            .address = b1 ? b1 : b2,
            .usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT
        },

        {   .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT,
            .address = b2,
            .usage = VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT
        }
    };

    uint32_t buffer_count = (b1 != 0) + (b2 != 0);
    ftable.vkCmdBindDescriptorBuffersEXT(std::get<0>(cmd_list), buffer_count, infos);
}

void wis::ImplVKDescriptorBufferExtension::SetDescriptorTableOffset(wis::VKCommandListView cmd_list,
        wis::VKRootSignatureView root_signature,
        uint32_t root_table_index,
        wis::VKDescriptorBufferGPUView buffer,
        uint32_t table_aligned_byte_offset) const noexcept
{
    auto binding = std::get<1>(buffer);
    uint32_t index = uint32_t(binding == wis::DescriptorHeapType::Sampler);
    VkDeviceSize offset = table_aligned_byte_offset;

    ftable.vkCmdSetDescriptorBufferOffsetsEXT(std::get<0>(cmd_list),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            std::get<0>(root_signature),
            root_table_index,
            1,
            &index,
            &offset);
}

// Auxiliary functions

VkDescriptorSetLayout
wis::ImplVKDescriptorBufferExtension::VKCreateDescriptorSetDescriptorLayout(wis::Result& result, const wis::DescriptorTable* table) const noexcept
{
    constexpr static VkDescriptorType cbvSrvUavTypes[] = {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR /* Need to check support if this is desired. */
    };

    constexpr static VkMutableDescriptorTypeListVALVE a{
        .descriptorTypeCount = sizeof(cbvSrvUavTypes) / sizeof(VkDescriptorType),
        .pDescriptorTypes = cbvSrvUavTypes
    };

    wis::detail::limited_allocator<VkDescriptorSetLayoutBinding, 32> bindings{ table->entry_count, true };
    wis::detail::limited_allocator<VkDescriptorBindingFlags, 32> binding_flags{ table->entry_count, true };
    wis::detail::limited_allocator<VkMutableDescriptorTypeListVALVE, 32> bindings_mutable{ table->entry_count };

    for (size_t i = 0; i < table->entry_count; i++)
        bindings_mutable.data()[i] = a;

    VkDescriptorSetLayoutBindingFlagsCreateInfoEXT binding_flags_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT,
        .pNext = nullptr,
        .bindingCount = table->entry_count,
        .pBindingFlags = binding_flags.data(),
    };

    for (size_t i = 0; i < table->entry_count; i++) {
        auto& entry = table->entries[i];
        bindings.data()[i] = {
            .binding = entry.binding,
            .descriptorType = VK_DESCRIPTOR_TYPE_MUTABLE_EXT,
            .descriptorCount = entry.count,
            .stageFlags = uint32_t(convert_vk(table->stage)),
            .pImmutableSamplers = nullptr,
        };
        if (entry.count == UINT32_MAX) {
            binding_flags.data()[i] = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT;
            bindings.data()[i].descriptorCount = 4096; // TODO: DO NOT HARDCODE
        }
    }
    VkMutableDescriptorTypeCreateInfoEXT mutableTypeInfo{
        .sType = VK_STRUCTURE_TYPE_MUTABLE_DESCRIPTOR_TYPE_CREATE_INFO_EXT,
        .pNext = &binding_flags_info,
        .mutableDescriptorTypeListCount = table->entry_count,
        .pMutableDescriptorTypeLists = bindings_mutable.data(),
    };

    VkDescriptorSetLayoutCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &mutableTypeInfo,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
        .bindingCount = table->entry_count,
        .pBindings = bindings.data(),
    };

    VkDescriptorSetLayout layout;
    auto vr = device.table().vkCreateDescriptorSetLayout(device.get(), &desc, nullptr, &layout);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a descriptor set layout">(vr);
    }
    return layout;
}

VkDescriptorSetLayout
wis::ImplVKDescriptorBufferExtension::VKCreateDescriptorSetSamplerLayout(wis::Result& result, const wis::DescriptorTable* table) const noexcept
{
    wis::detail::limited_allocator<VkDescriptorSetLayoutBinding, 32> bindings{ table->entry_count, true };

    for (size_t i = 0; i < table->entry_count; i++) {
        auto& entry = table->entries[i];
        bindings.data()[i] = {
            .binding = entry.binding,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .descriptorCount = entry.count,
            .stageFlags = uint32_t(convert_vk(table->stage)),
            .pImmutableSamplers = nullptr,
        };
    }

    VkDescriptorSetLayoutCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
        .bindingCount = table->entry_count,
        .pBindings = bindings.data(),
    };

    VkDescriptorSetLayout layout;
    auto vr = device.table().vkCreateDescriptorSetLayout(device.get(), &desc, nullptr, &layout);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a descriptor set layout">(vr);
    }
    return layout;
}

#endif // WISDOM_VULKAN
#endif // WISDOM_EXTENDED_ALLOCATION_CPP

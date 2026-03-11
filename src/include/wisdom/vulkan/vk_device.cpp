#ifndef WIS_VK_DEVICE_CPP
#define WIS_VK_DEVICE_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>
#include <bit>

namespace wis::detail {
struct VKMappingOffsetInfo {
    uint32_t offset : 31 = 0x7FFFFFF;
    uint32_t even   : 1  = 1; // After even stages there needs to be "all" maps, after odd stages there doesn't. This is a clever hack to avoid overcounting "all" maps in the total count calculation.
};
constexpr static VkSpirvResourceTypeFlagsEXT srv_mask = VK_SPIRV_RESOURCE_TYPE_SAMPLED_IMAGE_BIT_EXT |
        VK_SPIRV_RESOURCE_TYPE_READ_ONLY_IMAGE_BIT_EXT |
        VK_SPIRV_RESOURCE_TYPE_READ_ONLY_STORAGE_BUFFER_BIT_EXT |
        VK_SPIRV_RESOURCE_TYPE_ACCELERATION_STRUCTURE_BIT_EXT;
constexpr static VkSpirvResourceTypeFlagsEXT sampler_mask = VK_SPIRV_RESOURCE_TYPE_SAMPLER_BIT_EXT;
constexpr static VkSpirvResourceTypeFlagsEXT uav_mask     = VK_SPIRV_RESOURCE_TYPE_READ_WRITE_IMAGE_BIT_EXT |
        VK_SPIRV_RESOURCE_TYPE_READ_WRITE_STORAGE_BUFFER_BIT_EXT;
constexpr static VkSpirvResourceTypeFlagsEXT cbv_mask = VK_SPIRV_RESOURCE_TYPE_UNIFORM_BUFFER_BIT_EXT;

constexpr VkSpirvResourceTypeFlagsEXT GetResourceTypeFlags(const WisDescriptorType type) noexcept
{
    switch (type) {
    case WisDescriptorTypeSampler:
        return sampler_mask;
    case WisDescriptorTypeConstantBuffer:
        return cbv_mask;
    case WisDescriptorTypeTexture:
        return srv_mask;
    case WisDescriptorTypeRWTexture:
        return uav_mask;
    case WisDescriptorTypeRWBuffer:
        return uav_mask;
    case WisDescriptorTypeBuffer:
        return srv_mask;
    case WisDescriptorTypeAccelerationStructure:
        return uav_mask;
    default:
        return 0;
    }
}

inline std::array<uint32_t, WisShaderVisibilityCount>
GetMapCountPerShaderType(const WisRootSignatureDesc& desc) noexcept
{
    std::array<uint32_t, WisShaderVisibilityCount> counts{};
    // 1. Push constants
    for (std::size_t i = 0; i < desc.push_constant_count; ++i) {
        const auto& push_constant = desc.push_constants[i];
        counts[push_constant.visibility]++; // Convert size in bytes to number of 32-bit constants
    }

    // 2. Push descriptors
    for (std::size_t i = 0; i < desc.push_descriptor_count; ++i) {
        const auto& push_descriptor = desc.push_descriptors[i];
        counts[push_descriptor.visibility]++;
    }

    // 3. Descriptor tables
    for (std::size_t i = 0; i < desc.descriptor_table_count; ++i) {
        const auto& table = desc.descriptor_tables[i];
        counts[table.visibility] += table.entry_count;
    }

    return counts;
}

inline std::array<VKMappingOffsetInfo, WisShaderVisibilityCount>
GetMappingOffsetPerShaderType(wis::span<uint32_t, WisShaderVisibilityCount> map_count, uint32_t& total_count) noexcept
{
    total_count = 0;
    std::array<VKMappingOffsetInfo, WisShaderVisibilityCount> offsets;

    // We have to be smart about this number.
    // The 0th index of the map count array corresponds to the "all" visibility.
    // But blindly multiplying will overcount.
    // There is better approach - place the "all" maps in between 2 shader stages.
    // This way there will be 2 times less "all" maps, and we won't have to worry about overcounting.
    uint32_t all_count             = map_count[0];
    uint32_t non_empty_stage_count = 0;
    for (uint32_t i = 1; i < map_count.size(); ++i) {
        if (map_count[i] == 0) {
            continue;
        }

        offsets[i] = {
            total_count,
            (non_empty_stage_count & 1) == 0
        }; // Store odd/even stage information in the highest bit of the offset

        if ((non_empty_stage_count & 1) == 0) {
            total_count += all_count; // Place "all" maps in between stages
        }

        total_count += map_count[i];
        non_empty_stage_count++;
    }
    return offsets;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyDevice(WisVKDevice* self)
{
    auto& impl = *reinterpret_cast<wis::impl::VKDeviceImpl*>(self);
    if (!impl.device) {
        return;
    }
    wis::detail::release_vk_device(impl.device_header);
    impl.device_header = nullptr;
    impl.device        = VK_NULL_HANDLE;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandQueue(const WisVKDevice*  self,
                                                                WisCommandQueueType type,
                                                                WisVKCommandQueue*  queue)
{
    WisResult res      = wis::detail::vk_success;
    auto&     device   = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    VkQueue   vk_queue = VK_NULL_HANDLE;

    // Sanity check: lower and upper bound
    using QueueTypeUnderlying = std::underlying_type_t<WisCommandQueueType>;
    if (static_cast<QueueTypeUnderlying>(type) < 0 ||
        static_cast<size_t>(type) >= WisCommandQueueTypeCount) {
        return wis::detail::make_result<wis::detail::Func(), "Invalid command queue type specified">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get queue family index based on type
    uint8_t queue_family_index = device.device_header->header.queue_residency[static_cast<size_t>(type)];
    if (queue_family_index == wis::detail::VKQueueFamilyProperties::invalid_family_index) {
        return wis::detail::make_result<wis::detail::Func(), "No suitable queue family found for the requested queue type">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    auto& queue_family = device.device_header->header.queue_families[queue_family_index];

    VkDeviceQueueInfo2 queue_info{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext            = nullptr,
        .flags            = 0,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family.family_index),
        .queueIndex       = queue_family.GetNextQueueIndex(),
    };
    device.device_header->header.device_table.vkGetDeviceQueue2(device.device, &queue_info, &vk_queue);

    // Fill command queue impl
    auto& queue_impl         = *new (queue) wis::impl::VKCommandQueueImpl();
    queue_impl.queue         = vk_queue;
    queue_impl.device        = device.device;
    queue_impl.device_header = device.device_header;
    queue_impl.device_header->AddRef(); // hold reference to device header

    // Setup semaphore pointer
    queue_impl.semaphore_ptr = reinterpret_cast<uint8_t*>(device.device_header->header.GetSemaphoreForQueueType(type, queue_info.queueIndex));
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandAllocator(const WisVKDevice*     self,
                                                                    WisCommandQueueType    type,
                                                                    WisVKCommandAllocator* allocator)
{
    auto& device = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    auto& table  = device.device_header->header.device_table;

    // Sanity check: lower and upper bound
    using QueueTypeUnderlying = std::underlying_type_t<WisCommandQueueType>;
    if (static_cast<QueueTypeUnderlying>(type) < 0 ||
        static_cast<size_t>(type) >= WisCommandQueueTypeCount) {
        return wis::detail::make_result<wis::detail::Func(), "Invalid command queue type specified">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get queue family index based on type
    uint8_t queue_family_index = device.device_header->header.queue_residency[static_cast<size_t>(type)];
    if (queue_family_index == wis::detail::VKQueueFamilyProperties::invalid_family_index) {
        return wis::detail::make_result<wis::detail::Func(), "No suitable queue family found for the requested queue type">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    std::unique_ptr<wis::detail::VKCommandPoolControlBlock> pool_control_block = wis::make_unique<wis::detail::VKCommandPoolControlBlock>();
    if (!pool_control_block) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for command pool control block">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    uint8_t queue_family = device.device_header->header.queue_families[queue_family_index].family_index;

    // Create command pool
    VkCommandPoolCreateInfo pool_info{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext            = nullptr,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family),
    };
    VkCommandPool command_pool = VK_NULL_HANDLE;
    VkResult      vr           = table.vkCreateCommandPool(device.device, &pool_info, nullptr, &command_pool);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Vulkan command pool">(vr);
    }

    pool_control_block->header.device        = device.device;
    pool_control_block->header.device_header = device.device_header;
    pool_control_block->header.device_header->AddRef(); // hold reference to device header for command pool control block

    auto& allocator_impl               = *new (allocator) wis::impl::VKCommandAllocatorImpl();
    allocator_impl.command_pool        = command_pool;
    allocator_impl.command_pool_header = pool_control_block.release();

    return wis::detail::vk_success;
}

WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateFence(const WisVKDevice* self,
                                                         uint64_t           initial_value,
                                                         WisVKFence*        fence)
{
    WisResult res    = wis::detail::vk_success;
    auto&     device = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);

    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext         = nullptr,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue  = initial_value,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
        .flags = 0,
    };
    VkSemaphore semaphore = VK_NULL_HANDLE;
    auto&       table     = device.device_header->header.device_table;
    VkResult    vr        = table.vkCreateSemaphore(device.device, &desc, nullptr, &semaphore);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Vulkan timeline semaphore">(vr);
    }
    // Fill fence impl
    auto& out_fence         = *new (fence) wis::impl::VKFenceImpl();
    out_fence.fence         = semaphore;
    out_fence.device        = device.device;
    out_fence.device_header = device.device_header;
    out_fence.device_header->AddRef(); // hold reference to device header
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceGetResourceAllocator(const WisVKDevice*      self,
                                                                  WisVKResourceAllocator* allocator)
{
    auto& device = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);

    // Fill allocator impl
    auto& allocator_impl         = *new (allocator) wis::impl::VKResourceAllocatorImpl();
    allocator_impl.allocator     = device.device_header->header.allocator;
    allocator_impl.device_header = device.device_header;
    allocator_impl.device_header->AddRef(); // hold reference to device header
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateDescriptorHeap(const WisVKDevice*           self,
                                                                  const WisDescriptorHeapDesc* desc,
                                                                  WisVKDescriptorHeap*         heap)
{
    auto& device   = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    auto& header   = device.device_header->header;
    auto& features = header.features;
    auto& table    = header.device_table;

    // 0. If heap is supported
    if (!features.descriptor_heap) {
        return wis::detail::make_result<wis::detail::Func(), "Descriptor heaps are not supported by this Vulkan device">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    // 1. Calculate descriptor memory requirements based on desc
    bool is_shader_heap    = desc->memory_type == WisDescriptorMemoryTypeShaderVisible;
    bool is_sampler_heap   = desc->type == WisDescriptorHeapTypeSampler;
    bool embedded_samplers = !(desc->flags & WisDescriptorHeapFlagsDisallowEmbeddedSamplers);

    std::size_t heap_alignment = is_shader_heap ? is_sampler_heap
                    ? features.sampler_heap_alignment
                    : features.descriptor_heap_alignment
                                                : __STDCPP_DEFAULT_NEW_ALIGNMENT__;

    std::size_t descriptor_size = is_sampler_heap
            ? features.sampler_desc_size
            : features.resource_desc_size;

    std::size_t reserved_size = is_shader_heap ? is_sampler_heap
                    ? embedded_samplers ? features.sampler_heap_reserved_size_with_embedded
                                        : features.sampler_heap_reserved_size
                    : features.descriptor_heap_reserved_size
                                               : 0;

    std::size_t max_heap_size = is_shader_heap ? is_sampler_heap
                    ? features.max_sampler_heap_size
                    : features.max_descriptor_heap_size
                                               : std::numeric_limits<std::size_t>::max();

    std::size_t required_size = wis::aligned_size(
            desc->descriptor_count * descriptor_size + reserved_size,
            heap_alignment);

    if (is_shader_heap && required_size > max_heap_size) {
        return wis::detail::make_result<wis::detail::Func(), "Requested descriptor heap size exceeds the maximum supported by this Vulkan device">(VK_ERROR_INITIALIZATION_FAILED);
    }

    if (!is_shader_heap) {
        // 2a. For non-shader visible heaps, we can use a simple host allocation
        VkBuffer buffer = reinterpret_cast<VkBuffer>(std::malloc(required_size));
        if (!buffer) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for non-shader visible descriptor heap">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }

        // Fill descriptor heap impl
        auto& heap_impl           = *new (heap) wis::impl::VKDescriptorHeapImpl();
        heap_impl.buffer          = buffer;
        heap_impl.allocation      = VK_NULL_HANDLE; // No VMA allocation for non-shader visible heaps
        heap_impl.mapped_ptr      = buffer; // For non-shader visible heaps, the buffer pointer itself serves as the mapped pointer
        heap_impl.gpu_address     = 0; // No GPU address for non-shader visible heaps
        heap_impl.reserved_size   = 0;
        heap_impl.descriptor_size = descriptor_size;
        heap_impl.heap_size       = desc->descriptor_count;
        heap_impl.device          = device.device;
        heap_impl.device_header   = device.device_header;
        heap_impl.device_header->AddRef(); // hold reference to device header
        return wis::detail::vk_success;
    }

    // 2. Create buffer
    VkBufferCreateInfo buffer_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size  = required_size,
        .usage = VK_BUFFER_USAGE_DESCRIPTOR_HEAP_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
    };
    VmaAllocationCreateInfo alloc_info{
        .flags          = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage          = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
        .requiredFlags  = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        .preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    VkBuffer          buffer     = VK_NULL_HANDLE;
    VmaAllocation     allocation = VK_NULL_HANDLE;
    VmaAllocationInfo alloc_info_out{};
    VkResult          vr = vmaCreateBufferWithAlignment(
            header.allocator,
            &buffer_info,
            &alloc_info,
            heap_alignment,
            &buffer,
            &allocation,
            &alloc_info_out);

    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create buffer for shader visible descriptor heap">(vr);
    }

    // Get GPU address of the buffer
    VkBufferDeviceAddressInfo address_info{
        .sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext  = nullptr,
        .buffer = buffer
    };

    auto& heap_impl           = *new (heap) wis::impl::VKDescriptorHeapImpl();
    heap_impl.buffer          = buffer;
    heap_impl.allocation      = allocation;
    heap_impl.mapped_ptr      = alloc_info_out.pMappedData;
    heap_impl.gpu_address     = table.vkGetBufferDeviceAddress(device.device, &address_info);
    heap_impl.reserved_size   = reserved_size / descriptor_size;
    heap_impl.descriptor_size = descriptor_size;
    heap_impl.heap_size       = desc->descriptor_count;
    heap_impl.device          = device.device;
    heap_impl.device_header   = device.device_header;
    heap_impl.device_header->AddRef(); // hold reference to device header
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateRootSignature(const WisVKDevice*          self,
                                                                 const WisRootSignatureDesc* desc,
                                                                 WisVKRootSignature*         layout)
{
    auto& device   = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    auto& header   = device.device_header->header;
    auto& features = header.features;

    if (!features.descriptor_heap) {
        return wis::detail::make_result<wis::detail::Func(), "Descriptor heaps are not supported by this Vulkan device">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    // Use only 64 DWORDs, same as DX12
    static constexpr std::size_t max_root_parameters = 64;
    std::size_t                  push_constant_size  = 0;
    for (std::size_t i = 0; i < desc->push_constant_count; ++i) {
        const auto& push_constant = desc->push_constants[i];
        if (push_constant.size_bytes % 4 != 0) {
            return wis::detail::make_result<wis::detail::Func(), "Push constant size must be divisible by 4 bytes">(VK_ERROR_INITIALIZATION_FAILED);
        }
        push_constant_size += push_constant.size_bytes;
    }
    push_constant_size /= 4;

    // 1. Count the number of root parameters needed
    std::size_t total_dwords_needed = push_constant_size + desc->push_descriptor_count * 2 + desc->descriptor_table_count;

    if (total_dwords_needed > max_root_parameters) {
        return wis::detail::make_result<wis::detail::Func(), "Root signature requires more than 64 DWORDs, which is not supported by this implementation">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // 2. Count the number of VkDescriptorSetAndBindingMappingEXT structures
    // Hard part is to pack the tables into a contiguous arrays for each shader type
    uint32_t                                                               total_table_count        = 0;
    std::array<uint32_t, WisShaderVisibilityCount>                         table_counts_per_shader  = wis::detail::GetMapCountPerShaderType(*desc);
    std::array<wis::detail::VKMappingOffsetInfo, WisShaderVisibilityCount> local_offsets_per_shader = wis::detail::GetMappingOffsetPerShaderType(table_counts_per_shader, total_table_count);

    std::size_t root_param_count     = desc->push_constant_count + desc->push_descriptor_count + desc->descriptor_table_count;
    std::size_t static_sampler_count = 0;

    // allocate root signature table
    std::size_t root_sig_size = sizeof(wis::detail::VKRootSignatureControlBlock) +
            wis::aligned_size(root_param_count, 2u) * sizeof(uint32_t) + // Root parameter binding indices, aligned to 8 bytes
            total_table_count * sizeof(VkDescriptorSetAndBindingMappingEXT);

    std::unique_ptr<wis::detail::VKRootSignatureControlBlock> root_sig_control_block{
        reinterpret_cast<wis::detail::VKRootSignatureControlBlock*>(operator new(root_sig_size, std::nothrow))
    };

    // start lifetime
    std::construct_at(root_sig_control_block.get());
    root_sig_control_block->constant_data_size     = static_cast<uint32_t>(total_dwords_needed);
    root_sig_control_block->mapping_count          = static_cast<uint32_t>(total_table_count);
    root_sig_control_block->embedded_sampler_count = static_cast<uint32_t>(static_sampler_count);
    root_sig_control_block->root_parameter_count   = static_cast<uint32_t>(root_param_count);

    // Fill mapping data
    uint32_t all_offset = 0;
    for (uint32_t i = 1; i < local_offsets_per_shader.size(); ++i) {
        if (table_counts_per_shader[i] == 0) {
            continue;
        }

        root_sig_control_block->shader_mapping_offset[i] = local_offsets_per_shader[i].offset -
                (local_offsets_per_shader[i].even
                         ? 0
                         : table_counts_per_shader[0]); // If even, "all" maps are after this stage, if odd, "all" maps are before this stage
        if (!all_offset) {
            // Set as an offset after mapping[0]
            all_offset = local_offsets_per_shader[i].offset + table_counts_per_shader[i];
        }
    }
    root_sig_control_block->shader_mapping_offset[0] = all_offset;
    local_offsets_per_shader[0]                      = { all_offset, true };

    auto     mappings            = root_sig_control_block->GetMappings();
    auto     root_param_offsets  = root_sig_control_block->GetRootBindingOffsets();
    uint32_t push_address_offset = 0;
    uint32_t root_param_index    = 0;

    // Push constants
    for (std::size_t i = 0; i < desc->push_constant_count; ++i) {
        auto& src = desc->push_constants[i];

        mappings[local_offsets_per_shader[src.visibility].offset++] = {
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_AND_BINDING_MAPPING_EXT,
            .pNext         = nullptr,
            .descriptorSet = src.bind_space,
            .firstBinding  = src.bind_register,
            .bindingCount  = 1,
            .resourceMask  = wis::detail::cbv_mask,
            .source        = VK_DESCRIPTOR_MAPPING_SOURCE_PUSH_DATA_EXT,
            .sourceData    = { .pushAddressOffset = push_address_offset }
        };

        root_param_offsets[root_param_index++] = push_address_offset;
        push_address_offset += src.size_bytes;
    }

    // Push descriptors
    for (std::size_t i = 0; i < desc->push_descriptor_count; ++i) {
        WisPushDescriptor src = desc->push_descriptors[i];

        auto& m = mappings[local_offsets_per_shader[src.visibility].offset++] = {
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_AND_BINDING_MAPPING_EXT,
            .pNext         = nullptr,
            .descriptorSet = src.bind_space,
            .firstBinding  = src.bind_register,
            .bindingCount  = 1,
            .resourceMask  = wis::detail::GetResourceTypeFlags(src.type), // Push descriptors can be any type
            .source        = VK_DESCRIPTOR_MAPPING_SOURCE_PUSH_ADDRESS_EXT,
            .sourceData    = { .pushAddressOffset = static_cast<uint32_t>(push_address_offset + i * 2 * sizeof(uint32_t)) }
        };
        root_param_offsets[root_param_index++] = m.sourceData.pushAddressOffset;
    }
    push_address_offset += desc->push_descriptor_count * 2 * sizeof(uint32_t);

    // Descriptor tables
    for (std::size_t i = 0; i < desc->descriptor_table_count; ++i) {
        WisDescriptorTable src              = desc->descriptor_tables[i];
        auto               visibility       = src.visibility;
        uint32_t           heap_byte_offset = 0;

        for (std::size_t j = 0; j < src.entry_count; ++j) {
            auto& entry = src.entries[j];

            uint32_t local_count  = entry.count;
            uint32_t local_offset = heap_byte_offset;
            uint32_t heap_stride  = entry.type == WisDescriptorTypeSampler
                     ? features.sampler_desc_size
                     : features.resource_desc_size;

            // Check for unbounded array
            if (entry.count == std::numeric_limits<uint32_t>::max()) {
                if (j != src.entry_count - 1) {
                    return wis::detail::make_result<wis::detail::Func(), "Unbounded array descriptor table entry must be the last entry in the table">(VK_ERROR_INITIALIZATION_FAILED);
                }
                local_count = 1;
            }

            if (entry.descriptor_offset != std::numeric_limits<uint32_t>::max()) {
                // not offset appended, use provided offset
                local_offset = entry.descriptor_offset * heap_stride;
            }
            heap_byte_offset = local_offset + local_count * heap_stride;

            auto& mapping = mappings[local_offsets_per_shader[visibility].offset++] = {
                .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_AND_BINDING_MAPPING_EXT,
                .pNext         = nullptr,
                .descriptorSet = entry.bind_space,
                .firstBinding  = entry.bind_register,
                .bindingCount  = local_count,
                .resourceMask  = wis::detail::GetResourceTypeFlags(entry.type),
                .source        = VK_DESCRIPTOR_MAPPING_SOURCE_HEAP_WITH_PUSH_INDEX_EXT,
                .sourceData    = { .pushIndex = {
                                           .heapOffset      = local_offset,
                                           .pushOffset      = push_address_offset,
                                           .heapIndexStride = 1,
                                           .heapArrayStride = heap_stride,
                                } }
            };
        }

        root_param_offsets[root_param_index++] = push_address_offset;
        push_address_offset += sizeof(uint32_t);
    }

    // Fill "all" visibility mappings in between stages
    bool first_skipped = true;
    for (uint32_t i = 1; i < local_offsets_per_shader.size(); ++i) {
        if (local_offsets_per_shader[i].offset != 0x7ffffff && local_offsets_per_shader[i].even) {

            // Skip the first even stage, since this is where the mappings actually are
            if (first_skipped) {
                first_skipped = false;
                continue;
            }

            std::copy_n(mappings.data(), table_counts_per_shader[0], mappings.data() + local_offsets_per_shader[i].offset);
        }
    }

    // Fill root signature impl
    auto& layout_impl                 = *new (layout) wis::impl::VKRootSignatureImpl();
    layout_impl.root_signature_header = root_sig_control_block.release();

    return wis::detail::vk_success;
}

WIS_EXTERN_C WISDOM_API void wisVKDeviceQueryProperties(const WisVKDevice* self,
                                                        void*              properties)
{
    if (!properties) {
        return;
    }

    auto& device = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    auto& header = device.device_header->header;
    void* next   = properties;

    do {
        WisQueryStructHeader header_local{};
        std::memcpy(&header_local, next, sizeof(WisQueryStructHeader));

        switch (header_local.property_type) {
        case WisQueryPropertyTypeDeviceCommandQueueProperties: {
            auto* props = static_cast<WisDeviceCommandQueuesProperties*>(next);
            for (size_t i = 0; i < WisCommandQueueTypeCount; ++i) {
                auto&                   family_index = header.queue_residency[i];
                bool                    supported    = family_index != wis::detail::VKQueueFamilyProperties::invalid_family_index;
                WisCommandQueuePriority priority     = WisCommandQueuePriority(supported ? (header.queue_families[family_index].queue_priority) : 0);

                props->supported_queues[i]   = supported;
                props->max_queue_priority[i] = priority;
            }
            props->relaxed_queue_transition = header.features.maintenance9;
        } break;
        case WisQueryPropertyTypeDeviceDescriptorHeapProperties: {
            auto* props = static_cast<WisDeviceDescriptorHeapProperties*>(next);
            if (!header.features.descriptor_heap) {
                break;
            }

            auto real_dheap_size               = header.features.max_descriptor_heap_size - header.features.descriptor_heap_reserved_size;
            auto real_sheap_size               = header.features.max_sampler_heap_size - header.features.sampler_heap_reserved_size;
            auto real_sheap_size_with_embedded = header.features.max_sampler_heap_size - header.features.sampler_heap_reserved_size_with_embedded;

            props->max_descriptor_heap_size            = real_dheap_size / header.features.resource_desc_size;
            props->max_sampler_heap_size               = real_sheap_size / header.features.sampler_desc_size;
            props->max_sampler_heap_size_with_embedded = real_sheap_size_with_embedded / header.features.sampler_desc_size;
            props->descriptor_increment_size           = header.features.resource_desc_size;
            props->sampler_increment_size              = header.features.sampler_desc_size;
        } break;
        case WisQueryPropertyTypeDeviceMemoryProperties: {
            auto* props                      = static_cast<WisDeviceMemoryProperties*>(next);
            props->host_image_copy_supported = header.features.host_image_copy;

            const VkPhysicalDeviceMemoryProperties* mem_props;
            vmaGetMemoryProperties(header.allocator, &mem_props);

            for (uint32_t i = 0; i < mem_props->memoryTypeCount; ++i) {
                const VkMemoryPropertyFlags flags = mem_props->memoryTypes[i].propertyFlags;
                if ((flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
                    (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) &&
                    (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
                    props->gpu_upload_supported = true;
                    break;
                }
            }
        } break;
        default:
            break;
        }
        next = header_local.next_in_chain;
    } while (next);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceWaitForMultipleFences(const WisVKDevice*    self,
                                                                   const WisVKFenceView* fences,
                                                                   const uint64_t*       fence_values,
                                                                   size_t                fence_count,
                                                                   WisMutiWaitType       wait_for,
                                                                   uint64_t              timeout)
{
    auto&               device = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    VkSemaphoreWaitInfo waitInfo{
        .sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .pNext          = nullptr,
        .flags          = VkSemaphoreWaitFlags(wait_for),
        .semaphoreCount = static_cast<uint32_t>(fence_count),
        .pSemaphores    = reinterpret_cast<const VkSemaphore*>(fences),
        .pValues        = fence_values
    };
    VkResult result = device.device_header->header.device_table.vkWaitSemaphores(device.device, &waitInfo, timeout);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to wait for multiple fences">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreatePipelineCache(const WisVKDevice*  self,
                                                                 const uint8_t*      initial_data,
                                                                 size_t              data_size,
                                                                 WisVKPipelineCache* cache)
{
    if (data_size > 0 && data_size < sizeof(VkPipelineCacheHeaderVersionOne)) {
        return wis::detail::make_result<wis::detail::Func(), "Data size is too small to contain a valid pipeline cache header">(VK_ERROR_INITIALIZATION_FAILED);
    }

    auto& device  = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    auto& table   = device.device_header->header.device_table;
    auto  adapter = device.physical_device;
    auto  atable  = device.device_header->header.shared_header->header.adapter_table;

    // If initial data is provided, use its size. Otherwise, set size to 0 to indicate no initial data.
    data_size    = initial_data ? data_size : 0;
    initial_data = data_size ? initial_data : nullptr;

    // Check data size and initial data pointer consistency
    if (data_size && initial_data) {
        // Compare the pipeline cache UUID with the device's pipeline cache UUID
        VkPhysicalDeviceProperties2 properties{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
        };
        atable.vkGetPhysicalDeviceProperties2(adapter, &properties);
        VkPipelineCacheHeaderVersionOne cache_header_correct{
            .headerSize    = sizeof(VkPipelineCacheHeaderVersionOne),
            .headerVersion = VK_PIPELINE_CACHE_HEADER_VERSION_ONE,
            .vendorID      = properties.properties.vendorID,
            .deviceID      = properties.properties.deviceID,
        };
        std::memcpy(cache_header_correct.pipelineCacheUUID, properties.properties.pipelineCacheUUID, VK_UUID_SIZE);

        if (std::memcmp(initial_data, &cache_header_correct, sizeof(VkPipelineCacheHeaderVersionOne)) != 0) {
            return wis::detail::make_result<wis::detail::Func(), "Initial data pipeline cache header does not match the device's pipeline cache header, indicating it is incompatible">(VK_ERROR_INITIALIZATION_FAILED);
        }
    }

    VkPipelineCacheCreateInfo cache_info{
        .sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .initialDataSize = data_size,
        .pInitialData    = initial_data
    };

    VkPipelineCache cache_handle = VK_NULL_HANDLE;
    auto            vr           = table.vkCreatePipelineCache(device.device, &cache_info, nullptr, &cache_handle);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create pipeline cache">(vr);
    }

    auto& cache_impl = *new (cache) wis::impl::VKPipelineCacheImpl{
        .cache         = cache_handle,
        .device_header = device.device_header
    };
    device.device_header->AddRef();

    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateShader(const WisVKDevice* self,
                                                          const uint8_t*     data,
                                                          size_t             size,
                                                          WisVKShader*       shader)
{
    auto& device = *reinterpret_cast<const wis::impl::VKDeviceImpl*>(self);
    auto& table  = device.device_header->header.device_table;

    VkShaderModuleCreateInfo shader_info{
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = nullptr,
        .flags    = 0,
        .codeSize = size,
        .pCode    = reinterpret_cast<const uint32_t*>(data),
    };
    VkShaderModule shader_handle = VK_NULL_HANDLE;

    auto vr = table.vkCreateShaderModule(device.device, &shader_info, nullptr, &shader_handle);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create shader module">(vr);
    }

    auto& shader_impl = *new (shader) wis::impl::VKShaderImpl{
        .shader_module = shader_handle,
        .device_header = device.device_header
    };
    device.device_header->AddRef();
    return wis::detail::vk_success;
}

#endif // WIS_VK_DEVICE_CPP

#ifndef WIS_VK_ADAPTER_QUERY_CPP
#define WIS_VK_ADAPTER_QUERY_CPP

#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/vulkan/vk_types.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/util/allocation.hpp>
#include <algorithm>
#include <bit>

namespace wis::detail {
struct VKQueueResidencyInfo {
    static constexpr uint32_t invalid_index             = std::numeric_limits<uint32_t>::max();
    static constexpr uint32_t reasonable_queue_families = 32u;

    std::array<VkDeviceQueueCreateInfo, WisCommandQueueTypeCount> data;
    std::array<uint8_t, WisCommandQueueTypeCount>                 residency;
    std::array<uint32_t, WisCommandQueueTypeCount>                transfer_masks{};

    uint32_t queue_type_count = 0;

public:
    constexpr VKQueueResidencyInfo()
    {
        for (size_t i = 0; i < WisCommandQueueTypeCount; ++i) {
            data[i] = {
                .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext            = nullptr,
                .flags            = 0,
                .queueFamilyIndex = invalid_index, // Invalid index
                .queueCount       = 0,
                .pQueuePriorities = nullptr,
            };
            residency[i] = VKQueueFamilyProperties::invalid_family_index;
        }
    }
};

//-----------------------------------------------------------------------------
// For simplicity, we assign the same global priority to all queues.
// In a real implementation, you might want to differentiate based on queue type.
static constexpr VkDeviceQueueGlobalPriorityCreateInfo vk_global_priorities[]{
    {
     .sType          = VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO,
     .pNext          = nullptr,
     .globalPriority = VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_KHR, // Default priority
    },
    {
     .sType          = VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO,
     .pNext          = nullptr,
     .globalPriority = VK_QUEUE_GLOBAL_PRIORITY_HIGH_KHR,
     },
    {
     .sType          = VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO,
     .pNext          = nullptr,
     .globalPriority = VK_QUEUE_GLOBAL_PRIORITY_REALTIME_KHR,
     },
};

//-----------------------------------------------------------------------------
constexpr const VkDeviceQueueGlobalPriorityCreateInfo*
VKGetGlobalPriorityInfo(WisCommandQueuePriority type) noexcept
{
    switch (type) {
    default:
    case WisCommandQueuePriorityNormal:
        return &vk_global_priorities[0];
    case WisCommandQueuePriorityHigh:
        return &vk_global_priorities[1];
    case WisCommandQueuePriorityRealtime:
        return &vk_global_priorities[2];
    }
}

//-----------------------------------------------------------------------------
constexpr const VkDeviceQueueGlobalPriorityCreateInfo*
VKGetGlobalPriorityInfo(VkQueueGlobalPriority type) noexcept
{
    switch (type) {
    default:
    case VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_KHR:
        return &vk_global_priorities[0];
    case VK_QUEUE_GLOBAL_PRIORITY_HIGH_KHR:
        return &vk_global_priorities[1];
    case VK_QUEUE_GLOBAL_PRIORITY_REALTIME_KHR:
        return &vk_global_priorities[2];
    }
}

//-----------------------------------------------------------------------------
constexpr WisCommandQueuePriority
VKConvertGlobalPriority(VkQueueGlobalPriority vk_priority) noexcept
{
    switch (vk_priority) {
    default:
    case VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_KHR:
        return WisCommandQueuePriorityNormal;
    case VK_QUEUE_GLOBAL_PRIORITY_HIGH_KHR:
        return WisCommandQueuePriorityHigh;
    case VK_QUEUE_GLOBAL_PRIORITY_REALTIME_KHR:
        return WisCommandQueuePriorityRealtime;
    }
}

//-----------------------------------------------------------------------------
inline std::array<uint32_t, WisCommandQueueTypeCount>
VKGetSortedQueueFamilies(wis::span<VkQueueFamilyProperties2> props_span) noexcept
{
    static constexpr uint32_t                      invalid_index = VKQueueResidencyInfo::invalid_index;
    std::array<uint32_t, WisCommandQueueTypeCount> qcom{};
    std::ranges::fill(qcom, invalid_index); // Initialize all to invalid index

    for (uint32_t i = 0; i < props_span.size(); ++i) {
        auto&        props = props_span[i];
        VkQueueFlags flags = props.queueFamilyProperties.queueFlags;

        // --- GRAPHICS SELECTION ---
        // Simple: Take the first one (usually Family 0).
        // Optional: Pick the one with the most queues if you want to be fancy.
        if ((flags & VK_QUEUE_GRAPHICS_BIT) && qcom[WisCommandQueueTypeGraphics] == invalid_index) {
            qcom[WisCommandQueueTypeGraphics] = i;
        }

        // --- COMPUTE SELECTION ---
        // Goal: Prefer a dedicated Compute queue (Async Compute) over the Graphics queue.
        if (flags & VK_QUEUE_COMPUTE_BIT) {
            auto current = qcom[WisCommandQueueTypeCompute];

            // Scenario A: We haven't found any compute queue yet. Take this one.
            if (current == invalid_index) {
                qcom[WisCommandQueueTypeCompute] = i;
            }

            // Scenario B: We found a shared G+C queue, but now we found a DISTINCT Compute queue.
            // Overwrite the previous choice! This is how you get Async Compute.
            else if ((props_span[current].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(flags & VK_QUEUE_GRAPHICS_BIT)) {
                qcom[WisCommandQueueTypeCompute] = i;
            }

            // Scenario C: We have found another G+C, but it is different from WisCommandQueueTypeGraphics (probably impossible)
            else if ((props_span[current].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) && qcom[WisCommandQueueTypeGraphics] != i) {
                qcom[WisCommandQueueTypeCompute] = i;
            }
        }

        // --- VIDEO SELECTION ---
        if ((flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) && qcom[WisCommandQueueTypeVideoDecode] == invalid_index) {
            qcom[WisCommandQueueTypeVideoDecode] = i;
        }

        if ((flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) && qcom[WisCommandQueueTypeVideoEncode] == invalid_index) {
            qcom[WisCommandQueueTypeVideoEncode] = i;
        }

        constexpr static VkQueueFlags transfer_safe_mask =
                VK_QUEUE_GRAPHICS_BIT |
                VK_QUEUE_COMPUTE_BIT |
                VK_QUEUE_TRANSFER_BIT |
                VK_QUEUE_SPARSE_BINDING_BIT;

        // --- TRANSFER SELECTION ---
        // Goal: Dedicated Transfer > Compute (Async) > Graphics (Fallback).
        // CRITICAL: Avoid Video/Optical queues.
        if (flags & VK_QUEUE_TRANSFER_BIT) {
            // calculate "pollution" - bits we don't recognize or explicitly dislike
            VkQueueFlags pollution = flags & ~transfer_safe_mask;

            // Filter 1: The "Tainted" Check
            // If 'pollution' is non-zero, this queue has flags (Video, Optical Flow, FutureAI)
            // that we didn't account for. It is likely a specialized engine.
            if (pollution != 0) {
                continue; // HARD REJECT. Better to use the Graphics queue than a weird unknown engine.
            }

            auto current = qcom[WisCommandQueueTypeTransfer];

            if (current == invalid_index) {
                qcom[WisCommandQueueTypeTransfer] = i;
            } else {
                // Calculate "Distinctness Score"
                // 0 = Graphics (Worst for overlap)
                // 1 = Compute (Better)
                // 2 = Dedicated Transfer (Best)

                auto current_flags = props_span[current].queueFamilyProperties.queueFlags;

                int oldScore = 0;
                if (!(current_flags & VK_QUEUE_GRAPHICS_BIT) && !(current_flags & VK_QUEUE_COMPUTE_BIT)) {
                    oldScore = 2;
                } else if (!(current_flags & VK_QUEUE_GRAPHICS_BIT)) {
                    oldScore = 1;
                }

                int newScore = 0;
                if (!(flags & VK_QUEUE_GRAPHICS_BIT) && !(flags & VK_QUEUE_COMPUTE_BIT)) {
                    newScore = 2;
                } else if (!(flags & VK_QUEUE_GRAPHICS_BIT)) {
                    newScore = 1;
                }

                // Upgrade if we found a more specialized queue
                if (newScore > oldScore) {
                    qcom[WisCommandQueueTypeTransfer] = i;
                }
            }
        }
    }
    return qcom;
}

//-----------------------------------------------------------------------------
inline VKQueueResidencyInfo VKGetQueueResidencyInfo(const wis::impl::VKMainAdapter& adapter_table,
                                                    VkPhysicalDevice                adapter,
                                                    const WisVKDeviceRequirements*  requirements,
                                                    const VKDeviceFeatures&         device_features,
                                                    WisResult&                      out_result)
{
    VKQueueResidencyInfo      info{};
    constexpr static uint32_t reasonable_queue_family_count = VKQueueResidencyInfo::reasonable_queue_families;
    if (!requirements) {
        // No requirements provided, return empty info
        return info;
    }

    wis::span<const WisCommandQueueDesc> queue_descs{ requirements->queue_descs, requirements->queue_desc_count };

    if (queue_descs.size() > WisCommandQueueTypeCount) {
        out_result = wis::detail::make_result<wis::detail::Func(), "Too many queue types in requirements">(VK_ERROR_INITIALIZATION_FAILED);
        return info;
    }

    if (queue_descs.empty()) {
        // No queues requested, return empty info
        return info;
    }

    uint32_t queue_family_count = 0;
    adapter_table.vkGetPhysicalDeviceQueueFamilyProperties2(adapter, &queue_family_count, nullptr);
    if (queue_family_count == 0) {
        // No queues available, return empty info
        if (!queue_descs.empty()) {
            out_result = wis::detail::make_result<wis::detail::Func(), "No queue families found for the adapter">(VK_ERROR_INITIALIZATION_FAILED);
        }
        return info;
    }

    // Allocate array for queue family properties, use stack if count is reasonable to avoid heap allocation
    std::array<VkQueueFamilyProperties2, reasonable_queue_family_count>                    props_span;
    std::array<VkQueueFamilyGlobalPriorityProperties, reasonable_queue_family_count>       global_props_span;
    std::array<VkQueueFamilyOwnershipTransferPropertiesKHR, reasonable_queue_family_count> transfer_props_span;

    // Initialize the pNext chain for each queue family property to query global priority support
    for (uint32_t i = 0; i < queue_family_count; ++i) {
        props_span[i] = {
            .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
            .pNext = &global_props_span[i],
        };
        global_props_span[i] = {
            .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_GLOBAL_PRIORITY_PROPERTIES_KHR,
            .pNext = device_features.maintenance9 ? &transfer_props_span[i] : nullptr,
        };
        transfer_props_span[i] = {
            .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_OWNERSHIP_TRANSFER_PROPERTIES_KHR,
            .pNext = nullptr,
        };
    }

    if (props_span.data() == nullptr) {
        out_result = wis::detail::make_result<wis::detail::Func(), "Not enough memory for device queue family properties array">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }
    adapter_table.vkGetPhysicalDeviceQueueFamilyProperties2(adapter, &queue_family_count, props_span.data());

    // Sort all families
    uint32_t allocated_queue_count = 0;
    auto     selection             = VKGetSortedQueueFamilies(props_span);

    static constexpr std::array<float, 32> queue_priorities = []() {
        std::array<float, 32> pQueuePriorities;
        std::fill_n(pQueuePriorities.data(), pQueuePriorities.size(), 1.0f);
        return pQueuePriorities;
    }();

    for (std::size_t i = 0; i < queue_descs.size(); ++i) {
        auto& desc = queue_descs[i];
        if (desc.type >= WisCommandQueueTypeCount) {
            out_result = wis::detail::make_result<wis::detail::Func(), "Invalid command queue type in requirements">(VK_ERROR_INITIALIZATION_FAILED);
            return info;
        }

        if (info.residency[desc.type] != VKQueueFamilyProperties::invalid_family_index) {
            continue;
        }

        // Get the family properties for the selected queue family index
        auto& family_props = props_span[selection[desc.type]].queueFamilyProperties;
        auto& global_props = global_props_span[selection[desc.type]];

        const VkDeviceQueueGlobalPriorityCreateInfo* priority_next = nullptr;
        if (device_features.global_priority && desc.priority > WisCommandQueuePriorityNormal) {
            auto* global_priority_info = VKGetGlobalPriorityInfo(desc.priority);
            // Clamp the requested priority to the maximum supported by this family
            for (uint32_t p = global_props.priorityCount; p > 0; --p) {
                if (global_props.priorities[p - 1] <= global_priority_info->globalPriority) {
                    priority_next = VKGetGlobalPriorityInfo(global_props.priorities[p - 1]);
                    break;
                }
            }
        }

        info.transfer_masks[desc.type] = transfer_props_span[selection[desc.type]].optimalImageTransferToQueueFamilies;

        // If the queue count is zero, it means this family has already been allocated for a previous queue type.
        if (family_props.queueCount == 0) {
            // Get allocated family index from residency (stored in queueFlags)
            uint32_t allocated_family = props_span[info.residency[desc.type]].queueFamilyProperties.queueFlags;

            // Check if the global priority of the already allocated family is greater.
            if (priority_next > info.data[allocated_family].pNext) {
                info.data[allocated_family].pNext = priority_next; // Upgrade the global priority for the already allocated family
            }

            info.residency[desc.type] = family_props.queueFlags;
            continue; // This family has already been allocated
        }

        info.residency[desc.type] = family_props.queueFlags = allocated_queue_count; // Store where the family is allocated in the residency field (abusing queueFlags for this purpose)

        info.data[allocated_queue_count++] = {
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = priority_next,
            .flags            = 0, // could have added internally synchronized, but we have semaphores for that
            .queueFamilyIndex = selection[desc.type],
            .queueCount       = std::min(family_props.queueCount, 32u),
            .pQueuePriorities = queue_priorities.data(),
        };

        // Mark this family as allocated by setting its queueCount to 0 (since we won't be able to allocate it again)
        family_props.queueCount = 0;
    }
    info.queue_type_count = allocated_queue_count;

    return info;
}

inline WisResult VKInitResourceAllocator(VkDevice          device,
                                         VkPhysicalDevice  adapter,
                                         VKInstanceHeader& instance_header,
                                         VKDeviceHeader&   device_header)
{
    uint32_t version = instance_header.api_version;
    auto&    gtable  = instance_header.global_table;
    auto&    dtable  = device_header.device_table;
    auto&    atable  = instance_header.adapter_table;
    auto&    ctable  = device_header.command_list_table;

    VmaVulkanFunctions allocator_functions{
        .vkGetInstanceProcAddr                   = gtable.vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr                     = gtable.vkGetDeviceProcAddr,
        .vkGetPhysicalDeviceProperties           = atable.vkGetPhysicalDeviceProperties,
        .vkGetPhysicalDeviceMemoryProperties     = atable.vkGetPhysicalDeviceMemoryProperties,
        .vkAllocateMemory                        = dtable.vkAllocateMemory,
        .vkFreeMemory                            = dtable.vkFreeMemory,
        .vkMapMemory                             = dtable.vkMapMemory,
        .vkUnmapMemory                           = dtable.vkUnmapMemory,
        .vkFlushMappedMemoryRanges               = dtable.vkFlushMappedMemoryRanges,
        .vkInvalidateMappedMemoryRanges          = dtable.vkInvalidateMappedMemoryRanges,
        .vkBindBufferMemory                      = dtable.vkBindBufferMemory,
        .vkBindImageMemory                       = dtable.vkBindImageMemory,
        .vkGetBufferMemoryRequirements           = dtable.vkGetBufferMemoryRequirements,
        .vkGetImageMemoryRequirements            = dtable.vkGetImageMemoryRequirements,
        .vkCreateBuffer                          = dtable.vkCreateBuffer,
        .vkDestroyBuffer                         = dtable.vkDestroyBuffer,
        .vkCreateImage                           = dtable.vkCreateImage,
        .vkDestroyImage                          = dtable.vkDestroyImage,
        .vkCmdCopyBuffer                         = ctable.vkCmdCopyBuffer,
        .vkGetBufferMemoryRequirements2KHR       = dtable.vkGetBufferMemoryRequirements2,
        .vkGetImageMemoryRequirements2KHR        = dtable.vkGetImageMemoryRequirements2,
        .vkBindBufferMemory2KHR                  = dtable.vkBindBufferMemory2,
        .vkBindImageMemory2KHR                   = dtable.vkBindImageMemory2,
        .vkGetPhysicalDeviceMemoryProperties2KHR = atable.vkGetPhysicalDeviceMemoryProperties2,
        .vkGetDeviceBufferMemoryRequirements     = dtable.vkGetDeviceBufferMemoryRequirements,
        .vkGetDeviceImageMemoryRequirements      = dtable.vkGetDeviceImageMemoryRequirements,
        .vkGetMemoryWin32HandleKHR               = nullptr, // set later if available
    };

    VkPhysicalDeviceMemoryProperties2 mem_props{};
    mem_props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
    atable.vkGetPhysicalDeviceMemoryProperties2(adapter, &mem_props);

    VmaAllocatorCreateInfo allocatorInfo{
        .flags                          = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice                 = adapter,
        .device                         = device,
        .preferredLargeHeapBlockSize    = 0,
        .pAllocationCallbacks           = nullptr,
        .pDeviceMemoryCallbacks         = nullptr,
        .pHeapSizeLimit                 = nullptr,
        .pVulkanFunctions               = &allocator_functions,
        .instance                       = device_header.instance,
        .vulkanApiVersion               = version,
        .pTypeExternalMemoryHandleTypes = nullptr,
    };

    // Enable maintenance5 if available and maintenance4
    if (dtable.vkGetDeviceBufferMemoryRequirements) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE4_BIT;
    }
    if (device_header.features.index_buffer_range) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE5_BIT;
    }

#ifdef _WIN32
    // Only if there is an interop extension
    if (dtable.vkGetMemoryWin32HandleKHR) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT;
        allocator_functions.vkGetMemoryWin32HandleKHR = dtable.vkGetMemoryWin32HandleKHR;
    }
#endif // _WIN32

    VkResult vr = vmaCreateAllocator(&allocatorInfo, &device_header.allocator);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Vulkan memory allocator">(vr);
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyAdapterQuery(WisVKAdapterQuery* self)
{
    auto& impl = *wis::from_handle<wis::impl::VKAdapterQueryImpl>(self);
    if (impl.physical_devices) {
        delete[] impl.physical_devices;
        wis::detail::VKReleaseInstance(impl.shared_header);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API size_t wisVKAdapterQueryGetAdapterCount(const WisVKAdapterQuery* self)
{
    return wis::from_handle<const wis::impl::VKAdapterQueryImpl>(self)->adapter_count;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKAdapterQueryGetAdapterDesc(const WisVKAdapterQuery* self,
                                                                  size_t                   index,
                                                                  WisAdapterDesc*          desc)
{
    const auto& impl = *wis::from_handle<const wis::impl::VKAdapterQueryImpl>(self);
    if (index >= impl.adapter_count) {
        return wis::detail::make_result<wis::detail::Func(), "Adapter index out of bounds">(VK_ERROR_INITIALIZATION_FAILED);
    }
    const auto& atable  = impl.shared_header->header.adapter_table;
    auto        adapter = impl.physical_devices[index];

    VkPhysicalDeviceIDProperties id_props{};
    id_props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES;

    VkPhysicalDeviceProperties2 properties{};
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    properties.pNext = &id_props;

    atable.vkGetPhysicalDeviceProperties2(adapter, &properties);

    auto& got_desc = properties.properties;

    VkPhysicalDeviceMemoryProperties memory_props{};
    atable.vkGetPhysicalDeviceMemoryProperties(adapter, &memory_props);

    // Get flags
    WisAdapterFlags flag{};
    if ((got_desc.deviceType & VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
        flag = static_cast<WisAdapterFlags>(flag | WisAdapterFlags::WisAdapterFlagsRemote);
    }
    if ((got_desc.deviceType & VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU) == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU) {
        flag = static_cast<WisAdapterFlags>(flag | WisAdapterFlags::WisAdapterFlagsSoftware);
    }

    uint64_t dedicated_video_memory = 0;
    uint64_t shared_system_memory   = 0;

    wis::span types{ memory_props.memoryTypes };
    for (auto& i : types) {
        if (i.propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
            memory_props.memoryHeaps[i.heapIndex].flags &
                    VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
            dedicated_video_memory = memory_props.memoryHeaps[i.heapIndex].size;
        }

        if (i.propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            shared_system_memory = memory_props.memoryHeaps[i.heapIndex].size;
        }
        if ((dedicated_video_memory != 0u) && (shared_system_memory != 0u)) {
            break;
        }
    }

    *desc = WisAdapterDesc{
        .description = {},
        .vendor_id   = got_desc.vendorID,
        .device_id   = got_desc.deviceID,

        .dedicated_video_memory = dedicated_video_memory,
        .shared_system_memory   = shared_system_memory, // Vulkan does not expose shared system memory directly

        .adapter_id   = id_props.deviceLUIDValid ? *reinterpret_cast<const uint64_t*>(id_props.deviceUUID) : 0,
        .adapter_uuid = {},
        .flags        = flag,
    };

    std::copy_n(got_desc.deviceName, sizeof(desc->description) - 1, desc->description);
    std::copy_n(id_props.deviceUUID, sizeof(desc->adapter_uuid), desc->adapter_uuid);
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API bool wisVKAdapterQueryGetSurfaceSupport(const WisVKAdapterQuery* self,
                                                                size_t                   index,
                                                                WisVKSurfaceView         surface)
{
    auto& impl       = *wis::from_handle<const wis::impl::VKAdapterQueryImpl>(self);
    auto& atable     = impl.shared_header->header.adapter_table;
    auto  vk_surface = std::bit_cast<VkSurfaceKHR>(surface);

    // Get queue families
    uint32_t                queue_family_count = 0;
    VkQueueFamilyProperties props[32];
    atable.vkGetPhysicalDeviceQueueFamilyProperties(impl.physical_devices[index], &queue_family_count, nullptr);
    atable.vkGetPhysicalDeviceQueueFamilyProperties(impl.physical_devices[index], &queue_family_count, props);

    // Search for graphics queue family that supports presentation to the surface

    uint32_t family_index = 0;
    for (uint32_t i = 0; i < queue_family_count; ++i) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            VkBool32 supported = VK_FALSE;
            auto     vr        = atable.vkGetPhysicalDeviceSurfaceSupportKHR(impl.physical_devices[index], i, vk_surface, &supported);
            if (wis::detail::succeeded(vr) && supported == VK_TRUE) {
                return true;
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKAdapterQueryCreateDevice(const WisVKAdapterQuery*       self,
                                                                size_t                         index,
                                                                const WisVKDeviceRequirements* requirements,
                                                                WisVKDevice*                   device)
{
    auto& impl = *wis::from_handle<const wis::impl::VKAdapterQueryImpl>(self);
    if (index >= impl.adapter_count) {
        return wis::detail::make_result<wis::detail::Func(), "Adapter index out of bounds">(VK_ERROR_INITIALIZATION_FAILED);
    }

    auto& atable  = impl.shared_header->header.adapter_table;
    auto& adapter = impl.physical_devices[index];

    WisResult                       res = wis::detail::vk_success;
    wis::VKDeviceExtensionCollector collector{ atable, adapter, res };
    if (res.status != WisStatusOk) {
        return res;
    }

    // Let extensions collect their info
    if (requirements) {
        for (size_t i = 0; i < requirements->extension_count; ++i) {
            if (auto* ext_header = wis::from_handle<wis::VKDeviceExtensionHeader>(requirements->extensions[i]); ext_header && ext_header->init_fptr) {
                auto res2 = ext_header->init_fptr(ext_header, nullptr, &collector);
                // Non-fatal, allow to silently fail
                (void)res2;
            }
        }
    }

    wis::detail::DeviceExtension1 device_ext1;
    auto                          xres = device_ext1.CollectInfo(collector);
    // Non-fatal, allow to silently fail
    (void)xres;

    // Prepared enabled extensions array
    auto&& [ext_buffer, ext_strings, ext_count, feature_structs, property_structs] = collector.GetInitBuffer(res);
    if (res.status != WisStatusOk) {
        return res;
    }

    // Create default enabled features
    VkPhysicalDeviceVulkan13Features vulkan13_features{};
    vulkan13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    vulkan13_features.pNext = feature_structs; // link to extension features

    VkPhysicalDeviceVulkan12Features vulkan12_features{};
    vulkan12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    vulkan12_features.pNext = &vulkan13_features;

    VkPhysicalDeviceVulkan11Features vulkan11_features{};
    vulkan11_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    vulkan11_features.pNext = &vulkan12_features;

    VkPhysicalDeviceFeatures2 features{};
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features.pNext = &vulkan11_features;

    atable.vkGetPhysicalDeviceFeatures2(adapter, &features);

    // Add default features to collector
    collector.ForceBindFeatureStruct(&features);
    collector.ForceBindFeatureStruct(&vulkan11_features);
    collector.ForceBindFeatureStruct(&vulkan12_features);

    // Create properties structures
    VkPhysicalDeviceVulkan13Properties vulkan13_properties{};
    vulkan13_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;
    vulkan13_properties.pNext = property_structs; // link to extension properties

    VkPhysicalDeviceVulkan12Properties vulkan12_properties{};
    vulkan12_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
    vulkan12_properties.pNext = &vulkan13_properties;

    VkPhysicalDeviceVulkan11Properties vulkan11_properties{};
    vulkan11_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
    vulkan11_properties.pNext = &vulkan12_properties;

    VkPhysicalDeviceProperties2 properties{};
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    properties.pNext = &vulkan11_properties;

    atable.vkGetPhysicalDeviceProperties2(adapter, &properties);

    // Add default properties to collector
    collector.ForceBindPropertyStruct(&properties);
    collector.ForceBindPropertyStruct(&vulkan11_properties);
    collector.ForceBindPropertyStruct(&vulkan12_properties);

    // Initialize queues
    auto queue_info = VKGetQueueResidencyInfo(
            atable,
            adapter,
            requirements,
            device_ext1.features,
            res);
    if (res.status != WisStatusOk) {
        return res;
    }

    // Create device
    VkDeviceCreateInfo device_create_info{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = &features, // link to extension features
        .flags                   = 0,
        .queueCreateInfoCount    = queue_info.queue_type_count,
        .pQueueCreateInfos       = queue_info.data.data(),
        .enabledLayerCount       = 0, // deprecated
        .ppEnabledLayerNames     = nullptr, // deprecated
        .enabledExtensionCount   = static_cast<uint32_t>(ext_count),
        .ppEnabledExtensionNames = ext_strings,
        .pEnabledFeatures        = nullptr, // deprecated
    };
    VkDevice device_handle = VK_NULL_HANDLE;
    VkResult vr            = atable.vkCreateDevice(adapter, &device_create_info, nullptr, &device_handle);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Vulkan device">(vr);
    }

    static_assert(alignof(wis::detail::VKDeviceControlBlock) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "VKDeviceControlBlock must be well aligned to safely use operator new for allocation");
    std::size_t control_block_size = sizeof(wis::detail::VKDeviceControlBlock); // allocate extra space for semaphores
    std::size_t semaphore_count    = 0;
    for (size_t i = 0; i < queue_info.queue_type_count; ++i) {
        semaphore_count += queue_info.data[i].queueCount;
    }
    control_block_size += sizeof(std::binary_semaphore) * semaphore_count;

    std::unique_ptr<std::byte[]> header_storage{ static_cast<std::byte*>(operator new(control_block_size, std::nothrow)) };
    if (!header_storage) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for Vulkan device header">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    // Start header lifetime
    wis::detail::VKDeviceControlBlock* header = new (header_storage.get()) wis::detail::VKDeviceControlBlock{};
    header->header.instance                   = impl.instance; // store instance handle in device header for later use in resource allocator

    wis::span<std::binary_semaphore> semaphores{ reinterpret_cast<std::binary_semaphore*>(header + 1), semaphore_count };
    for (auto& sem : semaphores) {
        std::construct_at(&sem, 1); // Initialize all semaphores to the non-signaled state
    }

    // Store queue create info in device header for later use in command queue creation
    header->header.family_count = queue_info.queue_type_count;
    uint32_t semaphore_offset   = 0;
    for (size_t i = 0; i < queue_info.queue_type_count; ++i) {
        auto& family_info  = header->header.queue_families[i];
        auto& queue_family = queue_info.data[i];

        family_info.family_index     = static_cast<uint8_t>(queue_family.queueFamilyIndex);
        family_info.queue_count      = static_cast<uint8_t>(queue_family.queueCount);
        family_info.semaphore_offset = semaphore_offset;

        if (queue_family.pNext) {
            // Global priority info is present in the pNext chain, store it in the device header
            const auto* global_priority_info = reinterpret_cast<const VkDeviceQueueGlobalPriorityCreateInfo*>(queue_family.pNext);
            family_info.queue_priority       = static_cast<uint8_t>(wis::detail::VKConvertGlobalPriority(global_priority_info->globalPriority));
        }

        semaphore_offset += family_info.queue_count;
    }

    // store mapping of queue type to family index in device header for quick lookup during command queue creation
    for (size_t j = 0; j < WisCommandQueueTypeCount; ++j) {
        auto index                        = queue_info.residency[j];
        header->header.queue_residency[j] = index;
        if (index != wis::detail::VKQueueFamilyProperties::invalid_family_index) {
            header->header.queue_family_extras[j] = {
                .family_index           = static_cast<uint8_t>(queue_info.data[index].queueFamilyIndex),
                .compatible_to_families = queue_info.transfer_masks[index],
            };
        } else {
            header->header.queue_family_extras[j] = {
                .family_index           = wis::detail::VKQueueFamilyProperties::invalid_family_index,
                .compatible_to_families = 0,
            };
        }
    }

    // Initialize device table
    auto& device_table = header->header.device_table;
    auto& gtable       = impl.shared_header->header.global_table;
    if (!device_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan device function table">(VK_ERROR_UNKNOWN);
    }

    // Initialize command queue table
    if (!header->header.command_queue_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan command queue function table">(VK_ERROR_UNKNOWN);
    }

    // Initialize command list table
    if (!header->header.command_list_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan command list function table">(VK_ERROR_UNKNOWN);
    }

    if (!header->header.swapchain_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan swapchain function table">(VK_ERROR_UNKNOWN);
    }

    // Create resource allocator
    res = VKInitResourceAllocator(device_handle, adapter, impl.shared_header->header, header->header);
    if (res.status != WisStatusOk) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return res;
    }

    header_storage.release(); // release ownership of header storage, it will be managed by the device impl from now on

    // Fill device impl
    auto& device_impl = *new (device) wis::impl::VKDeviceImpl{
        .device          = device_handle,
        .physical_device = adapter,
        .device_header   = header,
    };

    auto& device_header         = device_impl.device_header->header;
    device_header.device        = device_handle; // Duplicate for infrequent access
    device_header.shared_header = impl.shared_header;
    device_header.shared_header->AddRef(); // hold reference to instance header

    auto res2 = device_ext1.Init(device_impl, collector);
    // Non-fatal, allow to silently fail
    (void)res2;

    // Store device extensions info
    device_header.features = device_ext1.features;

    // Initialize device extensions
    if (requirements) {
        for (auto* ext : wis::span<WisVKDeviceExtensionHeader*>{ requirements->extensions, requirements->extension_count }) {
            if (auto* ext_header = wis::from_handle<wis::VKDeviceExtensionHeader>(ext); ext_header && ext_header->init_fptr) {
                if (auto yres = ext_header->init_fptr(ext_header, &device_impl, &collector); yres.status != WisStatusOk) {
                    res.status        = WisStatusPartial; // mark as partial success if any extension fails
                    res.error         = yres.error;
                    res.platform_code = yres.platform_code;
                }
            }
        }
    }

    return res;
}

#endif // WIS_VK_ADAPTER_QUERY_CPP
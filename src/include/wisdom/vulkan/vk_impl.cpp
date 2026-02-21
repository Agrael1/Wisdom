#ifndef WIS_VK_IMPL_CPP
#define WIS_VK_IMPL_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandQueue(WisVKCommandQueue* self)
{
    auto& impl = *reinterpret_cast<VKCommandQueueImpl*>(self);
    if (impl.queue) {
        detail::release_vk_device(impl.device, impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
        impl.queue         = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
 WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self)
{
     auto& impl = *reinterpret_cast<VKCommandListImpl*>(self);
     if (impl.command_buffer != VK_NULL_HANDLE) {
         // Free command buffer
         auto& table = impl.device_header->header.device_table;
         table.vkDestroyCommandPool(impl.device, impl.command_pool, nullptr); // this also frees command buffers
         impl.command_buffer = VK_NULL_HANDLE;

         detail::release_vk_device(impl.device, impl.device_header);
         impl.device_header = nullptr;
         impl.device        = VK_NULL_HANDLE;
     }
 }

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self)
{
    auto& impl = *reinterpret_cast<VKResourceAllocatorImpl*>(self);
    if (impl.allocator) {
        vmaDestroyAllocator(impl.allocator);
        impl.allocator = nullptr;

        detail::release_vk_device(impl.device, impl.device_header);
        impl.device        = VK_NULL_HANDLE;
        impl.device_header = nullptr;
    }
}

////-----------------------------------------------------------------------------
// WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreatePipelineLayout(const WisVKDevice*           self,
//                                                                   const WisPipelineLayoutDesc* desc,
//                                                                   WisVKPipelineLayout*         layout)
//{
//     WisResult res      = vk_success;
//     auto&     device   = *reinterpret_cast<const VKDeviceImpl*>(self);
//     auto&     table    = device.device_header->header.device_table;
//     auto      features = device.device_header->header.features;
//
//     // Pre checks
//     uint32_t layout_count  = 0;
//     uint32_t binding_count = 0;
//     uint32_t flag_bindings = 0;
//     if (desc->push_descriptor_count > 0) {
//         layout_count += 1;
//         binding_count = static_cast<uint32_t>(desc->push_descriptor_count);
//         if (!features.push_descriptor) {
//             return make_result<Func(), "Push descriptors are not supported on this Vulkan device">(VK_ERROR_FEATURE_NOT_PRESENT);
//         }
//
//         // Check upper limit
//         if (desc->push_descriptor_count > features.max_push_descriptors) {
//             return make_result<Func(), "Exceeded maximum number of Vulkan push descriptors">(VK_ERROR_INITIALIZATION_FAILED);
//         }
//     }
//
//     // Static samplers
//     if (desc->static_sampler_count > 0) {
//         layout_count += 1;
//
//         // Reuse binding count for static samplers
//         binding_count = std::max(binding_count, static_cast<uint32_t>(desc->static_sampler_count));
//     }
//
//     // Tables
//     if (desc->descriptor_table_count > 0) {
//         layout_count += static_cast<uint32_t>(desc->descriptor_table_count);
//         uint32_t table_bindings = 0;
//         for (size_t i = 0; i < desc->descriptor_table_count; ++i) {
//             const auto& table_desc = desc->descriptor_tables[i];
//
//             // NOTE: Space overlap is made by duplicated sets, but it counts towards the limit
//             if (table_desc.space_overlap > 0) {
//                 layout_count += table_desc.space_overlap;
//                 // Check if there is single entry (artificial, but otherwise it will explode)
//                 if (table_desc.entry_count > 1) {
//                     return make_result<Func(), "Space overlap can only be used with single entry descriptor tables">(VK_ERROR_INITIALIZATION_FAILED);
//                 }
//                 binding_count = std::max(binding_count, 1u);
//                 flag_bindings = std::max(flag_bindings, 1u);
//                 continue;
//             }
//             binding_count = std::max(binding_count, static_cast<uint32_t>(table_desc.entry_count));
//             flag_bindings = std::max(flag_bindings, static_cast<uint32_t>(table_desc.entry_count));
//         }
//
//         // Check upper limit
//         if (layout_count > features.max_bound_descriptor_sets) {
//             return make_result<Func(), "Exceeded maximum number of Vulkan descriptor sets per pipeline layout">(VK_ERROR_INITIALIZATION_FAILED);
//         }
//     }
//
//     // How many DSLs are needed
//     std::unique_ptr<detail::VKDescriptorSetLayoutContainer> dsl_layouts;
//     if (layout_count > 0) {
//         dsl_layouts = std::unique_ptr<detail::VKDescriptorSetLayoutContainer>(static_cast<VKDescriptorSetLayoutContainer*>(
//                 ::operator new(sizeof(VKDescriptorSetLayoutContainer) + layout_count * sizeof(VkDescriptorSetLayout) + desc->static_sampler_count * sizeof(VkSampler))));
//         if (!dsl_layouts) {
//             return make_result<Func(), "Not enough memory for Vulkan descriptor set layouts container">(VK_ERROR_OUT_OF_HOST_MEMORY);
//         }
//         dsl_layouts->dsl_count = layout_count;
//     }
//
//     // Prepare push constant ranges
//     VkPushConstantRange push_constant_ranges[std::size_t(ShaderStages::Count)];
//     if (desc->push_constant_count > 0) {
//         auto res = set_push_constant_ranges({ desc->push_constants,
//                                               desc->push_constant_count },
//                                             push_constant_ranges,
//                                             device);
//         if (res.status != WisStatusOk) {
//             return res;
//         }
//     }
//
//     // Bindings
//     uint32_t                         current_set  = 0;
//     wis::span<VkDescriptorSetLayout> out_dsls     = dsl_layouts->vk_dsls();
//     wis::span<VkSampler>             out_samplers = dsl_layouts->vk_static_samplers();
//
//     // Allocate bindings array
//     std::unique_ptr<VkDescriptorSetLayoutBinding[]> bindings;
//     if (binding_count > 0) {
//         bindings = std::unique_ptr<VkDescriptorSetLayoutBinding[]>(static_cast<VkDescriptorSetLayoutBinding*>(
//                 ::operator new(sizeof(VkDescriptorSetLayoutBinding) * binding_count + sizeof(VkDescriptorBindingFlags) * flag_bindings)));
//         if (!bindings) {
//             return make_result<Func(), "Not enough memory for Vulkan descriptor set layout bindings">(VK_ERROR_OUT_OF_HOST_MEMORY);
//         }
//     }
//     wis::span<VkDescriptorSetLayoutBinding> binding_span{ bindings.get(), binding_count };
//     wis::span<VkDescriptorBindingFlags>     binding_flags_span{
//         reinterpret_cast<VkDescriptorBindingFlags*>(
//                 bindings.get() + binding_count),
//         flag_bindings,
//     };
//
//     // Push descriptors
//     wis::span<const WisPushDescriptor> push_descriptors{
//         desc->push_descriptors,
//         desc->push_descriptor_count,
//     };
//     if (desc->push_descriptor_count > 0) {
//         res = set_push_descriptors(push_descriptors,
//                                    out_dsls,
//                                    binding_span,
//                                    table,
//                                    device.device,
//                                    current_set);
//         if (res.status != WisStatusOk) {
//             return res;
//         }
//     }
//     // End of push descriptors (current_set updated)
//
//     // Static samplers
//     wis::span<const WisStaticSamplerDesc> static_samplers{
//         desc->static_samplers,
//         desc->static_sampler_count,
//     };
//     VkDescriptorSet static_sampler_set = VK_NULL_HANDLE;
//     if (desc->static_sampler_count > 0) {
//         res = set_static_samplers(static_samplers,
//                                   out_dsls,
//                                   binding_span,
//                                   table,
//                                   device.device,
//                                   device.device_header->header.static_sampler_pool_allocator,
//                                   current_set,
//                                   *dsl_layouts,
//                                   &static_sampler_set);
//         if (res.status != WisStatusOk) {
//             dsl_layouts->destroy(device.device, table, static_sampler_set);
//             return res;
//         }
//     }
//     // End of static samplers (current_set updated)
//
//     // Tables
//     wis::span<const WisDescriptorTable> descriptor_tables{
//         desc->descriptor_tables,
//         desc->descriptor_table_count,
//     };
//     if (desc->descriptor_table_count > 0) {
//         res = set_descriptor_tables(descriptor_tables,
//                                     out_dsls,
//                                     binding_span,
//                                     binding_flags_span,
//                                     device,
//                                     current_set);
//         if (res.status != WisStatusOk) {
//             dsl_layouts->destroy(device.device, table, static_sampler_set);
//             return res;
//         }
//     }
//
//     // Create pipeline layout
//     VkPipelineLayoutCreateInfo pipeline_layout_info{
//         .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
//         .pNext                  = nullptr,
//         .flags                  = 0,
//         .setLayoutCount         = layout_count,
//         .pSetLayouts            = dsl_layouts ? dsl_layouts->vk_dsls().data() : nullptr,
//         .pushConstantRangeCount = static_cast<uint32_t>(desc->push_constant_count),
//         .pPushConstantRanges    = push_constant_ranges,
//     };
//     VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
//     auto             vr              = table.vkCreatePipelineLayout(device.device,
//                                            &pipeline_layout_info,
//                                            nullptr,
//                                            &pipeline_layout);
//     if (!succeeded(vr)) {
//         dsl_layouts->destroy(device.device, table, static_sampler_set);
//         return make_result<Func(), "Failed to create Vulkan pipeline layout">(vr);
//     }
//
//     // Fill pipeline layout impl
//     auto& layout_impl         = *new (layout) wis::detail::VKPipelineLayoutImpl();
//     layout_impl.layout        = pipeline_layout;
//     layout_impl.device        = device.device;
//     layout_impl.device_header = device.device_header;
//     device.device_header->AddRef(); // hold reference to device header
//     layout_impl.dsl_container   = dsl_layouts.release();
//     layout_impl.static_samplers = static_sampler_set;
//
//     return res;
// }

////-----------------------------------------------------------------------------
// WIS_EXTERN_C WISDOM_API void wisVKDestroyPipelineLayout(WisVKPipelineLayout* self)
//{
//     auto& impl = *reinterpret_cast<wis::detail::VKPipelineLayoutImpl*>(self);
//     if (impl.layout != VK_NULL_HANDLE) {
//         auto& table = impl.device_header->header.device_table;
//         table.vkDestroyPipelineLayout(impl.device, impl.layout, nullptr);
//         impl.layout = VK_NULL_HANDLE;
//
//         // Release descriptor set layouts
//         if (impl.dsl_container) {
//             impl.dsl_container->destroy(impl.device, table, impl.static_samplers);
//
//             ::operator delete(impl.dsl_container);
//             impl.dsl_container = nullptr;
//         }
//
//         detail::release_vk_device(impl.device, impl.device_header);
//         impl.device_header = nullptr;
//         impl.device        = VK_NULL_HANDLE;
//     }
// }

#endif // WIS_VK_IMPL_CPP

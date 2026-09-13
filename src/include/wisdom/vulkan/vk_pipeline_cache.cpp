#ifndef WIS_VK_PIPELINE_CACHE_CPP
#define WIS_VK_PIPELINE_CACHE_CPP
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyPipelineCache(WisVKPipelineCache* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKPipelineCacheImpl>(self);
    if (impl.cache != VK_NULL_HANDLE) {
        auto& header = impl.device_header->header;
        auto& table = header.device_table;
        auto device = header.device;

        table.vkDestroyPipelineCache(device, impl.cache, nullptr);

        wis::detail::VKReleaseDevice(impl.device_header);
        impl.cache = VK_NULL_HANDLE;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisVKPipelineCacheSerialize(const WisVKPipelineCache* self, uint8_t* data, size_t data_size)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKPipelineCacheImpl>(self);
    auto& table = impl.device_header->header.device_table;
    auto vr = table.vkGetPipelineCacheData(impl.device_header->header.device, impl.cache, &data_size, data);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to serialize pipeline cache">(vr);
    }
    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API size_t wisVKPipelineCacheGetSerializedSize(const WisVKPipelineCache* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKPipelineCacheImpl>(self);
    auto& table = impl.device_header->header.device_table;
    std::size_t data_size = 0;
    table.vkGetPipelineCacheData(impl.device_header->header.device, impl.cache, &data_size, nullptr);
    return wis::aligned_size(
        data_size,
        4096u
    ); // Align to 4096 bytes for better memory management when this data is used to create a new pipeline cache
}

#endif // WIS_VK_PIPELINE_CACHE_CPP

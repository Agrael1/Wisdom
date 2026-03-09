#ifndef WIS_DX12_PIPELINE_CACHE_CPP
#define WIS_DX12_PIPELINE_CACHE_CPP

#include <wisdom/generated/dx12_api.h>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyPipelineCache(WisDX12PipelineCache* self)
{
    auto& [cache] = *reinterpret_cast<wis::impl::DX12PipelineCacheImpl*>(self);
    if (!cache) {
        return;
    }

    cache->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12PipelineCacheSerialize(const WisDX12PipelineCache* self,
                                                                uint8_t*                    data,
                                                                size_t                      data_size)
{
    auto& [cache] = *reinterpret_cast<const wis::impl::DX12PipelineCacheImpl*>(self);
    auto hr       = cache->Serialize(data, data_size);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to serialize pipeline cache">(hr);
    }
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API size_t wisDX12PipelineCacheGetSerializedSize(const WisDX12PipelineCache* self)
{
    auto& [cache] = *reinterpret_cast<const wis::impl::DX12PipelineCacheImpl*>(self);
    return cache->GetSerializedSize();
}

#endif // WIS_DX12_PIPELINE_CACHE_CPP
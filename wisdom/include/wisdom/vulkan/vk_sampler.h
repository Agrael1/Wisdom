#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

namespace wis {
class VKSampler;

template<>
struct Internal<VKSampler> {
    wis::shared_handle<vk::Sampler> sampler;
};

WIS_EXPORT class VKSampler : public QueryInternal<VKSampler>
{
public:
    VKSampler() = default;
    explicit VKSampler(wis::shared_handle<vk::Sampler> sampler) noexcept
        : QueryInternal(std::move(sampler))
    {
    }
    operator bool() const noexcept
    {
        return bool(sampler);
    }
};
} // namespace wis

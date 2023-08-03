#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

WIS_EXPORT namespace wis
{
    class VKSampler;

    template<>
    class Internal<VKSampler>
    {
    public:
        Internal() = default;
        Internal(wis::shared_handle<vk::Sampler> view)
            : view(std::move(view))
        {
        }

        [[nodiscard]] auto GetSamplerHandle() const noexcept
        {
            return view;
        }
        [[nodiscard]] auto GetSampler() const noexcept
        {
            return view.get();
        }

    protected:
        wis::shared_handle<vk::Sampler> view;
    };

    class VKSampler : public QueryInternal<VKSampler>
    {
    public:
        VKSampler() = default;
        explicit VKSampler(wis::shared_handle<vk::Sampler> view)
            : QueryInternal(std::move(view))
        {
        }
    };
}

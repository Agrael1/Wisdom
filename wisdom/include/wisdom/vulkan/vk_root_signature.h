#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_views.h>
#endif

namespace wis {
class VKRootSignature;

template<>
class Internal<VKRootSignature>
{
public:
    wis::shared_handle<vk::PipelineLayout> root;
};

/// @brief Root signature
WIS_EXPORT class VKRootSignature : public QueryInternal<VKRootSignature>
{
public:
    VKRootSignature() = default;
    explicit VKRootSignature(wis::shared_handle<vk::PipelineLayout> root) noexcept
        : QueryInternal(std::move(root))
    {
    }

    operator VKRootSignatureView() const noexcept
    {
        return root.get();
    }
    operator bool() const noexcept
	{
		return bool(root);
	}
};
} // namespace wis

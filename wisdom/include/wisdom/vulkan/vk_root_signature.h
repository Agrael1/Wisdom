#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_handles.h>

namespace wis {
class VKRootSignature;

template<>
struct Internal<VKRootSignature> {
    wis::managed_handle_ex<VkPipelineLayout> root;
};

/// @brief Root signature
class VKRootSignature : public QueryInternal<VKRootSignature>
{
public:
    VKRootSignature() = default;
    explicit VKRootSignature(wis::managed_handle_ex<VkPipelineLayout> root) noexcept
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

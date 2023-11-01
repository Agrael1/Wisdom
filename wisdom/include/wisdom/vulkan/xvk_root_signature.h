#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_views.h>
#include <wisdom/vulkan/xvk_handles.h>

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

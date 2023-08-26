#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/api/api_common.h>
#endif

namespace wis {
class VKDescriptorSetLayout;

using VKDescriptorSetLayoutView = vk::DescriptorSetLayout;

template<>
class Internal<VKDescriptorSetLayout>
{
public:
    wis::shared_handle<vk::DescriptorSetLayout> layout;
};

WIS_EXPORT class VKDescriptorSetLayout : public QueryInternal<VKDescriptorSetLayout>
{
public:
    using QueryInternal::QueryInternal;
    operator VKDescriptorSetLayoutView() const noexcept
    {
        return layout.get();
    }
};

class VKDescriptorSet;

template<>
class Internal<VKDescriptorSet>
{
public:
    wis::shared_handle<vk::DescriptorSet> set;
};

WIS_EXPORT class VKDescriptorSet : public QueryInternal<VKDescriptorSet>
{
public:
    using QueryInternal::QueryInternal;
    operator VKDescriptorSetView() const noexcept
    {
        return set.get();
    }
};

class VKDescriptorHeap;

template<>
class Internal<VKDescriptorHeap>
{
public:
    wis::shared_handle<vk::DescriptorPool> pool;
};

using VKDescriptorHeapView = vk::DescriptorPool;

/// @brief Descriptor Heap object
WIS_EXPORT class VKDescriptorHeap : public QueryInternal<VKDescriptorHeap>
{
public:
    VKDescriptorHeap() = default;
    explicit VKDescriptorHeap(wis::shared_handle<vk::DescriptorPool> pool)
        : QueryInternal(std::move(pool))
    {
    }
    operator bool() const noexcept
    {
        return bool(pool);
    }
    operator VKDescriptorHeapView() const noexcept
    {
        return pool.get();
    }

    VKDescriptorSet AllocateDescriptorSet(VKDescriptorSetLayoutView layout)
    {
        const auto& device = pool.getParent();

        vk::DescriptorSetAllocateInfo alloc_info{
            pool.get(), 1u, &layout
        };
        auto [result, sets] = device->allocateDescriptorSets(alloc_info);
        return succeeded(result)
               ? VKDescriptorSet{ shared_handle<vk::DescriptorSet>{ std::move(sets[0]), device, { pool } } }
               :
               VKDescriptorSet{};
    }
};
} // namespace wis

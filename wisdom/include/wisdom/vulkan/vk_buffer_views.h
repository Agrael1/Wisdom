#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <vulkan/vulkan.hpp>
#endif

namespace wis {
class VKVertexBufferView;

template<>
class Internal<VKVertexBufferView>
{
public:
    vk::Buffer buffer;
    vk::DeviceSize size_bytes;
    vk::DeviceSize stride_bytes;
};

/// @brief Vertex buffer view
WIS_EXPORT class VKVertexBufferView : public QueryInternal<VKVertexBufferView>
{
public:
    VKVertexBufferView() = default;
    explicit VKVertexBufferView(vk::Buffer buffer,
                                vk::DeviceSize size_bytes,
                                vk::DeviceSize stride_bytes) noexcept
        : QueryInternal(buffer, size_bytes, stride_bytes)
    {
    }
};
} // namespace wis

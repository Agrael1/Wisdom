#ifndef WIS_DX12_RESOURCE_H
#define WIS_DX12_RESOURCE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_memory.h>
#include <optional>
#endif

namespace wis {
WISDOM_EXPORT class DX12Buffer;

WISDOM_EXPORT
template<>
struct Internal<DX12Buffer> {
    DX12Memory memory;
    wis::com_ptr<ID3D12Resource> resource;
};

class ImplDX12Buffer : public QueryInternal<DX12Buffer>
{
public:
    ImplDX12Buffer() noexcept = default;
    operator DX12BufferView() const noexcept
    {
        return resource.get();
    }
    operator bool() const noexcept
    {
        return bool(resource);
    }

public:
    template<typename T = void>
    T* Map() const noexcept
    {
        void* data;
        resource->Map(0, nullptr, &data);
        return static_cast<T*>(data);
    }

    void* MapRaw() const noexcept
    {
        return Map();
    }
    void Unmap() const noexcept
    {
        resource->Unmap(0, nullptr);
    }

    [[nodiscard]]
    uint64_t GetGPUAddress() const noexcept
    {
        return resource->GetGPUVirtualAddress();
    }
};

WISDOM_EXPORT
using DX12Texture = DX12Buffer;

// =================================================================================================
WISDOM_EXPORT class DX12RenderTarget;

WISDOM_EXPORT
template<>
struct Internal<DX12RenderTarget> {
    wis::com_ptr<ID3D12DescriptorHeap> heap;
    D3D12_CPU_DESCRIPTOR_HANDLE handle{};
};

WISDOM_EXPORT
class DX12RenderTarget : public QueryInternal<DX12RenderTarget>
{
public:
    DX12RenderTarget() = default;
    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12RenderTargetView() const noexcept
    {
        return handle;
    }
};

// =================================================================================================

WISDOM_EXPORT class DX12Sampler;

WISDOM_EXPORT
template<>
struct Internal<DX12Sampler> {
    wis::com_ptr<ID3D12DescriptorHeap> heap;
};

WISDOM_EXPORT
class DX12Sampler : public QueryInternal<DX12Sampler>
{
public:
    DX12Sampler() noexcept = default;
    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12SamplerView() const noexcept
    {
        return heap->GetCPUDescriptorHandleForHeapStart();
    }
};

// =================================================================================================

WISDOM_EXPORT class DX12ShaderResource;

WISDOM_EXPORT
template<>
struct Internal<DX12ShaderResource> {
    wis::com_ptr<ID3D12DescriptorHeap> heap;
};

WISDOM_EXPORT
class DX12ShaderResource : public QueryInternal<DX12ShaderResource>
{
public:
    DX12ShaderResource() noexcept = default;
    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12ShaderResourceView() const noexcept
    {
        return heap->GetCPUDescriptorHandleForHeapStart();
    }
};

WISDOM_EXPORT
using DX12UnorderedAccessTexture = DX12ShaderResource;

#pragma region DX12Buffer
/**
 * @brief Represents buffer object for storing linear data.
 * */
WISDOM_EXPORT
class DX12Buffer : public wis::ImplDX12Buffer
{
public:
    using wis::ImplDX12Buffer::ImplDX12Buffer;

public:
    /**
     * @brief Maps the buffer memory to CPU address space.
     * @return The pointer to the mapped memory.
     * */
    inline void* MapRaw() const noexcept
    {
        return wis::ImplDX12Buffer::MapRaw();
    }
    /**
     * @brief Unmaps the buffer memory from CPU address space.
     * */
    inline void Unmap() const noexcept
    {
        wis::ImplDX12Buffer::Unmap();
    }
    /**
     * @brief Returns the address of the resource in GPU memory.
     * @return The address of the resource in GPU memory.
     * */
    inline uint64_t GetGPUAddress() const noexcept
    {
        return wis::ImplDX12Buffer::GetGPUAddress();
    }
};
#pragma endregion DX12Buffer

} // namespace wis

#endif // WIS_DX12_RESOURCE_H

#ifndef WIS_DX12_DESCRIPTOR_STORAGE_H
#define WIS_DX12_DESCRIPTOR_STORAGE_H
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/util/misc.h>

namespace wis {

class DX12DescriptorStorage;

template<>
struct Internal<DX12DescriptorStorage> {
    // sampler, Uniform buffer, storage RW buffer, sampled image, storage RW image. Maybe storage read buffer will be needed.
    static constexpr uint32_t max_sets = max_desc_storage_desc_sets_dx12;

    wis::com_ptr<ID3D12Device> device;
    wis::com_ptr<ID3D12DescriptorHeap> heap_resource;
    wis::com_ptr<ID3D12DescriptorHeap> heap_sampler;

    CD3DX12_CPU_DESCRIPTOR_HANDLE heap_starts[max_sets]{};
    CD3DX12_GPU_DESCRIPTOR_HANDLE heap_gpu_starts[2]{}; // 0 - resource, 1 - sampler. Used for SetDescriptorHeaps
    uint32_t heap_resource_increment = 0;
    uint32_t heap_sampler_increment = 0;
};

class ImplDX12DescriptorStorage : public QueryInternal<DX12DescriptorStorage>
{
    enum SetIndex : uint32_t {
        Sampler = 0,
        ConstantBuffer = 1,
        StorageBuffer = 2,
        SampledImage = 3,
        StorageImage = 4,
        StorageReadBuffer = 5
    };
public:
    ImplDX12DescriptorStorage() = default;
    explicit ImplDX12DescriptorStorage(Internal<DX12DescriptorStorage>&& internal) noexcept
        : QueryInternal(std::move(internal))
    {
    }

    operator bool() const noexcept
    {
        return bool(heap_resource) || bool(heap_sampler);
    }

    operator DX12DescriptorStorageView() const noexcept
    {
        return { (DX12DescriptorStorage*)this };
    }

public:
    void WriteSampler(uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_starts[Sampler], index, heap_sampler_increment);
        auto& sampler_handle = std::get<0>(sampler);
        device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    }
    void WriteConstantBuffer(uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        auto* cbv = std::get<0>(buffer);
        D3D12_CONSTANT_BUFFER_VIEW_DESC desc{
            .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress() + offset,
            .SizeInBytes = wis::detail::aligned_size(size, uint32_t(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT))
        };

        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_starts[ConstantBuffer], index, heap_resource_increment);
        device->CreateConstantBufferView(&desc, handle);
    }
};

#pragma region DX12DescriptorStorage
/**
 * @brief Represents descriptor storage object for storing descriptors.
 * Used for bindless and non-uniform bindings. Don't combine with Descriptor buffers, this may reduce performance.
 * */
class DX12DescriptorStorage : public wis::ImplDX12DescriptorStorage
{
public:
    using wis::ImplDX12DescriptorStorage::ImplDX12DescriptorStorage;

public:
    /**
     * @brief Writes the sampler to the sampler descriptor storage.
     * @param index Index in array of samplers to fill.
     * @param sampler The sampler to write.
     * */
    inline void WriteSampler(uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        return wis::ImplDX12DescriptorStorage::WriteSampler(index, std::move(sampler));
    }
};
#pragma endregion DX12DescriptorStorage

} // namespace wis

#endif // !WIS_DX12_DESCRIPTOR_STORAGE_H
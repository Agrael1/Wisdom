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
    static constexpr uint32_t max_sets = uint32_t(wis::BindingIndex::Count);

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
public:
    ImplDX12DescriptorStorage() = default;
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
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_starts[uint32_t(wis::BindingIndex::Sampler) - 1], index, heap_sampler_increment);
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

        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_starts[uint32_t(wis::BindingIndex::ConstantBuffer) - 1], index, heap_resource_increment);
        device->CreateConstantBufferView(&desc, handle);
    }
    void WriteTexture(uint32_t index, wis::DX12ShaderResourceView srv) noexcept
    {
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_starts[uint32_t(wis::BindingIndex::Texture) - 1], index, heap_resource_increment);
        auto& srv_handle = std::get<0>(srv);
        device->CopyDescriptorsSimple(1, handle, srv_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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
        wis::ImplDX12DescriptorStorage::WriteSampler(index, std::move(sampler));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor storage.
     * @param index Index in array of constant buffers to fill.
     * @param buffer The buffer to write.
     * @param size The size of the constant buffer in bytes.
     * @param offset The offset in the buffer to write the constant buffer to.
     * size + offset must be less or equal the overall size of the bound buffer.
     * */
    inline void WriteConstantBuffer(uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        wis::ImplDX12DescriptorStorage::WriteConstantBuffer(index, std::move(buffer), size, offset);
    }
    /**
     * @brief Writes the texture to the shader resource descriptor storage.
     * @param index Index in array of shader resources to fill.
     * @param resource The shader resource to write.
     * */
    inline void WriteTexture(uint32_t index, wis::DX12ShaderResourceView resource) noexcept
    {
        wis::ImplDX12DescriptorStorage::WriteTexture(index, std::move(resource));
    }
};
#pragma endregion DX12DescriptorStorage

} // namespace wis

#endif // !WIS_DX12_DESCRIPTOR_STORAGE_H

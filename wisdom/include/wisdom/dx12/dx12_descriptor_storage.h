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
    struct OffsetIndicator {
        uint32_t offset_in_bytes : 31;
        uint32_t sampler : 1;
    };

    wis::com_ptr<ID3D12Device> device;
    wis::com_ptr<ID3D12DescriptorHeap> heaps[2];

    CD3DX12_GPU_DESCRIPTOR_HANDLE heap_gpu_starts[2]{}; // 0 - resource, 1 - sampler. Used for SetDescriptorHeaps
    CD3DX12_CPU_DESCRIPTOR_HANDLE heap_cpu_starts[2]{}; // 0 - resource, 1 - sampler. Used for CopyDescriptorsSimple

    std::unique_ptr<OffsetIndicator[]> heap_offsets;
    uint32_t heap_sampler_increment = 0;
    uint32_t heap_resource_increment = 0;
    uint32_t heap_count = 0;
};

class ImplDX12DescriptorStorage : public QueryInternal<DX12DescriptorStorage>
{
public:
    ImplDX12DescriptorStorage() = default;
    operator bool() const noexcept
    {
        return bool(heaps[0]) || bool(heaps[1]);
    }

    operator DX12DescriptorStorageView() const noexcept
    {
        return { (DX12DescriptorStorage*)this };
    }

public:
    void WriteSampler(uint32_t binding, uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_cpu_starts[1].Offset(heap_offsets[binding].offset_in_bytes), index, heap_sampler_increment);
        auto& sampler_handle = std::get<0>(sampler);
        device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    }
    void WriteConstantBuffer(uint32_t binding, uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        auto* cbv = std::get<0>(buffer);
        D3D12_CONSTANT_BUFFER_VIEW_DESC desc{
            .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress() + offset,
            .SizeInBytes = wis::detail::aligned_size(size, uint32_t(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)) // is this correct?
        };

        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_cpu_starts[0].Offset(heap_offsets[binding].offset_in_bytes), index, heap_resource_increment);
        device->CreateConstantBufferView(&desc, handle);
    }
    void WriteTexture(uint32_t binding, uint32_t index, wis::DX12ShaderResourceView srv) noexcept
    {
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_cpu_starts[0].Offset(heap_offsets[binding].offset_in_bytes), index, heap_resource_increment);
        auto& srv_handle = std::get<0>(srv);
        device->CopyDescriptorsSimple(1, handle, srv_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    void WriteRWTexture(uint32_t binding, uint32_t index, wis::DX12UnorderedAccessTextureView uav) noexcept
    {
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_cpu_starts[0].Offset(heap_offsets[binding].offset_in_bytes), index, heap_resource_increment);
        auto& uav_handle = std::get<0>(uav);

        device->CopyDescriptorsSimple(1, handle, uav_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    /// TODO: Test this function with structured and unstructured buffers
    /// offset has to be in elements, not bytes (4 byte chunks)
    void WriteRWBuffer(uint32_t binding, uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset4b = 0) noexcept
    {
        D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{
            .Format = DXGI_FORMAT_R32_TYPELESS,
            .ViewDimension = D3D12_UAV_DIMENSION_BUFFER,
            .Buffer{
                    .FirstElement = offset4b,
                    .NumElements = size >> 2, // size is in bytes, we need to convert it to elements
                    .StructureByteStride = 0,
                    .CounterOffsetInBytes = 0,
                    .Flags = D3D12_BUFFER_UAV_FLAG_NONE },
        };
        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_cpu_starts[0].Offset(heap_offsets[binding].offset_in_bytes), index, heap_resource_increment);
        device->CreateUnorderedAccessView(std::get<0>(buffer), nullptr, &uav_desc, handle);
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
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of samplers to fill.
     * @param sampler The sampler to write.
     * */
    inline void WriteSampler(uint32_t set_index, uint32_t binding, wis::DX12SamplerView sampler) noexcept
    {
        wis::ImplDX12DescriptorStorage::WriteSampler(set_index, binding, std::move(sampler));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of constant buffers to fill.
     * @param buffer The buffer to write.
     * @param size The size of the constant buffer in bytes.
     * @param offset The offset in the buffer to write the constant buffer to.
     * size + offset must be less or equal the overall size of the bound buffer.
     * */
    inline void WriteConstantBuffer(uint32_t set_index, uint32_t binding, wis::DX12BufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        wis::ImplDX12DescriptorStorage::WriteConstantBuffer(set_index, binding, std::move(buffer), size, offset);
    }
    /**
     * @brief Writes the texture to the shader resource descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of shader resources to fill.
     * @param resource The shader resource to write.
     * */
    inline void WriteTexture(uint32_t set_index, uint32_t binding, wis::DX12ShaderResourceView resource) noexcept
    {
        wis::ImplDX12DescriptorStorage::WriteTexture(set_index, binding, std::move(resource));
    }
};
#pragma endregion DX12DescriptorStorage

} // namespace wis

#endif // !WIS_DX12_DESCRIPTOR_STORAGE_H

#include "wisdom.h"

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_dx12.hpp>

// DX12CommandQueue methods --
extern "C" void DX12CommandQueueDestroy(DX12CommandQueue self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandQueue*>(self);
    delete xself;
}
extern "C" void DX12CommandQueueExecuteCommandLists(DX12CommandQueue self, const DX12CommandListView* lists, uint32_t count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandQueue*>(self);
    xself->ExecuteCommandLists(reinterpret_cast<const wis::DX12CommandListView*&>(lists), count);
}
extern "C" WisResult DX12CommandQueueSignalQueue(DX12CommandQueue self, DX12Fence fence, uint64_t value)
{
    auto* xself = reinterpret_cast<wis::DX12CommandQueue*>(self);
    auto res = xself->SignalQueue(*reinterpret_cast<wis::DX12Fence*>(fence), value);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12CommandQueueWaitQueue(DX12CommandQueue self, DX12Fence fence, uint64_t value)
{
    auto* xself = reinterpret_cast<wis::DX12CommandQueue*>(self);
    auto res = xself->WaitQueue(*reinterpret_cast<wis::DX12Fence*>(fence), value);
    ;
    return reinterpret_cast<WisResult&>(res);
}

// DX12Factory methods --
extern "C" WisResult DX12FactoryGetAdapter(DX12Factory self, uint32_t index, WisAdapterPreference preference, DX12Adapter* adapter)
{
    auto* xself = reinterpret_cast<wis::DX12Factory*>(self);
    auto&& [res, value] = xself->GetAdapter(index, static_cast<wis::AdapterPreference>(preference));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *adapter = reinterpret_cast<DX12Adapter>(new (std::nothrow) wis::DX12Adapter(std::move(value)));
    if (!*adapter)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Adapter." };
    return reinterpret_cast<WisResult&>(res);
}

// DX12PipelineState methods --
extern "C" void DX12PipelineStateDestroy(DX12PipelineState self)
{
    auto* xself = reinterpret_cast<wis::DX12PipelineState*>(self);
    delete xself;
}

// DX12Adapter methods --
extern "C" void DX12AdapterDestroy(DX12Adapter self)
{
    auto* xself = reinterpret_cast<wis::DX12Adapter*>(self);
    delete xself;
}
extern "C" WisResult DX12AdapterGetDesc(DX12Adapter self, WisAdapterDesc* inout_desc)
{
    auto* xself = reinterpret_cast<wis::DX12Adapter*>(self);
    auto res = xself->GetDesc(reinterpret_cast<wis::AdapterDesc*&>(inout_desc));
    ;
    return reinterpret_cast<WisResult&>(res);
}

// DX12Device methods --
extern "C" WisResult DX12DeviceWaitForMultipleFences(DX12Device self, const DX12FenceView* fences, const uint64_t* fence_values, uint32_t fence_count, WisMutiWaitFlags wait_all, uint64_t timeout)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto res = xself->WaitForMultipleFences(reinterpret_cast<const wis::DX12FenceView*&>(fences), fence_values, fence_count, static_cast<wis::MutiWaitFlags>(wait_all), timeout);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateFence(DX12Device self, uint64_t initial_value, WisFenceFlags flags, DX12Fence* fence)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateFence(initial_value, static_cast<wis::FenceFlags>(flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *fence = reinterpret_cast<DX12Fence>(new (std::nothrow) wis::DX12Fence(std::move(value)));
    if (!*fence)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Fence." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateCommandQueue(DX12Device self, WisQueueType type, DX12CommandQueue* queue)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateCommandQueue(static_cast<wis::QueueType>(type));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *queue = reinterpret_cast<DX12CommandQueue>(new (std::nothrow) wis::DX12CommandQueue(std::move(value)));
    if (!*queue)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12CommandQueue." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateCommandList(DX12Device self, WisQueueType type, DX12CommandList* list)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateCommandList(static_cast<wis::QueueType>(type));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *list = reinterpret_cast<DX12CommandList>(new (std::nothrow) wis::DX12CommandList(std::move(value)));
    if (!*list)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12CommandList." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateGraphicsPipeline(DX12Device self, const DX12GraphicsPipelineDesc* desc, DX12PipelineState* pipeline)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateGraphicsPipeline(reinterpret_cast<const wis::DX12GraphicsPipelineDesc*&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *pipeline = reinterpret_cast<DX12PipelineState>(new (std::nothrow) wis::DX12PipelineState(std::move(value)));
    if (!*pipeline)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12PipelineState." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateRootSignature(DX12Device self, const WisRootConstant* root_constants, uint32_t constants_size, const WisDescriptorTable* tables, uint32_t tables_count, DX12RootSignature* signature)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateRootSignature(reinterpret_cast<const wis::RootConstant*&>(root_constants), constants_size, reinterpret_cast<const wis::DescriptorTable*&>(tables), tables_count);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *signature = reinterpret_cast<DX12RootSignature>(new (std::nothrow) wis::DX12RootSignature(std::move(value)));
    if (!*signature)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12RootSignature." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateShader(DX12Device self, void* data, uint32_t size, DX12Shader* shader)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateShader(data, size);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *shader = reinterpret_cast<DX12Shader>(new (std::nothrow) wis::DX12Shader(std::move(value)));
    if (!*shader)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Shader." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateAllocator(DX12Device self, DX12ResourceAllocator* allocator)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateAllocator();

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *allocator = reinterpret_cast<DX12ResourceAllocator>(new (std::nothrow) wis::DX12ResourceAllocator(std::move(value)));
    if (!*allocator)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12ResourceAllocator." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateRenderTarget(DX12Device self, DX12Texture texture, WisRenderTargetDesc desc, DX12RenderTarget* target)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateRenderTarget(*reinterpret_cast<wis::DX12Texture*>(texture), reinterpret_cast<wis::RenderTargetDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *target = reinterpret_cast<DX12RenderTarget>(new (std::nothrow) wis::DX12RenderTarget(std::move(value)));
    if (!*target)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12RenderTarget." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateDepthStencilTarget(DX12Device self, DX12Texture texture, WisRenderTargetDesc desc, DX12RenderTarget* target)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateDepthStencilTarget(*reinterpret_cast<wis::DX12Texture*>(texture), reinterpret_cast<wis::RenderTargetDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *target = reinterpret_cast<DX12RenderTarget>(new (std::nothrow) wis::DX12RenderTarget(std::move(value)));
    if (!*target)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12RenderTarget." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateSampler(DX12Device self, const WisSamplerDesc* desc, DX12Sampler* sampler)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateSampler(reinterpret_cast<const wis::SamplerDesc*&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *sampler = reinterpret_cast<DX12Sampler>(new (std::nothrow) wis::DX12Sampler(std::move(value)));
    if (!*sampler)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Sampler." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateShaderResource(DX12Device self, DX12Texture texture, WisShaderResourceDesc desc, DX12ShaderResource* resource)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateShaderResource(*reinterpret_cast<wis::DX12Texture*>(texture), reinterpret_cast<wis::ShaderResourceDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *resource = reinterpret_cast<DX12ShaderResource>(new (std::nothrow) wis::DX12ShaderResource(std::move(value)));
    if (!*resource)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12ShaderResource." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" uint32_t DX12DeviceGetDescriptorTableAlignment(DX12Device self, WisDescriptorHeapType heap)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto res = xself->GetDescriptorTableAlignment(static_cast<wis::DescriptorHeapType>(heap));
    ;
    return res;
}
extern "C" uint32_t DX12DeviceGetDescriptorBufferUnitSize(DX12Device self, WisDescriptorHeapType heap)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto res = xself->GetDescriptorBufferUnitSize(static_cast<wis::DescriptorHeapType>(heap));
    ;
    return res;
}
extern "C" WisResult DX12DeviceCreateDescriptorBuffer(DX12Device self, WisDescriptorHeapType heap_type, WisDescriptorMemory memory_type, uint64_t size_bytes, DX12DescriptorBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateDescriptorBuffer(static_cast<wis::DescriptorHeapType>(heap_type), static_cast<wis::DescriptorMemory>(memory_type), size_bytes);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<DX12DescriptorBuffer>(new (std::nothrow) wis::DX12DescriptorBuffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12DescriptorBuffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" bool DX12DeviceQueryFeatureSupport(DX12Device self, WisDeviceFeature feature)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto res = xself->QueryFeatureSupport(static_cast<wis::DeviceFeature>(feature));
    ;
    return res;
}

// DX12DescriptorBuffer methods --
extern "C" void DX12DescriptorBufferDestroy(DX12DescriptorBuffer self)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    delete xself;
}
extern "C" uint64_t DX12DescriptorBufferWriteSampler(DX12DescriptorBuffer self, uint64_t aligned_table_offset, uint32_t index, DX12Sampler sampler)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    auto res = xself->WriteSampler(aligned_table_offset, index, *reinterpret_cast<wis::DX12Sampler*>(sampler));
    ;
    return res;
}
extern "C" uint64_t DX12DescriptorBufferWriteShaderResource2(DX12DescriptorBuffer self, uint64_t aligned_table_offset, uint32_t index, DX12ShaderResource resource)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    auto res = xself->WriteShaderResource2(aligned_table_offset, index, *reinterpret_cast<wis::DX12ShaderResource*>(resource));
    ;
    return res;
}
extern "C" uint64_t DX12DescriptorBufferWriteConstantBuffer2(DX12DescriptorBuffer self, uint64_t aligned_table_offset, uint32_t index, DX12Buffer buffer, uint32_t buffer_size)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    auto res = xself->WriteConstantBuffer2(aligned_table_offset, index, *reinterpret_cast<wis::DX12Buffer*>(buffer), buffer_size);
    ;
    return res;
}
extern "C" uint64_t DX12DescriptorBufferWriteShaderResource(DX12DescriptorBuffer self, uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, DX12RootSignature root_signature, DX12ShaderResource resource)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    auto res = xself->WriteShaderResource(aligned_table_offset, root_table_index, binding, array_member, *reinterpret_cast<wis::DX12RootSignature*>(root_signature), *reinterpret_cast<wis::DX12ShaderResource*>(resource));
    ;
    return res;
}
extern "C" uint64_t DX12DescriptorBufferWriteConstantBuffer(DX12DescriptorBuffer self, uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, DX12RootSignature root_signature, DX12Buffer buffer, uint32_t buffer_size)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    auto res = xself->WriteConstantBuffer(aligned_table_offset, root_table_index, binding, array_member, *reinterpret_cast<wis::DX12RootSignature*>(root_signature), *reinterpret_cast<wis::DX12Buffer*>(buffer), buffer_size);
    ;
    return res;
}

// DX12ResourceAllocator methods --
extern "C" void DX12ResourceAllocatorDestroy(DX12ResourceAllocator self)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    delete xself;
}
extern "C" WisResult DX12ResourceAllocatorCreateBuffer(DX12ResourceAllocator self, uint64_t size, WisBufferUsage usage, WisMemoryType memory, WisMemoryFlags mem_flags, DX12Buffer* buffer)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->CreateBuffer(size, static_cast<wis::BufferUsage>(usage), static_cast<wis::MemoryType>(memory), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<DX12Buffer>(new (std::nothrow) wis::DX12Buffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Buffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12ResourceAllocatorCreateTexture(DX12ResourceAllocator self, const WisTextureDesc* desc, WisMemoryType memory, WisMemoryFlags mem_flags, DX12Texture* texture)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->CreateTexture(*reinterpret_cast<const wis::TextureDesc*>(desc), static_cast<wis::MemoryType>(memory), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *texture = reinterpret_cast<DX12Texture>(new (std::nothrow) wis::DX12Texture(std::move(value)));
    if (!*texture)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Texture." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisAllocationInfo DX12ResourceAllocatorGetTextureAllocationInfo(DX12ResourceAllocator self, const WisTextureDesc* desc)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto res = xself->GetTextureAllocationInfo(*reinterpret_cast<const wis::TextureDesc*>(desc));
    ;
    return reinterpret_cast<WisAllocationInfo&>(res);
}
extern "C" WisAllocationInfo DX12ResourceAllocatorGetBufferAllocationInfo(DX12ResourceAllocator self, uint64_t size, WisBufferUsage usage)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto res = xself->GetBufferAllocationInfo(size, static_cast<wis::BufferUsage>(usage));
    ;
    return reinterpret_cast<WisAllocationInfo&>(res);
}
extern "C" WisResult DX12ResourceAllocatorAllocateTextureMemory(DX12ResourceAllocator self, uint64_t size, WisTextureUsage usage, WisMemoryType mem_type, WisMemoryFlags mem_flags, DX12Memory* out_memory)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->AllocateTextureMemory(size, static_cast<wis::TextureUsage>(usage), static_cast<wis::MemoryType>(mem_type), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *out_memory = reinterpret_cast<DX12Memory>(new (std::nothrow) wis::DX12Memory(std::move(value)));
    if (!*out_memory)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Memory." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12ResourceAllocatorAllocateBufferMemory(DX12ResourceAllocator self, uint64_t size, WisBufferUsage usage, WisMemoryType mem_type, WisMemoryFlags mem_flags, DX12Memory* out_memory)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->AllocateBufferMemory(size, static_cast<wis::BufferUsage>(usage), static_cast<wis::MemoryType>(mem_type), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *out_memory = reinterpret_cast<DX12Memory>(new (std::nothrow) wis::DX12Memory(std::move(value)));
    if (!*out_memory)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Memory." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12ResourceAllocatorPlaceBuffer(DX12ResourceAllocator self, DX12Memory memory, uint64_t memory_offset, uint64_t size, WisBufferUsage usage, DX12Buffer* buffer)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceBuffer(*reinterpret_cast<wis::DX12Memory*>(memory), memory_offset, size, static_cast<wis::BufferUsage>(usage));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<DX12Buffer>(new (std::nothrow) wis::DX12Buffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Buffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12ResourceAllocatorPlaceTexture(DX12ResourceAllocator self, DX12Memory memory, uint64_t memory_offset, const WisTextureDesc* desc, DX12Texture* texture)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceTexture(*reinterpret_cast<wis::DX12Memory*>(memory), memory_offset, *reinterpret_cast<const wis::TextureDesc*>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *texture = reinterpret_cast<DX12Texture>(new (std::nothrow) wis::DX12Texture(std::move(value)));
    if (!*texture)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Texture." };
    return reinterpret_cast<WisResult&>(res);
}

// DX12Fence methods --
extern "C" void DX12FenceDestroy(DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    delete xself;
}
extern "C" uint64_t DX12FenceGetCompletedValue(DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto res = xself->GetCompletedValue();
    ;
    return res;
}
extern "C" WisResult DX12FenceWait(DX12Fence self, uint64_t value, uint64_t wait_ns)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto res = xself->Wait(value, wait_ns);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12FenceSignal(DX12Fence self, uint64_t value)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto res = xself->Signal(value);
    ;
    return reinterpret_cast<WisResult&>(res);
}

// DX12CommandList methods --
extern "C" void DX12CommandListDestroy(DX12CommandList self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    delete xself;
}
extern "C" bool DX12CommandListClosed(DX12CommandList self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    auto res = xself->Closed();
    ;
    return res;
}
extern "C" bool DX12CommandListClose(DX12CommandList self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    auto res = xself->Close();
    ;
    return res;
}
extern "C" WisResult DX12CommandListReset(DX12CommandList self, DX12PipelineState initial_state)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    auto res = xself->Reset(*reinterpret_cast<wis::DX12PipelineState*>(initial_state));
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" void DX12CommandListSetPipelineState(DX12CommandList self, DX12PipelineState pipeline)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->SetPipelineState(*reinterpret_cast<wis::DX12PipelineState*>(pipeline));
}
extern "C" void DX12CommandListCopyBuffer(DX12CommandList self, DX12Buffer source, DX12Buffer destination, WisBufferRegion region)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->CopyBuffer(*reinterpret_cast<wis::DX12Buffer*>(source), *reinterpret_cast<wis::DX12Buffer*>(destination), reinterpret_cast<wis::BufferRegion&>(region));
}
extern "C" void DX12CommandListCopyBufferToTexture(DX12CommandList self, DX12Buffer source, DX12Texture destination, const WisBufferTextureCopyRegion* regions, uint32_t region_count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->CopyBufferToTexture(*reinterpret_cast<wis::DX12Buffer*>(source), *reinterpret_cast<wis::DX12Texture*>(destination), reinterpret_cast<const wis::BufferTextureCopyRegion*&>(regions), region_count);
}
extern "C" void DX12CommandListCopyTextureToBuffer(DX12CommandList self, DX12Texture source, DX12Buffer destination, const WisBufferTextureCopyRegion* regions, uint32_t region_count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->CopyTextureToBuffer(*reinterpret_cast<wis::DX12Texture*>(source), *reinterpret_cast<wis::DX12Buffer*>(destination), reinterpret_cast<const wis::BufferTextureCopyRegion*&>(regions), region_count);
}
extern "C" void DX12CommandListBufferBarrier(DX12CommandList self, WisBufferBarrier barrier, DX12Buffer buffer)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->BufferBarrier(reinterpret_cast<wis::BufferBarrier&>(barrier), *reinterpret_cast<wis::DX12Buffer*>(buffer));
}
extern "C" void DX12CommandListBufferBarriers(DX12CommandList self, const DX12BufferBarrier2* barriers, uint32_t barrier_count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->BufferBarriers(reinterpret_cast<const wis::DX12BufferBarrier2*&>(barriers), barrier_count);
}
extern "C" void DX12CommandListTextureBarrier(DX12CommandList self, WisTextureBarrier barrier, DX12Texture texture)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->TextureBarrier(reinterpret_cast<wis::TextureBarrier&>(barrier), *reinterpret_cast<wis::DX12Texture*>(texture));
}
extern "C" void DX12CommandListTextureBarriers(DX12CommandList self, const DX12TextureBarrier2* barriers, uint32_t barrier_count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->TextureBarriers(reinterpret_cast<const wis::DX12TextureBarrier2*&>(barriers), barrier_count);
}
extern "C" void DX12CommandListBeginRenderPass(DX12CommandList self, const DX12RenderPassDesc* pass_desc)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->BeginRenderPass(reinterpret_cast<const wis::DX12RenderPassDesc*&>(pass_desc));
}
extern "C" void DX12CommandListEndRenderPass(DX12CommandList self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->EndRenderPass();
}
extern "C" void DX12CommandListSetRootSignature(DX12CommandList self, DX12RootSignature root_signature)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->SetRootSignature(*reinterpret_cast<wis::DX12RootSignature*>(root_signature));
}
extern "C" void DX12CommandListIASetPrimitiveTopology(DX12CommandList self, WisPrimitiveTopology topology)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->IASetPrimitiveTopology(static_cast<wis::PrimitiveTopology>(topology));
}
extern "C" void DX12CommandListIASetVertexBuffers(DX12CommandList self, const DX12VertexBufferBinding* resources, uint32_t count, uint32_t start_slot)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->IASetVertexBuffers(reinterpret_cast<const wis::DX12VertexBufferBinding*&>(resources), count, start_slot);
}
extern "C" void DX12CommandListIASetIndexBuffer(DX12CommandList self, DX12Buffer buffer, WisIndexType type, uint64_t offset)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->IASetIndexBuffer(*reinterpret_cast<wis::DX12Buffer*>(buffer), static_cast<wis::IndexType>(type), offset);
}
extern "C" void DX12CommandListIASetIndexBuffer2(DX12CommandList self, DX12Buffer buffer, WisIndexType type, uint32_t size, uint64_t offset)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->IASetIndexBuffer2(*reinterpret_cast<wis::DX12Buffer*>(buffer), static_cast<wis::IndexType>(type), size, offset);
}
extern "C" void DX12CommandListRSSetViewport(DX12CommandList self, WisViewport viewport)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->RSSetViewport(reinterpret_cast<wis::Viewport&>(viewport));
}
extern "C" void DX12CommandListRSSetViewports(DX12CommandList self, const WisViewport* viewports, uint32_t count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->RSSetViewports(reinterpret_cast<const wis::Viewport*&>(viewports), count);
}
extern "C" void DX12CommandListRSSetScissor(DX12CommandList self, WisScissor scissor)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->RSSetScissor(reinterpret_cast<wis::Scissor&>(scissor));
}
extern "C" void DX12CommandListRSSetScissors(DX12CommandList self, const WisScissor* scissors, uint32_t count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->RSSetScissors(reinterpret_cast<const wis::Scissor*&>(scissors), count);
}
extern "C" void DX12CommandListDrawIndexedInstanced(DX12CommandList self, uint32_t vertex_count_per_instance, uint32_t instance_count, uint32_t start_index, uint32_t base_vertex, uint32_t start_instance)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->DrawIndexedInstanced(vertex_count_per_instance, instance_count, start_index, base_vertex, start_instance);
}
extern "C" void DX12CommandListDrawInstanced(DX12CommandList self, uint32_t vertex_count_per_instance, uint32_t instance_count, uint32_t start_vertex, uint32_t start_instance)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->DrawInstanced(vertex_count_per_instance, instance_count, start_vertex, start_instance);
}
extern "C" void DX12CommandListSetRootConstants(DX12CommandList self, void* data, uint32_t size_4bytes, uint32_t offset_4bytes, WisShaderStages stage)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->SetRootConstants(data, size_4bytes, offset_4bytes, static_cast<wis::ShaderStages>(stage));
}
extern "C" void DX12CommandListSetDescriptorBuffers(DX12CommandList self, const DX12DescriptorBufferView* buffers, uint32_t buffer_count)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->SetDescriptorBuffers(reinterpret_cast<const wis::DX12DescriptorBufferView*&>(buffers), buffer_count);
}
extern "C" void DX12CommandListSetDescriptorTableOffset(DX12CommandList self, uint32_t root_table_index, DX12DescriptorBuffer buffer, uint32_t offset_bytes)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    xself->SetDescriptorTableOffset(root_table_index, *reinterpret_cast<wis::DX12DescriptorBuffer*>(buffer), offset_bytes);
}

// DX12Memory methods --
extern "C" void DX12MemoryDestroy(DX12Memory self)
{
    auto* xself = reinterpret_cast<wis::DX12Memory*>(self);
    delete xself;
}
extern "C" uint64_t DX12MemoryGetBlockOffset(DX12Memory self)
{
    auto* xself = reinterpret_cast<wis::DX12Memory*>(self);
    auto res = xself->GetBlockOffset();
    ;
    return res;
}

// DX12SwapChain methods --
extern "C" void DX12SwapChainDestroy(DX12SwapChain self)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    delete xself;
}
extern "C" uint32_t DX12SwapChainGetCurrentIndex(DX12SwapChain self)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    auto res = xself->GetCurrentIndex();
    ;
    return res;
}
extern "C" bool DX12SwapChainStereoSupported(DX12SwapChain self)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    auto res = xself->StereoSupported();
    ;
    return res;
}
extern "C" WisResult DX12SwapChainResize(DX12SwapChain self, uint32_t width, uint32_t height)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    auto res = xself->Resize(width, height);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12SwapChainPresent(DX12SwapChain self)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    auto res = xself->Present();
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12SwapChainPresent2(DX12SwapChain self, bool in_vsync)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    auto res = xself->Present2(in_vsync);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12SwapChainWaitForPresent(DX12SwapChain self, uint64_t timeout_ns)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    auto res = xself->WaitForPresent(timeout_ns);
    ;
    return reinterpret_cast<WisResult&>(res);
}

// DX12Buffer methods --
extern "C" void DX12BufferDestroy(DX12Buffer self)
{
    auto* xself = reinterpret_cast<wis::DX12Buffer*>(self);
    delete xself;
}
extern "C" void* DX12BufferMapRaw(DX12Buffer self)
{
    auto* xself = reinterpret_cast<wis::DX12Buffer*>(self);
    auto res = xself->MapRaw();
    ;
    return res;
}
extern "C" void DX12BufferUnmap(DX12Buffer self)
{
    auto* xself = reinterpret_cast<wis::DX12Buffer*>(self);
    xself->Unmap();
}

// DX12Texture methods --
extern "C" void DX12TextureDestroy(DX12Texture self)
{
    auto* xself = reinterpret_cast<wis::DX12Texture*>(self);
    delete xself;
}

// DX12DescriptorStorage methods --
extern "C" void DX12DescriptorStorageDestroy(DX12DescriptorStorage self)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorStorage*>(self);
    delete xself;
}
extern "C" void DX12DescriptorStorageWriteSampler(DX12DescriptorStorage self, uint32_t index, DX12Sampler sampler)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorStorage*>(self);
    xself->WriteSampler(index, *reinterpret_cast<wis::DX12Sampler*>(sampler));
}

// DX12RootSignature methods --
extern "C" void DX12RootSignatureDestroy(DX12RootSignature self)
{
    auto* xself = reinterpret_cast<wis::DX12RootSignature*>(self);
    delete xself;
}

// DX12Shader methods --
extern "C" void DX12ShaderDestroy(DX12Shader self)
{
    auto* xself = reinterpret_cast<wis::DX12Shader*>(self);
    delete xself;
}

// DX12DebugMessenger methods --
extern "C" void DX12DebugMessengerDestroy(DX12DebugMessenger self)
{
    auto* xself = reinterpret_cast<wis::DX12DebugMessenger*>(self);
    delete xself;
}

// DX12RenderTarget methods --
extern "C" void DX12RenderTargetDestroy(DX12RenderTarget self)
{
    auto* xself = reinterpret_cast<wis::DX12RenderTarget*>(self);
    delete xself;
}

// DX12Sampler methods --
extern "C" void DX12SamplerDestroy(DX12Sampler self)
{
    auto* xself = reinterpret_cast<wis::DX12Sampler*>(self);
    delete xself;
}

// DX12ShaderResource methods --
extern "C" void DX12ShaderResourceDestroy(DX12ShaderResource self)
{
    auto* xself = reinterpret_cast<wis::DX12ShaderResource*>(self);
    delete xself;
}

extern "C" DX12FenceView AsDX12FenceView(DX12Fence self)
{
    wis::DX12FenceView xself = reinterpret_cast<wis::DX12Fence&>(*self);
    return reinterpret_cast<DX12FenceView&>(xself);
}
extern "C" DX12BufferView AsDX12BufferView(DX12Buffer self)
{
    wis::DX12BufferView xself = reinterpret_cast<wis::DX12Buffer&>(*self);
    return reinterpret_cast<DX12BufferView&>(xself);
}
extern "C" DX12TextureView AsDX12TextureView(DX12Texture self)
{
    wis::DX12TextureView xself = reinterpret_cast<wis::DX12Texture&>(*self);
    return reinterpret_cast<DX12TextureView&>(xself);
}
extern "C" DX12RenderTargetView AsDX12RenderTargetView(DX12RenderTarget self)
{
    wis::DX12RenderTargetView xself = reinterpret_cast<wis::DX12RenderTarget&>(*self);
    return reinterpret_cast<DX12RenderTargetView&>(xself);
}
extern "C" DX12CommandListView AsDX12CommandListView(DX12CommandList self)
{
    wis::DX12CommandListView xself = reinterpret_cast<wis::DX12CommandList&>(*self);
    return reinterpret_cast<DX12CommandListView&>(xself);
}
extern "C" DX12ShaderView AsDX12ShaderView(DX12Shader self)
{
    wis::DX12ShaderView xself = reinterpret_cast<wis::DX12Shader&>(*self);
    return reinterpret_cast<DX12ShaderView&>(xself);
}
extern "C" DX12RootSignatureView AsDX12RootSignatureView(DX12RootSignature self)
{
    wis::DX12RootSignatureView xself = reinterpret_cast<wis::DX12RootSignature&>(*self);
    return reinterpret_cast<DX12RootSignatureView&>(xself);
}
extern "C" DX12DescriptorBufferView AsDX12DescriptorBufferView(DX12DescriptorBuffer self)
{
    wis::DX12DescriptorBufferView xself = reinterpret_cast<wis::DX12DescriptorBuffer&>(*self);
    return reinterpret_cast<DX12DescriptorBufferView&>(xself);
}
#endif

#if defined(WISDOM_VULKAN)
#include <wisdom/wisdom_vk.hpp>

// VKCommandQueue methods --
extern "C" void VKCommandQueueDestroy(VKCommandQueue self)
{
    auto* xself = reinterpret_cast<wis::VKCommandQueue*>(self);
    delete xself;
}
extern "C" void VKCommandQueueExecuteCommandLists(VKCommandQueue self, const VKCommandListView* lists, uint32_t count)
{
    auto* xself = reinterpret_cast<wis::VKCommandQueue*>(self);
    xself->ExecuteCommandLists(reinterpret_cast<const wis::VKCommandListView*&>(lists), count);
}
extern "C" WisResult VKCommandQueueSignalQueue(VKCommandQueue self, VKFence fence, uint64_t value)
{
    auto* xself = reinterpret_cast<wis::VKCommandQueue*>(self);
    auto res = xself->SignalQueue(*reinterpret_cast<wis::VKFence*>(fence), value);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKCommandQueueWaitQueue(VKCommandQueue self, VKFence fence, uint64_t value)
{
    auto* xself = reinterpret_cast<wis::VKCommandQueue*>(self);
    auto res = xself->WaitQueue(*reinterpret_cast<wis::VKFence*>(fence), value);
    ;
    return reinterpret_cast<WisResult&>(res);
}

// VKFactory methods --
extern "C" WisResult VKFactoryGetAdapter(VKFactory self, uint32_t index, WisAdapterPreference preference, VKAdapter* adapter)
{
    auto* xself = reinterpret_cast<wis::VKFactory*>(self);
    auto&& [res, value] = xself->GetAdapter(index, static_cast<wis::AdapterPreference>(preference));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *adapter = reinterpret_cast<VKAdapter>(new (std::nothrow) wis::VKAdapter(std::move(value)));
    if (!*adapter)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKAdapter." };
    return reinterpret_cast<WisResult&>(res);
}

// VKPipelineState methods --
extern "C" void VKPipelineStateDestroy(VKPipelineState self)
{
    auto* xself = reinterpret_cast<wis::VKPipelineState*>(self);
    delete xself;
}

// VKAdapter methods --
extern "C" void VKAdapterDestroy(VKAdapter self)
{
    auto* xself = reinterpret_cast<wis::VKAdapter*>(self);
    delete xself;
}
extern "C" WisResult VKAdapterGetDesc(VKAdapter self, WisAdapterDesc* inout_desc)
{
    auto* xself = reinterpret_cast<wis::VKAdapter*>(self);
    auto res = xself->GetDesc(reinterpret_cast<wis::AdapterDesc*&>(inout_desc));
    ;
    return reinterpret_cast<WisResult&>(res);
}

// VKDevice methods --
extern "C" WisResult VKDeviceWaitForMultipleFences(VKDevice self, const VKFenceView* fences, const uint64_t* fence_values, uint32_t fence_count, WisMutiWaitFlags wait_all, uint64_t timeout)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto res = xself->WaitForMultipleFences(reinterpret_cast<const wis::VKFenceView*&>(fences), fence_values, fence_count, static_cast<wis::MutiWaitFlags>(wait_all), timeout);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateFence(VKDevice self, uint64_t initial_value, WisFenceFlags flags, VKFence* fence)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateFence(initial_value, static_cast<wis::FenceFlags>(flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *fence = reinterpret_cast<VKFence>(new (std::nothrow) wis::VKFence(std::move(value)));
    if (!*fence)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKFence." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateCommandQueue(VKDevice self, WisQueueType type, VKCommandQueue* queue)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateCommandQueue(static_cast<wis::QueueType>(type));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *queue = reinterpret_cast<VKCommandQueue>(new (std::nothrow) wis::VKCommandQueue(std::move(value)));
    if (!*queue)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKCommandQueue." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateCommandList(VKDevice self, WisQueueType type, VKCommandList* list)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateCommandList(static_cast<wis::QueueType>(type));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *list = reinterpret_cast<VKCommandList>(new (std::nothrow) wis::VKCommandList(std::move(value)));
    if (!*list)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKCommandList." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateGraphicsPipeline(VKDevice self, const VKGraphicsPipelineDesc* desc, VKPipelineState* pipeline)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateGraphicsPipeline(reinterpret_cast<const wis::VKGraphicsPipelineDesc*&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *pipeline = reinterpret_cast<VKPipelineState>(new (std::nothrow) wis::VKPipelineState(std::move(value)));
    if (!*pipeline)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKPipelineState." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateRootSignature(VKDevice self, const WisRootConstant* root_constants, uint32_t constants_size, const WisDescriptorTable* tables, uint32_t tables_count, VKRootSignature* signature)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateRootSignature(reinterpret_cast<const wis::RootConstant*&>(root_constants), constants_size, reinterpret_cast<const wis::DescriptorTable*&>(tables), tables_count);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *signature = reinterpret_cast<VKRootSignature>(new (std::nothrow) wis::VKRootSignature(std::move(value)));
    if (!*signature)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKRootSignature." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateShader(VKDevice self, void* data, uint32_t size, VKShader* shader)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateShader(data, size);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *shader = reinterpret_cast<VKShader>(new (std::nothrow) wis::VKShader(std::move(value)));
    if (!*shader)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKShader." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateAllocator(VKDevice self, VKResourceAllocator* allocator)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateAllocator();

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *allocator = reinterpret_cast<VKResourceAllocator>(new (std::nothrow) wis::VKResourceAllocator(std::move(value)));
    if (!*allocator)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKResourceAllocator." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateRenderTarget(VKDevice self, VKTexture texture, WisRenderTargetDesc desc, VKRenderTarget* target)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateRenderTarget(*reinterpret_cast<wis::VKTexture*>(texture), reinterpret_cast<wis::RenderTargetDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *target = reinterpret_cast<VKRenderTarget>(new (std::nothrow) wis::VKRenderTarget(std::move(value)));
    if (!*target)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKRenderTarget." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateDepthStencilTarget(VKDevice self, VKTexture texture, WisRenderTargetDesc desc, VKRenderTarget* target)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateDepthStencilTarget(*reinterpret_cast<wis::VKTexture*>(texture), reinterpret_cast<wis::RenderTargetDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *target = reinterpret_cast<VKRenderTarget>(new (std::nothrow) wis::VKRenderTarget(std::move(value)));
    if (!*target)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKRenderTarget." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateSampler(VKDevice self, const WisSamplerDesc* desc, VKSampler* sampler)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateSampler(reinterpret_cast<const wis::SamplerDesc*&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *sampler = reinterpret_cast<VKSampler>(new (std::nothrow) wis::VKSampler(std::move(value)));
    if (!*sampler)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKSampler." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateShaderResource(VKDevice self, VKTexture texture, WisShaderResourceDesc desc, VKShaderResource* resource)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateShaderResource(*reinterpret_cast<wis::VKTexture*>(texture), reinterpret_cast<wis::ShaderResourceDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *resource = reinterpret_cast<VKShaderResource>(new (std::nothrow) wis::VKShaderResource(std::move(value)));
    if (!*resource)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKShaderResource." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" uint32_t VKDeviceGetDescriptorTableAlignment(VKDevice self, WisDescriptorHeapType heap)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto res = xself->GetDescriptorTableAlignment(static_cast<wis::DescriptorHeapType>(heap));
    ;
    return res;
}
extern "C" uint32_t VKDeviceGetDescriptorBufferUnitSize(VKDevice self, WisDescriptorHeapType heap)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto res = xself->GetDescriptorBufferUnitSize(static_cast<wis::DescriptorHeapType>(heap));
    ;
    return res;
}
extern "C" WisResult VKDeviceCreateDescriptorBuffer(VKDevice self, WisDescriptorHeapType heap_type, WisDescriptorMemory memory_type, uint64_t size_bytes, VKDescriptorBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateDescriptorBuffer(static_cast<wis::DescriptorHeapType>(heap_type), static_cast<wis::DescriptorMemory>(memory_type), size_bytes);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<VKDescriptorBuffer>(new (std::nothrow) wis::VKDescriptorBuffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKDescriptorBuffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" bool VKDeviceQueryFeatureSupport(VKDevice self, WisDeviceFeature feature)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto res = xself->QueryFeatureSupport(static_cast<wis::DeviceFeature>(feature));
    ;
    return res;
}

// VKDescriptorBuffer methods --
extern "C" void VKDescriptorBufferDestroy(VKDescriptorBuffer self)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    delete xself;
}
extern "C" uint64_t VKDescriptorBufferWriteSampler(VKDescriptorBuffer self, uint64_t aligned_table_offset, uint32_t index, VKSampler sampler)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    auto res = xself->WriteSampler(aligned_table_offset, index, *reinterpret_cast<wis::VKSampler*>(sampler));
    ;
    return res;
}
extern "C" uint64_t VKDescriptorBufferWriteShaderResource2(VKDescriptorBuffer self, uint64_t aligned_table_offset, uint32_t index, VKShaderResource resource)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    auto res = xself->WriteShaderResource2(aligned_table_offset, index, *reinterpret_cast<wis::VKShaderResource*>(resource));
    ;
    return res;
}
extern "C" uint64_t VKDescriptorBufferWriteConstantBuffer2(VKDescriptorBuffer self, uint64_t aligned_table_offset, uint32_t index, VKBuffer buffer, uint32_t buffer_size)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    auto res = xself->WriteConstantBuffer2(aligned_table_offset, index, *reinterpret_cast<wis::VKBuffer*>(buffer), buffer_size);
    ;
    return res;
}
extern "C" uint64_t VKDescriptorBufferWriteShaderResource(VKDescriptorBuffer self, uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, VKRootSignature root_signature, VKShaderResource resource)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    auto res = xself->WriteShaderResource(aligned_table_offset, root_table_index, binding, array_member, *reinterpret_cast<wis::VKRootSignature*>(root_signature), *reinterpret_cast<wis::VKShaderResource*>(resource));
    ;
    return res;
}
extern "C" uint64_t VKDescriptorBufferWriteConstantBuffer(VKDescriptorBuffer self, uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, VKRootSignature root_signature, VKBuffer buffer, uint32_t buffer_size)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    auto res = xself->WriteConstantBuffer(aligned_table_offset, root_table_index, binding, array_member, *reinterpret_cast<wis::VKRootSignature*>(root_signature), *reinterpret_cast<wis::VKBuffer*>(buffer), buffer_size);
    ;
    return res;
}

// VKResourceAllocator methods --
extern "C" void VKResourceAllocatorDestroy(VKResourceAllocator self)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    delete xself;
}
extern "C" WisResult VKResourceAllocatorCreateBuffer(VKResourceAllocator self, uint64_t size, WisBufferUsage usage, WisMemoryType memory, WisMemoryFlags mem_flags, VKBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->CreateBuffer(size, static_cast<wis::BufferUsage>(usage), static_cast<wis::MemoryType>(memory), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<VKBuffer>(new (std::nothrow) wis::VKBuffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKBuffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKResourceAllocatorCreateTexture(VKResourceAllocator self, const WisTextureDesc* desc, WisMemoryType memory, WisMemoryFlags mem_flags, VKTexture* texture)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->CreateTexture(*reinterpret_cast<const wis::TextureDesc*>(desc), static_cast<wis::MemoryType>(memory), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *texture = reinterpret_cast<VKTexture>(new (std::nothrow) wis::VKTexture(std::move(value)));
    if (!*texture)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKTexture." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisAllocationInfo VKResourceAllocatorGetTextureAllocationInfo(VKResourceAllocator self, const WisTextureDesc* desc)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto res = xself->GetTextureAllocationInfo(*reinterpret_cast<const wis::TextureDesc*>(desc));
    ;
    return reinterpret_cast<WisAllocationInfo&>(res);
}
extern "C" WisAllocationInfo VKResourceAllocatorGetBufferAllocationInfo(VKResourceAllocator self, uint64_t size, WisBufferUsage usage)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto res = xself->GetBufferAllocationInfo(size, static_cast<wis::BufferUsage>(usage));
    ;
    return reinterpret_cast<WisAllocationInfo&>(res);
}
extern "C" WisResult VKResourceAllocatorAllocateTextureMemory(VKResourceAllocator self, uint64_t size, WisTextureUsage usage, WisMemoryType mem_type, WisMemoryFlags mem_flags, VKMemory* out_memory)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->AllocateTextureMemory(size, static_cast<wis::TextureUsage>(usage), static_cast<wis::MemoryType>(mem_type), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *out_memory = reinterpret_cast<VKMemory>(new (std::nothrow) wis::VKMemory(std::move(value)));
    if (!*out_memory)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKMemory." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKResourceAllocatorAllocateBufferMemory(VKResourceAllocator self, uint64_t size, WisBufferUsage usage, WisMemoryType mem_type, WisMemoryFlags mem_flags, VKMemory* out_memory)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->AllocateBufferMemory(size, static_cast<wis::BufferUsage>(usage), static_cast<wis::MemoryType>(mem_type), static_cast<wis::MemoryFlags>(mem_flags));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *out_memory = reinterpret_cast<VKMemory>(new (std::nothrow) wis::VKMemory(std::move(value)));
    if (!*out_memory)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKMemory." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKResourceAllocatorPlaceBuffer(VKResourceAllocator self, VKMemory memory, uint64_t memory_offset, uint64_t size, WisBufferUsage usage, VKBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceBuffer(*reinterpret_cast<wis::VKMemory*>(memory), memory_offset, size, static_cast<wis::BufferUsage>(usage));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<VKBuffer>(new (std::nothrow) wis::VKBuffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKBuffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKResourceAllocatorPlaceTexture(VKResourceAllocator self, VKMemory memory, uint64_t memory_offset, const WisTextureDesc* desc, VKTexture* texture)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceTexture(*reinterpret_cast<wis::VKMemory*>(memory), memory_offset, *reinterpret_cast<const wis::TextureDesc*>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *texture = reinterpret_cast<VKTexture>(new (std::nothrow) wis::VKTexture(std::move(value)));
    if (!*texture)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKTexture." };
    return reinterpret_cast<WisResult&>(res);
}

// VKFence methods --
extern "C" void VKFenceDestroy(VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    delete xself;
}
extern "C" uint64_t VKFenceGetCompletedValue(VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto res = xself->GetCompletedValue();
    ;
    return res;
}
extern "C" WisResult VKFenceWait(VKFence self, uint64_t value, uint64_t wait_ns)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto res = xself->Wait(value, wait_ns);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKFenceSignal(VKFence self, uint64_t value)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto res = xself->Signal(value);
    ;
    return reinterpret_cast<WisResult&>(res);
}

// VKCommandList methods --
extern "C" void VKCommandListDestroy(VKCommandList self)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    delete xself;
}
extern "C" bool VKCommandListClosed(VKCommandList self)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    auto res = xself->Closed();
    ;
    return res;
}
extern "C" bool VKCommandListClose(VKCommandList self)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    auto res = xself->Close();
    ;
    return res;
}
extern "C" WisResult VKCommandListReset(VKCommandList self, VKPipelineState initial_state)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    auto res = xself->Reset(*reinterpret_cast<wis::VKPipelineState*>(initial_state));
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" void VKCommandListSetPipelineState(VKCommandList self, VKPipelineState pipeline)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->SetPipelineState(*reinterpret_cast<wis::VKPipelineState*>(pipeline));
}
extern "C" void VKCommandListCopyBuffer(VKCommandList self, VKBuffer source, VKBuffer destination, WisBufferRegion region)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->CopyBuffer(*reinterpret_cast<wis::VKBuffer*>(source), *reinterpret_cast<wis::VKBuffer*>(destination), reinterpret_cast<wis::BufferRegion&>(region));
}
extern "C" void VKCommandListCopyBufferToTexture(VKCommandList self, VKBuffer source, VKTexture destination, const WisBufferTextureCopyRegion* regions, uint32_t region_count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->CopyBufferToTexture(*reinterpret_cast<wis::VKBuffer*>(source), *reinterpret_cast<wis::VKTexture*>(destination), reinterpret_cast<const wis::BufferTextureCopyRegion*&>(regions), region_count);
}
extern "C" void VKCommandListCopyTextureToBuffer(VKCommandList self, VKTexture source, VKBuffer destination, const WisBufferTextureCopyRegion* regions, uint32_t region_count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->CopyTextureToBuffer(*reinterpret_cast<wis::VKTexture*>(source), *reinterpret_cast<wis::VKBuffer*>(destination), reinterpret_cast<const wis::BufferTextureCopyRegion*&>(regions), region_count);
}
extern "C" void VKCommandListBufferBarrier(VKCommandList self, WisBufferBarrier barrier, VKBuffer buffer)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->BufferBarrier(reinterpret_cast<wis::BufferBarrier&>(barrier), *reinterpret_cast<wis::VKBuffer*>(buffer));
}
extern "C" void VKCommandListBufferBarriers(VKCommandList self, const VKBufferBarrier2* barriers, uint32_t barrier_count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->BufferBarriers(reinterpret_cast<const wis::VKBufferBarrier2*&>(barriers), barrier_count);
}
extern "C" void VKCommandListTextureBarrier(VKCommandList self, WisTextureBarrier barrier, VKTexture texture)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->TextureBarrier(reinterpret_cast<wis::TextureBarrier&>(barrier), *reinterpret_cast<wis::VKTexture*>(texture));
}
extern "C" void VKCommandListTextureBarriers(VKCommandList self, const VKTextureBarrier2* barriers, uint32_t barrier_count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->TextureBarriers(reinterpret_cast<const wis::VKTextureBarrier2*&>(barriers), barrier_count);
}
extern "C" void VKCommandListBeginRenderPass(VKCommandList self, const VKRenderPassDesc* pass_desc)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->BeginRenderPass(reinterpret_cast<const wis::VKRenderPassDesc*&>(pass_desc));
}
extern "C" void VKCommandListEndRenderPass(VKCommandList self)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->EndRenderPass();
}
extern "C" void VKCommandListSetRootSignature(VKCommandList self, VKRootSignature root_signature)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->SetRootSignature(*reinterpret_cast<wis::VKRootSignature*>(root_signature));
}
extern "C" void VKCommandListIASetPrimitiveTopology(VKCommandList self, WisPrimitiveTopology topology)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->IASetPrimitiveTopology(static_cast<wis::PrimitiveTopology>(topology));
}
extern "C" void VKCommandListIASetVertexBuffers(VKCommandList self, const VKVertexBufferBinding* resources, uint32_t count, uint32_t start_slot)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->IASetVertexBuffers(reinterpret_cast<const wis::VKVertexBufferBinding*&>(resources), count, start_slot);
}
extern "C" void VKCommandListIASetIndexBuffer(VKCommandList self, VKBuffer buffer, WisIndexType type, uint64_t offset)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->IASetIndexBuffer(*reinterpret_cast<wis::VKBuffer*>(buffer), static_cast<wis::IndexType>(type), offset);
}
extern "C" void VKCommandListIASetIndexBuffer2(VKCommandList self, VKBuffer buffer, WisIndexType type, uint32_t size, uint64_t offset)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->IASetIndexBuffer2(*reinterpret_cast<wis::VKBuffer*>(buffer), static_cast<wis::IndexType>(type), size, offset);
}
extern "C" void VKCommandListRSSetViewport(VKCommandList self, WisViewport viewport)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->RSSetViewport(reinterpret_cast<wis::Viewport&>(viewport));
}
extern "C" void VKCommandListRSSetViewports(VKCommandList self, const WisViewport* viewports, uint32_t count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->RSSetViewports(reinterpret_cast<const wis::Viewport*&>(viewports), count);
}
extern "C" void VKCommandListRSSetScissor(VKCommandList self, WisScissor scissor)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->RSSetScissor(reinterpret_cast<wis::Scissor&>(scissor));
}
extern "C" void VKCommandListRSSetScissors(VKCommandList self, const WisScissor* scissors, uint32_t count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->RSSetScissors(reinterpret_cast<const wis::Scissor*&>(scissors), count);
}
extern "C" void VKCommandListDrawIndexedInstanced(VKCommandList self, uint32_t vertex_count_per_instance, uint32_t instance_count, uint32_t start_index, uint32_t base_vertex, uint32_t start_instance)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->DrawIndexedInstanced(vertex_count_per_instance, instance_count, start_index, base_vertex, start_instance);
}
extern "C" void VKCommandListDrawInstanced(VKCommandList self, uint32_t vertex_count_per_instance, uint32_t instance_count, uint32_t start_vertex, uint32_t start_instance)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->DrawInstanced(vertex_count_per_instance, instance_count, start_vertex, start_instance);
}
extern "C" void VKCommandListSetRootConstants(VKCommandList self, void* data, uint32_t size_4bytes, uint32_t offset_4bytes, WisShaderStages stage)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->SetRootConstants(data, size_4bytes, offset_4bytes, static_cast<wis::ShaderStages>(stage));
}
extern "C" void VKCommandListSetDescriptorBuffers(VKCommandList self, const VKDescriptorBufferView* buffers, uint32_t buffer_count)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->SetDescriptorBuffers(reinterpret_cast<const wis::VKDescriptorBufferView*&>(buffers), buffer_count);
}
extern "C" void VKCommandListSetDescriptorTableOffset(VKCommandList self, uint32_t root_table_index, VKDescriptorBuffer buffer, uint32_t offset_bytes)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    xself->SetDescriptorTableOffset(root_table_index, *reinterpret_cast<wis::VKDescriptorBuffer*>(buffer), offset_bytes);
}

// VKMemory methods --
extern "C" void VKMemoryDestroy(VKMemory self)
{
    auto* xself = reinterpret_cast<wis::VKMemory*>(self);
    delete xself;
}
extern "C" uint64_t VKMemoryGetBlockOffset(VKMemory self)
{
    auto* xself = reinterpret_cast<wis::VKMemory*>(self);
    auto res = xself->GetBlockOffset();
    ;
    return res;
}

// VKSwapChain methods --
extern "C" void VKSwapChainDestroy(VKSwapChain self)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    delete xself;
}
extern "C" uint32_t VKSwapChainGetCurrentIndex(VKSwapChain self)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    auto res = xself->GetCurrentIndex();
    ;
    return res;
}
extern "C" bool VKSwapChainStereoSupported(VKSwapChain self)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    auto res = xself->StereoSupported();
    ;
    return res;
}
extern "C" WisResult VKSwapChainResize(VKSwapChain self, uint32_t width, uint32_t height)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    auto res = xself->Resize(width, height);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKSwapChainPresent(VKSwapChain self)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    auto res = xself->Present();
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKSwapChainPresent2(VKSwapChain self, bool in_vsync)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    auto res = xself->Present2(in_vsync);
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKSwapChainWaitForPresent(VKSwapChain self, uint64_t timeout_ns)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    auto res = xself->WaitForPresent(timeout_ns);
    ;
    return reinterpret_cast<WisResult&>(res);
}

// VKBuffer methods --
extern "C" void VKBufferDestroy(VKBuffer self)
{
    auto* xself = reinterpret_cast<wis::VKBuffer*>(self);
    delete xself;
}
extern "C" void* VKBufferMapRaw(VKBuffer self)
{
    auto* xself = reinterpret_cast<wis::VKBuffer*>(self);
    auto res = xself->MapRaw();
    ;
    return res;
}
extern "C" void VKBufferUnmap(VKBuffer self)
{
    auto* xself = reinterpret_cast<wis::VKBuffer*>(self);
    xself->Unmap();
}

// VKTexture methods --
extern "C" void VKTextureDestroy(VKTexture self)
{
    auto* xself = reinterpret_cast<wis::VKTexture*>(self);
    delete xself;
}

// VKDescriptorStorage methods --
extern "C" void VKDescriptorStorageDestroy(VKDescriptorStorage self)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorStorage*>(self);
    delete xself;
}
extern "C" void VKDescriptorStorageWriteSampler(VKDescriptorStorage self, uint32_t index, VKSampler sampler)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorStorage*>(self);
    xself->WriteSampler(index, *reinterpret_cast<wis::VKSampler*>(sampler));
}

// VKRootSignature methods --
extern "C" void VKRootSignatureDestroy(VKRootSignature self)
{
    auto* xself = reinterpret_cast<wis::VKRootSignature*>(self);
    delete xself;
}

// VKShader methods --
extern "C" void VKShaderDestroy(VKShader self)
{
    auto* xself = reinterpret_cast<wis::VKShader*>(self);
    delete xself;
}

// VKDebugMessenger methods --
extern "C" void VKDebugMessengerDestroy(VKDebugMessenger self)
{
    auto* xself = reinterpret_cast<wis::VKDebugMessenger*>(self);
    delete xself;
}

// VKRenderTarget methods --
extern "C" void VKRenderTargetDestroy(VKRenderTarget self)
{
    auto* xself = reinterpret_cast<wis::VKRenderTarget*>(self);
    delete xself;
}

// VKSampler methods --
extern "C" void VKSamplerDestroy(VKSampler self)
{
    auto* xself = reinterpret_cast<wis::VKSampler*>(self);
    delete xself;
}

// VKShaderResource methods --
extern "C" void VKShaderResourceDestroy(VKShaderResource self)
{
    auto* xself = reinterpret_cast<wis::VKShaderResource*>(self);
    delete xself;
}

extern "C" VKFenceView AsVKFenceView(VKFence self)
{
    wis::VKFenceView xself = reinterpret_cast<wis::VKFence&>(*self);
    return reinterpret_cast<VKFenceView&>(xself);
}
extern "C" VKBufferView AsVKBufferView(VKBuffer self)
{
    wis::VKBufferView xself = reinterpret_cast<wis::VKBuffer&>(*self);
    return reinterpret_cast<VKBufferView&>(xself);
}
extern "C" VKTextureView AsVKTextureView(VKTexture self)
{
    wis::VKTextureView xself = reinterpret_cast<wis::VKTexture&>(*self);
    return reinterpret_cast<VKTextureView&>(xself);
}
extern "C" VKRenderTargetView AsVKRenderTargetView(VKRenderTarget self)
{
    wis::VKRenderTargetView xself = reinterpret_cast<wis::VKRenderTarget&>(*self);
    return reinterpret_cast<VKRenderTargetView&>(xself);
}
extern "C" VKCommandListView AsVKCommandListView(VKCommandList self)
{
    wis::VKCommandListView xself = reinterpret_cast<wis::VKCommandList&>(*self);
    return reinterpret_cast<VKCommandListView&>(xself);
}
extern "C" VKShaderView AsVKShaderView(VKShader self)
{
    wis::VKShaderView xself = reinterpret_cast<wis::VKShader&>(*self);
    return reinterpret_cast<VKShaderView&>(xself);
}
extern "C" VKRootSignatureView AsVKRootSignatureView(VKRootSignature self)
{
    wis::VKRootSignatureView xself = reinterpret_cast<wis::VKRootSignature&>(*self);
    return reinterpret_cast<VKRootSignatureView&>(xself);
}
extern "C" VKDescriptorBufferView AsVKDescriptorBufferView(VKDescriptorBuffer self)
{
    wis::VKDescriptorBufferView xself = reinterpret_cast<wis::VKDescriptorBuffer&>(*self);
    return reinterpret_cast<VKDescriptorBufferView&>(xself);
}
#endif

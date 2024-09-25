#include "wisdom.h"

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_dx12.hpp>

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
extern "C" WisResult DX12DeviceCreateRenderTarget(DX12Device self, DX12TextureView texture, WisRenderTargetDesc desc, DX12RenderTarget* target)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateRenderTarget(reinterpret_cast<wis::DX12TextureView&>(texture), reinterpret_cast<wis::RenderTargetDesc&>(desc));

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
extern "C" WisResult DX12DeviceCreateShaderResource(DX12Device self, DX12TextureView texture, WisShaderResourceDesc desc, DX12ShaderResource* resource)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateShaderResource(reinterpret_cast<wis::DX12TextureView&>(texture), reinterpret_cast<wis::ShaderResourceDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *resource = reinterpret_cast<DX12ShaderResource>(new (std::nothrow) wis::DX12ShaderResource(std::move(value)));
    if (!*resource)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12ShaderResource." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceGetDescriptorTableAlignment(DX12Device self, WisDescriptorHeapType heap, uint32_t* alignment)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->GetDescriptorTableAlignment(static_cast<wis::DescriptorHeapType>(heap));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *alignment = value;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceGetDescriptorBufferUnitSize(DX12Device self, WisDescriptorHeapType heap, uint32_t* size)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->GetDescriptorBufferUnitSize(static_cast<wis::DescriptorHeapType>(heap));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *size = value;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DeviceCreateDescriptorBuffer(DX12Device self, WisDescriptorHeapType heap_type, WisDescriptorMemory memory_type, uint32_t descriptor_count, DX12DescriptorBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& [res, value] = xself->CreateDescriptorBuffer(static_cast<wis::DescriptorHeapType>(heap_type), static_cast<wis::DescriptorMemory>(memory_type), descriptor_count);

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
extern "C" WisResult DX12ResourceAllocatorPlaceBuffer(DX12ResourceAllocator self, DX12MemoryView memory, uint64_t memory_offset, uint64_t size, WisBufferUsage usage, DX12Buffer* buffer)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceBuffer(reinterpret_cast<wis::DX12MemoryView&>(memory), memory_offset, size, static_cast<wis::BufferUsage>(usage));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<DX12Buffer>(new (std::nothrow) wis::DX12Buffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Buffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12ResourceAllocatorPlaceTexture(DX12ResourceAllocator self, DX12MemoryView memory, uint64_t memory_offset, const WisTextureDesc* desc, DX12Texture* texture)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceTexture(reinterpret_cast<wis::DX12MemoryView&>(memory), memory_offset, *reinterpret_cast<const wis::TextureDesc*>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *texture = reinterpret_cast<DX12Texture>(new (std::nothrow) wis::DX12Texture(std::move(value)));
    if (!*texture)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Texture." };
    return reinterpret_cast<WisResult&>(res);
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
extern "C" DX12MemoryView AsDX12MemoryView(DX12Memory self)
{
    wis::DX12MemoryView xself = reinterpret_cast<wis::DX12Memory&>(*self);
    return reinterpret_cast<DX12MemoryView&>(xself);
}
#endif

#if defined(WISDOM_VULKAN)
#include <wisdom/wisdom_vk.hpp>

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
extern "C" WisResult VKDeviceCreateRenderTarget(VKDevice self, VKTextureView texture, WisRenderTargetDesc desc, VKRenderTarget* target)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateRenderTarget(reinterpret_cast<wis::VKTextureView&>(texture), reinterpret_cast<wis::RenderTargetDesc&>(desc));

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
extern "C" WisResult VKDeviceCreateShaderResource(VKDevice self, VKTextureView texture, WisShaderResourceDesc desc, VKShaderResource* resource)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateShaderResource(reinterpret_cast<wis::VKTextureView&>(texture), reinterpret_cast<wis::ShaderResourceDesc&>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *resource = reinterpret_cast<VKShaderResource>(new (std::nothrow) wis::VKShaderResource(std::move(value)));
    if (!*resource)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKShaderResource." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceGetDescriptorTableAlignment(VKDevice self, WisDescriptorHeapType heap, uint32_t* alignment)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->GetDescriptorTableAlignment(static_cast<wis::DescriptorHeapType>(heap));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *alignment = value;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceGetDescriptorBufferUnitSize(VKDevice self, WisDescriptorHeapType heap, uint32_t* size)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->GetDescriptorBufferUnitSize(static_cast<wis::DescriptorHeapType>(heap));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *size = value;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDeviceCreateDescriptorBuffer(VKDevice self, WisDescriptorHeapType heap_type, WisDescriptorMemory memory_type, uint32_t descriptor_count, VKDescriptorBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& [res, value] = xself->CreateDescriptorBuffer(static_cast<wis::DescriptorHeapType>(heap_type), static_cast<wis::DescriptorMemory>(memory_type), descriptor_count);

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
extern "C" WisResult VKResourceAllocatorPlaceBuffer(VKResourceAllocator self, VKMemoryView memory, uint64_t memory_offset, uint64_t size, WisBufferUsage usage, VKBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceBuffer(reinterpret_cast<wis::VKMemoryView&>(memory), memory_offset, size, static_cast<wis::BufferUsage>(usage));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *buffer = reinterpret_cast<VKBuffer>(new (std::nothrow) wis::VKBuffer(std::move(value)));
    if (!*buffer)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKBuffer." };
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKResourceAllocatorPlaceTexture(VKResourceAllocator self, VKMemoryView memory, uint64_t memory_offset, const WisTextureDesc* desc, VKTexture* texture)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    auto&& [res, value] = xself->PlaceTexture(reinterpret_cast<wis::VKMemoryView&>(memory), memory_offset, *reinterpret_cast<const wis::TextureDesc*>(desc));

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *texture = reinterpret_cast<VKTexture>(new (std::nothrow) wis::VKTexture(std::move(value)));
    if (!*texture)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKTexture." };
    return reinterpret_cast<WisResult&>(res);
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
extern "C" VKMemoryView AsVKMemoryView(VKMemory self)
{
    wis::VKMemoryView xself = reinterpret_cast<wis::VKMemory&>(*self);
    return reinterpret_cast<VKMemoryView&>(xself);
}
#endif

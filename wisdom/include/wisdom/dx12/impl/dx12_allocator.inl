#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_allocator.h>
#endif

wis::DX12ResourceAllocator::DX12ResourceAllocator(const wis::DX12Device& device) noexcept
{
    auto& device_i = device.GetInternal();
    D3D12MA::ALLOCATOR_DESC desc{
        .Flags = D3D12MA::ALLOCATOR_FLAGS::ALLOCATOR_FLAG_NONE,
        .pDevice = device_i.device.get(),
        .PreferredBlockSize = 0,
        .pAllocationCallbacks = nullptr,
        .pAdapter = device_i.adapter.get()
    };
    wis::succeeded(D3D12MA::CreateAllocator(&desc, allocator.put()));
}
wis::DX12Buffer wis::DX12ResourceAllocator::CreatePersistentBuffer(size_t size, BufferFlags)const noexcept
{
    using namespace river::flags;
    winrt::com_ptr<ID3D12Resource> rc;
    winrt::com_ptr<D3D12MA::Allocation> al;
    auto desc = CD3DX12_RESOURCE_DESC1::Buffer(size);
    D3D12MA::ALLOCATION_DESC all_desc = {};
    all_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COPY_DEST;

    return wis::succeeded(allocator->CreateResource2(&all_desc, &desc,
                               state, nullptr,
                               al.put(), __uuidof(*rc), rc.put_void()))
        ? DX12Buffer{ std::move(rc), std::move(al) }
        : DX12Buffer{};
}
wis::DX12Buffer wis::DX12ResourceAllocator::CreateUploadBuffer(size_t size) const noexcept
{
    winrt::com_ptr<ID3D12Resource> rc;
    winrt::com_ptr<D3D12MA::Allocation> al;
    auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);
    D3D12MA::ALLOCATION_DESC all_desc = {};
    all_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

     return wis::succeeded(allocator->CreateResource(&all_desc, &desc,
                              D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                              al.put(), __uuidof(*rc), rc.put_void()))
         ? DX12Buffer{ std::move(rc), std::move(al) }
         : DX12Buffer{};
}
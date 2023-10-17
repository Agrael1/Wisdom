#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/dx12/xdx12_device.h>
#endif // !WISDOM_HEADER_ONLY

std::pair<wis::Result, wis::DX12Device>
wis::DX12CreateDevice(wis::DX12FactoryHandle factory, wis::DX12AdapterHandle adapter) noexcept
{
    auto in_factory = std::get<0>(factory);
    auto in_adapter = std::get<0>(adapter);

    wis::com_ptr<ID3D12Device10> device;

    HRESULT hr;
    return !wis::succeeded(hr = D3D12CreateDevice(in_adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device9), device.put_void()))
            ? std::pair{ wis::make_result<FUNC, "D3D12CreateDevice failed to create device">(hr), wis::DX12Device{} }
            : std::pair{ wis::success, wis::DX12Device(std::move(device), wis::com_ptr(in_adapter), wis::com_ptr(in_factory)) };
}

std::pair<wis::Result, wis::DX12Fence> 
wis::DX12Device::CreateFence(uint64_t initial_value) const noexcept
{
    HRESULT hr;
    wis::com_ptr<ID3D12Fence1> fence;
    return wis::succeeded(hr = device->CreateFence(initial_value, D3D12_FENCE_FLAG_NONE, __uuidof(*fence), fence.put_void()))
        ? std::pair{ wis::success, DX12Fence{ std::move(fence) } }
        : std::pair{ wis::make_result<FUNC, "ID3D12Device10::CreateFence failed to create fence">(hr), DX12Fence{} };
}

std::pair<wis::Result, wis::DX12ResourceAllocator>
wis::DX12Device::CreateAllocator() const noexcept
{
    D3D12MA::ALLOCATOR_DESC desc{
        .Flags = D3D12MA::ALLOCATOR_FLAGS::ALLOCATOR_FLAG_NONE,
        .pDevice = device.get(),
        .PreferredBlockSize = 0,
        .pAllocationCallbacks = nullptr,
        .pAdapter = adapter.get()
    };
    wis::com_ptr<D3D12MA::Allocator> allocator;
    HRESULT hr;
    return wis::succeeded(hr = D3D12MA::CreateAllocator(&desc, allocator.put()))
        ? std::pair{ wis::success, DX12ResourceAllocator{ std::move(allocator) } }
            : std::pair{ wis::make_result<FUNC, "Failed to create allocator">(hr), DX12ResourceAllocator{} };
}
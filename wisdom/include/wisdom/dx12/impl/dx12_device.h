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
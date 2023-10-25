#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/dx12/xdx12_device.h>
#endif // !WISDOM_HEADER_ONLY
#include <d3dx12/d3dx12_root_signature.h>

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

wis::Result
wis::DX12Device::WaitForMultipleFences(const DX12FenceView* fences,
                                       const uint64_t* values,
                                       uint32_t count,
                                       MutiWaitFlags wait_all,
                                       uint64_t timeout) const noexcept
{
    unique_event e;
    HRESULT hr = S_OK;

    if (!succeeded(hr = device->SetEventOnMultipleFenceCompletion(
                           reinterpret_cast<ID3D12Fence* const*>(fences),
                           values,
                           count,
                           static_cast<D3D12_MULTIPLE_FENCE_WAIT_FLAGS>(wait_all),
                           e.get())))
        return wis::make_result<FUNC, "ID3D12Device10::SetEventOnMultipleFenceCompletion failed to set event on multiple fence completion">(hr);

    auto st = e.wait(timeout);
    return st == wis::Status::Timeout
            ? wis::Result{ st, "Wait timed out" }
            : st != wis::Status::Error
            ? wis::success
            : wis::make_result<FUNC, "Failed to wait for event">(E_FAIL);
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

std::pair<wis::Result, wis::DX12RootSignature>
wis::DX12Device::CreateRootSignature(RootConstant* root_constants, uint32_t constants_size) const noexcept
{
    wis::com_ptr<ID3D12RootSignature> rsig;

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    auto root_params = std::make_unique<D3D12_ROOT_PARAMETER1[]>(constants_size);
    for (uint32_t i = 0; i < constants_size; ++i) {
        auto& param = root_params[i];
        auto& constant = root_constants[i];

        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY(constant.stage);
        param.Constants.Num32BitValues = constant.size_bytes / 4;
        param.Constants.RegisterSpace = 0;
        param.Constants.ShaderRegister = DX12RootSignature::root_const_register;
    }

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(constants_size, root_params.get(), 0, nullptr, flags);

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT hr;

    if (!wis::succeeded(hr = D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put())))
        return std::pair{ wis::make_result<FUNC, "Failed to serialize root signature">(hr), DX12RootSignature{} };

    return (!wis::succeeded(hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(*rsig), rsig.put_void())))
            ? std::pair{ wis::make_result<FUNC, "Failed to create root signature">(hr), DX12RootSignature{} }
            : std::pair{ wis::success, DX12RootSignature{ std::move(rsig) } };
}
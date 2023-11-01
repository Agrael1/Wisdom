#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/dx12/xdx12_device.h>
#endif // !WISDOM_HEADER_ONLY
#include <d3dx12/d3dx12_root_signature.h>
#include <d3dx12/d3dx12_pipeline_state_stream.h>
#include <wisdom/util/small_allocator.h>

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

    std::unique_ptr<D3D12_ROOT_PARAMETER1[]> root_params = constants_size ? std::make_unique<D3D12_ROOT_PARAMETER1[]>(constants_size) : nullptr;
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

std::pair<wis::Result, wis::DX12CommandQueue>
wis::DX12Device::CreateCommandQueue(wis::QueueType type, wis::QueuePriority priority) const noexcept
{
    wis::com_ptr<ID3D12CommandQueue> queue;
    D3D12_COMMAND_QUEUE_DESC desc{
        .Type = D3D12_COMMAND_LIST_TYPE(type),
        .Priority = int(priority),
    };

    HRESULT hr;
    return wis::succeeded(hr = device->CreateCommandQueue(&desc, __uuidof(*queue), queue.put_void()))
            ? std::pair{ wis::success, DX12CommandQueue{ std::move(queue) } }
            : std::pair{ wis::make_result<FUNC, "Failed to create command queue">(hr), DX12CommandQueue{} };
}

std::pair<wis::Result, wis::DX12Shader>
wis::DX12Device::CreateShader(void* data, size_t size) const noexcept
{
    auto x = std::make_unique_for_overwrite<std::byte[]>(size);
    std::copy_n(reinterpret_cast<std::byte*>(data), size, x.get());
    return std::pair{ wis::success, DX12Shader{ std::move(x), size } };
}

namespace wis::detail
{
template<typename Stage>
inline void DX12FillShaderStage(wis::detail::memory_pool<1024>& pipeline_stream, wis::DX12ShaderView shader) noexcept
{
    auto d = std::get<0>(shader);
    if (d) {
        auto s = std::get<1>(shader);
        pipeline_stream.allocate<Stage>() = { d, s };
    }
}
}


std::pair<wis::Result, wis::DX12PipelineState>
wis::DX12Device::CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc* desc) const noexcept
{
    wis::com_ptr<ID3D12PipelineState> state;

    wis::detail::memory_pool pipeline_stream;
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_VS>(pipeline_stream, desc->shaders.vertex);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_PS>(pipeline_stream, desc->shaders.pixel);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_GS>(pipeline_stream, desc->shaders.geometry);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_HS>(pipeline_stream, desc->shaders.hull);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_DS>(pipeline_stream, desc->shaders.domain);

    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE>() = std::get<0>(desc->root_signature);

    // D3D12_INPUT_LAYOUT_DESC iadesc{};
    // iadesc.NumElements = input_layout.size();

    // wis::internals::uniform_allocator<D3D12_INPUT_ELEMENT_DESC> ia;
    // for (auto& i : input_layout) {
    //     ia.allocate(D3D12_INPUT_ELEMENT_DESC{
    //             .SemanticName = i.semantic_name,
    //             .SemanticIndex = i.semantic_index,
    //             .Format = DXGI_FORMAT(i.format),
    //             .InputSlot = i.input_slot,
    //             .AlignedByteOffset = i.aligned_byte_offset,
    //             .InputSlotClass = D3D12_INPUT_CLASSIFICATION(i.input_slot_class),
    //             .InstanceDataStepRate = i.instance_data_step_rate });
    // }
    // iadesc.pInputElementDescs = ia.data();

    // psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT>() = iadesc;

    // auto& rpi = *desc.render_pass.GetInternal().desc;

    // if (rpi.ds_format != DXGI_FORMAT_UNKNOWN) {
    //     psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1>() = CD3DX12_DEPTH_STENCIL_DESC1{ CD3DX12_DEFAULT{} };
    //     psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT>(rpi.ds_format);
    // }
    // if (rpi.rt_formats.size()) {
    //     D3D12_RT_FORMAT_ARRAY rta{
    //         .NumRenderTargets = uint32_t(rpi.rt_formats.size())
    //     };
    //     std::memcpy(rta.RTFormats, rpi.rt_formats.data(), rpi.rt_formats.size() * sizeof(DXGI_FORMAT));
    //     psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;
    // }

    D3D12_PIPELINE_STATE_STREAM_DESC xdesc{
        .SizeInBytes = pipeline_stream.size_bytes(),
        .pPipelineStateSubobjectStream = pipeline_stream.data<void>(),
    };

    HRESULT hr;
    return wis::succeeded(hr = device->CreatePipelineState(&xdesc, __uuidof(*state), state.put_void()))
            ? std::pair{ wis::success, DX12PipelineState{ std::move(state) } }
            : std::pair{ wis::make_result<FUNC, "Failed to create pipeline state">(hr), DX12PipelineState{} };
}
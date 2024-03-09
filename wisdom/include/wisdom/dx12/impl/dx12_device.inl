#include "../dx12_device.h"

bool wis::DX12Device::Initialize(DX12FactoryView in_factory, wis::DX12AdapterView in_adapter) noexcept
{
    factory.copy_from(std::get<0>(in_factory));
    adapter.copy_from(std::get<0>(in_adapter));
    return wis::succeeded(D3D12CreateDevice(adapter.get(),
                                            D3D_FEATURE_LEVEL_11_0,
                                            __uuidof(ID3D12Device9),
                                            device.put_void()));
}

wis::DX12SwapChain wis::DX12Device::CreateSwapchain(wis::DX12CommandQueueView queue, wis::SwapchainOptions options, wis::SurfaceParameters surface) const noexcept
{
    DXGI_SWAP_CHAIN_DESC1 desc{
        .Width = options.width,
        .Height = options.height,
        .Format = DXGI_FORMAT(options.format),
        .Stereo = options.stereo && factory->IsWindowedStereoEnabled(),
        .SampleDesc{ .Count = 1, .Quality = 0 },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = options.frame_count,
        .Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = 0
    };

    winrt::com_ptr<IDXGISwapChain4> chain;
    switch (surface.type) {
    default:
    case SurfaceParameters::Type::Win32:
        chain = SwapChainForWin32(desc, surface.hwnd, std::get<0>(queue));
        break;
#ifdef WISDOM_UWP
    case SurfaceParameters::Type::WinRT:
        chain = SwapChainForCoreWindow(desc, surface.core_window, std::get<0>(queue));
        break;
#endif
    }
    return DX12SwapChain{ std::move(chain), options.frame_count, bool(desc.Stereo) };
}

wis::DX12CommandQueue wis::DX12Device::CreateCommandQueue(wis::QueueOptions options) const noexcept
{
    winrt::com_ptr<ID3D12CommandQueue> queue;
    D3D12_COMMAND_QUEUE_DESC desc{
        .Type = D3D12_COMMAND_LIST_TYPE(options.type),
        .Priority = int(options.priority),
        .Flags = D3D12_COMMAND_QUEUE_FLAGS(options.flags),
        .NodeMask = options.node_mask
    };
    return wis::succeeded(device->CreateCommandQueue(&desc, __uuidof(*queue), queue.put_void()))
            ? DX12CommandQueue{ std::move(queue) }
            : DX12CommandQueue{};
}

wis::DX12CommandList wis::DX12Device::CreateCommandList(QueueType list_type) const noexcept
{
    D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(list_type);
    winrt::com_ptr<ID3D12CommandAllocator> xallocator;
    winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list;

    return wis::succeeded(device->CreateCommandAllocator(clty, __uuidof(*xallocator), xallocator.put_void())) &&
                    wis::succeeded(device->CreateCommandList1(0, clty, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(ID3D12GraphicsCommandList7), xcommand_list.put_void()))
            ? DX12CommandList{
                  std::move(xallocator),
                  std::move(xcommand_list)
              }
            : DX12CommandList{};
}

wis::DX12Fence wis::DX12Device::CreateFence(uint64_t initial_value) const noexcept
{
    winrt::com_ptr<ID3D12Fence1> fence;
    wis::succeeded(device->CreateFence(initial_value, D3D12_FENCE_FLAG_NONE, __uuidof(*fence), fence.put_void()));
    return DX12Fence{ std::move(fence) };
}

wis::DX12RootSignature wis::DX12Device::CreateRootSignature(std::span<DX12DescriptorSetLayout> layouts) const
{
    winrt::com_ptr<ID3D12RootSignature> rsig;

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    wis::internals::uniform_allocator<CD3DX12_ROOT_PARAMETER1> root_parameters;
    for (auto& lay : layouts) {
        auto& ranges = lay.GetInternal().ranges;
        root_parameters.allocate()
                .InitAsDescriptorTable(ranges.size(), ranges.data(), D3D12_SHADER_VISIBILITY_ALL);
    }

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(uint32_t(root_parameters.size()), root_parameters.data(), 0, nullptr, flags);

    winrt::com_ptr<ID3DBlob> signature;
    winrt::com_ptr<ID3DBlob> error;
    wis::succeeded(D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put()));
    wis::succeeded(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(*rsig), rsig.put_void()));
    return DX12RootSignature{ std::move(rsig) };
}

wis::DX12PipelineState wis::DX12Device::CreateGraphicsPipeline(
        const DX12GraphicsPipelineDesc& desc,
        std::span<const InputLayoutDesc> input_layout) const noexcept
{
    winrt::com_ptr<ID3D12PipelineState> state;
    D3D12_PIPELINE_STATE_STREAM_DESC xdesc{};

    D3D12_INPUT_LAYOUT_DESC iadesc{};
    iadesc.NumElements = input_layout.size();

    wis::internals::uniform_allocator<D3D12_INPUT_ELEMENT_DESC> ia;
    for (auto& i : input_layout) {
        ia.allocate(D3D12_INPUT_ELEMENT_DESC{
                .SemanticName = i.semantic_name,
                .SemanticIndex = i.semantic_index,
                .Format = DXGI_FORMAT(i.format),
                .InputSlot = i.input_slot,
                .AlignedByteOffset = i.aligned_byte_offset,
                .InputSlotClass = D3D12_INPUT_CLASSIFICATION(i.input_slot_class),
                .InstanceDataStepRate = i.instance_data_step_rate });
    }
    iadesc.pInputElementDescs = ia.data();

    internals::memory_pool psta;
    psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE>() = desc.sig.GetInternal().root.get();
    psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT>() = iadesc;

    if (desc.vs) {
        auto& d = desc.vs.GetInternal().bytecode;
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_VS>() = { d.data(), d.size() };
    }
    if (desc.ps) {
        auto& d = desc.ps.GetInternal().bytecode;
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_PS>() = { d.data(), d.size() };
    }
    if (desc.gs) {
        auto& d = desc.gs.GetInternal().bytecode;
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_GS>() = { d.data(), d.size() };
    }

    if (desc.hs) {
        auto& d = desc.hs.GetInternal().bytecode;
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_HS>() = { d.data(), d.size() };
    }
    if (desc.ds) {
        auto& d = desc.ds.GetInternal().bytecode;
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DS>() = { d.data(), d.size() };
    }

    auto& rpi = *desc.render_pass.GetInternal().desc;

    if (rpi.ds_format != DXGI_FORMAT_UNKNOWN) {
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1>() = CD3DX12_DEPTH_STENCIL_DESC1{ CD3DX12_DEFAULT{} };
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT>(rpi.ds_format);
    }
    if (rpi.rt_formats.size()) {
        D3D12_RT_FORMAT_ARRAY rta{
            .NumRenderTargets = uint32_t(rpi.rt_formats.size())
        };
        std::memcpy(rta.RTFormats, rpi.rt_formats.data(), rpi.rt_formats.size() * sizeof(DXGI_FORMAT));
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;
    }

    xdesc.pPipelineStateSubobjectStream = psta.data<void>();
    xdesc.SizeInBytes = psta.size_bytes();

    return wis::succeeded(device->CreatePipelineState(&xdesc, __uuidof(*state), state.put_void()))
            ? DX12PipelineState{ std::move(state) }
            : DX12PipelineState{};
}

wis::DX12RenderPass wis::DX12Device::CreateRenderPass(
        wis::Size2D,
        std::span<const ColorAttachment> rtv_descs,
        DepthStencilAttachment dsv_desc) const noexcept
{
    if constexpr (wis::debug_mode) {
        if (rtv_descs.size() >= max_render_targets) {
            wis::lib_error(
                    wis::format("Render Pass has {} render targets, which is more, than {}, excessive targets are truncated.",
                                rtv_descs.size(),
                                wis::max_render_targets));
            return {};
        }
        if (rtv_descs.size() + (dsv_desc.format != DataFormat::unknown) == 0) {
            wis::lib_error("Render Pass has no render targets and no depth stencil target, which is invalid.");
            return {};
        }
    }

    auto render_pass_internal = std::make_shared<DX12RenderPassInternal>();

    for (size_t i = 0; i < rtv_descs.size(); i++) {
        auto& rtv = rtv_descs[i];
        render_pass_internal->rt_descs[i] = {
            .BeginningAccess = { .Type = convert_dx(rtv.load) },
            .EndingAccess = { .Type = convert_dx(rtv.store) }
        };
        if (rtv.load == PassLoadOperation::clear)
            render_pass_internal->rt_descs[i].BeginningAccess.Clear.ClearValue.Format = DXGI_FORMAT(rtv.format);
        render_pass_internal->rt_formats.allocate(DXGI_FORMAT(rtv.format));
    }

    if (dsv_desc.format == DataFormat::unknown)
        return DX12RenderPass{ std::move(render_pass_internal) };

    render_pass_internal->ds_format = DXGI_FORMAT(dsv_desc.format);
    render_pass_internal->ds_desc = {
        .DepthBeginningAccess = { convert_dx(dsv_desc.depth_load), {} },
        .StencilBeginningAccess = { convert_dx(dsv_desc.stencil_load), {} },
        .DepthEndingAccess = { convert_dx(dsv_desc.depth_store), {} },
        .StencilEndingAccess = { convert_dx(dsv_desc.stencil_store), {} }
    };

    if (dsv_desc.depth_load == PassLoadOperation::clear)
        render_pass_internal->ds_desc.DepthBeginningAccess.Clear.ClearValue.Format = DXGI_FORMAT(dsv_desc.format);

    return DX12RenderPass{ std::move(render_pass_internal) };
}

#include <d3d11.h>

inline winrt::com_ptr<ID3D11Device> CreateD3D11Device() noexcept
{
    constexpr D3D_FEATURE_LEVEL featureLevels[]{
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1
    };

    winrt::com_ptr<ID3D11Device> device11;
    D3D11CreateDevice(nullptr,
                      D3D_Dwis_TYPE_HARDWARE,
                      nullptr, 0,
                      featureLevels, 3, D3D11_SDK_VERSION, device11.put(), nullptr, nullptr);
    return device11;
}

winrt::com_ptr<IDXGISwapChain4>
wis::DX12Device::SwapChainForCoreWindow(const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue) const noexcept
{
    winrt::com_ptr<IDXGISwapChain1> swap;
    if (desc.Stereo) // until microsoft fixes this
    {
        wis::succeeded(factory->CreateSwapChainForCoreWindow(
                CreateD3D11Device().get(),
                core_window,
                &desc,
                nullptr,
                swap.put()));
    }

    wis::succeeded(factory->CreateSwapChainForCoreWindow(
            queue, // Swap chain needs the queue so that it can force a flush on it.
            core_window,
            &desc,
            nullptr,
            swap.put()));
    return swap.try_as<IDXGISwapChain4>();
}

winrt::com_ptr<IDXGISwapChain4>
wis::DX12Device::SwapChainForWin32(const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue) const noexcept
{
    winrt::com_ptr<IDXGISwapChain1> swap;
    if (desc.Stereo) // until microsoft fixes this
    {
        wis::succeeded(factory->CreateSwapChainForHwnd(
                CreateD3D11Device().get(), // Swap chain needs the queue so that it can force a flush on it.
                hwnd,
                &desc,
                nullptr,
                nullptr,
                swap.put()));
    }

    wis::succeeded(factory->CreateSwapChainForHwnd(
            queue, // Swap chain needs the queue so that it can force a flush on it.
            hwnd,
            &desc,
            nullptr,
            nullptr,
            swap.put()));
    return swap.try_as<IDXGISwapChain4>();
}

//#include "../dx12_device.h"

bool wis::DX12Device::Initialize(wis::DX12AdapterView adapter) noexcept
{
    if (!wis::succeded(D3D12CreateDevice(std::get<0>(adapter),
                                         D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device9), device.put_void())))
        return false;

    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = heap_size;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    wis::check_hresult(device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(*rtv_heap), rtv_heap.put_void()));
    rtv_increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    rtv_start = rtv_heap->GetCPUDescriptorHandleForHeapStart();

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = dsv_heap_size;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    wis::check_hresult(device->CreateDescriptorHeap(&dsvHeapDesc, __uuidof(*dsv_heap), dsv_heap.put_void()));
    dsv_increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    dsv_start = dsv_heap->GetCPUDescriptorHandleForHeapStart();
    return true;
}

wis::DX12SwapChain wis::DX12Device::CreateSwapchain(wis::DX12CommandQueueView queue, wis::SwapchainOptions options, wis::SurfaceParameters surface) const
{
    DXGI_SWAP_CHAIN_DESC1 desc{
        .Width = options.width,
        .Height = options.height,
        .Format = DXGI_FORMAT(options.format),
        .Stereo = DX12Factory::GetFactory()->IsWindowedStereoEnabled() && options.stereo,
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
        chain = DX12Factory::SwapChainForWin32(desc, surface.hwnd, queue);
        break;
#ifdef WISDOM_UWP
    case SurfaceParameters::Type::WinRT:
        chain = DX12Factory::SwapChainForCoreWindow(desc, surface.core_window, queue);
        break;
#endif
    }
    return DX12SwapChain{ std::move(chain), options.frame_count, bool(desc.Stereo) };
}

wis::DX12CommandQueue wis::DX12Device::CreateCommandQueue(wis::QueueOptions options) const
{
    winrt::com_ptr<ID3D12CommandQueue> queue;
    D3D12_COMMAND_QUEUE_DESC desc{
        .Type = D3D12_COMMAND_LIST_TYPE(options.type),
        .Priority = int(options.priority),
        .Flags = D3D12_COMMAND_QUEUE_FLAGS(options.flags),
        .NodeMask = options.node_mask
    };
    wis::check_hresult(device->CreateCommandQueue(&desc, __uuidof(*queue), queue.put_void()));
    return DX12CommandQueue{ std::move(queue) };
}

wis::DX12CommandList wis::DX12Device::CreateCommandList(QueueType list_type) const
{
    D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(list_type);
    winrt::com_ptr<ID3D12CommandAllocator> xallocator;
    winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list;

    wis::check_hresult(device->CreateCommandAllocator(clty, __uuidof(*xallocator), xallocator.put_void()));
    wis::check_hresult(device->CreateCommandList1(0, clty, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(ID3D12GraphicsCommandList7), xcommand_list.put_void()));

    return DX12CommandList{
        std::move(xallocator),
        std::move(xcommand_list)
    };
}

wis::DX12Fence wis::DX12Device::CreateFence() const
{
    winrt::com_ptr<ID3D12Fence1> fence;
    wis::check_hresult(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(*fence), fence.put_void()));
    return DX12Fence{ std::move(fence) };
}

wis::DX12RootSignature wis::DX12Device::CreateRootSignature(std::span<DX12DescriptorSetLayout> layouts) const
{
    winrt::com_ptr<ID3D12RootSignature> rsig;

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    wis::internals::uniform_allocator<CD3DX12_ROOT_PARAMETER1> root_parameters;
    for (auto& lay : layouts) {
        auto ranges = lay.GetInternal().GetRanges();
        root_parameters.allocate()
                .InitAsDescriptorTable(ranges.size(), ranges.data(), D3D12_SHADER_VISIBILITY_ALL);
    }

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(uint32_t(root_parameters.size()), root_parameters.data(), 0, nullptr, flags);

    winrt::com_ptr<ID3DBlob> signature;
    winrt::com_ptr<ID3DBlob> error;
    wis::check_hresult(D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put()));
    wis::check_hresult(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(*rsig), rsig.put_void()));
    return DX12RootSignature{ std::move(rsig) };
}

wis::DX12PipelineState wis::DX12Device::CreateGraphicsPipeline(
        const DX12GraphicsPipelineDesc& desc,
        std::span<const InputLayoutDesc> input_layout) const // movable
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
    psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE>() = desc.sig.GetInternal().GetRootSignature();
    psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT>() = iadesc;

    if (desc.depth_enabled)
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL2>() = CD3DX12_DEPTH_STENCIL_DESC2{ CD3DX12_DEFAULT{} };

    if (desc.vs) {
        auto d = desc.vs.GetInternal().GetShaderBytecode();
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_VS>() = { d.data(), d.size() };
    }
    if (desc.ps) {
        auto d = desc.ps.GetInternal().GetShaderBytecode();
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_PS>() = { d.data(), d.size() };
    }
    if (desc.gs) {
        auto d = desc.gs.GetInternal().GetShaderBytecode();
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_GS>() = { d.data(), d.size() };
    }

    if (desc.hs) {
        auto d = desc.hs.GetInternal().GetShaderBytecode();
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_HS>() = { d.data(), d.size() };
    }
    if (desc.ds) {
        auto d = desc.ds.GetInternal().GetShaderBytecode();
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DS>() = { d.data(), d.size() };
    }
    if (desc.target_formats.size()) {
        D3D12_RT_FORMAT_ARRAY rta{
            .NumRenderTargets = uint32_t(desc.target_formats.size())
        };
        std::memcpy(rta.RTFormats, desc.target_formats.data(), desc.target_formats.size() * sizeof(DataFormat));
        psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;
    }
    xdesc.pPipelineStateSubobjectStream = psta.data<void>();
    xdesc.SizeInBytes = psta.size_bytes();

    wis::check_hresult(device->CreatePipelineState(&xdesc, __uuidof(*state), state.put_void()));
    return DX12PipelineState{ std::move(state) };
}

wis::DX12RenderPass wis::DX12Device::CreateRenderPass(
        wis::Size2D,
        std::span<const ColorAttachment> rtv_descs,
        DepthStencilAttachment dsv_desc,
        SampleCount samples,
        DataFormat vrs_format) const
{
    std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> om_rtv;
    om_rtv.reserve(rtv_descs.size());

    for (auto& x : rtv_descs) {
        D3D12_RENDER_PASS_BEGINNING_ACCESS begin{
            .Type = convert_dx(x.load)
        };
        D3D12_RENDER_PASS_ENDING_ACCESS end{
            .Type = convert_dx(x.store)
        };
        om_rtv.push_back({ 0, begin, end });
    }

    wis::internals::uniform_allocator<DataFormat, max_render_targets> a;
    for (auto& i : rtv_descs)
        a.allocate() = i.format;

    if (dsv_desc.format == DataFormat::unknown)
        return DX12RenderPass{ a, std::move(om_rtv) };

    D3D12_RENDER_PASS_BEGINNING_ACCESS depth_begin{ convert_dx(dsv_desc.depth_load), {} };
    D3D12_RENDER_PASS_ENDING_ACCESS depth_end{ convert_dx(dsv_desc.depth_store), {} };
    D3D12_RENDER_PASS_BEGINNING_ACCESS stencil_begin{ convert_dx(dsv_desc.stencil_load), {} };
    D3D12_RENDER_PASS_ENDING_ACCESS stencil_end{ convert_dx(dsv_desc.stencil_store), {} };

    return DX12RenderPass{ a, std::move(om_rtv), D3D12_RENDER_PASS_DEPTH_STENCIL_DESC{ 0, depth_begin, stencil_begin, depth_end, stencil_end } };
}

wis::DX12RenderTargetView wis::DX12Device::CreateRenderTargetView(DX12TextureView texture, RenderSelector range)
{
    D3D12_RENDER_TARGET_VIEW_DESC desc{
        .Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,
        .ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2DARRAY,
        .Texture2DArray{
                .MipSlice = range.mip,
                .FirstArraySlice = range.base_layer,
                .ArraySize = range.extent_layers,
                .PlaneSlice = 0 }
    };
    device->CreateRenderTargetView(texture, &desc, rtv_start);
    DX12RenderTargetView rtvm{ rtv_start };
    rtv_start.Offset(1, rtv_increment);
    rtv_index++;
    if (rtv_index == heap_size) {
        rtv_index = 0;
        rtv_start = rtv_heap->GetCPUDescriptorHandleForHeapStart();
    }
    return rtvm;
}

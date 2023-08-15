#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_descriptor_heap.h>
#include <wisdom/dx12/dx12_state_builder.h>
#include <wisdom/api/api_input_layout.h>
#include <wisdom/global/assertions.h>
#include <wisdom/util/small_allocator.h>
#endif

namespace wis {
class DX12Device;

template<>
struct Internal<DX12Device> {
    winrt::com_ptr<ID3D12Device10> device;
    winrt::com_ptr<IDXGIAdapter1> adapter;
    winrt::com_ptr<IDXGIFactory4> factory;
};

/// @brief A DX12 device
WIS_EXPORT class DX12Device : public QueryInternal<DX12Device>
{
public:
    DX12Device() = default;
    explicit DX12Device(DX12FactoryHandle factory, DX12AdapterView adapter) noexcept
    {
        Initialize(factory, adapter);
    };

    /// @brief Initialize the device
    /// @param adapter The adapter to use
    /// @return true if the device was initialized successfully
    WIS_INLINE bool Initialize(DX12FactoryHandle factory, DX12AdapterView adapter) noexcept;

public:
    explicit operator bool() const noexcept
    {
        return bool(device);
    }
    operator DX12DeviceView() const noexcept
    {
        return device.get();
    }

public:
    /// @brief Create a swapchain
    /// @param queue The queue to use
    /// @param options The options to use
    /// @param surface The surface to use
    /// @return The created swapchain
    [[nodiscard]] WIS_INLINE DX12SwapChain
    CreateSwapchain(
            DX12CommandQueueView queue,
            wis::SwapchainOptions options,
            wis::SurfaceParameters surface) const noexcept;

    /// @brief Create a command queue
    /// @param options The options to use
    /// @return The created command queue
    [[nodiscard]] DX12CommandQueue
    CreateCommandQueue(QueueOptions options = QueueOptions{}) const noexcept
    {
        winrt::com_ptr<ID3D12CommandQueue> queue;
        D3D12_COMMAND_QUEUE_DESC desc{
            .Type = D3D12_COMMAND_LIST_TYPE(options.type),
            .Priority = int(options.priority),
            .Flags = D3D12_COMMAND_QUEUE_FLAGS(options.flags),
            .NodeMask = options.node_mask
        };
        return wis::succeded(device->CreateCommandQueue(&desc, __uuidof(*queue), queue.put_void()))
                ? DX12CommandQueue{ std::move(queue) }
                : DX12CommandQueue{};
    }

    /// @brief Create a command list
    /// @param list_type The type of list to create
    /// @return The created command list
    [[nodiscard]] DX12CommandList
    CreateCommandList(QueueType list_type) const noexcept
    {
        D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(list_type);
        winrt::com_ptr<ID3D12CommandAllocator> xallocator;
        winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list;

        return !wis::succeded(device->CreateCommandAllocator(clty, __uuidof(*xallocator), xallocator.put_void())) ||
                        !wis::succeded(device->CreateCommandList(0, clty, xallocator.get(), nullptr, __uuidof(*xcommand_list), xcommand_list.put_void()))
                ? DX12CommandList{}
                : DX12CommandList{
                      std::move(xallocator),
                      std::move(xcommand_list)
                  };
    }

    [[nodiscard]] DX12Fence
    CreateFence(uint64_t initial_value) const noexcept
    {
        winrt::com_ptr<ID3D12Fence1> fence;
        return wis::succeded(device->CreateFence(initial_value, D3D12_FENCE_FLAG_NONE, __uuidof(*fence), fence.put_void()))
                ? DX12Fence{ std::move(fence) }
                : DX12Fence{};
    }

    [[nodiscard]] DX12DescriptorHeap
    CreateDescriptorHeap(PoolType type, uint32_t num_descs, PoolFlags flags) const noexcept
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE(type),
            .NumDescriptors = num_descs,
            .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS(flags),
            .NodeMask = 0u
        };

        winrt::com_ptr<ID3D12DescriptorHeap> heap;
        return wis::succeded(device->CreateDescriptorHeap(&desc, __uuidof(*heap), heap.put_void()))
                ? DX12DescriptorHeap{ std::move(heap), device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE(type)) }
                : DX12DescriptorHeap{};
    }

    /// @brief Create a shader
    /// @param blob The shader blob
    /// @param type The shader type
    [[nodiscard]] DX12Shader
    CreateShader(shared_blob blob, ShaderType type) const noexcept
    {
        return DX12Shader{ std::move(blob), type };
    }

    /// @brief Create a render target view
    /// @param texture The texture to create the view for
    /// @param range The range of the view
    [[nodiscard]] DX12RenderTarget
    CreateRenderTarget(DX12TextureView texture, wis::DataFormat format, RenderTargetSelector range = {}) const noexcept
    {
        D3D12_RENDER_TARGET_VIEW_DESC desc{
            .Format = DXGI_FORMAT(format),
            .ViewDimension = D3D12_RTV_DIMENSION(range.type),
            .Texture2DArray{
                    .MipSlice = range.mip,
                    .FirstArraySlice = range.base_layer,
                    .ArraySize = range.extent_layers,
                    .PlaneSlice = 0 }
        };
        switch (range.type) {
        case TextureType::Texture1D:
            desc.Texture1D = {
                .MipSlice = range.mip
            };
            break;
        case TextureType::Texture1DArray:
            desc.Texture1DArray = {
                .MipSlice = range.mip,
                .FirstArraySlice = range.base_layer,
                .ArraySize = range.extent_layers
            };
            break;
        case TextureType::Texture2D:
            desc.Texture2D = {
                .MipSlice = range.mip
            };
            break;
        case TextureType::Texture2DArray:
            desc.Texture2DArray = {
                .MipSlice = range.mip,
                .FirstArraySlice = range.base_layer,
                .ArraySize = range.extent_layers
            };
            break;
        case TextureType::Texture2DMSArray:
            desc.Texture2DMSArray = {
                .FirstArraySlice = range.base_layer,
                .ArraySize = range.extent_layers
            };
            break;
        case TextureType::Texture3D:
            desc.Texture3D = {
                .MipSlice = range.mip,
                .FirstWSlice = range.base_layer,
                .WSize = range.extent_layers
            };
            break;
        default:
            break;
        }

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
            .NumDescriptors = 1,
            .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
            .NodeMask = 0u
        };

        winrt::com_ptr<ID3D12DescriptorHeap> heap;
        if (!wis::succeded(device->CreateDescriptorHeap(&heap_desc, __uuidof(*heap), heap.put_void())))
            return;

        device->CreateRenderTargetView(texture, &desc, heap->GetCPUDescriptorHandleForHeapStart());
        return DX12RenderTarget{ std::move(heap) };
    }

    [[nodiscard]] DX12DepthStencil
    CreateDepthStencil(DX12TextureView texture, wis::DataFormat format) const noexcept
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC desc{
            .Format = DXGI_FORMAT(format),
            .ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D, // only 2D for now
            .Texture2D{.MipSlice = 0,}
        };

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
            .NumDescriptors = 1,
            .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
            .NodeMask = 0u
        };

        winrt::com_ptr<ID3D12DescriptorHeap> heap;
        if (!wis::succeded(device->CreateDescriptorHeap(&heap_desc, __uuidof(*heap), heap.put_void())))
            return;

        device->CreateDepthStencilView(texture, &desc, heap->GetCPUDescriptorHandleForHeapStart());
        return DX12DepthStencil{ std::move(heap) };
    }

    /// @brief Create a root signature
    [[nodiscard]] DX12RootSignature CreateRootSignature(std::span<DX12DescriptorSetLayout> layouts = {}) const noexcept
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

    /// @brief Create a graphics pipeline
    [[nodiscard]] WIS_INLINE DX12PipelineState CreateGraphicsPipeline(
            const DX12GraphicsPipelineDesc& desc,
            std::span<const InputLayoutDesc> input_layout) const;

    /// @brief Create a render pass
    /// @param rtv_descs The render target descriptions
    /// @param dsv_desc The depth stencil description
    /// @param samples The sample count
    /// @param vrs_format The variable rate shading format
    [[nodiscard]] WIS_INLINE DX12RenderPass CreateRenderPass(
            wis::Size2D,
            std::span<const ColorAttachment> rtv_descs,
            DepthStencilAttachment dsv_desc = DepthStencilAttachment{},
            SampleCount samples = SampleCount::s1,
            DataFormat vrs_format = DataFormat::unknown) const;

    [[nodiscard]] DX12DescriptorSetLayout CreateDescriptorSetLayout(std::span<BindingDescriptor> descs) const
    {
        std::vector<CD3DX12_DESCRIPTOR_RANGE1> ranges;

        for (auto& desc : descs) {
            auto& range = ranges.emplace_back();
            range.Init(D3D12_DESCRIPTOR_RANGE_TYPE(desc.type), desc.count, desc.binding, 0);
        }
        return DX12DescriptorSetLayout{ std::move(ranges) };
    }

    void CreateConstantBufferView(DX12BufferView buffer, uint32_t size, DX12DescriptorSetView set, DX12DescriptorSetLayoutView layout, uint32_t binding = 0) const
    {
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = buffer->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = size;
        auto it = std::ranges::find_if(layout, [binding](auto& range) {
            return range.BaseShaderRegister == binding;
        });
        if (it == layout.end())
            return;
        size_t index = std::distance(layout.begin(), it);
        auto begin = std::get<0>(set);
        auto end = std::get<1>(set);
        auto increment = std::get<2>(set);
        auto handle = begin.Offset(index, increment);
        wis::assert_debug(handle.ptr < end.ptr, wis::format("Handle for constant buffer {} is out of range", index));
        device->CreateConstantBufferView(&cbvDesc, handle);
    }

    void CreateSampler(DX12DescriptorSetView set, uint32_t index = 0)
    {
        auto begin = std::get<0>(set);
        auto end = std::get<1>(set);
        auto increment = std::get<2>(set);
        auto handle = begin.Offset(index, increment);

        D3D12_SAMPLER_DESC sampler{
            .Filter = D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            .AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            .AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            .AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            .MipLODBias = 0,
            .MaxAnisotropy = 0,
            .ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER,
            .BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
            .MinLOD = 0,
            .MaxLOD = D3D12_FLOAT32_MAX
        };
        device->CreateSampler(&sampler, handle);
    }
};
} // namespace wis

// #if defined(WISDOM_HEADER_ONLY)
// #include "impl/dx12_device.inl"
// #endif

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
                      D3D_DRIVER_TYPE_HARDWARE,
                      nullptr, 0,
                      featureLevels, 3, D3D11_SDK_VERSION, device11.put(), nullptr, nullptr);
    return device11;
}

inline winrt::com_ptr<IDXGISwapChain4> SwapChainForCoreWindow(IDXGIFactory4* factory, const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue) noexcept
{
    winrt::com_ptr<IDXGISwapChain1> swap;
    if (desc.Stereo) // until microsoft fixes this
    {
        wis::succeded(factory->CreateSwapChainForCoreWindow(
                CreateD3D11Device().get(),
                core_window,
                &desc,
                nullptr,
                swap.put()));
    }

    wis::succeded(factory->CreateSwapChainForCoreWindow(
            queue, // Swap chain needs the queue so that it can force a flush on it.
            core_window,
            &desc,
            nullptr,
            swap.put()));
    return swap.try_as<IDXGISwapChain4>();
}

inline winrt::com_ptr<IDXGISwapChain4> SwapChainForWin32(IDXGIFactory4* factory, const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue) noexcept
{
    winrt::com_ptr<IDXGISwapChain1> swap;
    if (desc.Stereo) // until microsoft fixes this
    {
        wis::succeded(factory->CreateSwapChainForHwnd(
                CreateD3D11Device().get(), // Swap chain needs the queue so that it can force a flush on it.
                hwnd,
                &desc,
                nullptr,
                nullptr,
                swap.put()));
    }

    wis::succeded(factory->CreateSwapChainForHwnd(
            queue, // Swap chain needs the queue so that it can force a flush on it.
            hwnd,
            &desc,
            nullptr,
            nullptr,
            swap.put()));
    return swap.try_as<IDXGISwapChain4>();
}

bool wis::DX12Device::Initialize(DX12FactoryHandle hfactory, DX12AdapterView hadapter) noexcept
{
    adapter.copy_from(hadapter);
    factory.copy_from(std::get<0>(hfactory));
    return wis::succeded(D3D12CreateDevice(hadapter,
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
        .Stereo = factory->IsWindowedStereoEnabled() && options.stereo,
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
        chain = SwapChainForWin32(factory.get(), desc, surface.hwnd, queue);
        break;
#ifdef WISDOM_UWP
    case SurfaceParameters::Type::WinRT:
        chain = SwapChainForCoreWindow(factory.get(), desc, surface.core_window, queue);
        break;
#endif
    }
    return DX12SwapChain{ std::move(chain), options.frame_count, bool(desc.Stereo) };
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

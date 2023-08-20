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

WIS_EXPORT namespace wis
{
    class DX12Device;

    template<>
    class Internal<DX12Device>
    {
    public:
        static inline constexpr auto heap_size = 128u;
        static inline constexpr auto dsv_heap_size = 6u;

    public:
        winrt::com_ptr<ID3D12Device10> device;
        winrt::com_ptr<IDXGIAdapter1> adapter;
        winrt::com_ptr<IDXGIFactory4> factory;
    };

    /// @brief A DX12 device
    class DX12Device final : public QueryInternal<DX12Device>
    {
    public:
        DX12Device() = default;
        explicit DX12Device(DX12FactoryView factory, DX12AdapterView adapter)
        {
            Initialize(factory, adapter);
        };

    public:
        /// @brief Initialize the device
        /// @param adapter The adapter to use
        /// @return true if the device was initialized successfully
        WIS_INLINE bool Initialize(DX12FactoryView factory, DX12AdapterView adapter) noexcept;
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
        [[nodiscard]] WIS_INLINE DX12SwapChain CreateSwapchain(
                DX12CommandQueueView queue,
                wis::SwapchainOptions options,
                wis::SurfaceParameters surface) const;

        /// @brief Create a command queue
        /// @param options The options to use
        /// @return The created command queue
        [[nodiscard]] WIS_INLINE DX12CommandQueue
        CreateCommandQueue(QueueOptions options = QueueOptions{}) const;

        /// @brief Create a command list
        /// @param list_type The type of list to create
        /// @return The created command list
        [[nodiscard]] WIS_INLINE DX12CommandList CreateCommandList(QueueType list_type) const;

        /// @brief Create a fence
        [[nodiscard]] WIS_INLINE DX12Fence CreateFence() const;

        /// @brief Create a root signature (empty)
        [[nodiscard]] WIS_INLINE DX12RootSignature CreateRootSignature(std::span<DX12DescriptorSetLayout> layouts = {}) const;

        /// @brief Create a graphics pipeline
        [[nodiscard]] WIS_INLINE DX12PipelineState CreateGraphicsPipeline(
                const DX12GraphicsPipelineDesc& desc,
                std::span<const InputLayoutDesc> input_layout) const noexcept;

        /// @brief Create a shader
        /// @param blob The shader blob
        /// @param type The shader type
        [[nodiscard]] DX12Shader CreateShader(shared_blob blob, ShaderType type) const noexcept
        {
            return DX12Shader{ std::move(blob), type };
        }

        /// @brief Create a render pass
        /// @param rtv_descs The render target descriptions
        /// @param dsv_desc The depth stencil description
        /// @param samples The sample count
        /// @param vrs_format The variable rate shading format
        [[nodiscard]] WIS_INLINE DX12RenderPass CreateRenderPass(
                wis::Size2D,
                std::span<const ColorAttachment> rtv_descs,
                DepthStencilAttachment dsv_desc = DepthStencilAttachment{}) const noexcept;

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
                return {};

            device->CreateRenderTargetView(texture, &desc, heap->GetCPUDescriptorHandleForHeapStart());
            return DX12RenderTarget{ std::move(heap) };
        }

        [[nodiscard]] DX12DepthStencil
        CreateDepthStencil(DX12TextureView texture, wis::DataFormat format) const noexcept
        {
            D3D12_DEPTH_STENCIL_VIEW_DESC desc{
                .Format = DXGI_FORMAT(format),
                .ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D, // only 2D for now
                .Texture2D{
                        .MipSlice = 0,
                }
            };

            D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
                .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                .NumDescriptors = 1,
                .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                .NodeMask = 0u
            };

            winrt::com_ptr<ID3D12DescriptorHeap> heap;
            if (!wis::succeded(device->CreateDescriptorHeap(&heap_desc, __uuidof(*heap), heap.put_void())))
                return {};

            device->CreateDepthStencilView(texture, &desc, heap->GetCPUDescriptorHandleForHeapStart());
            return DX12DepthStencil{ std::move(heap) };
        }

        // TODO:Comment
        [[nodiscard]] DX12DescriptorHeap CreateDescriptorHeap(uint32_t num_descs, PoolType type) const
        {
            D3D12_DESCRIPTOR_HEAP_DESC desc{
                .Type = D3D12_DESCRIPTOR_HEAP_TYPE(type),
                .NumDescriptors = num_descs,
                .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                .NodeMask = 0u
            };

            winrt::com_ptr<ID3D12DescriptorHeap> heap;
            wis::check_hresult(device->CreateDescriptorHeap(&desc, __uuidof(*heap), heap.put_void()));
            return DX12DescriptorHeap{ std::move(heap), device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE(type)) };
        }

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

    protected:
        WIS_INLINE [[nodiscard]] winrt::com_ptr<IDXGISwapChain4>
        SwapChainForCoreWindow(const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue) const noexcept;

        WIS_INLINE [[nodiscard]] winrt::com_ptr<IDXGISwapChain4>
        SwapChainForWin32(const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue) const noexcept;
    };
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_device.inl"
#endif

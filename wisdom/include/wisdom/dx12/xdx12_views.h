#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <tuple>

namespace wis {
    using DX12AdapterHandle = std::tuple<IDXGIAdapter1*>;
    using DX12FactoryHandle = std::tuple<IDXGIFactory6*>;

    using DX12FenceView = std::tuple<ID3D12Fence*>;
}
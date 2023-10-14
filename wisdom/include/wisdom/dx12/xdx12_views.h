#pragma once
#include <dxgi1_6.h>
#include <tuple>

namespace wis {
    using DX12AdapterHandle = std::tuple<IDXGIAdapter1*>;
    using DX12FactoryHandle = std::tuple<IDXGIFactory6*>;
}
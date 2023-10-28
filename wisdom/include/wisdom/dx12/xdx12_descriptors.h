#pragma once
#include <wisdom/api/internal.h>
#include <d3d12.h>
#include <optional>

namespace wis {
struct DX12DescriptorLayout {
    D3D12_ROOT_PARAMETER1 layout;
};
} // namespace wis
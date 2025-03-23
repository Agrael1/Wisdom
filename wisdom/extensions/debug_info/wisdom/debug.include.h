#pragma once
#include <array>
#include <wisdom/global/definitions.h>
#include <wisdom/global/func.h>
#include <wisdom/bridge/format.h>

#if defined(WISDOM_DX12)
#include <d3d12.h>
#endif // defined(WISDOM_DX12)

#if defined(WISDOM_VULKAN)
#include <vulkan/vulkan.h>
#endif // defined(WISDOM_VULKAN)
#pragma once
#include <wisdom/bridge/format.h>

#if defined(_WIN32) && WISDOMDX12
#include <wisdom/dx12/dx12_definitions.h>
#elif defined(WISDOMVK)
#include <wisdom/vulkan/vk_definitions.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif
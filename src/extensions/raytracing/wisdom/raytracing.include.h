#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <cstring>
#include <span>
#include <wisdom/global/definitions.h>

#if defined(WISDOM_VULKAN)
#include <vulkan/vulkan.h>
#include <wisvk/vk_movable.hpp>
#include <wisdom/generated/vk_functions.hpp>
#endif

#if defined(WISDOM_DX12)
#include <d3d12.h>
#endif

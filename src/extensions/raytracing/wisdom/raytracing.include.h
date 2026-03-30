#pragma once
#include <wisdom/global/definitions.h>

#include <cstring>
#include <memory>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>

#if defined(WISDOM_VULKAN)
#    include <wisdom/generated/vk_functions.hpp>

#    include <vulkan/vulkan.h>
#    include <wisvk/vk_movable.hpp>
#endif

#if defined(WISDOM_DX12)
#    include <d3d12.h>
#endif

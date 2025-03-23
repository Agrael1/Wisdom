#pragma once
#include <vulkan/vulkan.h>
#include <wisvk/vk_libinit.hpp>
#include <wisvk/vk_managed_handles.hpp>
#include <wisvk/vk_movable.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/bridge/format.h>
#include <vk_mem_alloc.h>
#include <array>
#include <memory>
#include <span>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <ranges>

#if defined(__clang__) || defined(__GNUC__)
#define FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC __FUNCSIG__
#else
#define FUNC __func__
#endif
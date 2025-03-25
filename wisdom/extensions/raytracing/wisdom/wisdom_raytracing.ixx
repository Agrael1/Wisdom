module;
#define WISDOM_MODULE_DECL
#include <wisdom/raytracing.include.h>
export module wisdom.raytracing;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#if defined(WISDOM_DX12)
#include <wisdom/dx12_raytracing_pipeline.h>
#include <wisdom/impl.dx12.h>
#endif

#if defined(WISDOM_VULKAN)
#include <wisdom/vk_rtas.h>
#include <wisdom/vk_raytracing_pipeline.h>
#include <wisdom/impl.vk.h>
#endif

#include <wisdom/impl.h>
#include <wisdom/wisdom_raytracing.hpp>

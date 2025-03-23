module;
#include <span>
import wisdom.api;
import wisdom.internal;
#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif
#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif
#define WISDOM_SILENCE_API_HEADERS
#include <wisdom/wisdom_debug.hpp>
export module wisdom.debug;

#if defined(WISDOM_DX12)
export wis::DX12DebugExtension;
#endif

#if defined(WISDOM_VULKAN)
export wis::VKDebugExtension;
#endif

export namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using DebugExtension = DX12DebugExtension;
#elif defined(WISDOM_VULKAN)
using DebugExtension = VKDebugExtension;
#endif // WISDOM_DX12
} // namespace wis

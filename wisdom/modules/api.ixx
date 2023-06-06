module;
#include <wisdom/bridge/format.h>
#include <wisdom/bridge/source_location.h>
#include <wisdom/bridge/generator.h>
#include <ranges>
#include <exception>

#include <cassert>

#ifdef WISDOM_WINDOWS
#include <windows.h>
#endif // WISDOM_WINDOWS
#ifdef WISDOM_LINUX
#include <xcb/xproto.h>
#endif // WISDOM_LINUX

#pragma warning(disable: 5244) //includes are interop, and not a mistake
#define WISDOM_MODULES
export module wisdom.api;

#include <wisdom/global/definitions.h>
#include <wisdom/util/flags.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/exception.h>
#include <wisdom/util/misc.h>
#include <wisdom/util/small_allocator.h>

#include <wisdom/api/api_common.h>

#include <wisdom/api/api_adapter.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/api/api_factory.h>
#include <wisdom/api/api_input_layout.h>
#include <wisdom/api/api_render_pass.h>
#include <wisdom/api/api_shader.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>

export namespace wis
{
	using wis::generator;
	using wis::source_location;
	using wis::format;
	using wis::format_to;
}
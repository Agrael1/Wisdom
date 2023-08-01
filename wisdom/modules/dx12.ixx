module;
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d12sdklayers.h>
#include <d3dx12/d3dx12.h>
#include <dxgidebug.h>
#include <wisdom/global/definitions.h>
#include <D3D12MemAlloc.h>
#include <span>
#include <array>
#include <winrt/base.h>

#pragma warning(disable : 5244) // includes are interop, and not a mistake
#define WISDOM_MODULES
export module wisdom.dx12;

import wisdom.api;

#include <wisdom/dx12/dx12_format.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_info.h>

#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_buffer_views.h>
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_allocator.h>
#include <wisdom/dx12/dx12_render_pass.h>
#include <wisdom/dx12/dx12_pipeline_state.h>

#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_rtv.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_state_builder.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_swapchain.h>

#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_device.h>

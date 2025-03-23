#pragma once
#include <string>
#include <array>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3D12MemAlloc.h>
#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_property_format_table.h>
#include <d3dx12/d3dx12_resource_helpers.h>
#include <d3dx12/d3dx12_pipeline_state_stream.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <semaphore>
#include <unordered_map>
#include <memory>
#include <dxgidebug.h>
#include <bit>
#include <cassert>
#include <wisdom/global/definitions.h>

#if defined(__clang__) || defined(__GNUC__)
#define FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC __FUNCSIG__
#else
#define FUNC __func__
#endif


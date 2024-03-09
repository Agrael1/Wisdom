#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/util/flags.h>
#include <wisdom/api/api_render_pass.h>
#include <wisdom/api/api_barrier.h>
#include <d3d12.h>
#include <wisdom/api/api_factory.h>
#include <dxgi1_6.h>
#endif

namespace wis {
inline constexpr D3D12_BARRIER_LAYOUT convert_dx(TextureState op) noexcept
{
    if (op == TextureState::Present)
        return D3D12_BARRIER_LAYOUT::D3D12_BARRIER_LAYOUT_PRESENT;
    return D3D12_BARRIER_LAYOUT(op);
}
inline constexpr D3D12_BARRIER_ACCESS convert_dx(ResourceAccess op) noexcept
{
    return D3D12_BARRIER_ACCESS(op);
}

inline constexpr D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE convert_dx(PassLoadOperation op) noexcept
{
    switch (op) {
    default:
    case PassLoadOperation::load:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
    case PassLoadOperation::clear:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    case PassLoadOperation::discard:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
    }
}
inline constexpr D3D12_RENDER_PASS_ENDING_ACCESS_TYPE convert_dx(PassStoreOperation op) noexcept
{
    switch (op) {
    default:
    case PassStoreOperation::store:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
    case PassStoreOperation::discard:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
    }
}
inline constexpr D3D12_RESOURCE_FLAGS convert_dx(TextureFlags op) noexcept
{
    using namespace wis::flags;
    D3D12_RESOURCE_FLAGS ret = D3D12_RESOURCE_FLAG_NONE;
    if (!(op & TextureFlags::ShaderResource))
        ret |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    if (op & TextureFlags::RenderTarget)
        ret |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    if (op & TextureFlags::DepthStencil)
        ret |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    if (op & TextureFlags::Storage)
        ret |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    return ret;
}
inline constexpr D3D12_SHADER_VISIBILITY convert_dx(ShaderStage op) noexcept
{
    switch (op) {
    case wis::ShaderStage::vertex:
        return D3D12_SHADER_VISIBILITY_VERTEX;
    case wis::ShaderStage::hull:
        return D3D12_SHADER_VISIBILITY_HULL;
    case wis::ShaderStage::domain:
        return D3D12_SHADER_VISIBILITY_DOMAIN;
    case wis::ShaderStage::geometry:
        return D3D12_SHADER_VISIBILITY_GEOMETRY;
    case wis::ShaderStage::pixel:
        return D3D12_SHADER_VISIBILITY_PIXEL;
    case wis::ShaderStage::compute:
        return D3D12_SHADER_VISIBILITY_ALL;
    default:
    case wis::ShaderStage::all:
        break;
    }
}
inline constexpr DXGI_GPU_PREFERENCE to_dxgi(wis::AdapterPreference pref) noexcept
{
    switch (pref) {
    default:
    case wis::AdapterPreference::None:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_UNSPECIFIED;
    case wis::AdapterPreference::MinConsumption:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER;
    case wis::AdapterPreference::Performance:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
    }
}

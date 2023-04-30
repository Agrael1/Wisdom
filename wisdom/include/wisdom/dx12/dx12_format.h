#pragma once
#include <wisdom/api/api_render_pass.h>
#include <wisdom/api/api_barrier.h>
#include <d3d12.h>

namespace wis
{
    
    inline constexpr D3D12_BARRIER_LAYOUT convert_dx(TextureState op)noexcept
    {
        if (op == TextureState::Present)return D3D12_BARRIER_LAYOUT::D3D12_BARRIER_LAYOUT_PRESENT;
        return D3D12_BARRIER_LAYOUT(op);
    }
    inline constexpr D3D12_BARRIER_ACCESS convert_dx(ResourceAccess op)noexcept
    {
        return D3D12_BARRIER_ACCESS(op);
    }

    inline constexpr D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE convert_dx(PassLoadOperation op)noexcept
    {
        switch (op)
        {
        default:
        case PassLoadOperation::load:
            return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
        case PassLoadOperation::clear:
            return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
        case PassLoadOperation::discard:
            return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
        }
    } 
    inline constexpr D3D12_RENDER_PASS_ENDING_ACCESS_TYPE convert_dx(PassStoreOperation op)noexcept
    {
        switch (op)
        {
        default:
        case PassStoreOperation::store:
            return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
        case PassStoreOperation::discard:
            return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
        }
    }
}
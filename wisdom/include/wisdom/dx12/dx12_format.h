#pragma once
#include <wisdom/api/api_render_pass.h>
#include <d3d12.h>

namespace wis
{
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
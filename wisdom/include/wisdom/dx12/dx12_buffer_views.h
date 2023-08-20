#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <d3d12.h>
#endif

WIS_EXPORT namespace wis
{
    class DX12VertexBufferView;

    template<>
    class Internal<DX12VertexBufferView>
    {
    public:
        D3D12_VERTEX_BUFFER_VIEW view;
    };

    /// @brief Vertex buffer view
    class DX12VertexBufferView : public QueryInternal<DX12VertexBufferView>
    {
    public:
        DX12VertexBufferView() = default;
        explicit DX12VertexBufferView(D3D12_VERTEX_BUFFER_VIEW xhandle)
            : QueryInternal(xhandle) { }

        /// @brief Get the view
        /// @note This function is only available in the internal API
        operator D3D12_VERTEX_BUFFER_VIEW() const noexcept
        {
            return view;
        }
    };
}

#pragma once
#include <wisdom/api/api.h>
#include <wisdom/api/internal.h>
#include <wisdom/util/com_ptr.h>

#include <dxgi1_6.h>


namespace wis {
class DX12Adapter;

template<>
struct Internal<DX12Adapter> {
    wis::com_ptr<IDXGIAdapter1> adapter;
};


class DX12Adapter : public QueryInternal<DX12Adapter>
{
public:
    DX12Adapter() noexcept = default;
    explicit DX12Adapter(wis::com_ptr<IDXGIAdapter1> adapter) noexcept
        : QueryInternal(std::move(adapter))
    {}
    operator bool() const noexcept { return bool(adapter); }
public:

};
}
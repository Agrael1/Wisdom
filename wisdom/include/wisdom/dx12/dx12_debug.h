#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_info.h>
#include <memory>

namespace wis {
class DX12DebugMessenger;

template<>
struct Internal<DX12DebugMessenger> {
    DX12InfoToken info;
};

class DX12DebugMessenger : public QueryInternal<DX12DebugMessenger>
{
public:
    DX12DebugMessenger() = default;
    DX12DebugMessenger(DX12InfoToken info, DebugCallback callback, void* user_data) noexcept
        : QueryInternal(std::move(info))
    {
        DX12Info::AddCallback(this, callback, user_data);
    }
    DX12DebugMessenger(DX12DebugMessenger&& other) noexcept
        : QueryInternal(std::move(other))
    {
        DX12Info::RebindCallback(&other, this);
    }
    DX12DebugMessenger& operator=(DX12DebugMessenger&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        DX12Info::RebindCallback(&other, this);
        QueryInternal::operator=(std::move(other));
        return *this;
    }
    ~DX12DebugMessenger() noexcept
    {
        DX12Info::RemoveCallback(this);
    }

    operator bool() const noexcept
    {
        return info;
    }
};
} // namespace wis
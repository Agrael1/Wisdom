#ifndef WIS_DX12_DEBUG_H
#define WIS_DX12_DEBUG_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_info.h>
#include <memory>
#endif

namespace wis {
class DX12DebugMessenger;

WISDOM_EXPORT
template<>
struct Internal<DX12DebugMessenger> {
    DX12InfoToken info;
};

WISDOM_EXPORT
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

#endif // WIS_DX12_DEBUG_H

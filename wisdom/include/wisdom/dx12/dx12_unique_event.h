#ifndef DX12_UNIQUE_EVENT_H
#define DX12_UNIQUE_EVENT_H
#include <wisdom/generated/api/api.h>
#include <Windows.h>

namespace wis {
struct unique_event {
    unique_event(HANDLE event = nullptr) noexcept
        : hevent(event) { }
    unique_event(unique_event const&) = delete;
    unique_event& operator=(unique_event const&) = delete;
    unique_event(unique_event&& o) noexcept
        : hevent(std::exchange(o.hevent, nullptr)) { }
    unique_event& operator=(unique_event&& o) noexcept
    {
        std::swap(hevent, o.hevent);
        return *this;
    }
    ~unique_event() noexcept
    {
        if (hevent)
            CloseHandle(hevent);
    }
    auto get() const noexcept
    {
        return hevent;
    }
    operator bool() const noexcept
    {
        return bool(hevent);
    }
    wis::Status wait(uint32_t wait_ms) const noexcept
    {
        auto st = WaitForSingleObject(hevent, wait_ms);
        if (st == WAIT_OBJECT_0)
            return wis::Status::Ok;
        if (st == WAIT_TIMEOUT)
            return wis::Status::Timeout;
        return wis::Status::Error;
    }

public:
    HANDLE hevent;
};
} // namespace wis

#endif // !DX12_UNIQUE_EVENT_H

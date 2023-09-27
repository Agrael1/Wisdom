#pragma once
#include <wisdom/util/com_ptr.h>
#include <wisdom/api/api.h>
#include <dxgidebug.h>
#include <semaphore>

namespace wis {
/// @brief Information queue for debug and error messages
class DX12Info
{
public:
    static DX12Info& instance() noexcept
    {
        static DX12Info instance;
        return instance;
    }

private:
    WIS_INLINE DX12Info();
    WIS_INLINE ~DX12Info();
    DX12Info(const DX12Info&) = delete;
    DX12Info& operator=(const DX12Info&) = delete;

public:
    static void Poll() noexcept
    {
        auto& inst = instance();
        if (inst.info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL) == 0)
            return;

        inst.semaphore.acquire();
        inst.PollInternal();
        inst.semaphore.release();
    }
    static void SetCallback(DebugCallback callback) noexcept
    {
        instance().callback = callback;
    }

private:
    void PollInternal() noexcept;

private:
    wis::com_ptr<IDXGIInfoQueue> info_queue;
    std::binary_semaphore semaphore{ 1 };
    wis::DebugCallback callback;
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_info.cpp"
#endif
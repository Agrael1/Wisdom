#pragma once
#include <wisdom/util/com_ptr.h>
#include <wisdom/api/api.h>
#include <dxgidebug.h>
#include <semaphore>
#include <unordered_map>

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
    WIS_INLINE DX12Info() noexcept;
    WIS_INLINE ~DX12Info() noexcept;
    DX12Info(const DX12Info&) = delete;
    DX12Info& operator=(const DX12Info&) = delete;

public:
    static void Poll() noexcept
    {
        auto& inst = instance();
        if (!inst.info_queue || inst.info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL) == 0)
            return;

        if(!inst.semaphore.try_acquire())
            return;

        if (inst.callbacks.empty()) {
            inst.info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
            inst.semaphore.release();
            return;
        }

        if (inst.info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL) == 0)
        {
            inst.semaphore.release();
            return;
        }

        inst.PollInternal();
        inst.semaphore.release();
    }
    static void AddCallback(DebugCallback callback, void* user_data = nullptr) noexcept
    {
        auto& inst = instance();
        if (inst.callbacks.contains(callback))
            return;

        inst.callback_sem.acquire();
        inst.callbacks.emplace(callback, user_data);
        inst.callback_sem.release();
    }
    static void RemoveCallback(DebugCallback callback) noexcept
    {
        auto& inst = instance();
        if (!inst.callbacks.contains(callback))
            return;

        inst.callback_sem.acquire();
        inst.callbacks.erase(callback);
        inst.callback_sem.release();
    }

private:
    void PollInternal() noexcept;

private:
    wis::com_ptr<IDXGIInfoQueue> info_queue;
    std::binary_semaphore semaphore{ 1 };
    std::binary_semaphore callback_sem{ 1 };
    std::unordered_map<wis::DebugCallback, void*> callbacks;
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_info.h"
#endif
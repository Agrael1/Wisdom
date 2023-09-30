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
    friend struct DX12InfoToken;

public:
    static DX12Info& instance() noexcept
    {
        static DX12Info instance;
        return instance;
    }

private:
    DX12Info() noexcept = default;
    DX12Info(const DX12Info&) = delete;
    DX12Info& operator=(const DX12Info&) = delete;

    void Uninitialize() noexcept
    {
        semaphore.acquire();
        callback_sem.acquire();

        info_queue.reset();
        callbacks.clear();

        callback_sem.release();
        semaphore.release();
    }
    WIS_INLINE void PollInternal() noexcept;
    WIS_INLINE void Initialize() noexcept;

public:
    static void Poll() noexcept
    {
        auto& inst = instance();
        if (!inst.info_queue || inst.info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL) == 0)
            return;

        if (!inst.semaphore.try_acquire())
            return;

        if (inst.callbacks.empty()) {
            inst.info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
            inst.semaphore.release();
            return;
        }

        if (inst.info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL) == 0) {
            inst.semaphore.release();
            return;
        }

        inst.PollInternal();
        inst.semaphore.release();
    }
    static void AddCallback(DebugCallback callback, void* user_data = nullptr) noexcept
    {
        auto& inst = instance();
        inst.callback_sem.acquire();
        if (inst.callbacks.contains(callback)) {
            inst.callbacks.at(callback).second++;
            return;
        }

        inst.callbacks.emplace(callback, user_data);
        inst.callback_sem.release();
    }
    static void RemoveCallback(DebugCallback callback) noexcept
    {
        auto& inst = instance();
        inst.callback_sem.acquire();
        if (!inst.callbacks.contains(callback))
            return;

        if (!--inst.callbacks.at(callback).second)
            inst.callbacks.erase(callback);
        inst.callback_sem.release();
    }

private:
    wis::com_ptr<IDXGIInfoQueue> info_queue;
    std::binary_semaphore semaphore{ 1 };
    std::binary_semaphore callback_sem{ 1 };
    std::unordered_map<wis::DebugCallback, std::pair<void*, std::size_t>> callbacks;
    std::atomic_size_t ref_count{ 0 };
};

struct DX12InfoToken {
    void Acquire() noexcept
    {
        if (!DX12Info::instance().ref_count.fetch_add(1, std::memory_order_relaxed)) {
            DX12Info::instance().Initialize(); // no need to synchronize, since debug queue is only one
        }
    }
    void Release() noexcept
    {
        if ((DX12Info::instance().ref_count.fetch_sub(1, std::memory_order_release) == 1)) {
            std::atomic_thread_fence(std::memory_order_acquire);
            DX12Info::instance().Uninitialize();
        }
    }
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_info.h"
#endif
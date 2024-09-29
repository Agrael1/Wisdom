#ifndef WIS_DX12_INFO_H
#define WIS_DX12_INFO_H
#include <dxgidebug.h>
#include <semaphore>
#include <unordered_map>
#include <utility>
#include <wisdom/generated/api/api.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/com_ptr.h>

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

    WIS_INLINE void Uninitialize() noexcept;
    WIS_INLINE void PollInternal() noexcept;
    WIS_INLINE void Initialize() noexcept;

public:
    WIS_INLINE static void Poll() noexcept;
    WIS_INLINE static void AddCallback(void* factory, DebugCallback callback,
                                       void* user_data = nullptr) noexcept;
    WIS_INLINE static void RemoveCallback(void* factrory) noexcept;
    WIS_INLINE static bool RebindCallback(void* factory_from, void* factory_to) noexcept;

private:
    wis::com_ptr<IDXGIInfoQueue> info_queue;
    std::binary_semaphore semaphore{ 1 };
    std::binary_semaphore callback_sem{ 1 };
    std::unordered_map<void*, std::pair<wis::DebugCallback, void*>> callbacks;
    std::atomic_size_t ref_count{ 0 };
};

struct DX12InfoToken {
    DX12InfoToken() noexcept = default;
    DX12InfoToken(bool) noexcept
        : bound(true)
    {
        Acquire();
    }
    DX12InfoToken(DX12InfoToken&& other) noexcept
        : bound(std::exchange(other.bound, false))
    {
    }
    DX12InfoToken& operator=(DX12InfoToken&& other) noexcept
    {
        if (bound)
            Release();
        bound = std::exchange(other.bound, false);
        return *this;
    }
    ~DX12InfoToken()
    {
        if (bound)
            Release();
    }

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
    operator bool() const noexcept
    {
        return bound;
    }

private:
    bool bound = false;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_info.cpp"
#endif
#endif // WIS_DX12_INFO_H

#pragma once
#include <dxgidebug.h>
#include <semaphore>
#include <unordered_map>
#include <wisdom/generated/api/api.h>
#include <wisdom/util/com_ptr.h>

namespace wis {
/// @brief Information queue for debug and error messages
class DX12Info {
  friend struct DX12InfoToken;

public:
  static DX12Info& instance() noexcept {
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
  std::binary_semaphore semaphore{1};
  std::binary_semaphore callback_sem{1};
  std::unordered_map<void*, std::pair<wis::DebugCallback, void*>> callbacks;
  std::atomic_size_t ref_count{0};
};

struct DX12InfoToken {
  void Acquire() noexcept {
    if (!DX12Info::instance().ref_count.fetch_add(1, std::memory_order_relaxed)) {
      DX12Info::instance().Initialize(); // no need to synchronize, since debug queue is only one
    }
  }
  void Release() noexcept {
    if ((DX12Info::instance().ref_count.fetch_sub(1, std::memory_order_release) == 1)) {
      std::atomic_thread_fence(std::memory_order_acquire);
      DX12Info::instance().Uninitialize();
    }
  }
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/dx12_info.cpp"
#endif
#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/util/log_layer.h>
#include <winrt/base.h>
#include <dxgidebug.h>
#endif // !WISDOM_MODULES

namespace wis
{
/// @brief Message struct for debug and error messagesW
struct DXGIMessage {
    Severity severity; //< Severity of the message
    std::string description; //< Description of the message
};

/// @brief Information queue for debug and error messages
class DX12Info
{
public:
    WIS_INLINE DX12Info();
    WIS_INLINE ~DX12Info();
    DX12Info(const DX12Info&) = delete;
    DX12Info& operator=(const DX12Info&) = delete;

public:
    WIS_INLINE [[nodiscard]] static uint64_t GetNumMessages() noexcept;
    WIS_INLINE [[nodiscard]] static std::vector<DXGIMessage> GetMessages() noexcept;

private:
    static inline winrt::com_ptr<IDXGIInfoQueue> info_queue{};
};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_info.inl"
#endif

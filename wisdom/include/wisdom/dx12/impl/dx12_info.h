#ifndef WISDOM_HEADER_ONLY
#include <wisdom/dx12/xdx12_info.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/global/definitions.h>
#include <dxgi1_6.h>
#include <d3d12sdklayers.h>
#include <vector>

namespace wis {
inline constexpr wis::Severity Convert(DXGI_INFO_QUEUE_MESSAGE_SEVERITY sev) noexcept
{
    using enum wis::Severity;
    switch (sev) {
    default:
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
        return Critical;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
        return Error;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
        return Warning;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
        return Info;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
        return Debug;
    }
}
} // namespace wis

void wis::DX12Info::Initialize() noexcept
{
    if (info_queue)
        return;

    auto hr = DXGIGetDebugInterface1(0, __uuidof(IDXGIInfoQueue), info_queue.put_void());
    if constexpr (debug_mode) {
        info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
        info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true);

        if (auto d3dinfoqueue = info_queue.as<ID3D12InfoQueue>()) {
            d3dinfoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true); // Corruption
            d3dinfoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true); // Error
            d3dinfoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true); // Warning
        }
    }
    info_queue->AddApplicationMessage(DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO, "Debug layer creation succeded");
}

void wis::DX12Info::PollInternal() noexcept
{
    std::vector<byte> message;
    message.resize(sizeof(DXGI_INFO_QUEUE_MESSAGE));

    for (UINT64 i = 0;; i++) {
        SIZE_T messageLength = 0;
        HRESULT hr = info_queue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
        if (hr < 0)
            break;

        // allocate memory for message
        message.resize(messageLength);
        auto* pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(message.data());

        // get message and push it into vector
        hr = info_queue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
        if (hr < 0)
            continue;

        // call callbacks
        callback_sem.acquire();
        for (auto&& [k, v] : callbacks) {
            k(Convert(pMessage->Severity), pMessage->pDescription, v);
        }
        callback_sem.release();
    }
    info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
}

#ifndef WISDOM_MODULES
#include <dxgi1_6.h>
#include <d3d12sdklayers.h>
#endif

inline constexpr wis::Severity Convert(DXGI_INFO_QUEUE_MESSAGE_SEVERITY sev) noexcept
{
    using enum wis::Severity;
    switch (sev) {
    default:
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
        return critical;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
        return error;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
        return warn;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
        return info;
    case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
        return debug;
    }
}

wis::DX12Info::DX12Info()
{
    winrt::check_hresult(DXGIGetDebugInterface1(0, __uuidof(IDXGIInfoQueue), info_queue.put_void()));

    if constexpr (debug_mode) {
        info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
        info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true);

        if (auto d3dinfoqueue = info_queue.try_as<ID3D12InfoQueue>()) {
            d3dinfoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true); // Corruption
            d3dinfoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true); // Error
            d3dinfoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true); // Warning
        }
    }
}
wis::DX12Info::~DX12Info()
{
    info_queue = nullptr;
}
uint64_t wis::DX12Info::GetNumMessages() noexcept
{
    if (!info_queue)
        return 0;
    return info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}
std::vector<wis::DXGIMessage> wis::DX12Info::GetMessages() noexcept
{
    if (!info_queue)
        return {};
    std::vector<DXGIMessage> messages;
    const auto end = info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    if (!end)
        return {};

    for (UINT64 i = 0; i < end; i++) {
        SIZE_T messageLength = 0;
        // get the size of message[i]
        winrt::check_hresult(info_queue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
        // allocate memory for message
        auto bytes = std::make_unique<byte[]>(messageLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE *>(bytes.get());
        // get message and bush it into vector
        winrt::check_hresult(info_queue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
        messages.emplace_back(Convert(pMessage->Severity), pMessage->pDescription);
    }
    info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
    return messages;
}

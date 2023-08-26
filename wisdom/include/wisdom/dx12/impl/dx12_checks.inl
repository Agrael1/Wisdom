#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_info.h>
#endif

bool wis::log_dxgi_errors() noexcept
{
    using enum wis::Severity;

    if (!DX12Info::GetNumMessages())
        return false;
    auto messages = DX12Info::GetMessages();
    bool bError = false;
    for (auto& i : messages) {
        bError |= i.severity == error || i.severity == critical;
        lib_log(i.severity, i.description);
    }
    return bError;
}

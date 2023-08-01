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
    for (auto &i : messages) {
        bError |= i.severity == error || i.severity == critical;
        lib_log(i.severity, i.description);
    }
    return bError;
}

winrt::hresult wis::last_windows_error() noexcept
{
    return winrt::hresult(GetLastError());
}

// Window Exception
wis::hr_exception::hr_exception(winrt::hresult hr, wis::source_location sl)
    : exception(sl, false), hResult(hr)
{
    log();
}
std::string wis::hr_exception::description() const noexcept
{
    return winrt::to_string(winrt::to_hstring(hResult));
}
const char *wis::hr_exception::what() const noexcept
{
    if (whatBuffer.empty()) {
        whatBuffer = wis::format(
                "{}\n[Error Code]: 0x{:08X}({})\n"
                "[Description]: {}\n{}",
                type(), (unsigned long)error_code(), (unsigned long)error_code(),
                description(), origin());
    }
    return whatBuffer.c_str();
}
#include <example/util.h>
#include <Windows.h>

winrt::hresult wis::last_windows_error() noexcept
{
    return winrt::hresult(GetLastError());
}

//// Window Exception
// wis::hr_exception::hr_exception(winrt::hresult hr, wis::source_location sl)
//     : exception(sl, false), hResult(hr)
//{
//     log();
// }
// std::string wis::hr_exception::description() const noexcept
//{
//     return winrt::to_string(winrt::to_hstring(hResult));
// }
// const char* wis::hr_exception::what() const noexcept
//{
//     if (whatBuffer.empty()) {
//         whatBuffer = wis::format(
//                 "{}\n[Error Code]: 0x{:08X}({})\n"
//                 "[Description]: {}\n{}",
//                 type(), (unsigned long)error_code(), (unsigned long)error_code(),
//                 description(), origin());
//     }
//     return whatBuffer.c_str();
// }

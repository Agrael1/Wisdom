#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_info.h>
#include <wil/resource.h>


bool wis::log_dxgi_errors()noexcept
{
	using enum wis::Severity;

	if (!DX12Info::GetNumMessages())return false;
	auto messages = DX12Info::GetMessages();
	bool bError = false;
	for (auto& i : messages)
	{
		bError |= i.severity == error || i.severity == critical;
		lib_log(i.severity, i.description);
	}
	return bError;
}

winrt::hresult wis::last_windows_error()noexcept
{
	return winrt::hresult(GetLastError());
}

//Window Exception
wis::hr_exception::hr_exception(winrt::hresult hr, wis::source_location sl)
	:exception(sl, false), hResult(hr)
{
	log();
}
std::string wis::hr_exception::description() const noexcept
{
	wil::unique_hlocal_ansistring msgBuf;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(msgBuf.put()),
		0, nullptr);

	if (nMsgLen == 0)
		return "Unknown error";
	std::string errorString = msgBuf.get();
	return errorString;
}
const char* wis::hr_exception::what() const noexcept
{
	if (whatBuffer.empty())
	{
		whatBuffer = wis::format(
			"{}\n[Error Code]: 0x{:08X}({})\n"
			"[Description]: {}\n{}",
			type(), (unsigned long)error_code(), (unsigned long)error_code(),
			description(), origin()
		);
	}
	return whatBuffer.c_str();
}
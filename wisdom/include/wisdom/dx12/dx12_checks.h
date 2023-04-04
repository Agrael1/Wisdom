#pragma once
#include <wisdom/global/definitions.h>
#include <wisdom/util/exception.h>
#include <winrt/base.h>

namespace wis
{
	class hr_exception :public wis::exception
	{
	public:
		hr_exception(winrt::hresult hr, std::source_location sl = std::source_location::current());
	public:
		const char* what() const noexcept override;
		std::string_view type()const noexcept override
		{
			return "Vertas Window Exception";
		}
		winrt::hresult error_code() const noexcept
		{
			return hResult;
		}
		std::string description() const noexcept;
	private:
		winrt::hresult hResult;
	};



	// true if any errors
	bool log_dxgi_errors()noexcept;
	winrt::hresult last_windows_error() noexcept;


	inline void check_context(std::source_location sl = std::source_location::current())
	{
		if constexpr (debug_mode)
			if (log_dxgi_errors())
				throw wis::exception{ sl };
	}

	inline void check_bool(bool check, std::source_location sl = std::source_location::current())
	{
		if (check)return;
		log_dxgi_errors();
		throw wis::exception{ sl };
	}
	inline void check_hresult(winrt::hresult hr, std::source_location sl = std::source_location::current())
	{
		if (hr >= 0)return;
		log_dxgi_errors();
		throw wis::hr_exception{ hr, sl };
	}
	inline [[nodiscard]] bool check_hresult_nothrow(winrt::hresult hr)noexcept
	{
		bool a = hr >= 0;
		if (a) return true;
		log_dxgi_errors();
		return false;
	}
	inline void check_windows(bool check, std::source_location sl = std::source_location::current())
	{
		if (check)return;
		throw wis::hr_exception{ last_windows_error(), sl };
	}

	inline bool succeded(winrt::hresult hr)noexcept { return check_hresult_nothrow(hr); }

	// light version of check, disabled by compile time switches
	inline bool succeded_weak(winrt::hresult hr)
	{
		if constexpr (!debug_mode || !runtime_asserts)
			return hr >= 0;
		return succeded(hr);
	}
}

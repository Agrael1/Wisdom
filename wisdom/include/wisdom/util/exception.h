#pragma once
#include <exception>
#include <format>
#include <source_location>

namespace wis
{
	class exception :std::exception
	{
	public:
		exception(std::source_location sl = std::source_location::current(), bool write = true)noexcept;
	public:
		void log();
		const char* what()const noexcept override
		{
			if (whatBuffer.empty())
				whatBuffer = std::format("{}\n{}", type(), origin());
			return whatBuffer.c_str();
		}
		virtual std::string_view type()const noexcept
		{
			return "Veritas exception";
		}
		uint32_t line()const noexcept
		{
			return sl.line();
		}
		std::string_view file()const noexcept
		{
			return sl.file_name();
		}
		std::string_view function()const noexcept
		{
			return sl.function_name();
		}
		std::string origin()const noexcept
		{
			return std::format("[File]: {}\n[Line]: {}\n[Function]: {}", file(), line(), function());
		}
	private:
		std::source_location sl;
	protected:
		mutable std::string whatBuffer;
	};
}
#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/exception.h>
#endif // !WISDOM_MODULES

namespace wis {

WIS_EXPORT class message_exception : public wis::exception
{
public:
    message_exception(std::string message, wis::source_location sl = wis::source_location::current())
        : wis::exception(sl)
		, m_message(message)
	{

	}

public:
    const char* what() const noexcept override
    {
        if (whatBuffer.empty()) {
            whatBuffer = wis::format(
                    "\n{}\n ASSERT FAILED\n"
                    "[Description]: {}\n{}\nProgram terminated...",
                    type(), description(), origin());
        }
        return whatBuffer.c_str();
    }
    std::string_view type() const noexcept override
    {
        return "Vertas Assertion";
    }
    std::string message() const noexcept
    {
        return m_message;
    }
    std::string description() const noexcept
    {
        return message();
    }

private:
    std::string m_message;
};


#define WIS_ASSERT(b, message) ::wis::assert_debug(b, #b + message)
inline void assert_debug(bool b, std::string_view message, wis::source_location sl = wis::source_location::current())noexcept
{
    if constexpr (wis::debug_mode || wis::runtime_asserts)
        if (!b)
            throw wis::message_exception(std::string{ message }, sl);
}
} // namespace wis
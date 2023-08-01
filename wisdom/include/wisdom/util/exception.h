#pragma once
#ifndef WISDOM_MODULES
#include <exception>
#include <wisdom/bridge/format.h>
#include <wisdom/bridge/source_location.h>
#include <wisdom/util/log_layer.h>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
    class exception : std::exception
    {
    public:
        exception(wis::source_location sl = wis::source_location::current(), bool write = true) noexcept
            : sl(std::move(sl))
        {
            if (write)
                log();
        }

        void log() const noexcept
        {
            wis::lib_critical(what());
        }
        const char* what() const noexcept override
        {
            if (whatBuffer.empty())
                whatBuffer = wis::format("{}\n{}", type(), origin());
            return whatBuffer.c_str();
        }
        virtual std::string_view type() const noexcept
        {
            return "Veritas exception";
        }
        uint32_t line() const noexcept
        {
            return sl.line();
        }
        std::string_view file() const noexcept
        {
            return sl.file_name();
        }
        std::string_view function() const noexcept
        {
            return sl.function_name();
        }
        std::string origin() const noexcept
        {
            return wis::format("[File]: {}\n[Line]: {}\n[Function]: {}", file(), line(), function());
        }

    private:
        wis::source_location sl;

    protected:
        mutable std::string whatBuffer;
    };
}

//#pragma once
//#ifndef WISDOM_MODULES
//#include <wisdom/util/flags.h>
//#include <memory>
//#include <wisdom/bridge/source_location.h>
//#include <string>
//#endif // !WISDOM_MODULES
//
//WIS_EXPORT namespace wis
//{
//    using river::flags::operator+;
//
//    enum class Severity : uint8_t {
//        debug,
//        trace,
//        info,
//        warn,
//        error,
//        critical
//    };
//    constexpr const char* severity_strings[]{
//        "debug",
//        "trace",
//        "info",
//        "warn",
//        "error",
//        "critical"
//    };
//
//    /// @brief Create a custom log layer to receive logging from the library
//    struct LogLayer {
//        virtual ~LogLayer() = default;
//        virtual void Log(Severity sev, std::string message, wis::source_location sl = wis::source_location::current()){};
//    };
//
//    /// @brief Set the log layer for the library
//    class LibLogger
//    {
//        LibLogger() = default;
//        ~LibLogger() = default;
//
//    public:
//        LibLogger(const LibLogger&) = delete;
//        LibLogger(LibLogger&&) = delete;
//        LibLogger& operator=(const LibLogger&) = delete;
//        LibLogger& operator=(LibLogger&&) = delete;
//
//        /// @brief Instance of the library logger
//        /// @return Instance of the library logger
//        [[nodiscard]] static LibLogger& Instance()
//        {
//            static LibLogger log;
//            return log;
//        }
//
//        /// @brief Set the log layer for the library
//        /// @param log Log layer
//        static void SetLogLayer(std::shared_ptr<LogLayer> log) noexcept
//        {
//            Instance().log = std::move(log);
//        }
//
//        /// @brief Get the log layer for the library
//        /// @return Log layer
//        [[nodiscard]] static auto* Get() noexcept
//        {
//            return Instance().log.get();
//        }
//
//    private:
//        std::shared_ptr<LogLayer> log{};
//    };
//
//    inline void lib_log_internal(Severity sev, std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if (auto log_ptr = LibLogger::Get())
//            log_ptr->Log(sev, std::move(message), sl);
//    }
//
//    // Compile time resolved loggong for library
//
//    /// @brief Debug, this should be used for debugging purposes only
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_debug(std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if constexpr (WISDOM_LOG_LEVEL <= +Severity::debug)
//            lib_log_internal(Severity::debug, std::move(message), sl);
//    }
//
//    /// @brief Trace, this should be used for very detailed information
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_trace(std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if constexpr (WISDOM_LOG_LEVEL <= +Severity::trace)
//            lib_log_internal(Severity::trace, std::move(message), sl);
//    }
//
//    /// @brief Information, this should be used for general information
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_info(std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if constexpr (WISDOM_LOG_LEVEL <= +Severity::info)
//            lib_log_internal(Severity::info, std::move(message), sl);
//    }
//
//    /// @brief Log a warning message to the library log layer
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_warn(std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if constexpr (WISDOM_LOG_LEVEL <= +Severity::warn)
//            lib_log_internal(Severity::warn, std::move(message), sl);
//    }
//
//    /// @brief Error, this should be used when the library is in a recoverable state
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_error(std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if constexpr (WISDOM_LOG_LEVEL <= +Severity::error)
//            lib_log_internal(Severity::error, std::move(message), sl);
//    }
//
//    /// @brief Critical error, this should be used when the library is in an unrecoverable state
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_critical(std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        if constexpr (WISDOM_LOG_LEVEL <= +Severity::critical)
//            lib_log_internal(Severity::critical, std::move(message), sl);
//    }
//
//    /// @brief Log a message to the library log layer
//    /// @param sev Severity of the message
//    /// @param message Message text
//    /// @param sl Source location of the message
//    inline void lib_log(Severity sev, std::string message, wis::source_location sl = wis::source_location::current())
//    {
//        switch (sev) {
//        case Severity::debug: // NOLINT
//            return lib_debug(message, sl);
//        case Severity::trace:
//            return lib_trace(message, sl);
//        case Severity::info:
//            return lib_info(message, sl);
//        case Severity::warn:
//            return lib_warn(message, sl);
//        case Severity::error:
//            return lib_error(message, sl);
//        default:
//        case Severity::critical:
//            return lib_critical(message, sl);
//        }
//    }
//}

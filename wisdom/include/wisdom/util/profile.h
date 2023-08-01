#pragma once
#include <wisdom/util/log_layer.h>
#include <chrono>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>

namespace wis {
class timer
{
    using clock = std::chrono::high_resolution_clock;

public:
    void start()
    {
        start_time = clock::now();
    }
    float stop()
    {
        static constexpr float scale_factor = 1'000'000'000.0f;
        return static_cast<float>((clock::now() - start_time).count()) / scale_factor;
    }

private:
    std::chrono::time_point<clock> start_time;
};

class scoped_timer
{
public:
    scoped_timer()
    {
        t.start();
    }
    ~scoped_timer()
    {
        std::printf("%s", wis::format("{}\n", t.stop()).c_str());
    }

private:
    timer t;
};

template<bool x>
class scoped_profiler_p;

template<>
class scoped_profiler_p<true>
{
public:
    scoped_profiler_p(std::string message = "")
        : message(std::move(message))
    {
        t.start();
    }
    ~scoped_profiler_p()
    {
        wis::lib_info(wis::format("{}: {}", message, t.stop()));
    }

private:
    wis::timer t;
    std::string message;
};

template<>
class scoped_profiler_p<false>
{
    scoped_profiler_p(auto x) { }
};

using scoped_profiler = scoped_profiler_p<wis::debug_mode>;
} // namespace wis

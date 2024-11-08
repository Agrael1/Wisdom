#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.h>
#include <exception>
#include <filesystem>

namespace ex {
struct Exception : public std::exception {
    Exception(std::string message)
        : message(std::move(message)) { 
    }

    const char* what() const noexcept override
    {
        return message.c_str();
    }

    std::string message;
};

inline void CheckResult(wis::Result res)
{
    if (res.status != wis::Status::Ok)
        throw Exception(res.error);
}
template<typename T>
inline T Unwrap(wis::ResultValue<T>&& res)
{
    auto&& [status, value] = res;
    CheckResult(status);
    return std::move(value);
}

// static helpers
std::string LoadShader(std::filesystem::path p);

// constants
static inline constexpr uint32_t swap_buffer_count = 3;
static inline constexpr uint32_t flight_frames = 2;
static inline constexpr wis::DataFormat swapchain_format = wis::DataFormat::RGBA8Unorm; // supported by all APIs

// setup helpers
struct FramedCommandList {
    FramedCommandList() = default;
    FramedCommandList(wis::Device& device, wis::QueueType type = wis::QueueType::Graphics)
    {
        for (size_t i = 0; i < flight_frames; i++) {
            cmd_list[i] = Unwrap(device.CreateCommandList(type));
        }
    }

public:
    wis::CommandList& operator[](size_t i)
    {
        return cmd_list[i];
    }

    wis::CommandList cmd_list[flight_frames];
};

struct ExampleSetup {
    static void DebugCallback(wis::Severity severity, const char* message, void* user_data);

public:
    ExampleSetup() = default;

public:
    wis::Factory InitDefaultFactory(wis::FactoryExtension* platform_ext);
    void InitDefaultDevice(const wis::Factory& factory);

    void InitDefaultQueue();
    void InitDefault(wis::FactoryExtension* platform_ext);
    void WaitForGPU();

    FramedCommandList CreateLists(wis::QueueType type = wis::QueueType::Graphics)
    {
        return FramedCommandList(device, type);
    }

public:
    wis::DebugMessenger info;
    wis::Device device;
    wis::CommandQueue queue;
    wis::Fence fence;
    uint64_t fence_value = 0;
};



} // namespace ex
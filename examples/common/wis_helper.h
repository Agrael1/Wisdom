#pragma once
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.h>
#include <wisdom/wisdom_descriptor_buffer.h>
#include <exception>
#include <filesystem>
#include <span>

namespace ex {
struct Exception : public std::exception {
    Exception(std::string message)
        : message(std::move(message))
    {
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
static inline constexpr uint32_t swap_buffer_count = 2;
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

struct DescTable {
    uint32_t descriptor_count = 0;
};

// Not very efficient, but good for examples. Doesn't have static tables (frame independent)
struct FramedDescriptorSetup {

    FramedDescriptorSetup() = default;
    FramedDescriptorSetup(wis::DescriptorBufferExtension& device, wis::DescriptorHeapType desc_type, std::span<const DescTable> tables)
    {
        auto desc_alignment = device.GetDescriptorTableAlignment(desc_type);
        auto desc_increment = device.GetDescriptorSize(desc_type);

        // Create equal tables for each frame
        size_t bytes = 0;
        for (auto& table : tables) {
            bytes += wis::detail::aligned_size(table.descriptor_count * desc_increment, desc_alignment);
        }
        desc_buffer = Unwrap(device.CreateDescriptorBuffer(desc_type, wis::DescriptorMemory::ShaderVisible, bytes * flight_frames));

        // calculate offsets
        for (size_t i = 0; i < flight_frames; i++) {
            offsets[i] = i * bytes;
        }
    }

public:
    uint32_t offset_frame(uint32_t frame) const
    {
        return offsets[frame];
    }

public:
    wis::DescriptorBuffer desc_buffer; // rebinding the descriptor buffer is very expensive, so we keep single buffer for all frames
    std::array<uint32_t, flight_frames> offsets;
};

struct ExampleSetup {
    static void DebugCallback(wis::Severity severity, const char* message, void* user_data);

public:
    ExampleSetup() = default;

public:
    wis::Factory InitDefaultFactory(wis::FactoryExtension* platform_ext);
    void InitDefaultDevice(const wis::Factory& factory, std::span<wis::DeviceExtension*> device_exts);

    void InitDefaultQueue();
    void InitDefault(wis::FactoryExtension* platform_ext, std::span<wis::DeviceExtension*> device_exts = {});
    void WaitForGPU();

    FramedCommandList CreateLists(wis::QueueType type = wis::QueueType::Graphics)
    {
        return FramedCommandList(device, type);
    }

    // Not very efficient, but good for examples
    template<typename T>
    wis::Buffer CreateAndUploadBuffer(std::span<T> data, wis::BufferUsage usage)
    {
        using namespace wis;
        auto upload = Unwrap(allocator.CreateUploadBuffer(data.size_bytes()));
        auto buffer = Unwrap(allocator.CreateBuffer(data.size_bytes(), usage | wis::BufferUsage::CopyDst));

        auto memory = (uint8_t*)upload.MapRaw();
        std::copy_n(reinterpret_cast<const uint8_t*>(data.data()), data.size_bytes(), memory);
        upload.Unmap();

        // create command list
        auto cmd = Unwrap(device.CreateCommandList(wis::QueueType::Graphics));
        ex::CheckResult(cmd.Reset());
        cmd.BufferBarrier({ .sync_before = wis::BarrierSync::None,
                            .sync_after = wis::BarrierSync::Copy,
                            .access_before = wis::ResourceAccess::NoAccess,
                            .access_after = wis::ResourceAccess::CopySource },
                          upload);
        cmd.BufferBarrier({ .sync_before = wis::BarrierSync::None,
                            .sync_after = wis::BarrierSync::Copy,
                            .access_before = wis::ResourceAccess::NoAccess,
                            .access_after = wis::ResourceAccess::CopyDest },
                          buffer);
        cmd.CopyBuffer(upload, buffer, { .size_bytes = data.size_bytes() });
        cmd.Close();

        wis::CommandListView lists[] = { cmd };
        queue.ExecuteCommandLists(lists, std::size(lists));
        WaitForGPU();

        return std::move(buffer);
    }

public:
    wis::DebugMessenger info;
    wis::Device device;
    wis::CommandQueue queue;
    wis::ResourceAllocator allocator;
    wis::Fence fence;
    uint64_t fence_value = 0;
};

} // namespace ex

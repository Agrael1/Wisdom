#define WIS_USE_PLATFORM_XLIB 1

#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_platform.hpp>

#include <iostream>

void log_callback(wis::Severity severity, const char* message, uint64_t device, void* user_data) noexcept
{
    const char* severity_str = "";
    switch (severity) {
    case wis::Severity::Verbose:
        severity_str = "VERBOSE";
        break;
    case wis::Severity::Info:
        severity_str = "INFO";
        break;
    case wis::Severity::Warning:
        severity_str = "WARNING";
        break;
    case wis::Severity::Error:
        severity_str = "ERROR";
        break;
    case wis::Severity::Fatal:
        severity_str = "FATAL";
        break;
    default:
        severity_str = "UNKNOWN";
        break;
    }
    std::cout << "[" << severity_str << "] " << message << "\n";
}

// Entry point for testing
int main()
{
    wis::Result result;
    wis::DebugDesc debug_desc;
    debug_desc.enable_debug_layer = true;
    debug_desc.callback = log_callback;
    debug_desc.user_data = nullptr;

    wis::Instance instance = wis::CreateInstance(&debug_desc, {}, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create instance: " << result.error << "\n";
        return int(result.status);
    }
    std::cout << "Instance created successfully.\n";

    wis::AdapterQuery adapter_query = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to query adapters: " << result.error << "\n";
        return int(result.status);
    }
    size_t adapter_count = adapter_query.GetAdapterCount();
    std::cout << "Adapter count: " << adapter_count << "\n";

    wis::CommandQueueDesc queue_desc[] = {
        {wis::CommandQueueType::Graphics, wis::CommandQueuePriority::High},
        {wis::CommandQueueType::Compute, wis::CommandQueuePriority::Normal},
    };

    wis::DeviceRequirements device_requirements{};
    device_requirements.queue_descs = queue_desc, device_requirements.extensions = {};

    wis::Device device;
    for (size_t i = 0; i < adapter_count; ++i) {
        wis::AdapterDesc desc = adapter_query.GetAdapterDesc(i, result);
        if (result.status != wis::Status::Ok) {
            std::cerr << "Failed to get adapter desc for adapter " << i << ": " << result.error << "\n";
            continue;
        }
        std::cout << "Adapter " << i << ": Name: " << desc.description.data() << ", VendorID: " << desc.vendor_id
                  << ", DeviceID: " << desc.device_id << ", DedicatedVideoMemory: " << desc.dedicated_video_memory
                  << ", SharedSystemMemory: " << desc.shared_system_memory
                  << ", Flags: " << static_cast<uint32_t>(desc.flags) << "\n";

        device = adapter_query.CreateDevice(i, device_requirements, result);
        if (result.status != wis::Status::Ok) {
            std::cerr << "Failed to create device for adapter " << i << ": " << result.error << "\n";
            continue;
        }
        std::cout << "Device created successfully for adapter " << i << ".\n";
        break; // Successfully created a device, exit loop
    }

    wis::CommandQueue command_queue = device.CreateCommandQueue(wis::CommandQueueType::Graphics, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create command queue: " << result.error << "\n";
        return int(result.status);
    }

    wis::Fence fence = device.CreateFence(0, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create fence: " << result.error << "\n";
        return int(result.status);
    }

    wis::ResourceAllocator resource_allocator = device.GetResourceAllocator(result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create resource allocator: " << result.error << "\n";
        return int(result.status);
    }

    wis::CommandAllocator command_allocator = device.CreateCommandAllocator(wis::CommandQueueType::Graphics, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create resource allocator: " << result.error << "\n";
        return int(result.status);
    }

    wis::CommandList command_list = command_allocator.CreateCommandList(result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create command list: " << result.error << "\n";
        return int(result.status);
    }

    wis::DescriptorHeapDesc srv_heap_desc{
        .type = wis::DescriptorHeapType::Descriptor,
        .memory_type = wis::DescriptorMemoryType::ShaderVisible,
        .descriptor_count = 10,
        .flags = wis::DescriptorHeapFlags::None,
    };
    wis::DescriptorHeap srv_heap = device.CreateDescriptorHeap(srv_heap_desc, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create descriptor heap: " << result.error << "\n";
        return int(result.status);
    }

    wis::TextureDesc texture_desc{
        .width = 256,
        .height = 256,
        .depth_or_array_size = 1,
        .mip_levels = 1,
        .format = wis::DataFormat::BGRA8Unorm,
        .sample_count = wis::SampleCount::S1,
        .layout = wis::TextureLayout::Texture2D,
        .usage_flags = wis::TextureUsageFlags::CopyDst | wis::TextureUsageFlags::ShaderResource,
        .memory_type = wis::MemoryType::DeviceLocal,
        .memory_flags = wis::MemoryFlags::None,
    };
    wis::Texture texture = resource_allocator.CreateTexture(texture_desc, result);

    return 0;
}

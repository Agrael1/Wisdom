#include <wisdom/wisdom.hpp>
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
    debug_desc.debug_layer = true;
    debug_desc.callback    = log_callback;
    debug_desc.user_data   = nullptr;

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

    wis::Device device;
    for (size_t i = 0; i < adapter_count; ++i) {
        wis::AdapterDesc desc = adapter_query.GetAdapterDesc(i, result);
        if (result.status != wis::Status::Ok) {
            std::cerr << "Failed to get adapter desc for adapter " << i << ": " << result.error << "\n";
            continue;
        }
        std::cout << "Adapter " << i << ": Name: " << desc.description.data()
                  << ", VendorID: " << desc.vendor_id
                  << ", DeviceID: " << desc.device_id
                  << ", DedicatedVideoMemory: " << desc.dedicated_video_memory
                  << ", SharedSystemMemory: " << desc.shared_system_memory
                  << ", Flags: " << static_cast<uint32_t>(desc.flags) << "\n";

        device = adapter_query.CreateDevice(i, {}, result);
        if (result.status != wis::Status::Ok) {
            std::cerr << "Failed to create device for adapter " << i << ": " << result.error << "\n";
            continue;
        }
        std::cout << "Device created successfully for adapter " << i << ".\n";
        break; // Successfully created a device, exit loop
    }

    return 0;
}

#include <wisdom/wisdom.hpp>
#include <iostream>

// Entry point for testing
int main()
{
    wis::Result   result;
    wis::Instance instance = wis::CreateInstance(false, {}, result);
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
    }

    return 0;
}

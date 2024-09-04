#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_platform.h>
#include <wisdom/wisdom_debug.h>
#include <wisdom/wisdom_extended_allocation.h>
#include <iostream>
#include <chrono>

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

struct Test {
    wis::Device Init()
    {
        wis::Device device;
        wis::DebugExtension global_debug;

        wis::FactoryExtension* exts_i[] = {
            &global_debug,
        };

        auto [result, factory] = wis::CreateFactoryWithExtensions(true, exts_i, std::size(exts_i));
        auto [r2, m] = global_debug.CreateDebugMessenger(DebugCallback, &std::cout);
        global_messenger = std::move(m);

        for (size_t i = 0;; i++) {
            auto [res, adapter] = factory.GetAdapter(i);
            if (res.status == wis::Status::Ok) {
                wis::AdapterDesc desc;
                res = adapter.GetDesc(&desc);
                std::cout << "Adapter: " << desc.description.data() << "\n";

                wis::DeviceExtension* exts[] {
                    &global_interop,
                    //&global_extended_allocation,
                };

                auto [res, hdevice] = wis::CreateDeviceWithExtensions(std::move(adapter), exts, std::size(exts));
                if (res.status == wis::Status::Ok) {
                    device = std::move(hdevice);
                    break;
                };

            } else {
                break;
            }
        }
        return device;
    }

public:
    wis::DebugMessenger global_messenger;
    wis::platform::InteropDeviceExtension global_interop;
    wis::ExtendedAllocation global_extended_allocation;
};

int main()
{
    {
        Test test;
        auto device = test.Init();
        if (!device) {
            std::cerr << "Failed to create device\n";
            return 1;
        }
    }
}

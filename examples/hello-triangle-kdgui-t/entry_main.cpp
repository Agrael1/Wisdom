// #include "app.h"
#include <wisdom/vulkan/xvk_factory.h>
#include <wisdom/vulkan/xvk_device.h>
#include <wisdom/dx12/xdx12_factory.h>
#include <wisdom/dx12/xdx12_device.h>
#include <iostream>

#define PRE(X) VK##X

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}
void DebugCallback2(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

int main()
{


    {
        auto [res, factory] = wis::PRE(CreateFactory)(true, &DebugCallback, &std::cout);

        wis::PRE(Device) device;

        for (size_t i = 0;; i++) {
            auto [res, adapter] = factory.GetAdapter(i);
            if (res.status == wis::Status::Ok) {
                wis::AdapterDesc desc;
                adapter.GetDesc(&desc);
                std::cout << "Adapter: " << desc.description.data() << "\n";

                auto [res, xdevice] = PRE(CreateDevice)(factory, adapter);
                if (res.status == wis::Status::Ok) {
                    device = std::move(xdevice);
                    break;
                };

            } else {
                break;
            }
        }
    }

    return 0;

    // Test::App app(1920, 1080);
    // app.Start();
}
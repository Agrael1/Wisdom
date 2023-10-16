// #include "app.h"
#include <wisdom/wisdom.hpp>
#include <iostream>


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
        auto [res, factory] = wis::CreateFactory(true, &DebugCallback, &std::cout);

        wis::Device device;

        for (size_t i = 0;; i++) {
            auto [res, adapter] = factory.GetAdapter(i);
            if (res.status == wis::Status::Ok) {
                wis::AdapterDesc desc;
                adapter.GetDesc(&desc);
                std::cout << "Adapter: " << desc.description.data() << "\n";

                auto [res, xdevice] = wis::CreateDevice(factory, adapter);
                if (res.status == wis::Status::Ok) {
                    device = std::move(xdevice);
                    break;
                };

            } else {
                break;
            }
        }

        auto fence = device.CreateFence();

    }

    return 0;

    // Test::App app(1920, 1080);
    // app.Start();
}
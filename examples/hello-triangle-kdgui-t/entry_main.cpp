// #include "app.h"
//#include <wisdom/vulkan/xvk_factory.h>
#include <wisdom/dx12/xdx12_factory.h>
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
        auto[res, factory] = wis::DX12CreateFactory(true, &DebugCallback, &std::cout);

        wis::DX12Factory factory2;
        factory2 = std::move(factory);
    }

    return 0;

    // Test::App app(1920, 1080);
    // app.Start();
}
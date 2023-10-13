// #include "app.h"
#include <wisdom/vulkan/xvk_factory.h>
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
        auto[res, factory] = wis::VKCreateFactory(true, &DebugCallback, &std::cout);

        wis::VKFactory factory2;
        factory2 = std::move(factory);
    }

    return 0;

    // Test::App app(1920, 1080);
    // app.Start();
}
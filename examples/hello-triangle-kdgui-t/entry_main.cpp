// #include "app.h"
#include <wisdom/dx12/xdx12_factory.h>
#include <iostream>

void WISCALL DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

int main()
{
    wis::DX12Factory factory;
    auto r = factory.Initialize(true, &DebugCallback, &std::cout);
    printf("result: %d\nmessage: %s\n", r.status, r.error);

    return int(r.status);

    // Test::App app(1920, 1080);
    // app.Start();
}
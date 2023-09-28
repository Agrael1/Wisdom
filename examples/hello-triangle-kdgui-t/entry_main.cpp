// #include "app.h"
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

    wis::com_ptr<IDXGIDebug1> debugController;
    DXGIGetDebugInterface1(0, __uuidof(*debugController), debugController.put_void());

    {
        wis::DX12Factory factory;
        //debugController->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

        auto r = factory.Initialize(true, &DebugCallback, &std::cout);
        // debugController->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

        factory.SetName("Test");
        wis::DX12Factory factory2;
        factory2 = std::move(factory);
    }
    //debugController->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
    //
    //
    // debugController->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

    // auto r2 = factory2.Initialize(true, &DebugCallback2, &std::cout);
    // printf("result: %d\nmessage: %s\n", r.status, r.error);

    return 0;

    // Test::App app(1920, 1080);
    // app.Start();
}
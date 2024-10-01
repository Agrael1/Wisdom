#pragma once
#include "window.h"
#include "work_node.h"
#include "transfer_node.h"
#include <wisdom/wisdom_windows.h>

class App
{
    static constexpr uint32_t kFrameCount = 2;

public:
    App(uint32_t width, uint32_t height);
    ~App();

public:
    void CreateDevices();
    void CreateSwapChain(const wis::platform::WindowsExtension& platform);
    int Start();

    void Frame();
    void DumpFrame(const char* name);
    void OnResize(uint32_t width, uint32_t height);
    void WaitForGPU();

private:
    uint32_t width;
    uint32_t height;

    Window wnd;

    wis::DebugMessenger info;
    WorkNode work;
    TransferNode transfer;
};

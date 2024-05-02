#pragma once
#include <wisdom/wisdom.hpp>

class App
{
public:
    App();
    ~App();
    void Initialize(IUnknown* core_window, uint32_t width, uint32_t height);

public:
    void Frame();

private:
    void WaitForGPU();

private:
};

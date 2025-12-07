#include <wisdom/generated/dx12_cpp_api.hpp>
#include <iostream>

// Entry point for testing
int main()
{
    wis::Result       result;
    wis::DX12Instance instance = wis::DX12CreateInstance(false, {}, result);
    return 0;
}
#include <wisdom/wisdom.hpp>
#include <iostream>

// Entry point for testing
int main()
{
    wis::Result       result;
    wis::DX12TestExtension test_extension;

    wis::InstanceExtensionHeader* extensions[] = {
        &test_extension
    };
    wis::Instance instance = wis::CreateInstance(false, extensions, result);

    return 0;
}
#include <wisdom/wisdom.hpp>
#include <iostream>

// Entry point for testing
int main()
{
    wis::Result   result;
    wis::Instance instance = wis::CreateInstance(false, {}, result);

    return 0;
}
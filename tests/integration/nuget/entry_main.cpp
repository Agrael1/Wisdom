#include <wisdom/wisdom.hpp>
#include <iostream>

int main()
{
    wis::Result result;
    wis::Instance instance = wis::CreateInstance(nullptr, {}, result);
    std::cout << "Wisdom NuGet package successfully linked!" << std::endl;
    return 0;
}

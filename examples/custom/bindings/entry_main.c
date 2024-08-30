#define WISDOM_DX12
#include <wisdom.h>

int main()
{
    DX12Factory factory;
    DX12CreateFactory(true, &factory);




    DX12FactoryDestroy(factory);
    return 0;
}
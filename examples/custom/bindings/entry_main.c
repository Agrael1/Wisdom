#define WISDOM_DX12
#include <wisdom.h>

int main()
{
    DX12Factory factory;
    DX12CreateFactory(true, NULL, 0, &factory);

    DX12FactoryDestroy(factory);
    return 0;
}

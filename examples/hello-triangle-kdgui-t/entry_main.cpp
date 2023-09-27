//#include "app.h"
#include <wisdom/dx12/xdx12_factory.h>

int main()
{
    wis::DX12Factory factory;
    auto r = factory.Initialize();
    printf("result: %d\nmessage: %s\n", r.status, r.error);

    return int(r.status);

    //Test::App app(1920, 1080);
    //app.Start();
}
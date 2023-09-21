#include "wisdom.h"
#include <wisdom/wisdom.h>

DX12Factory DX12FactoryCreate()
{
    return reinterpret_cast<DX12Factory>(new wis::DX12Factory());
}
VKFactory VKFactoryCreate()
{
    return reinterpret_cast<VKFactory>(new wis::VKFactory());
}
void DX12FactoryDestroy(DX12Factory self)
{
    auto* xself = reinterpret_cast<wis::DX12Factory*>(self);
    delete xself;
}
void VKFactoryDestroy(VKFactory self)
{
    auto* xself = reinterpret_cast<wis::VKFactory*>(self);
    delete xself;
}

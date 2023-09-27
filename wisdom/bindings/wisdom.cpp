#include "wisdom.h"
#include <wisdom/wisdom.h>

WisResult DX12FactoryCreate(DX12Factory* out_handle, bool debug_layer)
{
    *out_handle = reinterpret_cast<DX12Factory>(new wis::DX12Factory(debug_layer));
}
WisResult VKFactoryCreate(VKFactory* out_handle, bool debug_layer)
{
    *out_handle = reinterpret_cast<VKFactory>(new wis::VKFactory(debug_layer));
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

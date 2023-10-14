#include "wisdom.h"
#include <wisdom/wisdom.h>

 WisResult  DX12CreateFactory( bool debug_layer,  WisDebugCallback callback,  void *user_data,  DX12Factory * out_factory)
{
    auto&& ret =     wis::DX12CreateFactory(debug_layer, reinterpret_cast<wis::DebugCallback>(callback), user_data);
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_factory = ok ? reinterpret_cast<DX12Factory>(new wis::DX12Factory(std::move(std::get<1>(ret)))) : reinterpret_cast<DX12Factory>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 WisResult  VKCreateFactory( bool debug_layer,  WisDebugCallback callback,  void *user_data,  VKFactory * out_factory)
{
    auto&& ret =     wis::VKCreateFactory(debug_layer, reinterpret_cast<wis::DebugCallback>(callback), user_data);
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_factory = ok ? reinterpret_cast<VKFactory>(new wis::VKFactory(std::move(std::get<1>(ret)))) : reinterpret_cast<VKFactory>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 WisResult  DX12FactoryGetAdapter( DX12Factory self,  uint32_t index,  WisAdapterPreference preference,  DX12Adapter * out_adapter)
{
    auto* xself = reinterpret_cast<wis::DX12Factory*>(self);
    auto&& ret =     xself->GetAdapter(index, preference);
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_adapter = ok ? reinterpret_cast<DX12Adapter>(new wis::DX12Adapter(std::move(std::get<1>(ret)))) : reinterpret_cast<DX12Adapter>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 WisResult  VKFactoryGetAdapter( VKFactory self,  uint32_t index,  WisAdapterPreference preference,  VKAdapter * out_adapter)
{
    auto* xself = reinterpret_cast<wis::VKFactory*>(self);
    auto&& ret =     xself->GetAdapter(index, preference);
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_adapter = ok ? reinterpret_cast<VKAdapter>(new wis::VKAdapter(std::move(std::get<1>(ret)))) : reinterpret_cast<VKAdapter>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 void  DX12FactoryDestroy( DX12Factory self)
{
    auto* xself = reinterpret_cast<wis::DX12Factory*>(self);
    delete xself;
}
 void  VKFactoryDestroy( VKFactory self)
{
    auto* xself = reinterpret_cast<wis::VKFactory*>(self);
    delete xself;
}
 WisResult  DX12AdapterGetDesc( DX12Adapter self,  WisAdapterDesc *desc)
{
    auto* xself = reinterpret_cast<wis::DX12Adapter*>(self);
    auto&& ret =     xself->GetDesc(reinterpret_cast<wis::AdapterDesc*>(desc));
    return reinterpret_cast<WisResult&>(ret);
}
 WisResult  VKAdapterGetDesc( VKAdapter self,  WisAdapterDesc *desc)
{
    auto* xself = reinterpret_cast<wis::VKAdapter*>(self);
    auto&& ret =     xself->GetDesc(reinterpret_cast<wis::AdapterDesc*>(desc));
    return reinterpret_cast<WisResult&>(ret);
}

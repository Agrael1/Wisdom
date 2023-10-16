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
 WisResult  DX12CreateDevice( DX12Factory factory,  DX12Adapter adapter,  DX12Device * out_device)
{
    auto&& ret =     wis::DX12CreateDevice(*reinterpret_cast<wis::DX12Factory*>(factory), *reinterpret_cast<wis::DX12Adapter*>(adapter));
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_device = ok ? reinterpret_cast<DX12Device>(new wis::DX12Device(std::move(std::get<1>(ret)))) : reinterpret_cast<DX12Device>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 WisResult  VKCreateDevice( VKFactory factory,  VKAdapter adapter,  VKDevice * out_device)
{
    auto&& ret =     wis::VKCreateDevice(*reinterpret_cast<wis::VKFactory*>(factory), *reinterpret_cast<wis::VKAdapter*>(adapter));
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_device = ok ? reinterpret_cast<VKDevice>(new wis::VKDevice(std::move(std::get<1>(ret)))) : reinterpret_cast<VKDevice>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 void  DX12DeviceDestroy( DX12Device self)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    delete xself;
}
 void  VKDeviceDestroy( VKDevice self)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    delete xself;
}
 WisResult  DX12DeviceCreateFence( DX12Device self,  uint64_t initial_value,  DX12Fence * out_fence)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& ret =     xself->CreateFence(initial_value);
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_fence = ok ? reinterpret_cast<DX12Fence>(new wis::DX12Fence(std::move(std::get<1>(ret)))) : reinterpret_cast<DX12Fence>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 WisResult  VKDeviceCreateFence( VKDevice self,  uint64_t initial_value,  VKFence * out_fence)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& ret =     xself->CreateFence(initial_value);
    bool ok = std::get<0>(ret).status == wis::Status::Success;
    *out_fence = ok ? reinterpret_cast<VKFence>(new wis::VKFence(std::move(std::get<1>(ret)))) : reinterpret_cast<VKFence>(nullptr);
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
 void  DX12FenceDestroy( DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    delete xself;
}
 void  VKFenceDestroy( VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    delete xself;
}
 uint64_t  DX12FenceGetCompletedValue( DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto&& ret =     xself->GetCompletedValue();
    return reinterpret_cast<uint64_t&>(ret);
}
 uint64_t  VKFenceGetCompletedValue( VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto&& ret =     xself->GetCompletedValue();
    return reinterpret_cast<uint64_t&>(ret);
}
 WisResult  DX12FenceWait( DX12Fence self,  uint64_t value,  uint64_t timeout_ns)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto&& ret =     xself->Wait(value, timeout_ns);
    return reinterpret_cast<WisResult&>(ret);
}
 WisResult  VKFenceWait( VKFence self,  uint64_t value,  uint64_t timeout_ns)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto&& ret =     xself->Wait(value, timeout_ns);
    return reinterpret_cast<WisResult&>(ret);
}
 WisResult  DX12FenceSignal( DX12Fence self,  uint64_t value)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto&& ret =     xself->Signal(value);
    return reinterpret_cast<WisResult&>(ret);
}
 WisResult  VKFenceSignal( VKFence self,  uint64_t value)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto&& ret =     xself->Signal(value);
    return reinterpret_cast<WisResult&>(ret);
}

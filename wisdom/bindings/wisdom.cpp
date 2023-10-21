#include "wisdom.h"
#include <wisdom/wisdom.h>

DX12FenceView AsDX12FenceView(DX12Fence* self)
{
    return reinterpret_cast<DX12FenceView&>(static_cast<wis::DX12FenceView>(self));
}
VKFenceView AsVKFenceView(VKFence* self)
{
    return reinterpret_cast<VKFenceView&>(static_cast<wis::VKFenceView>(self));
}
WisResult DX12CreateFactory( bool debug_layer,  DebugCallback callback,  void* user_data, DX12Factory* out_factory)
{
    auto&& ret = wis::DX12CreateFactory(debug_layer, reinterpret_cast<DebugCallback>(callback), user_data);
    *out_factory = reinterpret_cast<DX12Factory>(new wis::DX12Factory(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
WisResult VKCreateFactory( bool debug_layer,  DebugCallback callback,  void* user_data, VKFactory* out_factory)
{
    auto&& ret = wis::VKCreateFactory(debug_layer, reinterpret_cast<DebugCallback>(callback), user_data);
    *out_factory = reinterpret_cast<VKFactory>(new wis::VKFactory(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
WisResult DX12GetAdapter(DX12Factory self,  uint32_t index,  WisAdapterPreference preference, DX12Adapter* out_adapter)
{
    auto* xself = reinterpret_cast<wis::DX12Factory*>(self);
    auto&& ret = xself->GetAdapter(index, reinterpret_cast<wis::AdapterPreference>(preference));
    *out_adapter = reinterpret_cast<DX12Adapter>(new wis::DX12Adapter(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
WisResult VKGetAdapter(VKFactory self,  uint32_t index,  WisAdapterPreference preference, VKAdapter* out_adapter)
{
    auto* xself = reinterpret_cast<wis::VKFactory*>(self);
    auto&& ret = xself->GetAdapter(index, reinterpret_cast<wis::AdapterPreference>(preference));
    *out_adapter = reinterpret_cast<VKAdapter>(new wis::VKAdapter(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
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
WisResult DX12GetDesc(DX12Adapter self,  WisAdapterDesc* desc)
{
    auto* xself = reinterpret_cast<wis::DX12Adapter*>(self);
    auto&& ret = xself->GetDesc(reinterpret_cast<wis::AdapterDesc*>(desc));
    return reinterpret_cast<WisResult&>(ret);
}
WisResult VKGetDesc(VKAdapter self,  WisAdapterDesc* desc)
{
    auto* xself = reinterpret_cast<wis::VKAdapter*>(self);
    auto&& ret = xself->GetDesc(reinterpret_cast<wis::AdapterDesc*>(desc));
    return reinterpret_cast<WisResult&>(ret);
}
WisResult DX12CreateDevice( DX12Factory factory,  DX12Adapter adapter, DX12Device* out_device)
{
    auto&& ret = wis::DX12CreateDevice(*reinterpret_cast<wis::Factory*>(factory), *reinterpret_cast<wis::Adapter*>(adapter));
    *out_device = reinterpret_cast<DX12Device>(new wis::DX12Device(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
WisResult VKCreateDevice( VKFactory factory,  VKAdapter adapter, VKDevice* out_device)
{
    auto&& ret = wis::VKCreateDevice(*reinterpret_cast<wis::Factory*>(factory), *reinterpret_cast<wis::Adapter*>(adapter));
    *out_device = reinterpret_cast<VKDevice>(new wis::VKDevice(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
void DX12DeviceDestroy(DX12Device self)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    delete xself;
}
void VKDeviceDestroy(VKDevice self)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    delete xself;
}
WisResult DX12CreateFence(DX12Device self,  uint64_t initial_value, DX12Fence* out_fence)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& ret = xself->CreateFence(initial_value);
    *out_fence = reinterpret_cast<DX12Fence>(new wis::DX12Fence(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
WisResult VKCreateFence(VKDevice self,  uint64_t initial_value, VKFence* out_fence)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& ret = xself->CreateFence(initial_value);
    *out_fence = reinterpret_cast<VKFence>(new wis::VKFence(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
void DX12FenceDestroy(DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    delete xself;
}
void VKFenceDestroy(VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    delete xself;
}
uint64_t DX12GetCompletedValue(DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto&& ret = xself->GetCompletedValue();
    return reinterpret_cast<uint64_t&>(ret);
}
uint64_t VKGetCompletedValue(VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto&& ret = xself->GetCompletedValue();
    return reinterpret_cast<uint64_t&>(ret);
}
WisResult DX12Wait(DX12Fence self,  uint64_t value,  uint64_t timeout_ns)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto&& ret = xself->Wait(value, timeout_ns);
    return reinterpret_cast<WisResult&>(ret);
}
WisResult VKWait(VKFence self,  uint64_t value,  uint64_t timeout_ns)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto&& ret = xself->Wait(value, timeout_ns);
    return reinterpret_cast<WisResult&>(ret);
}
WisResult DX12Signal(DX12Fence self,  uint64_t value)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    auto&& ret = xself->Signal(value);
    return reinterpret_cast<WisResult&>(ret);
}
WisResult VKSignal(VKFence self,  uint64_t value)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    auto&& ret = xself->Signal(value);
    return reinterpret_cast<WisResult&>(ret);
}
WisResult DX12CreateAllocator(DX12Device self, DX12ResourceAllocator* out_allocator)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    auto&& ret = xself->CreateAllocator();
    *out_allocator = reinterpret_cast<DX12ResourceAllocator>(new wis::DX12ResourceAllocator(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
WisResult VKCreateAllocator(VKDevice self, VKResourceAllocator* out_allocator)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    auto&& ret = xself->CreateAllocator();
    *out_allocator = reinterpret_cast<VKResourceAllocator>(new wis::VKResourceAllocator(std::move(std::get<1>(ret))));
    return reinterpret_cast<WisResult&>(std::get<0>(ret));
}
void DX12ResourceAllocatorDestroy(DX12ResourceAllocator self)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    delete xself;
}
void VKResourceAllocatorDestroy(VKResourceAllocator self)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    delete xself;
}

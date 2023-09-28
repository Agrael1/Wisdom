#include "wisdom.h"
#include <wisdom/wisdom.h>

WisResult DX12FactoryCreate(DX12Factory* out_handle,  bool debug_layer,  WisDebugCallback callback,  void* user_data)
{
    std::unique_ptr<wis::DX12Factory> ret{ new wis::DX12Factory(); };
    WisResult result = reinterpret_cast<WisResult&>(ret->Initialize(debug_layer, reinterpret_cast<wis::DebugCallback>(callback), user_data));
    if (result != WisResult::Success) {{
*out_handle = nullptr;
return result;
}}
    *out_handle = reinterpret_cast<DX12Factory>(ret.release());
return result;
}
WisResult VKFactoryCreate(VKFactory* out_handle,  bool debug_layer,  WisDebugCallback callback,  void* user_data)
{
    std::unique_ptr<wis::VKFactory> ret{ new wis::VKFactory(); };
    WisResult result = reinterpret_cast<WisResult&>(ret->Initialize(debug_layer, reinterpret_cast<wis::DebugCallback>(callback), user_data));
    if (result != WisResult::Success) {{
*out_handle = nullptr;
return result;
}}
    *out_handle = reinterpret_cast<VKFactory>(ret.release());
return result;
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

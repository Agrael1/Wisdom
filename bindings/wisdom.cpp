#include "wisdom.h"

#ifdef WISDOM_DX12
#include <wisdom/wisdom_dx12.h>

extern "C" DX12FenceView AsDX12FenceView(DX12Fence self)
{
    wis::DX12FenceView xself = reinterpret_cast<wis::DX12Fence&>(*self);
    return reinterpret_cast<DX12FenceView&>(xself);
}
extern "C" DX12BufferView AsDX12BufferView(DX12Buffer self)
{
    wis::DX12BufferView xself = reinterpret_cast<wis::DX12Buffer&>(*self);
    return reinterpret_cast<DX12BufferView&>(xself);
}
extern "C" DX12TextureView AsDX12TextureView(DX12Texture self)
{
    wis::DX12TextureView xself = reinterpret_cast<wis::DX12Texture&>(*self);
    return reinterpret_cast<DX12TextureView&>(xself);
}
extern "C" DX12RenderTargetView AsDX12RenderTargetView(DX12RenderTarget self)
{
    wis::DX12RenderTargetView xself = reinterpret_cast<wis::DX12RenderTarget&>(*self);
    return reinterpret_cast<DX12RenderTargetView&>(xself);
}
extern "C" DX12CommandListView AsDX12CommandListView(DX12CommandList self)
{
    wis::DX12CommandListView xself = reinterpret_cast<wis::DX12CommandList&>(*self);
    return reinterpret_cast<DX12CommandListView&>(xself);
}
extern "C" DX12ShaderView AsDX12ShaderView(DX12Shader self)
{
    wis::DX12ShaderView xself = reinterpret_cast<wis::DX12Shader&>(*self);
    return reinterpret_cast<DX12ShaderView&>(xself);
}
extern "C" DX12RootSignatureView AsDX12RootSignatureView(DX12RootSignature self)
{
    wis::DX12RootSignatureView xself = reinterpret_cast<wis::DX12RootSignature&>(*self);
    return reinterpret_cast<DX12RootSignatureView&>(xself);
}
extern "C" DX12DescriptorBufferView AsDX12DescriptorBufferView(DX12DescriptorBuffer self)
{
    wis::DX12DescriptorBufferView xself = reinterpret_cast<wis::DX12DescriptorBuffer&>(*self);
    return reinterpret_cast<DX12DescriptorBufferView&>(xself);
}
extern "C" WISDOM_API void DX12FactoryDestroy(DX12Factory self)
{
    auto* xself = reinterpret_cast<wis::DX12Factory*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12AdapterDestroy(DX12Adapter self)
{
    auto* xself = reinterpret_cast<wis::DX12Adapter*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12DeviceDestroy(DX12Device self)
{
    auto* xself = reinterpret_cast<wis::DX12Device*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12FenceDestroy(DX12Fence self)
{
    auto* xself = reinterpret_cast<wis::DX12Fence*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12ResourceAllocatorDestroy(DX12ResourceAllocator self)
{
    auto* xself = reinterpret_cast<wis::DX12ResourceAllocator*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12RootSignatureDestroy(DX12RootSignature self)
{
    auto* xself = reinterpret_cast<wis::DX12RootSignature*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12CommandQueueDestroy(DX12CommandQueue self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandQueue*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12ShaderDestroy(DX12Shader self)
{
    auto* xself = reinterpret_cast<wis::DX12Shader*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12PipelineStateDestroy(DX12PipelineState self)
{
    auto* xself = reinterpret_cast<wis::DX12PipelineState*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12CommandListDestroy(DX12CommandList self)
{
    auto* xself = reinterpret_cast<wis::DX12CommandList*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12BufferDestroy(DX12Buffer self)
{
    auto* xself = reinterpret_cast<wis::DX12Buffer*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12TextureDestroy(DX12Texture self)
{
    auto* xself = reinterpret_cast<wis::DX12Texture*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12SwapChainDestroy(DX12SwapChain self)
{
    auto* xself = reinterpret_cast<wis::DX12SwapChain*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12DebugMessengerDestroy(DX12DebugMessenger self)
{
    auto* xself = reinterpret_cast<wis::DX12DebugMessenger*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12RenderTargetDestroy(DX12RenderTarget self)
{
    auto* xself = reinterpret_cast<wis::DX12RenderTarget*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12DescriptorBufferDestroy(DX12DescriptorBuffer self)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBuffer*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12SamplerDestroy(DX12Sampler self)
{
    auto* xself = reinterpret_cast<wis::DX12Sampler*>(self);
    delete xself;
}
extern "C" WISDOM_API void DX12ShaderResourceDestroy(DX12ShaderResource self)
{
    auto* xself = reinterpret_cast<wis::DX12ShaderResource*>(self);
    delete xself;
}
extern "C" WISDOM_API WisResult DX12CreateFactory( bool debug_layer, DX12Factory* out_factory)
{
    auto&& ret = wis::DX12CreateFactory(debug_layer);
    bool ok = wis::get<0>(ret).status == wis::Status::Ok;
    *out_factory = ok ? reinterpret_cast<DX12Factory>(new wis::DX12Factory(std::move(wis::get<1>(ret)))) : nullptr;
    return reinterpret_cast<WisResult&>(wis::get<0>(ret));
}
extern "C" WISDOM_API WisResult DX12CreateDevice( DX12Adapter adapter, DX12Device* out_device)
{
    auto&& ret = wis::DX12CreateDevice(*reinterpret_cast<wis::DX12Adapter*>(adapter));
    bool ok = wis::get<0>(ret).status == wis::Status::Ok;
    *out_device = ok ? reinterpret_cast<DX12Device>(new wis::DX12Device(std::move(wis::get<1>(ret)))) : nullptr;
    return reinterpret_cast<WisResult&>(wis::get<0>(ret));
}
#endif

#ifdef WISDOM_VULKAN
#include <wisdom/wisdom_vk.h>

extern "C" VKFenceView AsVKFenceView(VKFence self)
{
    wis::VKFenceView xself = reinterpret_cast<wis::VKFence&>(*self);
    return reinterpret_cast<VKFenceView&>(xself);
}
extern "C" VKBufferView AsVKBufferView(VKBuffer self)
{
    wis::VKBufferView xself = reinterpret_cast<wis::VKBuffer&>(*self);
    return reinterpret_cast<VKBufferView&>(xself);
}
extern "C" VKTextureView AsVKTextureView(VKTexture self)
{
    wis::VKTextureView xself = reinterpret_cast<wis::VKTexture&>(*self);
    return reinterpret_cast<VKTextureView&>(xself);
}
extern "C" VKRenderTargetView AsVKRenderTargetView(VKRenderTarget self)
{
    wis::VKRenderTargetView xself = reinterpret_cast<wis::VKRenderTarget&>(*self);
    return reinterpret_cast<VKRenderTargetView&>(xself);
}
extern "C" VKCommandListView AsVKCommandListView(VKCommandList self)
{
    wis::VKCommandListView xself = reinterpret_cast<wis::VKCommandList&>(*self);
    return reinterpret_cast<VKCommandListView&>(xself);
}
extern "C" VKShaderView AsVKShaderView(VKShader self)
{
    wis::VKShaderView xself = reinterpret_cast<wis::VKShader&>(*self);
    return reinterpret_cast<VKShaderView&>(xself);
}
extern "C" VKRootSignatureView AsVKRootSignatureView(VKRootSignature self)
{
    wis::VKRootSignatureView xself = reinterpret_cast<wis::VKRootSignature&>(*self);
    return reinterpret_cast<VKRootSignatureView&>(xself);
}
extern "C" VKDescriptorBufferView AsVKDescriptorBufferView(VKDescriptorBuffer self)
{
    wis::VKDescriptorBufferView xself = reinterpret_cast<wis::VKDescriptorBuffer&>(*self);
    return reinterpret_cast<VKDescriptorBufferView&>(xself);
}
extern "C" WISDOM_API void VKFactoryDestroy(VKFactory self)
{
    auto* xself = reinterpret_cast<wis::VKFactory*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKAdapterDestroy(VKAdapter self)
{
    auto* xself = reinterpret_cast<wis::VKAdapter*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKDeviceDestroy(VKDevice self)
{
    auto* xself = reinterpret_cast<wis::VKDevice*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKFenceDestroy(VKFence self)
{
    auto* xself = reinterpret_cast<wis::VKFence*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKResourceAllocatorDestroy(VKResourceAllocator self)
{
    auto* xself = reinterpret_cast<wis::VKResourceAllocator*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKRootSignatureDestroy(VKRootSignature self)
{
    auto* xself = reinterpret_cast<wis::VKRootSignature*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKCommandQueueDestroy(VKCommandQueue self)
{
    auto* xself = reinterpret_cast<wis::VKCommandQueue*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKShaderDestroy(VKShader self)
{
    auto* xself = reinterpret_cast<wis::VKShader*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKPipelineStateDestroy(VKPipelineState self)
{
    auto* xself = reinterpret_cast<wis::VKPipelineState*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKCommandListDestroy(VKCommandList self)
{
    auto* xself = reinterpret_cast<wis::VKCommandList*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKBufferDestroy(VKBuffer self)
{
    auto* xself = reinterpret_cast<wis::VKBuffer*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKTextureDestroy(VKTexture self)
{
    auto* xself = reinterpret_cast<wis::VKTexture*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKSwapChainDestroy(VKSwapChain self)
{
    auto* xself = reinterpret_cast<wis::VKSwapChain*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKDebugMessengerDestroy(VKDebugMessenger self)
{
    auto* xself = reinterpret_cast<wis::VKDebugMessenger*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKRenderTargetDestroy(VKRenderTarget self)
{
    auto* xself = reinterpret_cast<wis::VKRenderTarget*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKDescriptorBufferDestroy(VKDescriptorBuffer self)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBuffer*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKSamplerDestroy(VKSampler self)
{
    auto* xself = reinterpret_cast<wis::VKSampler*>(self);
    delete xself;
}
extern "C" WISDOM_API void VKShaderResourceDestroy(VKShaderResource self)
{
    auto* xself = reinterpret_cast<wis::VKShaderResource*>(self);
    delete xself;
}
extern "C" WISDOM_API WisResult VKCreateFactory( bool debug_layer, VKFactory* out_factory)
{
    auto&& ret = wis::VKCreateFactory(debug_layer);
    bool ok = wis::get<0>(ret).status == wis::Status::Ok;
    *out_factory = ok ? reinterpret_cast<VKFactory>(new wis::VKFactory(std::move(wis::get<1>(ret)))) : nullptr;
    return reinterpret_cast<WisResult&>(wis::get<0>(ret));
}
extern "C" WISDOM_API WisResult VKCreateDevice( VKAdapter adapter, VKDevice* out_device)
{
    auto&& ret = wis::VKCreateDevice(*reinterpret_cast<wis::VKAdapter*>(adapter));
    bool ok = wis::get<0>(ret).status == wis::Status::Ok;
    *out_device = ok ? reinterpret_cast<VKDevice>(new wis::VKDevice(std::move(wis::get<1>(ret)))) : nullptr;
    return reinterpret_cast<WisResult&>(wis::get<0>(ret));
}
#endif


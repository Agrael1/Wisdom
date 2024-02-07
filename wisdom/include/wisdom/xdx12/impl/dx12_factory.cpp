#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_factory.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xdx12/dx12_checks.h>

wis::DX12Factory::DX12Factory(wis::com_ptr<IDXGIFactory6> factory, bool debug_layer,
                              wis::DebugCallback callback,
                              void *user_data) noexcept
    : QueryInternal(std::move(factory)) {
  if constexpr (wis::debug_layer) {
    if (debug_layer && callback)
      EnableDebugLayer(callback, user_data);
  }
}

[[nodiscard]] std::pair<wis::Result, wis::DX12Factory>
wis::DX12CreateFactory(bool debug_layer, wis::DebugCallback callback, void *user_data) noexcept {
  wis::com_ptr<IDXGIFactory6> factory;
  auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG, __uuidof(*factory),
                               factory.put_void());

  if (!wis::succeeded(hr)) {
    hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory4),
                            factory.put_void());
    DX12Factory::has_preference = false;
    if (!wis::succeeded(hr)) {
      return {wis::make_result<FUNC, "Failed to create DXGI factory">(hr), wis::DX12Factory{}};
    }
  }
  return {wis::success, DX12Factory(std::move(factory), debug_layer, callback, user_data)};
}

wis::DX12Factory::~DX12Factory() noexcept {
  if constexpr (wis::debug_layer) {
    DX12Info::RemoveCallback(this);
    token.Release();
  }
}

wis::DX12Factory::DX12Factory(DX12Factory &&other) noexcept
    : QueryInternal<DX12Factory>(std::move(other)) {
  if constexpr (wis::debug_layer) {
    if (DX12Info::RebindCallback(this, &other))
      token.Acquire();
  }
}

[[nodiscard]] inline std::pair<wis::Result, wis::DX12Adapter>
wis::DX12Factory::GetAdapter(uint32_t index, AdapterPreference preference) const noexcept {
  auto gen = has_preference ? GetAdapterByGPUPreference(index, convert_dx(preference))
                            : GetAdapter1(index);
  return !gen ? std::pair{wis::make_result<FUNC, "Failed to get adapter">(gen.result),
                          wis::DX12Adapter{}}
              : std::pair{wis::success, wis::DX12Adapter(std::move(gen.ptr))};
}

void wis::DX12Factory::EnableDebugLayer(DebugCallback callback, void *user_data) noexcept {
  if (callback) {
    token.Acquire();
    DX12Info::AddCallback(this, callback, user_data);
  }

  wis::com_ptr<ID3D12Debug> debugController;
  if (wis::succeeded(
          D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
    debugController->EnableDebugLayer();

  if (auto dc = debugController.as<ID3D12Debug1>())
    dc->SetEnableGPUBasedValidation(true);
}

wis::DX12Factory & wis::DX12Factory::operator =(DX12Factory && other) noexcept {
  QueryInternal<DX12Factory>::operator=(std::move(other));
  if constexpr (wis::debug_layer) {
    if (DX12Info::RebindCallback(this, &other))
      token.Acquire();
  }
  return *this;
}

inline wis::com_with_result<IDXGIAdapter1>
wis::DX12Factory::GetAdapterByGPUPreference(uint32_t index,
                                            DXGI_GPU_PREFERENCE preference) const noexcept {
  wis::com_ptr<IDXGIAdapter1> adapter;
  auto hr = factory->EnumAdapterByGpuPreference(index, preference, __uuidof(*adapter),
                                                adapter.put_void());
  return {hr, std::move(adapter)};
}

inline wis::com_with_result<IDXGIAdapter1>
wis::DX12Factory::GetAdapter1(uint32_t index) const noexcept {
  wis::com_ptr<IDXGIAdapter1> adapter;
  auto hr = factory->EnumAdapters1(index, adapter.put());
  return {hr, std::move(adapter)};
}

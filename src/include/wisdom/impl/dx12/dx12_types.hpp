#ifndef WIS_DX12_TYPES_H
#define WIS_DX12_TYPES_H
#ifdef __cplusplus
#include <wisdom/generated/dx12_api.h>
#include <wisdom/util/com_ptr.h>
#include <dxgi1_6.h>

namespace wis::impl {
struct DX12FactoryInternal {
    wis::com_ptr<IDXGIFactory6> factory;
};
} // namespace wis::impl

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_instance.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // __cplusplus
#endif // DX12_FACTORY_H

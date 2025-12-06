#ifndef WIS_DX12_TYPES_H
#define WIS_DX12_TYPES_H
#ifdef __cplusplus
#include <dxgi1_6.h>

namespace wis {
namespace impl {
struct DX12InstanceImpl {
    IDXGIFactory6* factory = nullptr;
};
} // namespace impl
} // namespace wis

#endif // __cplusplus
#endif // DX12_FACTORY_H

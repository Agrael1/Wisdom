#ifndef WIS_DX12_FACTORY_H
#define WIS_DX12_FACTORY_H
#include <wisdom/generated/c_api.h>

WIS_EXTERN_C WisResult WisDX12CreateFactory(const struct WisFactoryCreateInfo* info, WisDX12Instance* out_factory);

///**
// * @brief Creates a DX12 factory for adapter enumeration
// * @param result Pointer to wis::Result for operation status
// * @param enable_debug Enable DirectX 12 debug layer if true
// * @param extensions Array of factory extensions (may be nullptr)
// * @param extension_count Number of extensions in array
// * @return wis::DX12Factory object
// *
// * @description
// * Creates the main factory object for DirectX 12 adapter enumeration.
// *
// * @validusage_begin
// * @vku result <b>MUST</b> be a valid pointer
// * @vku If extension_count > 0, extensions <b>MUST</b> be valid
// * @validusage_end
// *
// * For detailed documentation, see @ref dx12_factory_creation
// */
//[[nodiscard]] WIS_INLINE wis::DX12Factory
// ImplDX12CreateFactory(wis::Result& result, bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept;

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_factory.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // DX12_FACTORY_H

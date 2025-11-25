#ifndef WIS_DX12_FACTORY_H
#define WIS_DX12_FACTORY_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_factory_ext.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
WISDOM_EXPORT class DX12Factory;

WISDOM_EXPORT
template<>
struct Internal<DX12Factory> {
    wis::com_ptr<IDXGIFactory6> factory;
};

/// @brief Main Factory class
class ImplDX12Factory : public QueryInternal<DX12Factory>
{
    friend wis::DX12Factory
    ImplDX12CreateFactory(bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count, wis::Result& res) noexcept;

public:
    ImplDX12Factory() noexcept = default;
    ImplDX12Factory(ImplDX12Factory&& other) noexcept = default;
    ImplDX12Factory& operator=(ImplDX12Factory&& other) noexcept = default;

    operator bool() const noexcept
    {
        return bool(factory);
    }
    operator DX12FactoryHandle() const noexcept
    {
        return factory.get();
    }

public:
    [[nodiscard]] WIS_INLINE wis::DX12Adapter
    GetAdapter(wis::Result& result, uint32_t index,
               AdapterPreference preference = AdapterPreference::Performance) const noexcept;
};

#pragma region DX12Factory
/**
 * @brief Class for creating adapters.
 * Can be safely destroyed after adapter has been chosen.
 * */
WISDOM_EXPORT
class DX12Factory : public wis::ImplDX12Factory
{
public:
    using wis::ImplDX12Factory::ImplDX12Factory;
    DX12Factory(const DX12Factory&) = delete;
    DX12Factory(DX12Factory&&) noexcept = default;
    DX12Factory& operator=(const DX12Factory&) = delete;
    DX12Factory& operator=(DX12Factory&&) noexcept = default;

public:
    /**
     * @brief Creates the wis::DX12Adapter for the factory with provided index.
     * @param index The index of the adapter to get.
     * @param preference The preference of the adapter to get.
     * Default is wis::AdapterPreference::Performance.
     * @return wis::DX12Adapter on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12Adapter GetAdapter(wis::Result& result, uint32_t index, wis::AdapterPreference preference = wis::AdapterPreference::Performance) const noexcept
    {
        return wis::ImplDX12Factory::GetAdapter(result, index, preference);
    }
    /**
     * @brief Creates the wis::DX12Adapter for the factory with provided index.
     * @param index The index of the adapter to get.
     * @param preference The preference of the adapter to get.
     * Default is wis::AdapterPreference::Performance.
     * @return wis::DX12Adapter on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Adapter> GetAdapter(uint32_t index, wis::AdapterPreference preference = wis::AdapterPreference::Performance) const noexcept
    {
        return wis::ResultValue<wis::DX12Adapter>{ &wis::ImplDX12Factory::GetAdapter, this, index, preference };
    }
};
#pragma endregion DX12Factory

/**
 * @brief Creates a DX12 factory for adapter enumeration
 * @param result Pointer to wis::Result for operation status
 * @param enable_debug Enable DirectX 12 debug layer if true
 * @param extensions Array of factory extensions (may be nullptr)
 * @param extension_count Number of extensions in array
 * @return wis::DX12Factory object
 * 
 * @description
 * Creates the main factory object for DirectX 12 adapter enumeration.
 * 
 * @validusage_begin
 * @vku result <b>MUST</b> be a valid pointer
 * @vku If extension_count > 0, extensions <b>MUST</b> be valid
 * @validusage_end
 * 
 * For detailed documentation, see @ref dx12_factory_creation
 */
[[nodiscard]] WIS_INLINE wis::DX12Factory
ImplDX12CreateFactory(wis::Result& result, bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept;
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_factory.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // DX12_FACTORY_H

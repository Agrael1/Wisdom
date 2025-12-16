// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_API_HPP
#define WISDOM_CPP_DX12_API_HPP
#ifdef __cplusplus
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/impl/dx12/dx12_types.hpp>

namespace wis {

struct DX12AdapterQueryDeleter {
    void operator()(WisDX12AdapterQuery* handle) noexcept
    {
        ::wisDX12DestroyAdapterQuery(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
class DX12AdapterQuery : public wis::impl::Implements<wis::impl::DX12AdapterQueryImpl, WisDX12AdapterQuery, wis::DX12AdapterQueryDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12InstanceDeleter {
    void operator()(WisDX12Instance* handle) noexcept
    {
        ::wisDX12DestroyInstance(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
class DX12Instance : public wis::impl::Implements<wis::impl::DX12InstanceImpl, WisDX12Instance, wis::DX12InstanceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them.
     * @param preference defines the order in which adapters are listed.
     * @param out_result denoting the outcome of operation.
     * @return query points to wis::AdapterQuery, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12AdapterQuery QueryAdapters(wis::AdapterPreference preference,
                                                             wis::Result&           out_result) const noexcept
    {
        wis::DX12AdapterQuery query;
        out_result = convert_result(::wisDX12QueryAdapters(&_impl_storage,
                                                           static_cast<WisAdapterPreference>(preference),
                                                           query.GetStorage()));
        return query;
    }
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::DX12Instance DX12CreateInstance(bool                                         debug_layer,
                                                          wis::span<wis::DX12InstanceExtensionHeader*> extensions,
                                                          wis::Result&                                 out_result) noexcept
{
    wis::DX12Instance instance;
    out_result = convert_result(::wisDX12CreateInstance(debug_layer,
                                                        reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
                                                        extensions.size(),
                                                        instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_DX12_API_HPP

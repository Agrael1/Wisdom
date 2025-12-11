// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_API_HPP
#define WISDOM_CPP_DX12_API_HPP
#ifdef __cplusplus
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/impl/dx12/dx12_types.hpp>

namespace wis {

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
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success (`wis::Status::Ok`).
 *
 * */
inline WIS_NODISCARD wis::DX12Instance DX12CreateInstance(bool                                         debug_layer,
                                                          wis::span<wis::DX12InstanceExtensionHeader*> extensions,
                                                          wis::Result&                                 out_result) noexcept
{
    wis::DX12Instance instance;
    out_result = reinterpret_cast<wis::Result&&>(::wisDX12CreateInstance(debug_layer,
                                                                         reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
                                                                         extensions.size(),
                                                                         instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_DX12_API_HPP

// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_API_HPP
#define WISDOM_CPP_DX12_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/impl/dx12/dx12_types.hpp>

namespace wis {

struct DX12DeviceDeleter {
    void operator()(WisDX12Device* handle) noexcept
    {
        ::wisDX12DestroyDevice(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
class DX12Device : public wis::impl::Implements<wis::impl::DX12DeviceImpl, WisDX12Device, wis::DX12DeviceDeleter>
{
public:
    using ImplType::ImplType;

public:
};

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
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
     * @return size is a number of adapters present on the system.
     *
     * */
    WIS_NODISCARD inline std::size_t GetAdapterCount() const noexcept
    {
        return (::wisDX12AdapterQueryGetAdapterCount(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
     * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wis::GetAdapterCount.
     * @param out_result denoting the outcome of operation.
     * @return desc points to wis::AdapterDesc, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::AdapterDesc GetAdapterDesc(std::size_t  index,
                                                         wis::Result& out_result) const noexcept
    {
        wis::AdapterDesc desc;
        out_result = convert_result(::wisDX12AdapterQueryGetAdapterDesc(&_impl_storage,
                                                                        index,
                                                                        reinterpret_cast<WisAdapterDesc*>(&desc)));
        return desc;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
     * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wis::GetAdapterCount.
     * @param extensions points to an array of extensions that are to be initialized with pointers to wis::DeviceExtensionHeader.
     * @param out_result denoting the outcome of operation.
     * @return device points to wis::Device, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Device CreateDevice(std::size_t                                index,
                                                      wis::span<wis::DX12DeviceExtensionHeader*> extensions,
                                                      wis::Result&                               out_result) const noexcept
    {
        wis::DX12Device device;
        out_result = convert_result(::wisDX12AdapterQueryCreateDevice(&_impl_storage,
                                                                      index,
                                                                      reinterpret_cast<WisDX12DeviceExtensionHeader**>(extensions.data()),
                                                                      extensions.size(),
                                                                      device.GetStorage()));
        return device;
    }
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
     * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software; Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete, Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
     * @param preference defines the order in which adapters are listed.
     * @param out_result denoting the outcome of operation.
     * @return query points to wis::AdapterQuery, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12AdapterQuery QueryAdapters(wis::AdapterPreference preference,
                                                             wis::Result&           out_result) const noexcept
    {
        wis::DX12AdapterQuery query;
        out_result = convert_result(::wisDX12InstanceQueryAdapters(&_impl_storage,
                                                                   static_cast<WisAdapterPreference>(preference),
                                                                   query.GetStorage()));
        return query;
    }
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc points to wis::DebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::DX12Instance DX12CreateInstance(const wis::DebugDesc*                        debug_desc,
                                                          wis::span<wis::DX12InstanceExtensionHeader*> extensions,
                                                          wis::Result&                                 out_result) noexcept
{
    wis::DX12Instance instance;
    out_result = convert_result(::wisDX12CreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
                                                        reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
                                                        extensions.size(),
                                                        instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // WISDOM_CPP_DX12_API_HPP

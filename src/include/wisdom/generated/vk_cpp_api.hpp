// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_VK_API_HPP
#define WISDOM_CPP_VK_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/impl/vulkan/vk_types.hpp>

namespace wis {

struct VKDeviceDeleter {
    void operator()(WisVKDevice* handle) noexcept
    {
        ::wisVKDestroyDevice(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
class VKDevice : public wis::impl::Implements<wis::impl::VKDeviceImpl, WisVKDevice, wis::VKDeviceDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct VKAdapterQueryDeleter {
    void operator()(WisVKAdapterQuery* handle) noexcept
    {
        ::wisVKDestroyAdapterQuery(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
class VKAdapterQuery : public wis::impl::Implements<wis::impl::VKAdapterQueryImpl, WisVKAdapterQuery, wis::VKAdapterQueryDeleter>
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
        return (::wisVKAdapterQueryGetAdapterCount(&_impl_storage));
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
        out_result = convert_result(::wisVKAdapterQueryGetAdapterDesc(&_impl_storage,
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
    WIS_NODISCARD inline wis::VKDevice CreateDevice(std::size_t                              index,
                                                    wis::span<wis::VKDeviceExtensionHeader*> extensions,
                                                    wis::Result&                             out_result) const noexcept
    {
        wis::VKDevice device;
        out_result = convert_result(::wisVKAdapterQueryCreateDevice(&_impl_storage,
                                                                    index,
                                                                    reinterpret_cast<WisVKDeviceExtensionHeader**>(extensions.data()),
                                                                    extensions.size(),
                                                                    device.GetStorage()));
        return device;
    }
};

struct VKInstanceDeleter {
    void operator()(WisVKInstance* handle) noexcept
    {
        ::wisVKDestroyInstance(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
class VKInstance : public wis::impl::Implements<wis::impl::VKInstanceImpl, WisVKInstance, wis::VKInstanceDeleter>
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
    WIS_NODISCARD inline wis::VKAdapterQuery QueryAdapters(wis::AdapterPreference preference,
                                                           wis::Result&           out_result) const noexcept
    {
        wis::VKAdapterQuery query;
        out_result = convert_result(::wisVKInstanceQueryAdapters(&_impl_storage,
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
WIS_NODISCARD inline wis::VKInstance VKCreateInstance(const wis::DebugDesc*                      debug_desc,
                                                      wis::span<wis::VKInstanceExtensionHeader*> extensions,
                                                      wis::Result&                               out_result) noexcept
{
    wis::VKInstance instance;
    out_result = convert_result(::wisVKCreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
                                                      reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
                                                      extensions.size(),
                                                      instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // WISDOM_CPP_VK_API_HPP

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
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::VKInstance VKCreateInstance(bool                                       debug_layer,
                                                      wis::span<wis::VKInstanceExtensionHeader*> extensions,
                                                      wis::Result&                               out_result) noexcept
{
    wis::VKInstance instance;
    out_result = convert_result(::wisVKCreateInstance(debug_layer,
                                                      reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
                                                      extensions.size(),
                                                      instance.GetStorage()));
    return instance;
}

} // namespace wis
#endif // WISDOM_CPP_VK_API_HPP

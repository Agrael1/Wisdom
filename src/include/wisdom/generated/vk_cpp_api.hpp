// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_VK_API_HPP
#define WISDOM_CPP_VK_API_HPP
#ifdef __cplusplus
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/impl/vulkan/vk_types.hpp>

namespace wis {

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
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success (`wis::Status::Ok`).
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
#endif // __cplusplus
#endif // WISDOM_CPP_VK_API_HPP

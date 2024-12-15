#ifndef WIS_VK_FACTORY_EXT_H
#define WIS_VK_FACTORY_EXT_H
#include <wisdom/generated/api/api.hpp>
#include <memory>
#include <span>

namespace wis {
class VKFactory;
}

namespace wis {
struct VKFactoryExtension {
    virtual ~VKFactoryExtension() = default;
    virtual bool Supported() const noexcept
    {
        return false;
    }
    virtual wis::Result Init(const wis::VKFactory& instance) noexcept
    {
        return {};
    }
    virtual std::span<const char* const> GetRequiredExtensions() const noexcept
    {
        return {};
    }
    virtual size_t RequiredExtensionsSize() const noexcept
    {
        return 0;
    }
    virtual std::span<const char* const> GetRequiredLayers() const noexcept
    {
        return {};
    }
    virtual size_t RequiredLayersSize() const noexcept
    {
        return 0;
    }
};

// Automatically generates everything for C api
template<typename T>
struct VKFactoryExtensionImpl : public VKFactoryExtension {
    virtual std::span<const char* const>
    GetRequiredExtensions() const noexcept override
    {
        if constexpr (requires { T::required_extensions; })
            return T::required_extensions;
        else
            return {};
    }

    virtual size_t
    RequiredExtensionsSize() const noexcept override
    {
        if constexpr (requires { T::required_extensions; })
            return T::required_extensions.size();
        else
            return 0;
    }

    virtual std::span<const char* const>
    GetRequiredLayers() const noexcept override
    {
        if constexpr (requires { T::required_layers; })
            return T::required_layers;
        else
            return {};
    }

    virtual size_t
    RequiredLayersSize() const noexcept
    {
        if constexpr (requires { T::required_layers; })
            return T::required_layers.size();
        else
            return 0;
    }
};
} // namespace wis

#endif // VK_FACTORY_EXT_H

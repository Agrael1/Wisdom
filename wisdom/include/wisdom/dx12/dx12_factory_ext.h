#ifndef WIS_DX12_FACTORY_EXT_H
#define WIS_DX12_FACTORY_EXT_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/generated/api/api.hpp>
#endif

namespace wis {
class DX12Factory;
}

WISDOM_EXPORT
namespace wis {
struct DX12FactoryExtension {
    virtual ~DX12FactoryExtension() = default;
    virtual wis::Result Init(const wis::DX12Factory& instance) noexcept
    {
        return {};
    }
    virtual bool Supported() const noexcept
    {
        return false;
    }
};
} // namespace wis
#endif // DX12_FACTORY_EXT_H

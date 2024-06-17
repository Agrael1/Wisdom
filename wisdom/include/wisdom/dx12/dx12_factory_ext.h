#ifndef DX12_FACTORY_EXT_H
#define DX12_FACTORY_EXT_H
#include <wisdom/generated/api/api.h>

namespace wis {
class DX12Factory;
}

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

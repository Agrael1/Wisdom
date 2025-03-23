#ifndef WIS_DX12_SHADER_H
#define WIS_DX12_SHADER_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <memory>
#endif

namespace wis {
class DX12Shader;

WISDOM_EXPORT
template<>
struct Internal<DX12Shader> {
    std::unique_ptr<std::byte[]> bytecode;
    size_t size = 0;
};

WISDOM_EXPORT
class DX12Shader : public QueryInternal<DX12Shader>
{
public:
    DX12Shader() = default;
    operator DX12ShaderView() const noexcept
    {
        return DX12ShaderView((void*)(bytecode.get()), uint32_t(size));
    }
    operator bool() const noexcept
    {
        return bool(bytecode);
    }
};
} // namespace wis

#endif // WIS_DX12_SHADER_H

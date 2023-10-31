#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/dx12/xdx12_views.h>
#include <memory>

namespace wis {
class DX12Shader;

template<>
struct Internal<DX12Shader> {
    std::unique_ptr<std::byte[]> bytecode;
    size_t size;
};

class DX12Shader : public QueryInternal<DX12Shader>
{
public:
    DX12Shader() = default;
    explicit DX12Shader(std::unique_ptr<std::byte[]> bytecode, size_t size) noexcept
        : QueryInternal(std::move(bytecode), size)
    {
    }
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

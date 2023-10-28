#pragma once
#include <wisdom/api/internal.h>
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
    operator bool() const noexcept
    {
        return bool(bytecode);
    }
};
} // namespace wis

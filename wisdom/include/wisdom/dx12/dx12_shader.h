#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#include <vector>
#include <span>

namespace wis
{
	class DX12Shader;

	template<>
	class Internal<DX12Shader>
	{
	public:
		std::span<const std::byte> GetShaderBytecode()const noexcept
		{
			return bytecode;
		}
	protected:
		std::vector<std::byte> bytecode;
	};

	class DX12Shader : public QueryInternal<DX12Shader>
	{
	public:
		DX12Shader() = default;
		explicit DX12Shader(std::vector<std::byte> xbytecode, ShaderType type)
			:type(type)
		{
			bytecode = std::move(xbytecode);
		}
	public:
		operator bool()const noexcept
		{
			return type != ShaderType::unknown && !bytecode.empty();
		}
		auto GetType()const noexcept
		{
			return type;
		}
	public:
		ShaderType type = ShaderType::unknown;
	};
}
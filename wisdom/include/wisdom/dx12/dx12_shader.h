#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#include <span>

namespace wis
{
	class DX12Shader;

	template<>
	class Internal<DX12Shader>
	{
	public:
		Internal() = default;
		Internal(shared_blob<std::byte> blob) :bytecode(std::move(blob)) {};
	public:
		std::span<const std::byte> GetShaderBytecode()const noexcept
		{
			return bytecode.GetSpan();
		}
	protected:
		wis::shared_blob<std::byte> bytecode;
	};

	class DX12Shader : public QueryInternal<DX12Shader>
	{
	public:
		using DataType = std::byte;
		static constexpr inline ShaderLang language = ShaderLang::dxil;
	public:
		DX12Shader() = default;
		explicit DX12Shader(shared_blob<std::byte> blob, ShaderType type)
			:QueryInternal(std::move(blob)), type(type)
		{}
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
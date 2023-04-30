#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#include <vector>
#include <span>

namespace wis
{
	class VKShader;

	template<>
	class Internal<VKShader>
	{
	public:
		std::span<const uint32_t> GetShaderBytecode()const noexcept
		{
			return bytecode.GetSpan();
		}
	protected:
		wis::shared_blob<uint32_t> bytecode;
	};

	class VKShader : public QueryInternal<VKShader>
	{
	public:
		using DataType = uint32_t;
		static constexpr inline ShaderLang language = ShaderLang::spirv;
	public:
		VKShader() = default;
		explicit VKShader(std::shared_ptr<DataType[]> xbytecode, size_t size, ShaderType type)
			:type(type)
		{
			bytecode = { std::move(xbytecode), size };
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
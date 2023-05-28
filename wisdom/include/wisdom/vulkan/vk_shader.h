#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#include <wisdom/vulkan/vk_shared_handle.h>

namespace wis
{
	class VKShader;

	template<>
	class Internal<VKShader>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::ShaderModule> module):module(std::move(module)) {};
	public:
		auto GetShaderModule()const noexcept
		{
			return module.get();
		}
	protected:
		wis::shared_handle<vk::ShaderModule> module;
	};

	/// @brief Shader object
	class VKShader : public QueryInternal<VKShader>
	{
	public:
		using DataType = uint32_t;
		static constexpr inline ShaderLang language = ShaderLang::spirv;
	public:
		VKShader() = default;
		explicit VKShader(wis::shared_handle<vk::ShaderModule> module, ShaderType type)
			:QueryInternal(std::move(module)), type(type)
		{}
	public:
		operator bool()const noexcept
		{
			return type != ShaderType::unknown && module;
		}
		/// @brief Get shader type e.g. vertex, fragment
		/// @return Type of shader
		[[nodiscard]]
		auto GetType()const noexcept
		{
			return type;
		}
	public:
		ShaderType type = ShaderType::unknown;
	};
}
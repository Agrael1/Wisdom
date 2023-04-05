#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>
#include <filesystem>

namespace wis
{
	class DX12Shader;

	template<>
	class Internal<DX12Shader>
	{
	public:
		auto GetShaderBytecode()const noexcept
		{
			return shader;
		}
	protected:
		winrt::com_ptr<ID3DBlob> shader;
	};

	class DX12Shader : public QueryInternal<DX12Shader>
	{
	public:
		DX12Shader() = default;
		explicit DX12Shader(winrt::com_ptr<ID3DBlob> xshader)
		{
			shader = std::move(xshader);
		}
	public:

	};
}
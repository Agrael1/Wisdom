#pragma once
#include <memory>
#include <span>

namespace wis
{
	enum class ShaderLang
	{
		dxil,
		spirv
	};

	enum class ShaderType
	{
		unknown,
		vertex,
		pixel,
		geometry,
		hull,
		domain,
		amplification,
		mesh,
		compute
	};

	template<typename DataTy>
	struct shared_blob
	{
	public:
		shared_blob() = default;
		shared_blob(std::shared_ptr<const DataTy[]> data, size_t size)
			:data(std::move(data)), size(size){}
	public:
		std::span<const DataTy> GetSpan()const noexcept
		{
			return { data.get(), size };
		}
		bool empty()const noexcept
		{
			return size == 0;
		}
	private:
		std::shared_ptr<const DataTy[]> data;
		size_t size = 0;
	};
}
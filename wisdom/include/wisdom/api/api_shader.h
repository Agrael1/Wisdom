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
		shared_blob(size_t size)
		{
			allocate(size);
		}
	public:
		void allocate(size_t size)
		{
			this->xsize = size;
			xdata = std::make_shared_for_overwrite<DataTy[]>(size);
		}
		const DataTy* data()const noexcept
		{
			return xdata.get();
		}
		DataTy* data()noexcept
		{
			return xdata.get();
		}

		const DataTy* begin()const noexcept
		{
			return data();
		}
		DataTy* begin()noexcept
		{
			return data();
		}

		const DataTy* end()const noexcept
		{
			return data()+xsize;
		}
		DataTy* end()noexcept
		{
			return data()+xsize;
		}
		size_t size()const noexcept
		{
			return xsize;
		}


		std::span<const DataTy> GetSpan()const noexcept
		{
			return { data(), xsize };
		}
		bool empty()const noexcept
		{
			return xsize == 0;
		}
	private:
		std::shared_ptr<DataTy[]> xdata;
		size_t xsize = 0;
	};
}
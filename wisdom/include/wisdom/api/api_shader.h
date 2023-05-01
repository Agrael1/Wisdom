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
		template<class Self>
		cv_type_t<Self, DataTy>* data(this Self s)noexcept
		{
			return s.xdata.get();
		}

		template<class Self>
		cv_type_t<Self, DataTy>* begin(this Self s)noexcept
		{
			return s.data();
		}

		template<class Self>
		cv_type_t<Self, DataTy>* end(this Self s)noexcept
		{
			return s.data()+s.xsize;
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
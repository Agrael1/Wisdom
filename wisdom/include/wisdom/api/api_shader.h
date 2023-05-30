#pragma once
#include <memory>
#include <span>


WIS_EXPORT namespace wis
{
	/// @brief Shader intermediate language
	enum class ShaderLang
	{
		dxil,
		spirv
	};

	/// @brief Type of shader
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

	/// @brief Compiled shader blob
	struct shared_blob
	{
	public:
		shared_blob() = default;

		/// @brief Create a blob with the given size
		/// @param size Size in bytes to allocate
		shared_blob(size_t size)
		{
			allocate(size);
		}

	public:

		/// @brief Allocate a blob with the given size
		/// @param size Size in bytes to allocate
		void allocate(size_t size)
		{
			this->xsize = size;
			xdata = std::make_shared_for_overwrite<std::byte[]>(size);
		}

		/// @brief Get a pointer to the data of the blob
		/// @tparam DataTy Type of data to get
		/// @return Pointer to the const data
		template <typename DataTy>
		const DataTy* data()const noexcept
		{
			return reinterpret_cast<const DataTy*>(xdata.get());
		}

		/// @brief Get a pointer to the data of the blob
		/// @tparam DataTy Type of data to get
		/// @return Pointer to the data
		template <typename DataTy>
		DataTy* data()noexcept
		{
			return reinterpret_cast<DataTy*>(xdata.get());
		}

		/// @brief Get the size of the blob
		/// @return Size of the blob in bytes
		size_t size()const noexcept
		{
			return xsize;
		}

		/// @brief Get a span of the data	
		/// @tparam DataTy Type of data to get
		/// @return Immutable span of the data
		template <typename DataTy>
		std::span<const DataTy> span()const noexcept
		{
			return { data<DataTy>(), xsize };
		}

		/// @brief Tells if the blob is empty
		/// @return true if the blob is empty
		bool empty()const noexcept
		{
			return xsize == 0;
		}
	private:
		std::shared_ptr<std::byte[]> xdata;
		size_t xsize = 0;
	};
}
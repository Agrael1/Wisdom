#pragma once
#include <memory>
#include <span>

namespace wis
{
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

	struct shared_blob
	{
	public:
		shared_blob() = default;
		shared_blob(std::unique_ptr<std::byte[]> data, size_t size)
			:data(std::move(data)), size(size){}
	public:
		std::span<const std::byte> GetSpan()const noexcept
		{
			return { data.get(), size };
		}
		bool empty()const noexcept
		{
			return size == 0;
		}
	private:
		std::shared_ptr<std::byte[]> data;
		size_t size = 0;
	};
}
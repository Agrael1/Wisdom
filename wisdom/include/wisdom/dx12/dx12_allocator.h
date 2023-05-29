#pragma once
#include <wisdom/api/api_barrier.h>
#include <wisdom/dx12/dx12_resource.h>
#include <D3D12MemAlloc.h>

namespace wis
{
	class DX12ResourceAllocator;

	template<>
	class Internal<DX12ResourceAllocator>
	{
	public:
		[[nodiscard]] D3D12MA::Allocator* GetAllocator()const noexcept
		{
			return allocator.get();
		}
	protected:
		winrt::com_ptr<D3D12MA::Allocator> allocator;
	};


	/// @brief Resource allocator for DX12
	class DX12ResourceAllocator : public QueryInternal<DX12ResourceAllocator>
	{
	public:
		DX12ResourceAllocator() = default;
		WIS_INLINE explicit DX12ResourceAllocator(DX12DeviceView device, DX12AdapterView adapter);
	public:
		/// @brief Create a buffer that is persistently mapped to the GPU
		/// @param size Size of the buffer
		/// @param flags Type of buffer
		/// @return Buffer object
		WIS_INLINE [[nodiscard]] DX12Buffer CreatePersistentBuffer(size_t size, [[maybe_unused]] BufferFlags flags = BufferFlags::None);


		/// @brief Create a buffer that is accessible by the CPU and serves as a staging buffer for GPU uploads
		/// @param size Size of the buffer
		/// @return Buffer object
		WIS_INLINE [[nodiscard]] DX12Buffer CreateUploadBuffer(size_t size);
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_allocator.inl"
#endif
#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_command_list.h>
#include <d3d12.h>


namespace wis
{
	class DX12CommandList;

	template<>
	class Internal<DX12CommandList>
	{
	public:
		template<class Self>
		auto GetCommandList(this Self&& s)noexcept
		{
			return command_list;
		}
	protected:
		winrt::com_ptr<ID3D12CommandAllocator> allocator;
		winrt::com_ptr<ID3D12GraphicsCommandList9> command_list;
	};

	class DX12CommandList : public QueryInternal<DX12CommandList>
	{
	public:
		DX12CommandList() = default;
		explicit DX12CommandList(winrt::com_ptr<ID3D12CommandAllocator> xallocator,
			winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list)
		{
			allocator = std::move(xallocator);
			command_list = std::move(xcommand_list);
		}
	};
}
#pragma once
#include <wisdom/vulkan/vk_shared_handle.h>
#include <vk_mem_alloc.hpp>

namespace wis
{
	template<>
	struct parent_of<vma::Allocator>
	{
		using parent = vk::Device;
	};

	template<>
	struct parent_of<vma::Allocation>
	{
		using parent = vma::Allocator;
	};

	template<class Dispatcher>
	class shared_handle<vma::Allocator, Dispatcher> : public shared_handle_base<vma::Allocator, shared_handle<vma::Allocator, Dispatcher>, basic_control_block<vma::Allocator>>
	{
		using base = shared_handle_base<vma::Allocator, shared_handle<vma::Allocator, Dispatcher>, basic_control_block<vma::Allocator>>;
		using base::handle;
		using base::control;
		friend base;
	public:
		shared_handle() = default;
		explicit shared_handle(vma::Allocator handle, shared_handle<vk::Device, Dispatcher> xparent)
			:base(handle, std::move(xparent))
		{
		}
	private:
		void internal_destroy()noexcept
		{
			handle.destroy();
			handle = nullptr;
		}
	};

	template<class Dispatcher>
	class shared_handle<vma::Allocation, Dispatcher> : public shared_handle_base<vma::Allocation, shared_handle<vma::Allocation, Dispatcher>, basic_control_block<vma::Allocation>>
	{
		using base = shared_handle_base<vma::Allocation, shared_handle<vma::Allocation, Dispatcher>, basic_control_block<vma::Allocation>>;
		using base::handle;
		using base::control;
		friend base;
	public:
		shared_handle() = default;
		explicit shared_handle(vma::Allocation handle, shared_handle<vma::Allocator, Dispatcher> xparent)
			:base(handle, std::move(xparent))
		{
		}
	private:
		void internal_destroy()noexcept
		{
			control.get_parent().freeMemory(
				handle
			);
			handle = nullptr;
		}
	};
}
#pragma once
#include <wisdom/vulkan/vk_xshared_handle.h>


namespace wis
{
	template<class HandleType, class Dispatcher>
	class shared_handle : public shared_handle_base<HandleType, shared_handle<HandleType, Dispatcher>, basic_control_block<HandleType>>
	{
		using base = shared_handle_base<HandleType, shared_handle<HandleType, Dispatcher>, basic_control_block<HandleType>>;
		using base::handle;
		friend base;
	public:
		shared_handle() = default;
		explicit shared_handle(HandleType handle, shared_handle<typename base::parent, Dispatcher> xparent)requires has_parent<HandleType>
			:base(handle, std::move(xparent))
		{
		}
		explicit shared_handle(HandleType handle)requires has_no_parent<HandleType>
			: base(handle, {true})
		{
		}
	private:
		void internal_destroy()noexcept requires has_no_parent<HandleType>
		{
			handle.destroy(nullptr, Dispatcher::loader);
			handle = nullptr;
		}
		void internal_destroy()noexcept requires has_parent<HandleType>
		{
			auto p = base::control.get_parent();
			p.destroy(handle, nullptr, Dispatcher::loader);
			handle = nullptr;
		}
	};



	struct image_header :shared_header<parent_of_t<vk::Image>>
	{
		bool swapchain_owned = false;
	};

	struct image_control_block : control_block_base<image_header>
	{
		using parent = parent_of_t<vk::Image>;
		using base = control_block_base<image_header>;
		using base::control;
	public:
		image_control_block() = default;
		image_control_block(shared_handle<parent> xparent, bool swapchain_owned)
		{
			allocate();
			control->parent = std::move(xparent);
			control->swapchain_owned = swapchain_owned;
		}

		parent get_parent()noexcept
		{
			return control->parent.get();
		}
		auto get_parent_handle()noexcept
		{
			return control->parent;
		}
		bool swapchain_owned()const noexcept
		{
			return control->swapchain_owned;
		}
	};


	template<class Dispatcher>
	class shared_handle<vk::Image, Dispatcher> : public shared_handle_base<vk::Image, shared_handle<vk::Image>, image_control_block>
	{
		using base = shared_handle_base<vk::Image, shared_handle<vk::Image>, image_control_block>;
		friend base;

	public:
		shared_handle() = default;
		explicit shared_handle(vk::Image handle, shared_handle<vk::Device, Dispatcher> xparent, bool swapchain_owned = false)
			:base(handle, { std::move(xparent),swapchain_owned })
		{
		}
	private:
		void internal_destroy()noexcept
		{
			if (!control.swapchain_owned())
			{
				auto p = control.get_parent();
				p.destroy(handle, nullptr, Dispatcher::loader);
			}
			handle = nullptr;
		}
	};
}

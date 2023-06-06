#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_dynamic_loader.h>
#include <utility>
#include <atomic>
#endif

namespace wis
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Default deleter for vulkan handles
	template<class HandleType>
	using default_vk_deleter = typename vk::UniqueHandleTraits<HandleType, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>::deleter;

	template<class HandleType>
	concept has_no_parent = std::same_as<default_vk_deleter<HandleType>, default_vk_deleter<vk::Instance>>;

	template<class T>
	concept has_parent = !has_no_parent<T>;



	template<class HandleType>
	struct parent_of
	{
		using parent = decltype(std::declval<default_vk_deleter<HandleType>>().getOwner());
	};

	template<class T> requires has_no_parent<T>
	struct parent_of<T>
	{
		using parent = vk::NoParent;
	};

	template<>
	struct parent_of<vk::NoParent>
	{
		using parent = vk::NoParent;
	};

	template<class HandleType>
	using parent_of_t = typename parent_of<HandleType>::parent;	// Parent of a handle for destruction

	///////////////////////////////////////////////////////////////////////////////////////////////



	template<class Header>
	struct control_block_base
	{
		using header_type = Header;
	public:
		control_block_base() = default;
		control_block_base(const control_block_base& o)
			:control(o.control)
		{
			add_ref();
		}
		control_block_base(control_block_base&& o)noexcept
			:control(o.control)
		{
			o.control = nullptr;
		}
		control_block_base& operator=(control_block_base&& o)noexcept
		{
			control = o.control;
			o.control = nullptr;
			return *this;
		}
		control_block_base& operator=(const control_block_base& o)noexcept
		{
			control = o.control;
			add_ref();
			return *this;
		}
	public:
		size_t ref_count()const noexcept
		{
			if (!control)return 0;
			return control->ref_cnt;
		}
		size_t add_ref()noexcept
		{
			if (!control)return 0;
			return ++control->ref_cnt;
		}
		size_t release()noexcept
		{
			if (!control)return 0;
			auto r = --control->ref_cnt;
			if (!r)
			{
				delete control;
				control = nullptr;
			}
			return r;
		}
		void allocate()
		{
			control = new header_type;
		}
	protected:
		header_type* control = nullptr;
	};


	template<class HandleType, class Dispatcher = DynamicLoader>
	class shared_handle;


	template<class ParentType>
	struct shared_header
	{
		shared_handle<ParentType> parent{};
		std::atomic_size_t ref_cnt{ 1 };
	};
	template<>
	struct shared_header<vk::NoParent>
	{
		std::atomic_size_t ref_cnt{ 1 };
	};


	template<class HandleType>
	struct basic_control_block : control_block_base<shared_header<parent_of_t<HandleType>>>
	{
		using parent = parent_of_t<HandleType>;
		using base = control_block_base<shared_header<parent_of_t<HandleType>>>;
		using base::control;
	public:
		basic_control_block() = default;
		basic_control_block(shared_handle<parent> xparent)requires has_parent<HandleType>
		{
			base::allocate();
			control->parent = std::move(xparent);
		}
		basic_control_block(bool)requires has_no_parent<HandleType>
		{
			base::allocate();
		}

		parent get_parent()noexcept requires has_parent<HandleType>
		{
			return control->parent.get();
		}
		auto get_parent_handle()noexcept requires has_parent<HandleType>
		{
			return control->parent;
		}
	};




	template<class HandleType, class CRTP, class ControlBlock = basic_control_block<HandleType>>
	class shared_handle_base
	{
	public:
		using control_block = ControlBlock;
		using parent = parent_of_t<HandleType>;
		using handle_type = HandleType;
	public:
		shared_handle_base() = default;
		explicit shared_handle_base(handle_type handle, control_block xcontrol)noexcept
			:handle(handle), control(std::move(xcontrol))
		{}
		shared_handle_base(const shared_handle_base& o)noexcept
			:handle(o.handle), control(o.control)
		{}
		shared_handle_base(shared_handle_base&& o)noexcept
			:handle(o.handle), control(std::move(o.control))
		{
			o.handle = nullptr;
		}
		shared_handle_base& operator=(shared_handle_base&& o)noexcept
		{
			release();
			handle = o.handle;
			control = std::move(o.control);
			o.handle = nullptr;
			return *this;
		}
		shared_handle_base& operator=(const shared_handle_base& o)noexcept
		{
			release();
			handle = o.handle;
			control = o.control;
			return *this;
		}
		~shared_handle_base()
		{
			release();
		}
	public:
		auto get()const noexcept
		{
			return handle;
		}
		operator bool()const noexcept
		{
			return bool(handle);
		}
		auto* operator->()const noexcept
		{
			return &handle;
		}
		size_t add_ref()noexcept
		{
			if (!handle)return 0;
			return control.add_ref();
		}
		size_t release()noexcept
		{
			if (!handle)return 0;

			auto r = control.ref_count();
			if (r == 1)static_cast<CRTP*>(this)->internal_destroy();
			return control.release();
		}
		parent get_parent()noexcept requires has_parent<handle_type>
		{
			return control.get_parent();
		}
		auto get_parent_handle()noexcept requires has_parent<handle_type>
		{
			return control.get_parent_handle();
		}
	protected:
		control_block control{};
		handle_type handle = nullptr;
	};
}

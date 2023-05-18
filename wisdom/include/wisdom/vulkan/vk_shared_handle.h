#pragma once
#include <atomic>
#include <wisdom/vulkan/vk_dynamic_loader.h>


namespace wis
{
	template<class T>
	using default_vk_deleter = typename vk::UniqueHandleTraits<T, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>::deleter;

	template<class T>
	concept has_no_parent = std::same_as<default_vk_deleter<T>,
		default_vk_deleter<vk::Instance>>;

	template<class T>
	concept has_parent = !has_no_parent<T>;

	template<class T>
	struct parent_of
	{
		using parent = decltype(std::declval<default_vk_deleter<T>>().getOwner());
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
	template<class T>
	using parent_of_t = typename parent_of<T>::parent;



	template<class T, class Dispatcher = DynamicLoader>
	class shared_handle
	{
		using parent = parent_of_t<T>;
		static constexpr inline bool is_dynamic = std::same_as<Dispatcher, DynamicLoader>;

		template<class U>
		struct shared_header
		{
			shared_handle<parent> parent{};
			std::atomic_size_t ref_cnt{1};
		};
		template<>
		struct shared_header<vk::NoParent>
		{
			std::atomic_size_t ref_cnt{1};
		};

		struct control_block
		{
		public:
			control_block() = default;
			control_block(shared_handle<parent> xparent)requires has_parent<T>
			{
				allocate();
				control->parent = std::move(xparent);
			}
			control_block(const control_block& o)
				:control(o.control)
			{
				add_ref();
			}
			control_block(control_block&& o)noexcept
				:control(o.control)
			{
				o.control = nullptr;
			}
			control_block& operator=(control_block&& o)noexcept
			{
				control = o.control;
				o.control = nullptr;
				return *this;
			}
			control_block& operator=(const control_block& o)noexcept
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
				control = new shared_header<parent>;
			}
			parent get_parent()noexcept requires has_parent<T>
			{
				return control->parent.get();
			}
			auto get_parent_handle()noexcept requires has_parent<T>
			{
				return control->parent;
			}
		private:
			shared_header<parent>* control;
		};

	public:
		shared_handle() = default;
		explicit shared_handle(T handle, shared_handle<parent> xparent)requires has_parent<T>
			:handle(handle), control(std::move(xparent))
		{}
		explicit shared_handle(T handle) requires has_no_parent<T>
			: handle(handle)
		{
			control.allocate();
		}
		shared_handle(const shared_handle& o)noexcept
			:handle(o.handle), control(o.control)
		{}
		shared_handle(shared_handle&& o)noexcept
			:handle(o.handle), control(std::move(o.control))
		{
			o.handle = nullptr;
		}
		shared_handle& operator=(shared_handle&& o)noexcept
		{
			release();
			handle = o.handle;
			control = std::move(o.control);
			o.handle = nullptr;
			return *this;
		}
		shared_handle& operator=(const shared_handle& o)noexcept
		{
			release();
			handle = o.handle;
			control = o.control;
			return *this;
		}
		~shared_handle()
		{
			release();
		}
	public:
		template<class Self>
		auto get(this Self&& s)noexcept
		{
			return s.handle;
		}
		auto unsafe_detach()noexcept
		{
			control.release();
			return std::exchange(handle, nullptr);
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
			if (r == 1)internal_destroy();
			return control.release();
		}
		parent get_parent()noexcept requires has_parent<T>
		{
			return control.get_parent();
		}
		auto get_parent_handle()noexcept requires has_parent<T>
		{
			return control.get_parent_handle();
		}
	private:
		void internal_destroy()noexcept requires has_no_parent<T>
		{
			handle.destroy(nullptr, Dispatcher::loader);
			handle = nullptr;
		}
		void internal_destroy()noexcept
		{
			auto p = control.get_parent();
			p.destroy(handle, nullptr, Dispatcher::loader);
			handle = nullptr;
		}
	private:
		control_block control{};
		T handle = nullptr;
	};

}


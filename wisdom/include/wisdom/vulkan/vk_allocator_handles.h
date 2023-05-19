#pragma once
#include <wisdom/vulkan/vk_shared_handle.h>
#include <vk_mem_alloc.hpp>

namespace wis
{
	template<>
	class shared_handle<vma::Allocator>
	{
		using parent = vk::Device;

		struct shared_header
		{
			shared_handle<parent> parent{};
			std::atomic_size_t ref_cnt{1};
		};

		struct control_block
		{
		public:
			control_block() = default;
			control_block(shared_handle<parent> xparent)
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
				control = new shared_header;
			}
			parent get_parent()noexcept
			{
				return control->parent.get();
			}
			shared_handle<parent> get_parent_handle()const
			{
				return control->parent;
			}
		private:
			shared_header* control;
		};

	public:
		shared_handle() = default;
		explicit shared_handle(vma::Allocator handle, shared_handle<parent> xparent)
			:handle(handle), control(std::move(xparent))
		{}
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
		auto get()const noexcept
		{
			return handle;
		}
		auto* put()noexcept
		{
			return &handle;
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
		shared_handle<parent> get_device_handle()const
		{
			return control.get_parent_handle();
		}
	private:
		void internal_destroy()noexcept
		{
			handle.destroy();
			handle = nullptr;
		}

	private:
		control_block control{};
		vma::Allocator handle = nullptr;
	};
	
	

	template<>
	class shared_handle<vma::Allocation>
	{
		using parent = vma::Allocator;

		struct shared_header
		{
			shared_handle<parent> parent{};
			std::atomic_size_t ref_cnt{1};
		};

		struct control_block
		{
		public:
			control_block() = default;
			control_block(shared_handle<parent> xparent)
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
				control = new shared_header;
			}
			parent get_parent()const noexcept
			{
				return control->parent.get();
			}
		private:
			shared_header* control;
		};

	public:
		shared_handle() = default;
		explicit shared_handle(vma::Allocation handle, shared_handle<parent> xparent)
			:handle(handle), control(std::move(xparent))
		{}
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
		auto get()const noexcept
		{
			return handle;
		}
		auto* put()noexcept
		{
			return &handle;
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
		auto get_parent()const noexcept
		{
			return control.get_parent();
		}
	private:
		void internal_destroy()noexcept
		{
			control.get_parent().freeMemory(
				handle
			);
			handle = nullptr;
		}

	private:
		control_block control{};
		vma::Allocation handle = nullptr;
	};


}
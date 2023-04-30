#pragma once
#include <cassert>
#include <array>

namespace wis
{
	inline constexpr auto allocator_size = 1024u;

	template<size_t max_size = allocator_size>
	class stack_allocator
	{
	public:
		stack_allocator() = default;
		stack_allocator(const stack_allocator&) = delete;
		stack_allocator(stack_allocator&&) = delete;
	public:
		template<typename T> requires std::is_trivially_destructible_v<T>
		T& allocate()noexcept
		{
			T& x = *reinterpret_cast<T*>(allocator.data() + rsize);
			rsize += sizeof(T);
			assert(rsize <= max_size);
			return x;
		};
		template<typename T>
		T* get() noexcept
		{
			return reinterpret_cast<T*>(allocator.data());
		}

		size_t size_bytes()const noexcept
		{
			return rsize;
		}
	private:
		alignas(void*)std::array<std::byte, max_size> allocator{};
		size_t rsize = 0;
	};

	template<class T, size_t max_size = 16> requires std::is_trivially_destructible_v<T>
	class uniform_allocator
	{
	public:
		uniform_allocator() = default;
		uniform_allocator(const uniform_allocator&) = delete;
		uniform_allocator(uniform_allocator&&) = delete;
	public:
		T& allocate()noexcept
		{
			T* x = allocator.data() + rsize;
			rsize++;
			assert(rsize <= max_size);
			return *x;
		};
		const T* get()const noexcept
		{
			return allocator.data();
		}
		size_t size()const noexcept
		{
			return rsize;
		}
	private:
		alignas(void*)std::array<T, max_size> allocator{};
		size_t rsize = 0;
	};
}
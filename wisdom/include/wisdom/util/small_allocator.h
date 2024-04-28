#pragma once
#include <cassert>
#include <array>
#include <bitset>

namespace wis::detail {
/// @brief Default size of the stack allocator
inline constexpr auto allocator_size = 1024u;

/// @brief A heterogenius memory pool that allocates objects of different types on stack
/// It can't be used to allocate objects of types that are not trivially destructible and can't be moved or copied, due to memory fixed nature
/// Not thread safe, used for small objects that are allocated frequently in a single function
/// Uses stack model of allocation
/// @tparam max_size Maximum size of the pool in bytes
template<size_t max_size = allocator_size>
class memory_pool
{
public:
    constexpr memory_pool() = default;
    memory_pool(const memory_pool&) = delete;
    memory_pool(memory_pool&&) = delete;

    /// @brief Allocates an object of type T in the allocator and returns a reference to it
    /// @tparam T Type of the object to allocate
    /// @tparam ...Args Argument types that constructor requires, deduced from the arguments
    /// @param ...args Arguments to pass to the constructor
    /// @return Reference to the allocated object
    template<typename T, typename... Args>
        requires std::is_trivially_destructible_v<T>
    constexpr T& allocate(Args&&... args) noexcept
    {
        T* x = new (allocator.data() + byte_size) T(std::forward<Args>(args)...);
        byte_size += sizeof(T);
        assert(byte_size <= max_size);
        return *x;
    };

    /// @brief Returns a pointer to the allocator data as a pointer to type T
    /// @tparam T Type of the pointer to return
    /// @return Data pointer as a pointer to type T
    template<typename T>
    constexpr T* data() noexcept
    {
        return reinterpret_cast<T*>(allocator.data());
    }

    /// @brief Returns a pointer to the allocator data as a pointer to const type T
    /// @tparam T Type of the pointer to return
    /// @return Data pointer as a pointer to const type T
    template<typename T>
    constexpr const T* data() const noexcept
    {
        return reinterpret_cast<const T*>(allocator.data());
    }

    /// @brief Get the size of the allocated data
    /// @return Size of the allocated data in bytes
    [[nodiscard]] constexpr size_t size_bytes() const noexcept
    {
        return byte_size;
    }

private:
    alignas(void*) std::array<std::byte, max_size> allocator{};
    size_t byte_size = 0;
};

/// @brief A homogeneous allocator that allocates objects of the same type on stack
/// It can't be used to allocate objects of types that are not trivially destructible
/// Not thread safe, used for small objects that are allocated frequently in a single function
/// @tparam T Type of the object to allocate
/// @tparam max_size Size of the allocator in objects
template<class T, size_t max_size = 16> // NOLINT
class uniform_allocator : public std::array<T, max_size>
{
    static_assert(std::is_trivially_destructible_v<T>, "Uniform allocator can't be used to allocate objects of types that are not trivially destructible");

public:
    constexpr uniform_allocator() = default;

    /// @brief Allocates an object of type T in the allocator and returns a reference to it
    /// @tparam ...Args Argument types that constructor requires, deduced from the arguments
    /// @param ...args Arguments to pass to the constructor
    /// @return Reference to the allocated object
    template<typename... Args>
    constexpr T& allocate(Args&&... args) noexcept
    {
        T* x = new (this->data() + rsize) T(std::forward<Args>(args)...);
        rsize++;
        assert(rsize <= max_size);
        return *x;
    };

    /// @brief Returns a size of the allocated data
    /// @return Size of the allocated data in objects
    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return rsize;
    }

    /// @brief Returns a capacity of the allocator
    /// @return Capacity of the allocator in objects
    [[nodiscard]] constexpr size_t capacity() const noexcept
    {
        return max_size;
    }

    /// @brief Returns a size of the allocated data in bytes
    /// @return Size of the allocated data in bytes
    [[nodiscard]] constexpr size_t size_bytes() const noexcept
    {
        return rsize * sizeof(T);
    }

    /// @brief Iterator to the beginning of the allocated data
    /// @return Pointer to the beginning of the allocated data
    auto* begin() noexcept
    {
        return this->data();
    }

    /// @brief Iterator to the end of the allocated data
    /// @return Pointer to the end of the allocated data
    auto* end() noexcept
    {
        return this->data() + rsize;
    }

    /// @brief Const iterator to the beginning of the allocated data
    /// @return Const pointer to the beginning of the allocated data
    const auto* begin() const noexcept
    {
        return this->data();
    }

    /// @brief Const iterator to the end of the allocated data
    /// @return Const pointer to the end of the allocated data
    const auto* end() const noexcept
    {
        return this->data() + rsize;
    }

    /// @brief Checks if the allocator contains an object
    /// @param ref Reference to the object to check
    template<typename C = std::equal_to<T>>
    bool contains(const T& ref) const noexcept
    {
        for (size_t i = 0; i < rsize; i++) {
            if (C{}(ref, this->data()[i]))
                return true;
        }
        return false;
    }

private:
    size_t rsize = 0;
};

template<typename Type, size_t initial_alloc = 16u>
    requires ::std::is_trivially_destructible_v<Type>
class limited_allocator
{
public:
    limited_allocator() noexcept
        : allocator{}, allocated{ 0 }, is_heap{ false }
    {
    }
    limited_allocator(uint32_t limit, bool exact = true) noexcept
        : allocator{}, allocated{ 0 }, is_heap{ false }, limit{ limit }
    {
        if (!exact)
            return;
        if (limit > initial_alloc) {
            is_heap = true;
            ptr = allocate_heap(limit);
            capacity = limit;
        }
    }
    limited_allocator(const limited_allocator&) = delete;
    limited_allocator(limited_allocator&&) = delete;
    ~limited_allocator() noexcept
    {
        if (is_heap) {
            deallocate_heap(ptr);
        }
    }

public:
    Type* allocate() noexcept
    {
        return is_heap ? allocate_on_heap() : allocate_on_stack();
    }
    uint32_t size() const noexcept
    {
        return is_heap ? count : allocated;
    }
    Type* data() noexcept
    {
        return is_heap ? ptr : allocator.data();
    }

private:
    Type* allocate_on_stack() noexcept
    {
        if (allocated <= initial_alloc) {
            return &allocator[allocated++];
        } else if (allocated <= limit) {
            reallocate();
            return allocate_on_heap();
        } else {
            return nullptr;
        }
    }
    Type* allocate_on_heap() noexcept
    {
        if (count < capacity) {
            return &ptr[count++];
        } else if (count < limit) {
            reallocate();
            return &ptr[count++];
        } else {
            return nullptr;
        }
    }
    void reallocate() noexcept
    {
        if (is_heap) {
            uint32_t xcapacity = this->capacity + (this->capacity >> 1);
            xcapacity = std::min(limit, xcapacity);
            ptr = reallocate_heap(ptr, xcapacity);
            capacity = xcapacity;
        } else {
            uint32_t xcapacity = initial_alloc + (initial_alloc >> 1);
            xcapacity = std::min(limit, xcapacity);
            auto* xptr = allocate_heap(xcapacity);
            std::ranges::copy(allocator, xptr);
            is_heap = true;
            count = allocated;
            ptr = xptr;
            capacity = xcapacity;
        }
    }

private:
    static Type* allocate_heap(size_t count) noexcept
    {
        return reinterpret_cast<Type*>(malloc(sizeof(Type) * count));
    }
    static Type* reallocate_heap(Type* ptr, size_t count) noexcept
    {
        return reinterpret_cast<Type*>(realloc(ptr, sizeof(Type) * count));
    }
    static void deallocate_heap(Type* ptr) noexcept
    {
        free(ptr);
    }

private:
    union {
        struct {
            std::array<Type, initial_alloc> allocator;
            uint32_t allocated;
        };
        struct {
            Type* ptr;
            uint32_t count;
            uint32_t capacity;
        };
    };
    uint32_t limit : sizeof(uint32_t) * CHAR_BIT - 1 = uint32_t(0x7FFFFFFF);
    uint32_t is_heap : 1 = 0;
};
} // namespace wis::detail

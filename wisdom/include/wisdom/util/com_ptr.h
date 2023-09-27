#pragma once
#include <concepts>
#include <span>

namespace wis {
struct take_ownership_t {
};
constexpr take_ownership_t take_ownership;


template<class T>
class com_ptr
{
    template<typename U>
    friend struct com_ptr;

public:
    using pointer = T*;
    using const_pointer = const T*;

public:
    com_ptr()
        : ptr(nullptr) { }
    com_ptr(pointer p)
        : ptr(p) { }
    com_ptr(void* p, take_ownership_t)
        : ptr(static_cast<pointer>(other.ptr)) { }

    template<class U>
    com_ptr(const com_ptr<U>& other)
        : ptr(other.ptr)
    {
        add_ref();
    }

    template<class U>
    com_ptr(com_ptr<U>&& other)
        : ptr(std::exchange(other.ptr, {}))
    {
    }

    ~com_ptr()
    {
        release();
    }

    template<class U>
    com_ptr& operator=(const com_ptr<U>& ptr) noexcept
    {
        copy_ref(ptr.ptr);
        return *this;
    }
    template<class U>
    com_ptr& operator=(com_ptr<U>&& ptr) noexcept
    {
        if constexpr (std::same_as<U, T>) {
            if (this == &other)
                return *this;
        }

        release();
        copy_ref(std::exchange(ptr.ptr, {}));
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return ptr != nullptr;
    }
    auto operator->() const noexcept
    {
        return ptr;
    }
    auto operator*() const noexcept
    {
        return *ptr;
    }

public:
    auto get() const noexcept
    {
        return ptr;
    }
    std::span<const_pointer, 1> as_array() const noexcept
    {
        return { &ptr, 1 };
    }
    auto** put() noexcept
    {
        release();
        return &ptr;
    }
    void** put_void() noexcept
    {
        return reinterpret_cast<void**>(put());
    }
    void attach(pointer value) noexcept
    {
        if (ptr != value)
            release();
        ptr = value;
    }
    pointer detach() noexcept
    {
        return std::exchange(ptr, {});
    }
    void swap(com_ptr& other) noexcept
    {
        std::swap(ptr, other.ptr);
    }
    //template<typename To>


public:
private:
    void copy_ref(type* other) noexcept
    {
        if (m_ptr != other) {
            release();
            m_ptr = other;
            add_ref();
        }
    }
    void add_ref() const noexcept
    {
        if (ptr)
            ptr->AddRef();
    }
    void release() noexcept
    {
        if (ptr)
            std::exchange(m_ptr, {})->Release();
    }

private:
    pointer ptr;
};

} // namespace wis
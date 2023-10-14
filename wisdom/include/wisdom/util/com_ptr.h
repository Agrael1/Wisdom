#pragma once
#include <concepts>
#include <span>

namespace wis {
struct take_ownership_t {
};
constexpr take_ownership_t take_ownership;

template<typename T>
struct guid_of {
    static constexpr auto value = __uuidof(T);
};

template<typename T>
constexpr auto guid_of_v = guid_of<T>::value;

using hresult = int32_t;

template<class T>
class com_ptr;

template<typename T>
struct com_with_result {
    hresult result;
    com_ptr<T> ptr;
    operator bool() const noexcept
    {
        return result >= 0 && bool(ptr);
    }
    auto* operator->() const noexcept
    {
        return ptr.get();
    }
};

template<class T>
class com_ptr
{
    template<typename U>
    friend struct com_ptr;

public:
    using pointer = T*;
    using const_pointer = const T*;

public:
    com_ptr() noexcept
        : ptr(nullptr) { }
    com_ptr(pointer p) noexcept
        : ptr(p)
    {
        add_ref();
    }

    com_ptr(void* p, take_ownership_t) noexcept
        : ptr(static_cast<pointer>(p)) { }

    template<class U>
    com_ptr(const com_ptr<U>& other) noexcept
        : ptr(other.ptr)
    {
        add_ref();
    }

    template<class U>
    com_ptr(com_ptr<U>&& other) noexcept
        : ptr(std::exchange(other.ptr, {}))
    {
    }

    ~com_ptr() noexcept
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
    com_ptr& operator=(com_ptr<U>&& other) noexcept
    {
        if constexpr (std::same_as<U, T>) {
            if (get() == other.get()) {
                std::exchange(other.ptr, {});
                return *this;
            }
        }

        release();
        ptr = std::exchange(other.ptr, {});
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
    auto& operator*() const noexcept
    {
        return *ptr;
    }

public:
    pointer get() const noexcept
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
    template<typename To>
    hresult as(com_ptr<To>* out) const noexcept
    {
        return ptr->QueryInterface(guid_of_v<To>, out->put_void());
    }
    template<typename To>
    com_with_result<To> as() const noexcept
    {
        com_ptr<To> out;
        auto hr = ptr->QueryInterface(guid_of_v<To>, reinterpret_cast<void**>(&out.ptr));
        return { hr, std::move(out) };
    }
    void copy_from(T* other) noexcept
    {
        copy_ref(other);
    }
    void copy_to(T** other) const noexcept
    {
        add_ref();
        *other = ptr;
    }
    void reset() noexcept
    {
        release();
    }

private:
    void copy_ref(T* other) noexcept
    {
        if (ptr != other) {
            release();
            ptr = other;
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
            std::exchange(ptr, {})->Release();
    }

private:
    pointer ptr;
};

} // namespace wis
#ifndef WIS_UTIL_COM_PTR_HPP
#define WIS_UTIL_COM_PTR_HPP
/**
 * @file com_ptr.hpp
 * @brief A lightweight, header-only COM smart pointer implementation.
 * @details Provides automatic reference counting for COM interfaces with
 *          support for MSVC's __uuidof as well as manual GUID specialization
 *          for MinGW/Clang/GCC compilers. C++11 compatible.
 */

#include <atomic>
#include <cstdint>
#include <utility>

// ============================================================================
// GUID Support Detection and Traits
// ============================================================================

#if defined(_WIN32)
#    include <guiddef.h>
#    include <unknwn.h>
namespace wis {
using GUID = ::GUID;
using hresult = HRESULT;
} // namespace wis
#else
namespace wis {
/**
 * @brief Platform-independent GUID structure.
 * @details Used on non-Windows platforms where guiddef.h is unavailable.
 */
struct GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
};
} // namespace wis

/**
 * @brief HRESULT type alias for COM method return values.
 */
using hresult = std::int32_t;
#endif

// Define STDMETHODCALLTYPE macro for non-Windows platforms to avoid compilation errors
#ifndef STDMETHODCALLTYPE
#    define STDMETHODCALLTYPE
#endif // STDMETHODCALLTYPE

namespace wis {
// ============================================================================
// GUID Support Detection and Traits
// ============================================================================

#if defined(_MSC_VER)
/**
 * @brief Indicates whether the compiler supports __uuidof.
 * @details True for MSVC, false otherwise.
 */
struct has_uuidof_support_tag {
    static constexpr bool value = true;
};

/**
 * @brief Retrieves the GUID associated with a COM interface type.
 * @tparam T The COM interface type.
 * @details On MSVC, uses the built-in __uuidof operator.
 */
template <typename T>
struct guid_of {
    static constexpr GUID get() { return __uuidof(T); }
};

/**
 * @brief Helper function to get GUID for a type.
 * @tparam T The COM interface type.
 * @return The GUID associated with type T.
 */
template <typename T>
inline constexpr GUID guid_of_v()
{
    return guid_of<T>::get();
}

/**
 * @brief Macro to define GUID for a type (no-op on MSVC).
 */
#    define WIS_DEFINE_GUID(type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)

#else
/**
 * @brief Indicates whether the compiler supports __uuidof.
 * @details False for MinGW/Clang/GCC compilers.
 */
struct has_uuidof_support_tag {
    static constexpr bool value = false;
};

/**
 * @brief Retrieves the GUID associated with a COM interface type.
 * @tparam T The COM interface type.
 * @details Must be specialized using WIS_DEFINE_GUID macro.
 */
template <typename T>
struct guid_of;

/**
 * @brief Helper function to get GUID for a type.
 * @tparam T The COM interface type.
 * @return The GUID associated with type T.
 */
template <typename T>
inline constexpr GUID guid_of_v()
{
    return guid_of<T>::get();
}

/**
 * @def WIS_DEFINE_GUID
 * @brief Macro to define GUID specialization for a COM interface type.
 * @param type The COM interface type.
 * @param l Data1 field (32-bit).
 * @param w1 Data2 field (16-bit).
 * @param w2 Data3 field (16-bit).
 * @param b1-b8 Data4 field bytes.
 *
 * @details Example usage:
 * @code
 * WIS_DEFINE_GUID(ID3D12Device, 0x189819f1, 0x1db6, 0x4b57,
 *                 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7);
 * @endcode
 */
#    define WIS_DEFINE_GUID(type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        template <>                                                          \
        struct wis::guid_of<type> {                                          \
            static constexpr wis::GUID get() noexcept                        \
            {                                                                \
                return wis::GUID{                                            \
                    l,                                                       \
                    w1,                                                      \
                    w2,                                                      \
                    {b1, b2, b3, b4, b5, b6, b7, b8}                         \
                };                                                           \
            }                                                                \
        };
#endif

/**
 * @brief Tag type for taking ownership of a raw pointer.
 * @details Used to construct com_ptr without incrementing reference count.
 */
struct take_ownership_t {};

/**
 * @brief Tag instance for take_ownership_t.
 * @see take_ownership_t
 */
static constexpr take_ownership_t take_ownership = {};

/**
 * @brief A smart pointer for COM interface reference counting.
 * @tparam T The COM interface type (must support AddRef/Release).
 *
 * @details Provides automatic reference counting with RAII semantics.
 *          Supports copy/move operations, type conversions between related
 *          COM interfaces, and QueryInterface calls.
 *
 * @par Example Usage:
 * @code
 * wis::detail::com_ptr<ID3D12Device> device;
 * factory->CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,
 *                       device.iid(), device.put_void());
 *
 * // Query for another interface
 * wis::detail::com_ptr<ID3D12Device5> device5;
 * if (SUCCEEDED(device.as(device5.iid(), &device5))) {
 *     // Use device5...
 * }
 * @endcode
 */
template <class T>
class com_ptr
{
    template <typename U>
    friend class com_ptr;

public:
    /** @brief Pointer type alias. */
    typedef T* pointer;
    /** @brief Const pointer type alias. */
    typedef const T* const_pointer;

public:
    // ========================================================================
    // Constructors and Destructor
    // ========================================================================

    /**
     * @brief Default constructor. Initializes to nullptr.
     */
    com_ptr() noexcept
        : ptr(nullptr)
    {}

    /**
     * @brief Nullptr constructor. Initializes to nullptr.
     * @param Null pointer literal.
     */
    com_ptr(std::nullptr_t) noexcept
        : ptr(nullptr)
    {}

    /**
     * @brief Constructs from a raw pointer and increments reference count.
     * @param p Raw pointer to take ownership of.
     * @note Calls AddRef() on the pointer if non-null.
     */
    explicit com_ptr(pointer p) noexcept
        : ptr(p)
    {
        add_ref();
    }

    /**
     * @brief Constructs from a raw pointer without incrementing reference count.
     * @param p Raw pointer (as void*) to take ownership of.
     * @param Tag to indicate ownership transfer.
     * @note Does NOT call AddRef(). Use when receiving already-incremented pointers.
     */
    com_ptr(void* p, take_ownership_t) noexcept
        : ptr(static_cast<pointer>(p))
    {}

    /**
     * @brief Converting copy constructor from compatible com_ptr.
     * @tparam U Source type (must be convertible to T*).
     * @param other Source com_ptr to copy from.
     * @note Increments reference count.
     */
    template <class U>
    com_ptr(const com_ptr<U>& other) noexcept
        : ptr(other.ptr)
    {
        add_ref();
    }

    /**
     * @brief Copy constructor.
     * @param other Source com_ptr to copy from.
     * @note Increments reference count.
     */
    com_ptr(const com_ptr<T>& other) noexcept
        : ptr(other.ptr)
    {
        add_ref();
    }

    /**
     * @brief Converting move constructor from compatible com_ptr.
     * @tparam U Source type (must be convertible to T*).
     * @param other Source com_ptr to move from (will be nulled).
     * @note Does NOT increment reference count.
     */
    template <class U>
    com_ptr(com_ptr<U>&& other) noexcept
        : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    /**
     * @brief Move constructor.
     * @param other Source com_ptr to move from (will be nulled).
     * @note Does NOT increment reference count.
     */
    com_ptr(com_ptr<T>&& other) noexcept
        : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    /**
     * @brief Destructor. Releases the held pointer.
     */
    ~com_ptr() noexcept { release(); }

    // ========================================================================
    // Assignment Operators
    // ========================================================================

    /**
     * @brief Converting copy assignment from compatible com_ptr.
     * @tparam U Source type (must be convertible to T*).
     * @param other Source com_ptr to copy from.
     * @return Reference to this.
     */
    template <class U>
    com_ptr& operator=(const com_ptr<U>& other) noexcept
    {
        copy_ref(other.ptr);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other Source com_ptr to copy from.
     * @return Reference to this.
     */
    com_ptr& operator=(const com_ptr<T>& other) noexcept
    {
        copy_ref(other.ptr);
        return *this;
    }

    /**
     * @brief Converting move assignment from compatible com_ptr.
     * @tparam U Source type (must be convertible to T*).
     * @param other Source com_ptr to move from (will be nulled).
     * @return Reference to this.
     */
    template <class U>
    com_ptr& operator=(com_ptr<U>&& other) noexcept
    {
        release();
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other Source com_ptr to move from (will be nulled).
     * @return Reference to this.
     */
    com_ptr& operator=(com_ptr<T>&& other) noexcept
    {
        if (this != &other) {
            release();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // ========================================================================
    // Operators
    // ========================================================================

    /**
     * @brief Boolean conversion operator.
     * @return True if pointer is non-null.
     */
    explicit operator bool() const noexcept { return ptr != nullptr; }

    /**
     * @brief Member access operator.
     * @return The raw pointer.
     */
    pointer operator->() const noexcept { return ptr; }

    /**
     * @brief Dereference operator.
     * @return Reference to the pointed object.
     */
    T& operator*() const noexcept { return *ptr; }

    // ========================================================================
    // Public Methods
    // ========================================================================

    /**
     * @brief Gets the IID (interface identifier) for type T.
     * @return The GUID associated with type T.
     * @note Only available when __uuidof is supported or GUID is specialized.
     */
    static constexpr GUID iid() noexcept { return guid_of_v<T>(); }

    /**
     * @brief Gets the raw pointer.
     * @return The raw pointer (does not transfer ownership).
     */
    pointer get() const noexcept { return ptr; }

    /**
     * @brief Releases current pointer and returns address for output parameter.
     * @return Address of the internal pointer.
     * @note Releases any currently held pointer before returning.
     *
     * @par Example:
     * @code
     * com_ptr<IFoo> foo;
     * factory->CreateFoo(foo.put());
     * @endcode
     */
    pointer* put() noexcept
    {
        release();
        return &ptr;
    }

    /**
     * @brief Releases current pointer and returns void** for output parameter.
     * @return Address of the internal pointer as void**.
     * @note Releases any currently held pointer before returning.
     *
     * @par Example:
     * @code
     * com_ptr<IFoo> foo;
     * factory->CreateFoo(foo.iid(), foo.put_void());
     * @endcode
     */
    void** put_void() noexcept { return reinterpret_cast<void**>(put()); }

    /**
     * @brief Attaches a raw pointer without incrementing reference count.
     * @param value Raw pointer to attach.
     * @note Releases any currently held pointer first.
     */
    void attach(pointer value) noexcept
    {
        if (ptr != value) {
            release();
        }
        ptr = value;
    }

    /**
     * @brief Detaches and returns the raw pointer.
     * @return The raw pointer (ownership transferred to caller).
     * @note The internal pointer is set to nullptr.
     */
    pointer detach() noexcept
    {
        pointer tmp = ptr;
        ptr = nullptr;
        return tmp;
    }

    /**
     * @brief Swaps contents with another com_ptr.
     * @param other The com_ptr to swap with.
     */
    void swap(com_ptr& other) noexcept
    {
        pointer tmp = ptr;
        ptr = other.ptr;
        other.ptr = tmp;
    }

    /**
     * @brief Queries for another interface using __uuidof.
     * @tparam To Target interface type.
     * @param out Output com_ptr to receive the queried interface.
     * @return HRESULT indicating success or failure.
     * @note Only available when __uuidof is supported (MSVC).
     */
    template <typename To>
    hresult as(com_ptr<To>* out) const noexcept
    {
        return ptr->QueryInterface(guid_of_v<To>(), out->put_void());
    }

    /**
     * @brief Queries for another interface using an explicit GUID.
     * @tparam To Target interface type.
     * @param guid The interface identifier to query for.
     * @param out Output com_ptr to receive the queried interface.
     * @return HRESULT indicating success or failure.
     */
    template <typename To>
    hresult as(GUID guid, com_ptr<To>* out) const noexcept
    {
        return ptr->QueryInterface(guid, out->put_void());
    }

    /**
     * @brief Copies from another raw pointer (increments ref count).
     * @param other Raw pointer to copy from.
     */
    void copy_from(T* other) noexcept { copy_ref(other); }

    /**
     * @brief Copies to another raw pointer location (increments ref count).
     * @param other Output pointer location.
     */
    void copy_to(T** other) const noexcept
    {
        add_ref();
        *other = ptr;
    }

    /**
     * @brief Releases the held pointer and sets to nullptr.
     */
    void reset() noexcept { release(); }

    /**
     * @brief Returns void** without releasing (unchecked).
     * @return Address of the internal pointer as void**.
     * @warning Use only when you know the pointer is already null or
     *          you're handling ownership manually.
     */
    void** put_void_unchecked() noexcept { return reinterpret_cast<void**>(&ptr); }

    /**
     * @brief Returns T** without releasing (unchecked).
     * @return Address of the internal pointer.
     * @warning Use only when you know the pointer is already null or
     *          you're handling ownership manually.
     */
    pointer* put_unchecked() noexcept { return &ptr; }

private:
    // ========================================================================
    // Private Implementation Helpers
    // ========================================================================

    /**
     * @brief Copies a reference (releases old, acquires new).
     * @param other Pointer to copy from.
     */
    void copy_ref(T* other) noexcept
    {
        if (ptr != other) {
            release();
            ptr = other;
            add_ref();
        }
    }

    /**
     * @brief Increments reference count if pointer is non-null.
     */
    void add_ref() const noexcept
    {
        if (ptr) {
            ptr->AddRef();
        }
    }

    /**
     * @brief Releases reference and sets pointer to null.
     */
    void release() noexcept
    {
        T* tmp = ptr;
        if (tmp) {
            ptr = nullptr;
            tmp->Release();
        }
    }

private:
    pointer ptr; ///< The raw COM interface pointer.
};

/**
 * @brief A helper base class to implement IUnknown for COM objects.
 * @tparam T The COM interface type that inherits from IUnknown.
 *
 * @details Provides a thread-safe reference counting implementation of IUnknown.
 *          Classes can inherit from IUnknownImpl<T> to get default QueryInterface,
 *          AddRef, and Release implementations. The template parameter T is used
 *          to determine the correct GUID for QueryInterface.
 */
template <class CRTP, class T>
class IUnknownImpl : public T
{
protected:
    ~IUnknownImpl() = default;

private:
    static_assert(std::is_base_of<IUnknown, T>::value, "T must inherit from IUnknown");

public:
    using T::T; // Inherit constructors

public:
    // IUnknown methods
    hresult STDMETHODCALLTYPE QueryInterface(const GUID& iid, void** ppv) noexcept override
    {
        if (ppv == nullptr) {
            return E_POINTER;
        }
        if (iid == guid_of_v<T>()) {
            *ppv = static_cast<T*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    unsigned long STDMETHODCALLTYPE AddRef() noexcept override
    {
        return ref_count.fetch_add(1, std::memory_order_relaxed);
    }
    unsigned long STDMETHODCALLTYPE Release() noexcept override
    {
        uint32_t count = ref_count.fetch_sub(1, std::memory_order_release) - 1;
        if (count == 0) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete static_cast<CRTP*>(this);
        }
        return count;
    }

private:
    std::atomic<unsigned long> ref_count{1}; ///< Reference count for the COM object.
};
} // namespace wis

#endif // WIS_UTIL_COM_PTR_HPP

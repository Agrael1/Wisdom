#ifndef WIS_GLOBAL_DEFINITIONS_H
#define WIS_GLOBAL_DEFINITIONS_H
#if defined(WISDOM_BUILD_BINARIES) || defined(WISDOM_MODULE_DECL) // If we are building the binaries
#define WIS_INLINE
#else
#define WIS_INLINE inline
#endif // WISDOM_BUILD_BINARIES

#ifndef WISDOM_EXPORT
#ifndef WISDOM_MODULE_DECL
#define WISDOM_EXPORT
#else
#define WISDOM_EXPORT export
#endif
#endif // WISDOM_EXPORT

#ifndef WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT
#define WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT 0 // Descriptor heap support in Vulkan is still in alpha stage, and the API is subject to change. Enable this at your own risk, and be prepared for breaking changes in future versions of Wisdom and the Vulkan extension itself.
#endif // WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT

// C++ feature detection
#ifdef __cplusplus
#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define WIS_HAS_CPP17 1
#else
#define WIS_HAS_CPP17 0
#endif

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#define WIS_HAS_CPP20 1
#else
#define WIS_HAS_CPP20 0
#endif

#if __cpp_constexpr >= 202211L
#define WIS_CONSTEXPR23 constexpr
#else
#define WIS_CONSTEXPR23
#endif

#define WIS_EXTERN_C extern "C"

#define WISDOM_DEFINE_ENUM_OPERATORS(T)                                                                                    \
    inline T  operator|(T a, T b) noexcept { return static_cast<T>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); } \
    inline T  operator&(T a, T b) noexcept { return static_cast<T>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b)); } \
    inline T  operator^(T a, T b) noexcept { return static_cast<T>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b)); } \
    inline T  operator~(T a) noexcept { return static_cast<T>(~static_cast<uint32_t>(a)); }                                \
    inline T& operator|=(T& a, T b) noexcept { return a = a | b; }                                                        \
    inline T& operator&=(T& a, T b) noexcept { return a = a & b; }                                                        \
    inline T& operator^=(T& a, T b) noexcept { return a = a ^ b; }

#else
#define WIS_EXTERN_C
#endif // __cplusplus

#if defined(__GNUC__) || defined(__clang__)
/* GCC and Clang support __attribute__((warn_unused_result)) */
#define NODISCARD_ATTRIBUTE __attribute__((warn_unused_result))
#else
/* Fallback: no warning for unsupported compilers */
#define NODISCARD_ATTRIBUTE
#endif

// [[nodiscard]] attributes on STL functions
#ifndef WIS_NODISCARD

// Check for C++
#ifdef __has_cpp_attribute

// C++17 or later
#if __has_cpp_attribute(nodiscard) >= 201603L // C++17 or later
#define WIS_NODISCARD [[nodiscard]]
#else // Older C++, use compiler-specific attributes
#define WIS_NODISCARD NODISCARD_ATTRIBUTE
#endif // __has_cpp_attribute(nodiscard) >= 201603L

#elif defined(__has_c_attribute) // Try C23
// C23 or later
#if __has_c_attribute(nodiscard)
#define WIS_NODISCARD [[nodiscard]]
#else
#define WIS_NODISCARD NODISCARD_ATTRIBUTE
#endif

#else // Fallback
#define WIS_NODISCARD NODISCARD_ATTRIBUTE
#endif // __has_cpp_attribute
#endif // WIS_NODISCARD

#define WIS_DEFINE_HANDLE(name, size) \
    typedef struct name {             \
        uint64_t opaque[size];        \
    } name

#define WIS_DEFINE_HANDLE_VIEW(name, size) \
    typedef struct name##View {            \
        uint64_t opaque[size];             \
    } name##View

#define WIS_DEFINE_DEVICE_EXT_HANDLE(name, size)     \
    typedef struct name {                            \
        const WisDeviceExtensionHeader ext_header;   \
        uint64_t                       opaque[size]; \
    } name

#define WIS_DEFINE_INSTANCE_EXT_HANDLE(name, size)     \
    typedef struct name {                              \
        const WisInstanceExtensionHeader ext_header;   \
        uint64_t                         opaque[size]; \
    } name

// TODO: CMake definition for WISDOM_API when building shared library
#ifdef WISDOM_SHARED_LIBRARY
#include <wisdom/generated/wisdom_exports.h>
#endif // WISDOM_SHARED_LIBRARY

#ifndef WISDOM_API
#define WISDOM_API
#endif // WISDOM_API

#endif // !WIS_GLOBAL_DEFINITIONS_H

#ifndef WIS_GLOBAL_DEFINITIONS_H
#define WIS_GLOBAL_DEFINITIONS_H
#if defined(WISDOM_HEADER_ONLY) && !defined(WISDOM_MODULE_DECL)
#    define WIS_INLINE inline
#else
#    define WIS_INLINE
#endif // WISDOM_HEADER_ONLY

#ifndef WISDOM_EXPORT
#    ifndef WISDOM_MODULE_DECL
#        define WISDOM_EXPORT
#    else
#        define WISDOM_EXPORT export
#    endif
#endif // WISDOM_EXPORT

// C++ feature detection
#ifdef __cplusplus
#    if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#        define WIS_HAS_CPP17 1
#    else
#        define WIS_HAS_CPP17 0
#    endif

#    if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#        define WIS_HAS_CPP20 1
#    else
#        define WIS_HAS_CPP20 0
#    endif

#    if __cpp_constexpr >= 202211L
#        define WIS_CONSTEXPR23 constexpr
#    else
#        define WIS_CONSTEXPR23
#    endif

#    define WIS_EXTERN_C extern "C"

#    define WISDOM_DEFINE_ENUM_OPERATORS(T)                                                                   \
        inline T operator|(T a, T b) noexcept                                                                 \
        {                                                                                                     \
            return static_cast<T>(                                                                            \
                static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b) \
            );                                                                                                \
        }                                                                                                     \
        inline T operator&(T a, T b) noexcept                                                                 \
        {                                                                                                     \
            return static_cast<T>(                                                                            \
                static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b) \
            );                                                                                                \
        }                                                                                                     \
        inline T operator^(T a, T b) noexcept                                                                 \
        {                                                                                                     \
            return static_cast<T>(                                                                            \
                static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b) \
            );                                                                                                \
        }                                                                                                     \
        inline T operator~(T a) noexcept                                                                      \
        {                                                                                                     \
            return static_cast<T>(~static_cast<std::underlying_type<T>::type>(a));                            \
        }                                                                                                     \
        inline T& operator|=(T& a, T b) noexcept                                                              \
        {                                                                                                     \
            return a = a | b;                                                                                 \
        }                                                                                                     \
        inline T& operator&=(T& a, T b) noexcept                                                              \
        {                                                                                                     \
            return a = a & b;                                                                                 \
        }                                                                                                     \
        inline T& operator^=(T& a, T b) noexcept                                                              \
        {                                                                                                     \
            return a = a ^ b;                                                                                 \
        }                                                                                                     \
        inline std::underlying_type_t<T> to_underlying(T value) noexcept                                      \
        {                                                                                                     \
            return static_cast<std::underlying_type_t<T>>(value);                                             \
        }                                                                                                     \
        inline std::underlying_type_t<T> operator+(T value) noexcept                                          \
        {                                                                                                     \
            return static_cast<std::underlying_type_t<T>>(value);                                             \
        }

#else
#    define WIS_EXTERN_C
#endif // __cplusplus

#if defined(__GNUC__) || defined(__clang__)
/* GCC and Clang support __attribute__((warn_unused_result)) */
#    define NODISCARD_ATTRIBUTE __attribute__((warn_unused_result))
#else
/* Fallback: no warning for unsupported compilers */
#    define NODISCARD_ATTRIBUTE
#endif

// [[nodiscard]] attributes on STL functions
#ifndef WIS_NODISCARD

// Check for C++
#    ifdef __has_cpp_attribute

// C++17 or later
#        if __has_cpp_attribute(nodiscard) >= 201603L // C++17 or later
#            define WIS_NODISCARD [[nodiscard]]
#        else                                         // Older C++, use compiler-specific attributes
#            define WIS_NODISCARD NODISCARD_ATTRIBUTE
#        endif                                        // __has_cpp_attribute(nodiscard) >= 201603L

#    elif defined(__has_c_attribute)                  // Try C23
// C23 or later
#        if __has_c_attribute(nodiscard)
#            define WIS_NODISCARD [[nodiscard]]
#        else
#            define WIS_NODISCARD NODISCARD_ATTRIBUTE
#        endif

#    else  // Fallback
#        define WIS_NODISCARD NODISCARD_ATTRIBUTE
#    endif // __has_cpp_attribute
#endif     // WIS_NODISCARD

#define WIS_DEFINE_HANDLE(name, size) \
    typedef struct name {             \
        uint64_t opaque[size];        \
    } name

#define WIS_DEFINE_HANDLE_VIEW(name, size) \
    typedef struct name##View {            \
        uint64_t opaque[size];             \
    } name##View

#define WIS_DEFINE_DX12_DEVICE_EXT_HANDLE(name, size) \
    typedef struct name {                             \
        WisDX12DeviceExtensionHeader header;          \
        uint64_t opaque[size];                        \
    } name

#define WIS_DEFINE_VK_DEVICE_EXT_HANDLE(name, size) \
    typedef struct name {                           \
        WisVKDeviceExtensionHeader header;          \
        uint64_t opaque[size];                      \
    } name

#define WIS_DEFINE_DX12_INSTANCE_EXT_HANDLE(name, size) \
    typedef struct name {                               \
        WisDX12InstanceExtensionHeader header;          \
        uint64_t opaque[size];                          \
    } name

#define WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(name, size) \
    typedef struct name {                             \
        WisVKInstanceExtensionHeader header;          \
        uint64_t opaque[size];                        \
    } name

#ifdef WISDOM_SHARED_LIBRARY
#    include <wisdom/generated/wisdom_exports.h>
#endif // WISDOM_SHARED_LIBRARY

#ifndef WISDOM_API
#    define WISDOM_API
#endif // WISDOM_API

// platform detection
#if !defined(WISDOM_UWP) && !defined(WISDOM_WINDOWS) && !defined(WISDOM_LINUX)

// clang-format off
#ifndef WISDOM_VULKAN
    #if defined __has_include && (__has_include(<vulkan/vulkan.h>) || __has_include(<vulkan.h>))
    #define WISDOM_VULKAN 1
    #endif // __has_include && (__has_include(<vulkan/vulkan.h>) || __has_include(<vulkan.h>))
#endif // !WISDOM_VULKAN
// clang-format on

// Try to detect the platform
#    if defined(_WIN32)

#        ifndef WISDOM_DX12
#            define WISDOM_DX12 1
#        endif            // !WISDOM_DX12

#        ifdef _WINRT_DLL // UWP
#            define WISDOM_UWP 1
#        else             // _WINRT_DLL
#            define WISDOM_WINDOWS 1
#        endif            // _WINRT_DLL
#    elif defined(__linux__)
#        define WISDOM_LINUX 1
#    else
#        error "Platform not supported"
#    endif // _WIN32
#endif     // !WISDOM_UWP && !WISDOM_WINDOWS && !WISDOM_LINUX

#endif     // !WIS_GLOBAL_DEFINITIONS_H

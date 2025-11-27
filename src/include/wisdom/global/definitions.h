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

// C++23 and later

#ifdef __cplusplus
#if __cpp_constexpr >= 202211L
#define WIS_CONSTEXPR23 constexpr
#else
#define WIS_CONSTEXPR23
#endif

#define WIS_EXTERN_C extern "C"
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

#endif // !WIS_GLOBAL_DEFINITIONS_H

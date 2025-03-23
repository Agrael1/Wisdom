#pragma once
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
#if __cplusplus > 202211L
#define WIS_CONSTEXPR23 constexpr
#else
#define WIS_CONSTEXPR23
#endif


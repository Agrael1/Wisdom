
#ifndef WISDOM_API_H
#define WISDOM_API_H

#ifdef WISDOM_STATIC
#  define WISDOM_API
#  define WISDOM_NO_EXPORT
#else
#  ifndef WISDOM_API
#    ifdef wisdom_bindings_EXPORTS
        /* We are building this library */
#      define WISDOM_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define WISDOM_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef WISDOM_NO_EXPORT
#    define WISDOM_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef WISDOM_DEPRECATED
#  define WISDOM_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef WISDOM_DEPRECATED_EXPORT
#  define WISDOM_DEPRECATED_EXPORT WISDOM_API WISDOM_DEPRECATED
#endif

#ifndef WISDOM_DEPRECATED_NO_EXPORT
#  define WISDOM_DEPRECATED_NO_EXPORT WISDOM_NO_EXPORT WISDOM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef WISDOM_NO_DEPRECATED
#    define WISDOM_NO_DEPRECATED
#  endif
#endif

#endif /* WISDOM_API_H */

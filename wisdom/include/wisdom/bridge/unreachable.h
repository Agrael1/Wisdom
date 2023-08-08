#pragma once
#ifdef __cpp_lib_unreachable
namespace wis {
#include <utility>
using std::unreachable;
}
#else
namespace wis {
#ifdef __cpp_lib_unreachable
using std::unreachable;
#else
[[noreturn]] inline void unreachable()
{
    // Uses compiler specific extensions if possible.
    // Even if no extension is used, undefined behavior is still raised by
    // an empty function body and the noreturn attribute.
#ifdef _MSC_VER // MSVC
    __assume(false);
#else // GCC, Clang, ICC
    __builtin_unreachable();
#endif
}
#endif
} // namespace wis
#endif
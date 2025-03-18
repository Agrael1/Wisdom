module;
#include <wisdom/util/com_ptr.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/util/misc.h>
export module wisdom.internal;

// misc
export wis::string_hash;
export wis::com_ptr;

export namespace wis {
namespace detail {
using wis::detail::fixed_allocation;
using wis::detail::limited_allocator;
using wis::detail::uniform_allocator;
using wis::detail::make_fixed_allocation;
using wis::detail::make_unique;
using wis::detail::make_unique_for_overwrite;
using wis::detail::memory_pool;
} // namespace detail
} // namespace wis

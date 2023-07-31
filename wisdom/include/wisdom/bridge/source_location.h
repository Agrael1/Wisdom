#pragma once
#if __has_include(<source_location>)
#include <source_location>
namespace wis {
using std::source_location;
}
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
namespace wis {
using std::experimental::source_location;
}
#else
#error "No source_location header found"
#endif

#pragma once
#if __has_include(<generator>)
#include <generator>
namespace wis {
	using std::generator;
}
#elif __has_include(<experimental/generator>)
#include <experimental/generator>
namespace wis {
	using std::experimental::generator;
}
#else
#error Generator not supported, please update the c++ STL version or compiler
#endif
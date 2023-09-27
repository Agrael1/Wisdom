#pragma once
#include <wisdom/api/api.h>

namespace wis {
static inline constexpr Result success{
    wis::Status::Ok, "Operation succeeded"
};

}; // namespace wis
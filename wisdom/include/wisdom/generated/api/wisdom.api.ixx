/** \mainpage Wisdom API Documentation

<b>Version 0.6.8</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

module;
#define WISDOM_MODULE_DECL
#include <wisdom/generated/api/api.include.h>
export module wisdom.api;

#include <wisdom/generated/api/api.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/global/constants.h>
#include <wisdom/util/string_literal.h>
#include <wisdom/bridge/source_location.h>
#include <wisdom/generated/api/api.hpp>
#include <wisdom/util/com_ptr.h>
#include <wisdom/util/flags.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/error_messages.h>
#include <wisdom/util/misc.h>
#include <wisdom/util/shader_compiler.h>
#include <wisdom/util/small_allocator.h>

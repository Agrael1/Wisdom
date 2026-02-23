#include "generator.hpp"
#include <fstream>
#include <ranges>

//-----------------------------------------------------------------------------

void Generator::ParseFile(std::filesystem::path file)
{
    // open and parse XML file
    auto  absolute = std::filesystem::absolute(file);
    auto& doc      = documents[absolute];
    doc.LoadFile(absolute.string().c_str());
    return ParseFile(doc);
}

void Generator::WriteMainAPI()
{
    std::filesystem::path cpp_output_path     = main_output_dir;
    std::filesystem::path cpp_output_path_api = cpp_output_path / "generated";
    WriteCAPI(cpp_output_path_api / "c_api.h");
    WriteCPPAPI(cpp_output_path_api / "cpp_api.hpp");
    WriteCDependentAPI(cpp_output_path_api);
    WriteCPPDependentAPI(cpp_output_path_api);

    WriteCIndependentAPI(cpp_output_path);
    WriteCPPIndependentAPI(cpp_output_path);

    WriteConversions(cpp_output_path_api);
}

void Generator::WriteMainAPIDoc()
{
    std::filesystem::path doc_output_path    = doc_output_dir;
    std::filesystem::path enum_output_path   = doc_output_path / "wisdom/enum";
    std::filesystem::path struct_output_path = doc_output_path / "wisdom/struct";
    std::filesystem::path handle_output_path = doc_output_path / "wisdom/handle";
    std::filesystem::path func_output_path   = doc_output_path / "wisdom/func";

    WriteEnumDocumentation(enum_output_path);
    WriteBitmaskDocumentation(enum_output_path);
    WriteStructDocumentation(struct_output_path);
    WriteVariantDocumentation(struct_output_path);
    WriteHandleDocumentation(handle_output_path);
    WriteFunctionDocumentation(func_output_path);
    WriteDelegateDocumentation(func_output_path);
}

//-----------------------------------------------------------------------------
void Generator::ParseFile(tinyxml2::XMLDocument& doc)
{
    auto* root = doc.FirstChildElement("registry");
    if (!root) {
        throw std::runtime_error("Invalid XML file: missing <registry> root element");
    }

    if (auto* include = root->FirstChildElement("includes")) {
        ParseIncludes(include);
    }

    if (auto* handles = root->FirstChildElement("handles")) {
        ParseHandles(handles);
    }

    if (auto* types = root->FirstChildElement("types")) {
        ParseTypes(types);
    }

    if (auto* funcs = root->FirstChildElement("functions")) {
        ParseFunctions(funcs);
    }

    if (auto* va = root->FirstChildElement("validations")) {
        ParseValidations(va);
    }
}

void Generator::ParseIncludes(tinyxml2::XMLElement* includes)
{
    for (auto* include = includes->FirstChildElement("include"); include;
         include       = include->NextSiblingElement("include")) {
        auto file     = include->GetText();
        auto rpath    = std::filesystem::path(INPUT_FILE).parent_path() / file;
        auto absolute = std::filesystem::absolute(rpath);

        if (std::filesystem::exists(absolute) && !documents.contains(absolute)) {
            auto& doc = documents[absolute];
            doc.LoadFile(absolute.string().c_str());
            ParseFile(doc);
        }
    }
}

void Generator::ParseTypes(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("type"); type;
         type       = type->NextSiblingElement("type")) {
        auto category = type->FindAttribute("category")->Value();
        if (std::string_view(category) == "struct") {
            ParseStruct(type);
        } else if (std::string_view(category) == "enum") {
            ParseEnum(type);
        } else if (std::string_view(category) == "bitmask") {
            ParseBitmask(type);
        } else if (std::string_view(category) == "delegate") {
            ParseDelegate(type);
        } else if (std::string_view(category) == "variant") {
            ParseVariant(type);
        }
    }
}

//-----------------------------------------------------------------------------

void Generator::WriteCAPI(std::filesystem::path path)
{
    files.push_back(path);
    std::ofstream file{ path, std::ios::out | std::ios::trunc };
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path.string());
    }
    // Write header
    file << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_C_API_H
#define WISDOM_C_API_H
#include <wisdom/global/definitions.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
)";

    file << "\n//==============================================================\n"
            "// Enums\n"
            "//==============================================================\n\n";

    // Write enums
    for (auto& enum_name : enums_in_order) {
        auto& enum_def = enum_map[enum_name];
        file << MakeCEnum(enum_def);
        file << "\n";
    }

    // Write bitmasks
    for (auto& bitmask_name : bitmasks_in_order) {
        auto& bitmask_def = bitmask_map[bitmask_name];
        file << MakeCBitmask(bitmask_def);
        file << "\n";
    }

    file << "\n//==============================================================\n"
            "// Delegates\n"
            "//==============================================================\n\n";
    // Write delegates (before structs, as structs may reference delegates)
    for (auto& delegate_name : delegates_in_order) {
        auto& delegate_def = delegate_map[delegate_name];
        file << MakeCDelegate(delegate_def);
        file << "\n";
    }

    file << "\n//==============================================================\n"
            "// Structs\n"
            "//==============================================================\n\n";
    // Write structs
    for (auto& struct_name : structs_in_order) {
        auto& struct_def = struct_map[struct_name];
        file << MakeCStruct(struct_def);
        file << "\n";
    }

    // Write footer
    file << R"(
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_API_H
)";
}

void Generator::WriteCPPAPI(std::filesystem::path path)
{
    files.push_back(path);
    std::ofstream file{ path, std::ios::out | std::ios::trunc };
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path.string());
    }
    // Write header
    file << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_API_HPP
#define WISDOM_CPP_API_HPP
#ifdef __cplusplus
#include <wisdom/global/definitions.h>
#include <wisdom/bridge/span.hpp>

namespace wis {
)";

    file << "\n//==============================================================\n"
            "// Enums\n"
            "//==============================================================\n\n";

    // Write enums
    for (auto& enum_name : enums_in_order) {
        auto& enum_def = enum_map[enum_name];
        file << MakeCPPEnum(enum_def);
        file << "\n";
    }

    // Write bitmasks
    for (auto& bitmask_name : bitmasks_in_order) {
        auto& bitmask_def = bitmask_map[bitmask_name];
        file << MakeCPPBitmask(bitmask_def);
        file << "\n";
    }

    file << "\n//==============================================================\n"
            "// Delegates\n"
            "//==============================================================\n\n";
    // Write delegates (before structs, as structs may reference delegates)
    for (auto& delegate_name : delegates_in_order) {
        auto& delegate_def = delegate_map[delegate_name];
        file << MakeCPPDelegate(delegate_def);
        file << "\n";
    }

    file << "\n//==============================================================\n"
            "// Structs\n"
            "//==============================================================\n\n";
    // Write structs
    for (auto& struct_name : structs_in_order) {
        auto& struct_def = struct_map[struct_name];
        file << MakeCPPStruct(struct_def);
        file << "\n";
    }

    // Write footer
    file << R"(
}
#endif // __cplusplus
#endif // WISDOM_CPP_API_HPP
)";
}

//-----------------------------------------------------------------------------

void Generator::WriteCDependentAPI(std::filesystem::path dir)
{
    std::filesystem::path path_dx = dir / "dx12_api.h";
    std::filesystem::path path_vk = dir / "vk_api.h";
    files.push_back(path_dx);
    files.push_back(path_vk);

    std::ofstream file_dx{ path_dx, std::ios::out | std::ios::trunc };
    if (!file_dx.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_dx.string());
    }
    std::ofstream file_vk{ path_vk, std::ios::out | std::ios::trunc };
    if (!file_vk.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_vk.string());
    }

    // Write header
    file_dx << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_C_DX12_API_H
#define WISDOM_C_DX12_API_H
#include <wisdom/generated/c_api.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
)";
    file_vk << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_C_VK_API_H
#define WISDOM_C_VK_API_H
#include <wisdom/generated/c_api.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
)";

    // Write handles
    for (auto& handle_name : handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        file_dx << MakeCHandle(handle_def, "dx");
        file_dx << "\n";
        file_vk << MakeCHandle(handle_def, "vk");
        file_vk << "\n";
    }

    // Write variants
    for (auto& variant_name : variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        file_dx << MakeCVariant(variant_def, "dx");
        file_dx << "\n";
        file_vk << MakeCVariant(variant_def, "vk");
        file_vk << "\n";
    }

    // Write functions
    for (auto& func_name : functions_in_order) {
        auto& func_def = function_map[func_name];
        file_dx << MakeCFunctionDecl(func_def, "dx", "WISDOM_API ");
        file_dx << "\n";
        file_vk << MakeCFunctionDecl(func_def, "vk", "WISDOM_API ");
        file_vk << "\n";
    }

    // Write footer
    file_dx << R"(
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_DX12_API_H
)";
    file_vk << R"(
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_VK_API_H
)";
}

void Generator::WriteCIndependentAPI(std::filesystem::path dir)
{
    std::filesystem::path path_w = dir / "wisdom.h";
    files.push_back(path_w);

    std::ofstream file_w{ path_w, std::ios::out | std::ios::trunc };
    if (!file_w.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_w.string());
    }

    // Write header
    file_w << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_H
#define WISDOM_H

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
#include "generated/dx12_api.h"
)";
    constexpr static auto impl_dx = GetImplString(ImplementedFor::DX12);
    constexpr static auto impl_vk = GetImplString(ImplementedFor::Vulkan);

    file_w << "\n\n//==============================================================\n"
              "// Handles\n"
              "//==============================================================\n\n";

    // Write handles
    for (auto& handle_name : handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        file_w << wis::format("typedef struct {} {};\n", GetCFullTypename(handle_def.name, impl_dx), GetCFullTypename(handle_def.name));
    }

    // Write Views for handles
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::DX12) > 0) {
            file_w << wis::format("typedef struct {}View {}View;\n", GetCFullTypename(handle_def.name, impl_dx), GetCFullTypename(handle_def.name));
        }
    }

    file_w << "\n\n//==============================================================\n"
              "// Variants\n"
              "//==============================================================\n\n";

    // Write variants
    for (auto& variant_name : variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        file_w << wis::format("typedef struct {} {};\n", GetCFullTypename(variant_def.name, impl_dx), GetCFullTypename(variant_def.name));
    }

    file_w << "\n\n//==============================================================\n"
              "// Functions\n"
              "//==============================================================\n\n";

    // Write functions
    for (auto& func_name : functions_in_order) {
        auto& func_def = function_map[func_name];
        file_w << wis::format("#define {} {}\n",
                              wis::format("wis{}{}", func_def.name.starts_with("Destroy") ? "" : func_def.this_type, func_def.name),
                              wis::format("wis{}{}{}", impl_dx, func_def.name.starts_with("Destroy") ? "" : func_def.this_type, func_def.name));
    }

    // Write functions that convert handles to views
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::DX12) > 0) {
            file_w << wis::format("#define {} {}\n",
                                  wis::format("wisGet{}View", handle_def.name),
                                  wis::format("wisGet{}{}View", impl_dx, handle_def.name));
        }
    }

        file_w << R"(
#define wisGetView(handle) \ 
    _Generic((handle), \ 
)";
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::DX12) > 0) {
            file_w << wis::format("const {}*: wisGet{}{}View, \\\n", GetCFullTypename(handle_def.name, impl_dx), impl_dx, handle_def.name);
            file_w << wis::format("{}*: wisGet{}{}View, \\\n", GetCFullTypename(handle_def.name, impl_dx), impl_dx, handle_def.name);
        }
    }
    file_w << "default: (void)0 \\\n)(handle)";


    file_w << R"(
#elif defined(WISDOM_VULKAN)
#include "generated/vk_api.h"
)";

    file_w << "\n\n//==============================================================\n"
              "// Handles\n"
              "//==============================================================\n\n";

    // Write handles
    for (auto& handle_name : handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        file_w << wis::format("typedef struct {} {};\n", GetCFullTypename(handle_def.name, impl_vk), GetCFullTypename(handle_def.name));
    }

    // Write Views for handles
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::Vulkan) > 0) {
            file_w << wis::format("typedef struct {}View {}View;\n", GetCFullTypename(handle_def.name, impl_vk), GetCFullTypename(handle_def.name));
        }
    }

    file_w << "\n\n//==============================================================\n"
              "// Variants\n"
              "//==============================================================\n\n";

    // Write variants
    for (auto& variant_name : variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        file_w << wis::format("typedef struct {} {};\n", GetCFullTypename(variant_def.name, impl_vk), GetCFullTypename(variant_def.name));
    }

    file_w << "\n\n//==============================================================\n"
              "// Functions\n"
              "//==============================================================\n\n";

    // Write functions
    for (auto& func_name : functions_in_order) {
        auto& func_def = function_map[func_name];
        file_w << wis::format("#define {} {}\n",
                              wis::format("wis{}{}", func_def.name.starts_with("Destroy") ? "" : func_def.this_type, func_def.name),
                              wis::format("wis{}{}{}", impl_vk, func_def.name.starts_with("Destroy") ? "" : func_def.this_type, func_def.name));
    }

    // Write functions that convert handles to views
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::Vulkan) > 0) {
            file_w << wis::format("#define {} {}\n",
                                  wis::format("wisGet{}View", handle_def.name),
                                  wis::format("wisGet{}{}View", impl_vk, handle_def.name));
        }
    }

        file_w << R"(
#define wisGetView(handle) \ 
    _Generic((handle), \ 
)";
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::Vulkan) > 0) {
            file_w << wis::format("const {}*: wisGet{}{}View, \\\n", GetCFullTypename(handle_def.name, impl_vk), impl_vk, handle_def.name);
            file_w << wis::format("{}*: wisGet{}{}View, \\\n", GetCFullTypename(handle_def.name, impl_vk), impl_vk, handle_def.name);
        }
    }
    file_w << R"(default: (void)0 \ 
    )(handle))";

    file_w << R"(
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_H
)";
}

//-----------------------------------------------------------------------------
void Generator::WriteCPPDependentAPI(std::filesystem::path dir)
{
    std::filesystem::path path_dx = dir / "dx12_cpp_api.hpp";
    std::filesystem::path path_vk = dir / "vk_cpp_api.hpp";
    files.push_back(path_dx);
    files.push_back(path_vk);

    std::ofstream file_dx{ path_dx, std::ios::out | std::ios::trunc };
    if (!file_dx.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_dx.string());
    }
    std::ofstream file_vk{ path_vk, std::ios::out | std::ios::trunc };
    if (!file_vk.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_vk.string());
    }

    // Write header
    file_dx << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_API_HPP
#define WISDOM_CPP_DX12_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/dx12/dx12_types.hpp>

namespace wis {
)";
    file_vk << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_VK_API_HPP
#define WISDOM_CPP_VK_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/vulkan/vk_types.hpp>

namespace wis {
)";

    // Write variants
    for (auto& variant_name : variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        file_dx << MakeCPPVariant(variant_def, "dx");
        file_dx << "\n";
        file_vk << MakeCPPVariant(variant_def, "vk");
        file_vk << "\n";
    }

    // Write handles
    for (auto& handle_name : handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        file_dx << MakeCPPHandle(handle_def, "dx");
        file_dx << "\n";
        file_vk << MakeCPPHandle(handle_def, "vk");
        file_vk << "\n";
    }

    // Write functions
    for (auto& func_name : free_functions_in_order) {
        auto& func_def = function_map[func_name];
        file_dx << MakeCPPFunctionImpl(func_def, "dx", "inline ");
        file_dx << "\n";
        file_vk << MakeCPPFunctionImpl(func_def, "vk", "inline ");
        file_vk << "\n";
    }

    // Write footer
    file_dx << R"(
}
#endif // WISDOM_CPP_DX12_API_HPP
)";
    file_vk << R"(
}
#endif // WISDOM_CPP_VK_API_HPP
)";
}

void Generator::WriteCPPIndependentAPI(std::filesystem::path dir)
{
    std::filesystem::path path_w = dir / "wisdom.hpp";
    files.push_back(path_w);

    std::ofstream file_w{ path_w, std::ios::out | std::ios::trunc };
    if (!file_w.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_w.string());
    }

    // Write header
    file_w << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_HPP
#define WISDOM_HPP

#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
#include "generated/dx12_cpp_api.hpp"

namespace wis {
)";
    constexpr static auto impl_dx = GetImplString(ImplementedFor::DX12);
    constexpr static auto impl_vk = GetImplString(ImplementedFor::Vulkan);

    file_w << "\n\n//==============================================================\n"
              "// Handles\n"
              "//==============================================================\n\n";

    // Write handles
    for (auto& handle_name : handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        file_w << wis::format("using {} = {};\n", handle_def.name, GetCPPFullTypename(handle_def.name, impl_dx));
    }

    // Write Views for handles
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::DX12) > 0) {
            file_w << wis::format("using {}View = {};\n", handle_def.name, GetCPPFullTypename(handle_def.name, impl_dx) + "View");
        }
    }

    file_w << "\n\n//==============================================================\n"
              "// Variants\n"
              "//==============================================================\n\n";

    // Write variants
    for (auto& variant_name : variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        file_w << wis::format("using {} = {};\n", variant_def.name, GetCPPFullTypename(variant_def.name, impl_dx));
    }

    file_w << "\n\n//==============================================================\n"
              "// Functions\n"
              "//==============================================================\n\n";

    // Write functions
    for (auto& func_name : free_functions_in_order) {
        auto& func_def = function_map[func_name];
        file_w << MakeCPPFunctionImpl(func_def, "dx", "inline ", DocKind::Full, ProtoType::Universal);
        file_w << '\n';
    }

    file_w << R"(
} // namespace wis

#elif defined(WISDOM_VULKAN)
#include "generated/vk_cpp_api.hpp"

namespace wis {
)";

    file_w << "\n\n//==============================================================\n"
              "// Handles\n"
              "//==============================================================\n\n";

    // Write handles
    for (auto& handle_name : handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        file_w << wis::format("using {} = {};\n", handle_def.name, GetCPPFullTypename(handle_def.name, impl_vk));
    }

    // Write Views for handles
    for (auto& handle_name : views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(ImplementedFor::Vulkan) > 0) {
            file_w << wis::format("using {}View = {};\n", handle_def.name, GetCPPFullTypename(handle_def.name, impl_vk) + "View");
        }
    }

    file_w << "\n\n//==============================================================\n"
              "// Variants\n"
              "//==============================================================\n\n";

    // Write variants
    for (auto& variant_name : variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        file_w << wis::format("using {} = {};\n", variant_def.name, GetCPPFullTypename(variant_def.name, impl_vk));
    }

    file_w << "\n\n//==============================================================\n"
              "// Functions\n"
              "//==============================================================\n\n";

    // Write functions
    for (auto& func_name : free_functions_in_order) {
        auto& func_def = function_map[func_name];
        file_w << MakeCPPFunctionImpl(func_def, "vk", "inline ", DocKind::Full, ProtoType::Universal);
        file_w << '\n';
    }

    file_w << R"(
} // namespace wis
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_HPP
)";
}

void Generator::WriteConversions(std::filesystem::path dir)
{
    std::filesystem::path path_dx = dir / "dx12_convert.hpp";
    std::filesystem::path path_vk = dir / "vk_convert.hpp";
    files.push_back(path_dx);
    files.push_back(path_vk);

    std::ofstream file_dx{ path_dx, std::ios::out | std::ios::trunc };
    if (!file_dx.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_dx.string());
    }
    std::ofstream file_vk{ path_vk, std::ios::out | std::ios::trunc };
    if (!file_vk.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_vk.string());
    }

    // Write header
    file_dx << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_CONVERT_HPP
#define WISDOM_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <dxgi1_6.h>
#include <d3d12.h>

namespace wis{ namespace detail {
)";
    file_vk << R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_VK_CONVERT_HPP
#define WISDOM_CPP_VK_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <vulkan/vulkan.h>

namespace wis{ namespace detail {
)";

    // Write enums
    for (auto& enum_name : enums_in_order) {
        auto& enum_def = enum_map[enum_name];
        file_dx << MakeEnumConverter(enum_def, "dx");
        file_dx << "\n";
        file_vk << MakeEnumConverter(enum_def, "vk");
        file_vk << "\n";
    }
    // Write bitmasks
    for (auto& bitmask_name : bitmasks_in_order) {
        auto& bitmask_def = bitmask_map[bitmask_name];
        file_dx << MakeBitmaskConverter(bitmask_def, "dx");
        file_dx << "\n";
        file_vk << MakeBitmaskConverter(bitmask_def, "vk");
        file_vk << "\n";
    }

    // Write footer
    file_dx << R"(
}}
#endif // WISDOM_DX12_CONVERT_HPP
)";
    file_vk << R"(
}}
#endif // WISDOM_VK_CONVERT_HPP
)";
}

void Generator::WriteDocumentation(std::filesystem::path doc_output_path,
                                   std::string_view      doc_template,
                                   std::string_view      object_name,
                                   std::string_view      code,
                                   std::string_view      vuids,
                                   std::string_view      desc,
                                   std::string_view      refs)
{
    if (refs.empty()) {
        refs = " * ";
    }

    // If file exists, only edit the generated code section, else create new file
    bool         file_exists = std::filesystem::exists(doc_output_path);
    std::fstream enum_file{ doc_output_path, file_exists ? std::ios::in | std::ios::out : std::ios::out };

    if (!file_exists) {
        std::string xenum = wis::vformat(doc_template, wis::make_format_args(object_name, code, desc));

        enum_file << FinalizeCDocumentation(xenum, object_name);
        enum_file.close();
        return;
    }

    // Otherwise, we would need to parse the existing file and replace the generated section
    // Read entire file content
    std::string existing_content((std::istreambuf_iterator<char>(enum_file)),
                                 std::istreambuf_iterator<char>());
    enum_file.close();
    // Find the generated section
    size_t gen_start = existing_content.find(R"(\cond WIS_GEN_CODE)");
    size_t gen_end   = existing_content.find(R"(\endcond)", gen_start);

    // Find the vuids section
    size_t vuid_start = existing_content.find(R"(\cond WIS_GEN_WIS_IDS)");
    size_t vuid_end   = existing_content.find(R"(\endcond)", vuid_start);

    // Find the description section
    size_t desc_start = existing_content.find(R"(\cond WIS_GEN_DESC)");
    size_t desc_end   = existing_content.find(R"(\endcond)", desc_start);

    // Find the references section
    size_t ref_start = existing_content.find(R"(\cond WIS_GEN_REFS)");
    size_t ref_end   = existing_content.find(R"(\endcond)", ref_start);

    // Replace the references section
    if (ref_start != std::string::npos && ref_end != std::string::npos && ref_end > ref_start) {
        existing_content = existing_content.substr(0, ref_start) + "\\cond WIS_GEN_REFS\n" + std::string(refs) + existing_content.substr(ref_end);
    }
    // Replace the vuids section
    if (vuid_start != std::string::npos && vuid_end != std::string::npos && vuid_end > vuid_start) {
        existing_content = existing_content.substr(0, vuid_start) + "\\cond WIS_GEN_WIS_IDS\n" + std::string(vuids) + existing_content.substr(vuid_end);
    }
    // Replace the description section
    if (desc_start != std::string::npos && desc_end != std::string::npos && desc_end > desc_start) {
        existing_content = existing_content.substr(0, desc_start) + "\\cond WIS_GEN_DESC\n" + std::string(desc) + existing_content.substr(desc_end);
    }
    // Replace the generated section
    if (gen_start != std::string::npos && gen_end != std::string::npos && gen_end > gen_start) {
        existing_content = existing_content.substr(0, gen_start) + "\\cond WIS_GEN_CODE\n" + std::string(code) + existing_content.substr(gen_end);
    }

    // Write back to file
    std::ofstream enum_file_out{ doc_output_path, std::ios::trunc };
    enum_file_out << existing_content;
    enum_file_out.close();
}

// Helpers
TypeKind Generator::GetType(std::string_view type_name) const noexcept
{
    if (type_name.empty()) {
        return TypeKind::None;
    }
    if (auto it = enum_map.find(type_name); it != enum_map.end()) {
        return TypeKind::Enum;
    }
    if (auto it = bitmask_map.find(type_name); it != bitmask_map.end()) {
        return TypeKind::Bitmask;
    }
    if (auto it = struct_map.find(type_name); it != struct_map.end()) {
        return TypeKind::Struct;
    }
    if (auto it = variant_map.find(type_name); it != variant_map.end()) {
        return TypeKind::Variant;
    }
    if (auto it = handle_map.find(type_name); it != handle_map.end()) {
        return TypeKind::Handle;
    }
    if (auto it = function_map.find(type_name); it != function_map.end()) {
        return TypeKind::Function;
    }
    if (delegate_map.contains(type_name)) {
        return TypeKind::FuncPointer;
    }
    if (view_set.contains(type_name.substr(0, type_name.size() - 4))) {
        return TypeKind::View; // A view, but we don't need extra type
    }
    return TypeKind::Base;
}

void Generator::TryMakeRef(std::string_view type, std::string_view ref)
{
    // ref is reverse, meaning ref uses type
    auto xtype = GetType(type);
    if (xtype == TypeKind::None || xtype == TypeKind::Base) {
        return;
    }
    dependency_tree[type].dependencies.push_back(ref);
}
std::string Generator::GetCFullTypename(std::string_view type, std::string_view impl)
{
    switch (GetType(type)) {
    case TypeKind::Base:
        return std::string(standard_types.at(type));
    default:
    case TypeKind::None:
        return "";
    case TypeKind::Struct:
        return wis::format("Wis{}", type);
    case TypeKind::Variant:
        return wis::format("Wis{}{}", impl, type);
    case TypeKind::Union:
        break;
    case TypeKind::Enum:
    case TypeKind::Bitmask:
    case TypeKind::FuncPointer:
        return wis::format("Wis{}", type);
    case TypeKind::Handle:
    case TypeKind::View:
        return wis::format("Wis{}{}", impl, type);
    case TypeKind::Function: {
        auto& func = function_map.at(type);
        if (!func.this_type.empty() && !func.name.starts_with("Destroy")) {
            return wis::format("wis{}{}{}", impl, func.this_type, type);
        }
        return wis::format("wis{}{}", impl, type);
    }
    case TypeKind::Alias:
        break;
    }
    return "";
}
std::string Generator::GetCPPFullTypename(std::string_view type, std::string_view impl)
{
    switch (GetType(type)) {
    case TypeKind::Base:
        return std::string(standard_types_cpp.at(type));
    default:
    case TypeKind::None:
        return "";
    case TypeKind::FuncPointer:
    case TypeKind::Bitmask:
    case TypeKind::Struct:
    case TypeKind::Enum:
        return wis::format("wis::{}", type);
    case TypeKind::Variant:
    case TypeKind::Handle:
    case TypeKind::View:
    case TypeKind::Function:
        return wis::format("wis::{}{}", impl, type);
    case TypeKind::Union:
        break;
    case TypeKind::Alias:
        break;
    }
    return "";
}
std::string Generator::FinalizeCDocumentation(std::string doc, std::string_view this_type, std::string_view impl)
{
    if (doc.empty()) {
        return doc;
    }

    std::string_view this_type_view = this_type;

    while (true) {
        auto&& [type, value, first, last] = FindInlineType(doc);
        if (type.empty() && value.empty()) {
            break;
        }

        // Replace with this_type
        if (!type.empty()) {
            this_type_view = type;
        }

        std::string replacement;

        if (auto x = enum_map.find(this_type_view); x != enum_map.end()) {
            auto evalue = x->second.HasValue(value);
            replacement = evalue ? wis::format("`Wis{}{}`", x->second.name, evalue->name)
                                 : GetCPPFullTypename(x->second.name, impl);

        } else if (auto y = bitmask_map.find(this_type_view); y != bitmask_map.end()) {
            auto evalue = y->second.HasValue(value);
            replacement = evalue ? wis::format("`{}::{}`", GetCFullTypename(y->second.name, impl), evalue->name)
                                 : GetCFullTypename(y->second.name, impl);
        } else if (auto z = struct_map.find(this_type_view); z != struct_map.end()) {
            auto member = z->second.HasValue(value);
            replacement = member ? wis::format("`{}::{}`", GetCFullTypename(z->second.name, impl), member->name)
                                 : GetCFullTypename(z->second.name, impl);
        } else if (auto z = variant_map.find(this_type_view); z != variant_map.end()) {
            auto member = z->second.HasValue(value);
            replacement = member ? wis::format("`{}::{}`", GetCFullTypename(z->second.name, impl), member->name)
                                 : GetCFullTypename(z->second.name, impl);
        } /*else if (auto d = delegate_map.find(this_type_view); d != delegate_map.end()) {
            auto member = d->second.HasValue(value);
            replacement = member ? wis::format("{}::{}", GetCFullTypename(d->second.name, impl), member->name)
                                 : GetCFullTypename(d->second.name, impl);
        }*/
        else if (auto h = handle_map.find(this_type_view); h != handle_map.end()) {
            replacement = GetCFullTypename(h->second.name, impl);
        } else if (auto f = function_map.find(std::string(this_type_view)); f != function_map.end()) {
            auto member = f->second.HasValue(value);
            replacement = member ? wis::format("`{}`", member->name)
                                 : GetCFullTypename(f->second.name, impl);
        }
        doc.replace(first, last - first + 1, replacement);
    }

    // replave should, must, may with @wis_should, @wis_must, @wis_may
    ReplaceAll(doc, " should not", " @wis_shouldnot ");
    ReplaceAll(doc, " must not", " @wis_mustnot");
    ReplaceAll(doc, " may not", " @wis_maynot");

    ReplaceAll(doc, " should ", " @wis_should ");
    ReplaceAll(doc, " must ", " @wis_must ");
    ReplaceAll(doc, " may ", " @wis_may ");
    return doc;
}
std::string Generator::FinalizeCPPDocumentation(std::string doc, std::string_view this_type, std::string_view impl)
{
    if (doc.empty()) {
        return doc;
    }

    std::string_view this_type_view = this_type;

    while (true) {
        auto&& [type, value, first, last] = FindInlineType(doc);
        if (type.empty() && value.empty()) {
            break;
        }

        // Replace with this_type
        if (!type.empty()) {
            this_type_view = type;
        }

        std::string replacement;

        if (auto x = enum_map.find(this_type_view); x != enum_map.end()) {
            auto evalue = x->second.HasValue(value);
            replacement = evalue ? wis::format("`{}::{}`", GetCPPFullTypename(x->second.name, impl), evalue->name)
                                 : GetCPPFullTypename(x->second.name, impl);

        } else if (auto y = bitmask_map.find(this_type_view); y != bitmask_map.end()) {
            auto evalue = y->second.HasValue(value);
            replacement = evalue ? wis::format("`{}::{}`", GetCPPFullTypename(y->second.name, impl), evalue->name)
                                 : GetCFullTypename(y->second.name, impl);
        } else if (auto z = struct_map.find(this_type_view); z != struct_map.end()) {
            auto member = z->second.HasValue(value);
            replacement = member ? wis::format("`{}::{}`", GetCPPFullTypename(z->second.name, impl), member->name)
                                 : GetCPPFullTypename(z->second.name, impl);
        } else if (auto z = variant_map.find(this_type_view); z != variant_map.end()) {
            auto member = z->second.HasValue(value);
            replacement = member ? wis::format("`{}::{}`", GetCPPFullTypename(z->second.name, impl), member->name)
                                 : GetCPPFullTypename(z->second.name, impl);
        } /*else if (auto d = delegate_map.find(this_type_view); d != delegate_map.end()) {
            auto member = d->second.HasValue(value);
            replacement = member ? wis::format("{}::{}", GetCFullTypename(d->second.name, impl), member->name)
                                 : GetCFullTypename(d->second.name, impl);
        }*/
        else if (auto h = handle_map.find(this_type_view); h != handle_map.end()) {
            replacement = GetCPPFullTypename(h->second.name, impl);
        } else if (auto f = function_map.find(std::string(this_type_view)); f != function_map.end()) {
            auto member = f->second.HasValue(value);
            replacement = member ? wis::format("`{}`", member->name)
                                 : GetCPPFullTypename(f->second.name, impl);
        }
        doc.replace(first, last - first + 1, replacement);
    }

    // replave should, must, may with @wis_should, @wis_must, @wis_may
    ReplaceAll(doc, " should not", " @wis_shouldnot ");
    ReplaceAll(doc, " must not", " @wis_mustnot");
    ReplaceAll(doc, " may not", " @wis_maynot");

    ReplaceAll(doc, " should ", " @wis_should ");
    ReplaceAll(doc, " must ", " @wis_must ");
    ReplaceAll(doc, " may ", " @wis_may ");
    return doc;
}

std::string Generator::GetSpecificationCode(std::string_view c_code, std::string_view c_impl_code, std::string_view cpp_code, std::string_view cpp_impl_code)
{
    std::string template_content_c;
    if (!c_code.empty()) {
        template_content_c = wis::format(" C Version:\n```c\n{}```\n", c_code);
        if (!c_impl_code.empty()) {
            // append a details section
            template_content_c += wis::format("<details>\n<summary>C Implementation Specific Version:</summary>\n```c\n{}```\n</details>\n",
                                              c_impl_code);
        }
    }

    std::string template_content_cpp;
    if (!cpp_code.empty()) {
        template_content_cpp = wis::format("C++ Version:\n```cpp\nnamespace wis{{\n{}}}\n```\n", cpp_code);
        if (!cpp_impl_code.empty()) {
            // append a details section
            template_content_cpp += wis::format("<details>\n<summary>C++ Implementation Specific Version:</summary>\n```cpp\nnamespace wis{{\n{}}}\n```\n</details>\n",
                                                cpp_impl_code);
        }
    }

    std::string output = wis::format(" * {}\n{}", template_content_c, template_content_cpp);
    ReplaceAll(output, "\n", "\n * ");
    return output;
}

ImplementedFor Generator::ImplCode(std::string_view impl) noexcept
{
    if (impl == "dx") {
        return ImplementedFor::DX12;
    }
    if (impl == "vk") {
        return ImplementedFor::Vulkan;
    }
    return ImplementedFor::Both;
}

void Generator::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

InlineTypeInfo Generator::FindInlineType(std::string_view str)
{
    auto end   = str.rfind('}');
    auto end_n = str.rfind('\n');
    if (end == std::string_view::npos || end_n < end) {
        return {};
    }

    // Find caption enclosed in { }
    auto pos = str.rfind('{', end);
    if (pos == std::string_view::npos) {
        return {};
    }

    std::string_view caption = str.substr(pos + 1, end - pos - 1);
    std::string_view type, value;

    // seek if there is :: inside the caption
    auto colon = caption.find("::");
    if (colon != std::string_view::npos) {
        value = caption.substr(colon + 2);
        type  = caption.substr(0, colon);
    }

    return { type, value, pos, end };
}

std::string Generator::MakeVersionString(std::string_view version, bool newline)
{
    return version.empty() ? "" : wis::format("Provided by Wisdom {}.{}", version, newline ? "\n" : " ");
}

std::string Generator::MakeSnakeCase(std::string_view str)
{
    std::string result;
    for (char ch : str) {
        if (std::isupper(static_cast<unsigned char>(ch))) {
            if (!result.empty()) {
                result += '_';
            }
            result += std::tolower(static_cast<unsigned char>(ch));
        } else {
            result += ch;
        }
    }
    return result;
}

Modifier Generator::GetModifiers(std::string_view mod_str) noexcept
{
    // tokenize by comma lazily
    Modifier mods = Modifier::None;
    for (auto&& tk : std::views::split(mod_str, std::string_view{ "," })) {
        // trim
        std::string_view tk_view{ tk.begin(), tk.end() };
        switch (tk_view[0]) {
        case 'p':
            if (tk_view == "ptr") {
                mods = Modifier(mods | Modifier::Pointer);
            } else if (tk_view == "pp") {
                mods = Modifier(mods | Modifier::PointerToPointer);
            }
            break;
        case 'r':
            if (tk_view == "ref") {
                mods = Modifier(mods | Modifier::Reference);
            }
            break;
        case 'c':
            if (tk_view == "const") {
                mods = Modifier(mods | Modifier::Const);
            }
            if (tk_view == "c-only") {
                mods = Modifier(mods | Modifier::COnly);
            }
            break;
        case 'n':
            if (tk_view == "nodiscard") {
                mods = Modifier(mods | Modifier::Nodiscard);
            }
            break;
        case 's':
            if (tk_view == "span") {
                mods = Modifier(mods | Modifier::Span);
            }
            break;
        case 'd':
            if (tk_view == "destroy") {
                mods = Modifier(mods | Modifier::Destroy);
            }
            break;
        default:
            break;
        }
    }
    return mods;
}

std::string Generator::GetRefs(std::string_view for_type)
{
    auto it = dependency_tree.find(for_type);
    if (it == dependency_tree.end()) {
        return "";
    }
    auto& xrefs = it->second.dependencies;

    std::size_t                  ref_count     = 0;
    static constexpr std::size_t max_ref_count = 10;
    // Gather references
    std::string refs;
    for (auto& ref : xrefs) {
        if (ref.empty()) {
            continue;
        }

        refs += GetCFullTypename(ref, "");
        if (++ref_count > max_ref_count) {
            break;
        }
        refs += ", ";
    }
    refs.pop_back(); // remove last space
    refs.pop_back(); // remove last comma
    if (!refs.empty()) {
        refs = wis::format(" * @see {}\n", refs);
    }
    return refs;
}

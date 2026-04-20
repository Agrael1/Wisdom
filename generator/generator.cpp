#include "generator.hpp"
#include <fstream>
#include <ranges>

//----------------------------------------------------------------------------------------------------------------------

void Generator::ParseFile(std::filesystem::path file)
{
    // open and parse XML file
    auto absolute = std::filesystem::absolute(file);
    auto& doc = documents[absolute];
    doc.LoadFile(absolute.string().c_str());

    auto* root = doc.FirstChildElement("registry");
    if (!root) {
        throw std::runtime_error("Invalid XML file: missing <registry> root element");
    }

    bool has_modules = false;
    for (auto* module_node = root->FirstChildElement("module"); module_node;
         module_node = module_node->NextSiblingElement("module")) {
        has_modules = true;

        auto* module_attr = module_node->FindAttribute("name");
        auto* version_attr = module_node->FindAttribute("version");
        auto* base_dir_attr = module_node->FindAttribute("base_dir");
        auto* doc_dir_attr = module_node->FindAttribute("doc_dir");
        if (!module_attr || !version_attr || !base_dir_attr || !doc_dir_attr) {
            throw std::runtime_error(
                "Module metadata is missing. Required attributes: name, version, base_dir, doc_dir."
            );
        }

        auto module_name = std::string_view(module_attr->Value());
        auto [it, inserted] = module_map.try_emplace(module_name);
        if (inserted) {
            modules_in_order.emplace_back(module_name);
        }

        auto& module = it->second;
        module.name = module_name;
        module.version = version_attr->Value();
        module.gen_path = base_dir_attr->Value();
        module.doc_path = doc_dir_attr->Value();
        if (auto* backend = module_node->FindAttribute("backend")) {
            module.backend = ParseBackend(backend->Value());
        }
        if (auto* os = module_node->FindAttribute("os")) {
            module.os = GetImplOs(os->Value());
        }

        active_module_name = module_name;
        ParseRegistrySections(module_node);
    }
}

void Generator::WriteModuleAPI()
{
    auto it = module_map.find(active_module_name);
    if (it == module_map.end()) {
        throw std::runtime_error("Module metadata is not available for API output.");
    }

    auto& module = it->second;
    std::filesystem::path cpp_output_path = std::filesystem::path(main_output_dir) / module.gen_path;
    std::filesystem::path cpp_output_path_api = cpp_output_path / "generated";
    WriteCAPI(cpp_output_path_api);
    WriteCPPAPI(cpp_output_path_api);

    WriteConversions(cpp_output_path_api);

    auto independent_path = cpp_output_path / "../wisdom";
    WriteCIndependentAPI(independent_path);
    WriteCPPIndependentAPI(independent_path);
}

void Generator::WriteModuleAPIDoc(std::string_view module_name)
{
    if (module_name.empty()) {
        module_name = active_module_name;
    }
    if (module_name.empty()) {
        throw std::runtime_error("Module name for documentation output is empty.");
    }

    auto it = module_map.find(module_name);
    if (it == module_map.end()) {
        throw std::runtime_error("Module metadata is not available for documentation output.");
    }

    auto previous_module = active_module_name;
    active_module_name = module_name;

    std::filesystem::path module_doc_output_path = std::filesystem::path(doc_output_dir) / it->second.doc_path;
    std::filesystem::path enum_output_path = module_doc_output_path / "enum";
    std::filesystem::path struct_output_path = module_doc_output_path / "struct";
    std::filesystem::path handle_output_path = module_doc_output_path / "handle";
    std::filesystem::path func_output_path = module_doc_output_path / "func";

    WriteEnumDocumentation(enum_output_path);
    WriteBitmaskDocumentation(enum_output_path);
    WriteStructDocumentation(struct_output_path);
    WriteVariantDocumentation(struct_output_path);
    WriteHandleDocumentation(handle_output_path);
    WriteFunctionDocumentation(func_output_path);
    WriteDelegateDocumentation(func_output_path);
    WriteConstantDocumentation(module_doc_output_path);

    active_module_name = previous_module;
}

//----------------------------------------------------------------------------------------------------------------------
void Generator::ParseRegistrySections(tinyxml2::XMLElement* root)
{
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

    if (auto* consts = root->FirstChildElement("constants")) {
        ParseConstants(consts);
    }
}

void Generator::ParseIncludes(tinyxml2::XMLElement* includes)
{
    for (auto* include = includes->FirstChildElement("include"); include;
         include = include->NextSiblingElement("include")) {
        auto file = include->GetText();
        auto rpath = std::filesystem::path(INPUT_FILE).parent_path() / file;
        auto absolute = std::filesystem::absolute(rpath);

        if (std::filesystem::exists(absolute) && !documents.contains(absolute)) {
            auto& doc = documents[absolute];
            doc.LoadFile(absolute.string().c_str());
            auto* include_root = doc.FirstChildElement("registry");
            if (!include_root) {
                throw std::runtime_error("Invalid included XML file: missing <registry> root element");
            }
            if (include_root->FirstChildElement("module")) {
                throw std::runtime_error("Included XML files cannot contain <module> nodes.");
            }
            ParseRegistrySections(include_root);
        }
    }
}

void Generator::ParseTypes(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("type"); type; type = type->NextSiblingElement("type")) {
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

//----------------------------------------------------------------------------------------------------------------------

void Generator::WriteCAPI(std::filesystem::path dir)
{
    auto& module = module_map.at(active_module_name);

    bool has_independent_api = !module.enums_in_order.empty() || !module.bitmasks_in_order.empty()
                            || !module.structs_in_order.empty() || !module.constants_in_order.empty()
                            || !module.delegates_in_order.empty();

    auto path = dir / "c_api.h";
    if (!has_independent_api) {
        return;
    }

    files.push_back(path);
    std::ofstream file{path, std::ios::out | std::ios::trunc};
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path.string());
    }

    auto header_guard = MakeUpperSnakeCase(module.name);

    auto includes = module.name == "Core" ? R"(#include <wisdom/global/definitions.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
)"
                                          : R"(#include <wisdom/generated/c_api.h>
#include "wisdom_exports.h"
)";

    auto api_macro = module.name == "Core" ? "WISDOM_API " : std::format("WISDOM_{}_API ", header_guard);

    // Write header
    // clang-format off
    file << std::format(R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_{0}_C_API_H
#define WISDOM_{0}_C_API_H
{1}

#ifdef __cplusplus
extern "C" {{
#endif // __cplusplus
)", header_guard, includes);
    // clang-format on

    if (!module.enums_in_order.empty() || !module.bitmasks_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Enums\n"
                "//==============================================================\n\n";

        // Write enums
        for (auto& enum_name : module.enums_in_order) {
            auto& enum_def = enum_map[enum_name];
            file << MakeCEnum(enum_def);
            file << "\n";
        }

        // Write bitmasks
        for (auto& bitmask_name : module.bitmasks_in_order) {
            auto& bitmask_def = bitmask_map[bitmask_name];
            file << MakeCBitmask(bitmask_def);
            file << "\n";
        }
    }

    if (!module.delegates_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Delegates\n"
                "//==============================================================\n\n";
        // Write delegates (before structs, as structs may reference delegates)
        for (auto& delegate_name : module.delegates_in_order) {
            auto& delegate_def = delegate_map[delegate_name];
            file << MakeCDelegate(delegate_def);
            file << "\n";
        }
    }

    if (!module.structs_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Structs\n"
                "//==============================================================\n\n";
        // Write structs
        for (auto& struct_name : module.structs_in_order) {
            auto& struct_def = struct_map[struct_name];
            file << MakeCStruct(struct_def);
            file << "\n";
        }
    }

    if (!module.constants_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Constants\n"
                "//==============================================================\n\n";
        // Write constants
        for (auto& const_name : module.constants_in_order) {
            auto& const_def = constant_map[const_name];
            file << MakeCConstant(const_def);
            file << "\n";
        }
    }

    file << R"(
#ifdef WISDOM_DX12
)";

    // Write handles
    for (auto& handle_name : module.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        auto supported = handle_def.GetBackend();
        if (has(supported, Backend::DX12)) {
            file << MakeCHandle(handle_def, Backend::DX12);
            file << "\n";
        }
    }

    // Write variants
    for (auto& variant_name : module.variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        if (has(variant_def.backend, Backend::DX12)) {
            file << MakeCVariant(variant_def, Backend::DX12);
            file << "\n";
        }
    }

    // Write functions
    for (auto& func_name : module.functions_in_order) {
        auto& func_def = function_map[func_name];
        if (has(func_def.backend, Backend::DX12)) {
            file << MakeCFunctionDecl(func_def, Backend::DX12, api_macro);
            file << "\n";
        }
    }

    file << R"(
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
)";

    // Write Vulkan handles
    for (auto& handle_name : module.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        auto supported = handle_def.GetBackend();
        if (has(supported, Backend::Vulkan)) {
            file << MakeCHandle(handle_def, Backend::Vulkan);
            file << "\n";
        }
    }

    // Write Vulkan variants
    for (auto& variant_name : module.variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        if (has(variant_def.backend, Backend::Vulkan)) {
            file << MakeCVariant(variant_def, Backend::Vulkan);
            file << "\n";
        }
    }

    // Write Vulkan functions
    for (auto& func_name : module.functions_in_order) {
        auto& func_def = function_map[func_name];
        if (has(func_def.backend, Backend::Vulkan)) {
            file << MakeCFunctionDecl(func_def, Backend::Vulkan, api_macro);
            file << "\n";
        }
    }

    file << R"(
#endif // WISDOM_VULKAN
)";

    // Write footer
    // clang-format off
    file << std::format(R"(
#ifdef __cplusplus
}}
#endif // __cplusplus
#endif // WISDOM_{}_C_API_H
)", header_guard);
    // clang-format on
}

void Generator::WriteCPPAPI(std::filesystem::path dir)
{
    auto& module = module_map.at(active_module_name);
    bool has_independent_api = !module.enums_in_order.empty() || !module.bitmasks_in_order.empty()
                            || !module.structs_in_order.empty() || !module.constants_in_order.empty()
                            || !module.delegates_in_order.empty();

    auto path = dir / "cpp_api.hpp";
    if (!has_independent_api) {
        return;
    }

    files.push_back(path);
    std::ofstream file{path, std::ios::out | std::ios::trunc};
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path.string());
    }

    auto header_guard = MakeUpperSnakeCase(module.name);
    auto include_root = dir.parent_path().filename().generic_string();

    auto includes = module.name == "Core" ? R"(#include <wisdom/global/definitions.h>
#include <wisdom/bridge/span.hpp>
#include <wisdom/global/internal.hpp>
#include "c_api.h"
)"
                                          : R"(#include <wisdom/generated/cpp_api.hpp>
#include "wisdom_exports.h"
#include "c_api.h"
)";

    // Write header
    // clang-format off
    file << std::format(R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_{0}_CPP_API_HPP
#define WISDOM_{0}_CPP_API_HPP
#ifndef __cplusplus
#error C++ is required to include this header.
#endif // __cplusplus

{1}

namespace wis {{
)", header_guard, includes);
    // clang-format on

    if (!module.enums_in_order.empty() || !module.bitmasks_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Enums\n"
                "//==============================================================\n\n";

        // Write enums
        for (auto& enum_name : module.enums_in_order) {
            auto& enum_def = enum_map[enum_name];
            file << MakeCPPEnum(enum_def);
            file << "\n";
        }

        // Write bitmasks
        for (auto& bitmask_name : module.bitmasks_in_order) {
            auto& bitmask_def = bitmask_map[bitmask_name];
            file << MakeCPPBitmask(bitmask_def);
            file << "\n";
        }
    }

    if (!module.delegates_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Delegates\n"
                "//==============================================================\n\n";
        // Write delegates (before structs, as structs may reference delegates)
        for (auto& delegate_name : module.delegates_in_order) {
            auto& delegate_def = delegate_map[delegate_name];
            file << MakeCPPDelegate(delegate_def);
            file << "\n";
        }
    }

    if (!module.structs_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Structs\n"
                "//==============================================================\n\n";
        // Write structs
        for (auto& struct_name : module.structs_in_order) {
            auto& struct_def = struct_map[struct_name];
            file << MakeCPPStruct(struct_def);
            file << "\n";
        }
    }

    if (!module.constants_in_order.empty()) {
        file << "\n//==============================================================\n"
                "// Constants\n"
                "//==============================================================\n\n";
        // Write constants
        for (auto& const_name : module.constants_in_order) {
            auto& const_def = constant_map[const_name];
            file << MakeCPPConstant(const_def);
            file << "\n";
        }
    }

    file << std::format(
        R"(
}} // namespace wis

#ifdef WISDOM_DX12
#include <{0}/dx12/dx12_types.hpp>

namespace wis {{
)",
        include_root
    );

    // Write Views for handles
    for (auto& handle_name : module.views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(Backend::DX12) > 0) {
            file << MakeCPPView(handle_def, Backend::DX12);
            file << "\n";
        }
    }

    // Write variants
    for (auto& variant_name : module.variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        if (has(variant_def.backend, Backend::DX12)) {
            file << MakeCPPVariant(variant_def, Backend::DX12);
            file << "\n";
        }
    }

    // Write handles
    for (auto& handle_name : module.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        auto supported = handle_def.GetBackend();
        if (has(supported, Backend::DX12)) {
            file << MakeCPPHandle(handle_def, Backend::DX12);
            file << "\n";
        }
    }

    // Write functions
    for (auto& func_name : module.free_functions_in_order) {
        FunctionKey key = MakeFunctionKey("", func_name);
        auto& func_def = function_map[key];
        if (has(func_def.backend, Backend::DX12)) {
            file << MakeCPPFunctionImpl(func_def, Backend::DX12, "inline ");
            file << "\n";
        }
    }

    file << std::format(
        R"(
}} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#include <{0}/vulkan/vk_types.hpp>

namespace wis {{
)",
        include_root
    );

    // Write Views for handles
    for (auto& handle_name : module.views_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetViewSize(Backend::Vulkan) > 0) {
            file << MakeCPPView(handle_def, Backend::Vulkan);
            file << "\n";
        }
    }

    // Write variants
    for (auto& variant_name : module.variants_in_order) {
        auto& variant_def = variant_map[variant_name];
        if (has(variant_def.backend, Backend::Vulkan)) {
            file << MakeCPPVariant(variant_def, Backend::Vulkan);
            file << "\n";
        }
    }

    // Write handles
    for (auto& handle_name : module.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        auto supported = handle_def.GetBackend();
        if (has(supported, Backend::Vulkan)) {
            file << MakeCPPHandle(handle_def, Backend::Vulkan);
            file << "\n";
        }
    }

    // Write functions
    for (auto& func_name : module.free_functions_in_order) {
        FunctionKey key = MakeFunctionKey("", func_name);
        auto& func_def = function_map[key];
        if (has(func_def.backend, Backend::Vulkan)) {
            file << MakeCPPFunctionImpl(func_def, Backend::Vulkan, "inline ");
            file << "\n";
        }
    }

    // Write footer
    // clang-format off
    file << std::format(R"(
}} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_{}_CPP_API_HPP
)", header_guard);
    // clang-format on
}

//----------------------------------------------------------------------------------------------------------------------

void Generator::WriteCIndependentAPI(std::filesystem::path dir)
{
    auto& module = module_map.at(active_module_name);

    auto independent_name = module.name == "Core" ? std::string("wisdom")
                                                  : std::format("wisdom_{}", MakeSnakeCase(module.name));
    auto module_folder = std::filesystem::path(module.gen_path).filename().generic_string();
    if (module_folder.empty()) {
        module_folder = std::filesystem::path(module.gen_path).parent_path().filename().generic_string();
    }

    auto backend_include = module_folder == "wisdom" ? std::string("generated/c_api.h")
                                                     : std::format("../{}/generated/c_api.h", module_folder);
    auto header_guard = std::format("WISDOM_{}_H", MakeUpperSnakeCase(module.name));

    std::filesystem::path path_w = dir / (independent_name + ".h");
    files.push_back(path_w);

    std::ofstream file_w{path_w, std::ios::out | std::ios::trunc};
    if (!file_w.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_w.string());
    }

    // Write header
    file_w << std::format(
        R"(// This file is generated. Do not edit directly.
#ifndef {0}
#define {0}

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifndef FORCEVK_SWITCH
#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "{1}"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
)",
        header_guard,
        backend_include
    );
    constexpr static auto impl_dx = GetBackendSuffix(Backend::DX12);
    constexpr static auto impl_vk = GetBackendSuffix(Backend::Vulkan);

    if (module.name == "Core") {
        file_w << "\n#define WIS_SHADER_INTERMEDIATE_DXIL 1\n";
    }

    bool dx_has_handles = false;
    for (auto& handle_name : module.handles_in_order) {
        if (has(handle_map[handle_name].GetBackend(), Backend::DX12)) {
            dx_has_handles = true;
            break;
        }
    }
    if (!dx_has_handles) {
        for (auto& handle_name : module.views_in_order) {
            if (handle_map[handle_name].GetViewSize(Backend::DX12) > 0) {
                dx_has_handles = true;
                break;
            }
        }
    }

    bool dx_has_variants = false;
    for (auto& variant_name : module.variants_in_order) {
        if (has(variant_map[variant_name].backend, Backend::DX12)) {
            dx_has_variants = true;
            break;
        }
    }

    if (dx_has_handles) {
        file_w << "\n\n//==============================================================\n"
                  "// Handles\n"
                  "//==============================================================\n\n";

        // Write handles
        for (auto& handle_name : module.handles_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (has(handle_def.GetBackend(), Backend::DX12)) {
                file_w << std::format(
                    "typedef struct {} {};\n",
                    GetCFullTypename(handle_def.name, Backend::DX12),
                    GetCFullTypename(handle_def.name)
                );
            }
        }

        // Write Views for handles
        for (auto& handle_name : module.views_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (handle_def.GetViewSize(Backend::DX12) > 0) {
                file_w << std::format(
                    "typedef struct {}View {}View;\n",
                    GetCFullTypename(handle_def.name, Backend::DX12),
                    GetCFullTypename(handle_def.name)
                );
            }
        }
    }

    if (dx_has_variants) {
        file_w << "\n\n//==============================================================\n"
                  "// Variants\n"
                  "//==============================================================\n\n";

        // Write variants
        for (auto& variant_name : module.variants_in_order) {
            auto& variant_def = variant_map[variant_name];
            if (has(variant_def.backend, Backend::DX12)) {
                file_w << std::format(
                    "typedef struct {} {};\n",
                    GetCFullTypename(variant_def.name, Backend::DX12),
                    GetCFullTypename(variant_def.name)
                );
            }
        }
    }

    file_w << "\n\n//==============================================================\n"
              "// Functions\n"
              "//==============================================================\n\n";

    // Write view getters for handles
    for (auto& handle_name : module.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (has(handle_def.GetBackend(), Backend::DX12) && handle_def.GetViewSize(Backend::DX12) > 0) {
            file_w << std::format(
                "#define wisGet{}View wisGet{}{}View\n",
                handle_def.name,
                GetBackendSuffix(Backend::DX12),
                handle_def.name
            );
        }
    }

    // Write functions
    for (auto& func_name : module.functions_in_order) {
        auto& func_def = function_map[func_name];
        if (has(func_def.backend, Backend::DX12)) {
            file_w << std::format(
                "#define {} {}\n",
                GetCFullFunctionName(func_name),
                GetCFullFunctionName(func_name, Backend::DX12)
            );
        }
    }

    file_w << R"(
#elif defined(WISDOM_VULKAN)
)";

    if (module.name == "Core") {
        file_w << "\n#define WIS_SHADER_INTERMEDIATE_SPIRV 1\n";
    }

    bool vk_has_handles = false;
    for (auto& handle_name : module.handles_in_order) {
        if (has(handle_map[handle_name].GetBackend(), Backend::Vulkan)) {
            vk_has_handles = true;
            break;
        }
    }
    if (!vk_has_handles) {
        for (auto& handle_name : module.views_in_order) {
            if (handle_map[handle_name].GetViewSize(Backend::Vulkan) > 0) {
                vk_has_handles = true;
                break;
            }
        }
    }

    bool vk_has_variants = false;
    for (auto& variant_name : module.variants_in_order) {
        if (has(variant_map[variant_name].backend, Backend::Vulkan)) {
            vk_has_variants = true;
            break;
        }
    }

    if (vk_has_handles) {
        file_w << "\n\n//==============================================================\n"
                  "// Handles\n"
                  "//==============================================================\n\n";

        // Write handles
        for (auto& handle_name : module.handles_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (has(handle_def.GetBackend(), Backend::Vulkan)) {
                file_w << std::format(
                    "typedef struct {} {};\n",
                    GetCFullTypename(handle_def.name, Backend::Vulkan),
                    GetCFullTypename(handle_def.name)
                );
            }
        }

        // Write Views for handles
        for (auto& handle_name : module.views_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (handle_def.GetViewSize(Backend::Vulkan) > 0) {
                file_w << std::format(
                    "typedef struct {}View {}View;\n",
                    GetCFullTypename(handle_def.name, Backend::Vulkan),
                    GetCFullTypename(handle_def.name)
                );
            }
        }
    }

    if (vk_has_variants) {
        file_w << "\n\n//==============================================================\n"
                  "// Variants\n"
                  "//==============================================================\n\n";

        // Write variants
        for (auto& variant_name : module.variants_in_order) {
            auto& variant_def = variant_map[variant_name];
            if (has(variant_def.backend, Backend::Vulkan)) {
                file_w << std::format(
                    "typedef struct {} {};\n",
                    GetCFullTypename(variant_def.name, Backend::Vulkan),
                    GetCFullTypename(variant_def.name)
                );
            }
        }
    }

    file_w << "\n\n//==============================================================\n"
              "// Functions\n"
              "//==============================================================\n\n";

    // Write view getters for handles
    for (auto& handle_name : module.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (has(handle_def.GetBackend(), Backend::Vulkan) && handle_def.GetViewSize(Backend::Vulkan) > 0) {
            file_w << std::format(
                "#define wisGet{}View wisGet{}{}View\n",
                handle_def.name,
                GetBackendSuffix(Backend::Vulkan),
                handle_def.name
            );
        }
    }

    // Write functions
    for (auto& func_name : module.functions_in_order) {
        auto& func_def = function_map[func_name];
        if (has(func_def.backend, Backend::Vulkan)) {
            file_w << std::format(
                "#define {} {}\n",
                GetCFullFunctionName(func_name),
                GetCFullFunctionName(func_name, Backend::Vulkan)
            );
        }
    }

    file_w << R"(
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection

#ifndef WISDOM_HANDLE_VALID_DEFINED
#define WISDOM_HANDLE_VALID_DEFINED
static inline bool wisHandleValid(const void* handle) {
    const uint64_t zero = 0;
    return memcmp(handle, &zero, sizeof(uint64_t)) != 0;
}

#endif // WISDOM_HANDLE_VALID_DEFINED

)";
    file_w << std::format("#endif // {}\n", header_guard);
}

//----------------------------------------------------------------------------------------------------------------------
void Generator::WriteCPPIndependentAPI(std::filesystem::path dir)
{
    auto& module = module_map.at(active_module_name);

    auto independent_name = module.name == "Core" ? std::string("wisdom")
                                                  : std::format("wisdom_{}", MakeSnakeCase(module.name));
    auto module_folder = std::filesystem::path(module.gen_path).filename().generic_string();
    if (module_folder.empty()) {
        module_folder = std::filesystem::path(module.gen_path).parent_path().filename().generic_string();
    }

    auto backend_include = module_folder == "wisdom" ? std::string("generated/cpp_api.hpp")
                                                     : std::format("../{}/generated/cpp_api.hpp", module_folder);
    auto header_guard = std::format("WISDOM_{}_HPP", MakeUpperSnakeCase(module.name));

    std::filesystem::path path_w = dir / (independent_name + ".hpp");
    files.push_back(path_w);

    std::ofstream file_w{path_w, std::ios::out | std::ios::trunc};
    if (!file_w.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_w.string());
    }

    // Write header
    file_w << std::format(
        R"(// This file is generated. Do not edit directly.
#ifndef {0}
#define {0}

#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#ifndef FORCEVK_SWITCH
#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "{1}"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

namespace wis {{
)",
        header_guard,
        backend_include
    );

    if (module.name == "Core") {
        file_w << "static constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::DXIL;\n";
    }

    bool dx_has_handles = false;
    for (auto& handle_name : module.handles_in_order) {
        if (has(handle_map[handle_name].GetBackend(), Backend::DX12)) {
            dx_has_handles = true;
            break;
        }
    }
    if (!dx_has_handles) {
        for (auto& handle_name : module.views_in_order) {
            if (handle_map[handle_name].GetViewSize(Backend::DX12) > 0) {
                dx_has_handles = true;
                break;
            }
        }
    }

    bool dx_has_variants = false;
    for (auto& variant_name : module.variants_in_order) {
        if (has(variant_map[variant_name].backend, Backend::DX12)) {
            dx_has_variants = true;
            break;
        }
    }

    bool dx_has_functions = false;
    for (auto& func_name : module.free_functions_in_order) {
        FunctionKey key = MakeFunctionKey("", func_name);
        if (has(function_map[key].backend, Backend::DX12)) {
            dx_has_functions = true;
            break;
        }
    }

    if (dx_has_handles) {
        file_w << "\n\n//==============================================================\n"
                  "// Handles\n"
                  "//==============================================================\n\n";

        // Write handles
        for (auto& handle_name : module.handles_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (has(handle_def.GetBackend(), Backend::DX12)) {
                file_w << std::format(
                    "using {} = {};\n",
                    handle_def.name,
                    GetCPPFullTypename(handle_def.name, Backend::DX12)
                );
            }
        }

        // Write Views for handles
        for (auto& handle_name : module.views_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (handle_def.GetViewSize(Backend::DX12) > 0) {
                file_w << std::format(
                    "using {}View = {};\n",
                    handle_def.name,
                    GetCPPFullTypename(handle_def.name, Backend::DX12) + "View"
                );
            }
        }
    }

    if (dx_has_variants) {
        file_w << "\n\n//==============================================================\n"
                  "// Variants\n"
                  "//==============================================================\n\n";

        // Write variants
        for (auto& variant_name : module.variants_in_order) {
            auto& variant_def = variant_map[variant_name];
            if (has(variant_def.backend, Backend::DX12)) {
                file_w << std::format(
                    "using {} = {};\n",
                    variant_def.name,
                    GetCPPFullTypename(variant_def.name, Backend::DX12)
                );
            }
        }
    }

    if (dx_has_functions) {
        file_w << "\n\n//==============================================================\n"
                  "// Functions\n"
                  "//==============================================================\n\n";

        // Write functions
        for (auto& func_name : module.free_functions_in_order) {
            FunctionKey key = MakeFunctionKey("", func_name);
            auto& func_def = function_map[key];
            if (has(func_def.backend, Backend::DX12)) {
                file_w << MakeCPPFunctionImpl(func_def, Backend::DX12, "inline ", DocKind::Full, ProtoType::Universal);
                file_w << '\n';
            }
        }
    }

    file_w << R"(
} // namespace wis

#elif defined(WISDOM_VULKAN)

namespace wis {
)";

    if (module.name == "Core") {
        file_w << "static constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::SPIRV;\n";
    }

    bool vk_has_handles = false;
    for (auto& handle_name : module.handles_in_order) {
        if (has(handle_map[handle_name].GetBackend(), Backend::Vulkan)) {
            vk_has_handles = true;
            break;
        }
    }
    if (!vk_has_handles) {
        for (auto& handle_name : module.views_in_order) {
            if (handle_map[handle_name].GetViewSize(Backend::Vulkan) > 0) {
                vk_has_handles = true;
                break;
            }
        }
    }

    bool vk_has_variants = false;
    for (auto& variant_name : module.variants_in_order) {
        if (has(variant_map[variant_name].backend, Backend::Vulkan)) {
            vk_has_variants = true;
            break;
        }
    }

    bool vk_has_functions = false;
    for (auto& func_name : module.free_functions_in_order) {
        FunctionKey key = MakeFunctionKey("", func_name);
        if (has(function_map[key].backend, Backend::Vulkan)) {
            vk_has_functions = true;
            break;
        }
    }

    if (vk_has_handles) {
        file_w << "\n\n//==============================================================\n"
                  "// Handles\n"
                  "//==============================================================\n\n";

        // Write handles
        for (auto& handle_name : module.handles_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (has(handle_def.GetBackend(), Backend::Vulkan)) {
                file_w << std::format(
                    "using {} = {};\n",
                    handle_def.name,
                    GetCPPFullTypename(handle_def.name, Backend::Vulkan)
                );
            }
        }

        // Write Views for handles
        for (auto& handle_name : module.views_in_order) {
            auto& handle_def = handle_map[handle_name];
            if (handle_def.GetViewSize(Backend::Vulkan) > 0) {
                file_w << std::format(
                    "using {}View = {};\n",
                    handle_def.name,
                    GetCPPFullTypename(handle_def.name, Backend::Vulkan) + "View"
                );
            }
        }
    }

    if (vk_has_variants) {
        file_w << "\n\n//==============================================================\n"
                  "// Variants\n"
                  "//==============================================================\n\n";

        // Write variants
        for (auto& variant_name : module.variants_in_order) {
            auto& variant_def = variant_map[variant_name];
            if (has(variant_def.backend, Backend::Vulkan)) {
                file_w << std::format(
                    "using {} = {};\n",
                    variant_def.name,
                    GetCPPFullTypename(variant_def.name, Backend::Vulkan)
                );
            }
        }
    }

    if (vk_has_functions) {
        file_w << "\n\n//==============================================================\n"
                  "// Functions\n"
                  "//==============================================================\n\n";

        // Write functions
        for (auto& func_name : module.free_functions_in_order) {
            FunctionKey key = MakeFunctionKey("", func_name);
            auto& func_def = function_map[key];
            if (has(func_def.backend, Backend::Vulkan)) {
                file_w
                    << MakeCPPFunctionImpl(func_def, Backend::Vulkan, "inline ", DocKind::Full, ProtoType::Universal);
                file_w << '\n';
            }
        }
    }

    file_w << R"(
} // namespace wis
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
)";
    file_w << std::format("#endif // {}\n", header_guard);
}

void Generator::WriteConversions(std::filesystem::path dir)
{
    auto& module = module_map.at(active_module_name);
    bool has_conversions = !module.enums_in_order.empty() || !module.bitmasks_in_order.empty();
    if (!has_conversions) {
        return;
    }

    std::filesystem::path path_dx = dir / "dx12_convert.hpp";
    std::filesystem::path path_vk = dir / "vk_convert.hpp";
    files.push_back(path_dx);
    files.push_back(path_vk);

    std::ofstream file_dx{path_dx, std::ios::out | std::ios::trunc};
    if (!file_dx.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_dx.string());
    }
    std::ofstream file_vk{path_vk, std::ios::out | std::ios::trunc};
    if (!file_vk.is_open()) {
        throw std::runtime_error("Failed to open output file: " + path_vk.string());
    }

    auto header_guard = MakeUpperSnakeCase(module.name);

    // Write header
    file_dx << std::format(
        R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_{0}_CPP_DX12_CONVERT_HPP
#define WISDOM_{0}_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include "c_api.h"
#include <dxgi1_6.h>
#include <D3D12MemAlloc.h>

namespace wis{{ namespace detail {{
)",
        header_guard
    );
    file_vk << std::format(
        R"(// This file is generated. Do not edit directly.
#ifndef WISDOM_{0}_CPP_VK_CONVERT_HPP
#define WISDOM_{0}_CPP_VK_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include "c_api.h"
#include <vulkan/vulkan.h>

namespace wis{{ namespace detail {{
)",
        header_guard
    );

    // Write enums
    for (auto& enum_name : module.enums_in_order) {
        auto& enum_def = enum_map[enum_name];
        file_dx << MakeEnumConverter(enum_def, Backend::DX12);
        file_dx << "\n";
        file_vk << MakeEnumConverter(enum_def, Backend::Vulkan);
        file_vk << "\n";
    }
    // Write bitmasks
    for (auto& bitmask_name : module.bitmasks_in_order) {
        auto& bitmask_def = bitmask_map[bitmask_name];
        file_dx << MakeBitmaskConverter(bitmask_def, Backend::DX12);
        file_dx << "\n";
        file_vk << MakeBitmaskConverter(bitmask_def, Backend::Vulkan);
        file_vk << "\n";
    }

    // Write footer
    file_dx << std::format(
        R"(
}}}}
#endif // WISDOM_{}_CPP_DX12_CONVERT_HPP
)",
        header_guard
    );
    file_vk << std::format(
        R"(
}}}}
#endif // WISDOM_{}_CPP_VK_CONVERT_HPP
)",
        header_guard
    );
}

void Generator::WriteDocumentation(
    std::filesystem::path doc_output_path,
    std::string_view doc_template,
    std::string_view object_name,
    std::string_view code,
    std::string_view vuids,
    std::string_view desc,
    std::string_view refs
)
{
    if (refs.empty()) {
        refs = " * ";
    }

    // If file exists, only edit the generated code section, else create new file
    bool file_exists = std::filesystem::exists(doc_output_path);
    std::fstream enum_file{doc_output_path, file_exists ? std::ios::in | std::ios::out : std::ios::out};

    if (!file_exists) {
        std::string xenum = std::vformat(
            doc_template,
            std::make_format_args(object_name, code, desc, active_module_name)
        );

        enum_file << FinalizeCDocumentation(xenum, object_name);
        enum_file.close();
        return;
    }

    // Otherwise, we would need to parse the existing file and replace the generated section
    // Read entire file content
    std::string existing_content((std::istreambuf_iterator<char>(enum_file)), std::istreambuf_iterator<char>());
    enum_file.close();
    // Find the generated section
    size_t gen_start = existing_content.find(R"(\cond WIS_GEN_CODE)");
    size_t gen_end = existing_content.find(R"(\endcond)", gen_start);

    // Find the vuids section
    size_t vuid_start = existing_content.find(R"(\cond WIS_GEN_WIS_IDS)");
    size_t vuid_end = existing_content.find(R"(\endcond)", vuid_start);

    // Find the description section
    size_t desc_start = existing_content.find(R"(\cond WIS_GEN_DESC)");
    size_t desc_end = existing_content.find(R"(\endcond)", desc_start);

    // Find the references section
    size_t ref_start = existing_content.find(R"(\cond WIS_GEN_REFS)");
    size_t ref_end = existing_content.find(R"(\endcond)", ref_start);

    // Replace the references section
    if (ref_start != std::string::npos && ref_end != std::string::npos && ref_end > ref_start) {
        existing_content = existing_content.substr(0, ref_start) + "\\cond WIS_GEN_REFS\n" + std::string(refs)
                         + existing_content.substr(ref_end);
    }
    // Replace the vuids section
    if (vuid_start != std::string::npos && vuid_end != std::string::npos && vuid_end > vuid_start) {
        existing_content = existing_content.substr(0, vuid_start) + "\\cond WIS_GEN_WIS_IDS\n" + std::string(vuids)
                         + existing_content.substr(vuid_end);
    }
    // Replace the description section
    if (desc_start != std::string::npos && desc_end != std::string::npos && desc_end > desc_start) {
        existing_content = existing_content.substr(0, desc_start) + "\\cond WIS_GEN_DESC\n" + std::string(desc)
                         + existing_content.substr(desc_end);
    }
    // Replace the generated section
    if (gen_start != std::string::npos && gen_end != std::string::npos && gen_end > gen_start) {
        existing_content = existing_content.substr(0, gen_start) + "\\cond WIS_GEN_CODE\n" + std::string(code)
                         + existing_content.substr(gen_end);
    }

    // Write back to file
    std::ofstream enum_file_out{doc_output_path, std::ios::trunc};
    enum_file_out << existing_content;
    enum_file_out.close();
}

// Helpers
TypeKind Generator::GetType(std::string_view type_name) const noexcept
{
    if (type_name.empty()) {
        return TypeKind::None;
    }
    if (standard_types.contains(type_name)) {
        return TypeKind::Base;
    }
    if (auto it = type_map.find(type_name); it != type_map.end()) {
        return it->second;
    }

    // Special case - views
    if (type_name.ends_with("View")) {
        auto base_type = type_name.substr(0, type_name.size() - 4);
        if (handle_map.contains(base_type)) {
            return TypeKind::View;
        }
    }

    return TypeKind::None;
}

Backend Generator::GetTypeBackendSupport(std::string_view type_name) const noexcept
{
    auto xtype = GetType(type_name);
    switch (xtype) {
    case TypeKind::Variant:
        return variant_map.at(type_name).backend;
    case TypeKind::Handle:
        return handle_map.at(type_name).GetBackend();
    case TypeKind::View:
        return handle_map.at(type_name.substr(0, type_name.size() - 4)).GetBackend();
    default:
        return Backend::All;
    }
}

void Generator::TryMakeRef(std::string_view type, std::string_view ref)
{
    // ref is reverse, meaning ref uses type
    auto xtype = GetType(ref);
    switch (xtype) {
    case TypeKind::Struct:
    case TypeKind::Variant:
        dependency_tree[type].structs.push_back(ref);
        return;
    case TypeKind::Handle:
        dependency_tree[type].handles.push_back(ref);
        return;

    default:
        return;
    }
}

void Generator::TryMakeRef(std::string_view type, FunctionKey ref) { dependency_tree[type].functions.push_back(ref); }

std::string Generator::GetCFullTypename(std::string_view type, Backend backend)
{
    auto suffix = GetBackendSuffix(backend);
    switch (GetType(type)) {
    case TypeKind::Base:
        return std::string(standard_types.at(type));
    default:
    case TypeKind::None:
        return "";
    case TypeKind::Enum:
    case TypeKind::Bitmask:
    case TypeKind::FuncPointer:
    case TypeKind::Struct:
        return std::format("Wis{}", type);
    case TypeKind::Handle:
    case TypeKind::View:
    case TypeKind::Function:
    case TypeKind::Variant:
        return std::format("Wis{}{}", suffix, type);
    }
    return "";
}
std::string Generator::GetCFullFunctionName(FunctionKey type, Backend backend)
{
    auto& func_def = function_map[type];
    if (func_def.IsCD()) {
        return std::format("wis{}{}", GetBackendSuffix(backend), func_def.name);
    } else {
        return std::format("wis{}{}{}", GetBackendSuffix(backend), func_def.this_type, func_def.name);
    }
}
std::string Generator::GetCPPFullTypename(std::string_view type, Backend backend)
{
    auto suffix = GetBackendSuffix(backend);
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
        return std::format("wis::{}", type);
    case TypeKind::Variant:
    case TypeKind::Handle:
    case TypeKind::View:
    case TypeKind::Function:
        return std::format("wis::{}{}", suffix, type);
    case TypeKind::Union:
        break;
    case TypeKind::Alias:
        break;
    }
    return "";
}
std::string Generator::GetCPPFullFunctionName(FunctionKey type, Backend backend)
{
    auto& func_def = function_map[type];
    if (func_def.IsCD()) {
        return std::format("wis::{}{}", GetBackendSuffix(backend), func_def.name);
    } else {
        return std::format("wis::{}{}::{}", GetBackendSuffix(backend), func_def.this_type, func_def.name);
    }
}
std::string Generator::FinalizeCDocumentation(std::string doc, std::string_view this_type, Backend backend)
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

        switch (GetType(this_type_view)) {
        case TypeKind::Base:
            replacement = GetCFullTypename(this_type_view, backend);
            break;
        case TypeKind::None:
            break;
        case TypeKind::Enum: {
            auto& x = enum_map.at(this_type_view);
            auto evalue = x.HasValue(value);
            replacement = evalue ? std::format("`{}{}`", GetCFullTypename(x.name, backend), evalue->name)
                                 : GetCFullTypename(x.name, backend);
            break;
        }
        case TypeKind::Bitmask: {
            auto& b = bitmask_map.at(this_type_view);
            auto evalue = b.HasValue(value);
            replacement = evalue ? std::format("`{}{}`", GetCFullTypename(b.name, backend), evalue->name)
                                 : GetCFullTypename(b.name, backend);
            break;
        }
        case TypeKind::Struct: {
            auto& s = struct_map.at(this_type_view);
            auto member = s.HasValue(value);
            replacement = member ? std::format("`{}::{}`", GetCFullTypename(s.name, backend), member->name)
                                 : GetCFullTypename(s.name, backend);
            break;
        }
        case TypeKind::Variant: {
            auto& v = variant_map.at(this_type_view);
            auto m = v.HasValue(value);
            replacement = m ? std::format("`{}::{}`", GetCFullTypename(v.name, backend), m->name)
                            : GetCFullTypename(v.name, backend);
            break;
        }
        case TypeKind::FuncPointer: {
            auto& d = delegate_map.at(this_type_view);
            auto m = d.HasValue(value);
            replacement = m ? std::format("`{}::{}`", GetCFullTypename(d.name, backend), m->name)
                            : GetCFullTypename(d.name, backend);
            break;
        }
        case TypeKind::Handle: {
            auto& h = handle_map.at(this_type_view);
            if (value.empty()) {
                replacement = GetCFullTypename(h.name, backend);
            } else {
                // member function
                FunctionKey key = MakeFunctionKey(type, value);
                auto f = function_map.find(key);
                if (f != function_map.end()) {
                    replacement = GetCFullFunctionName(key);
                }
            }
            break;
        }
        case TypeKind::View: {
            replacement = GetCFullTypename(this_type_view, backend);
        }
        }
        doc.replace(first, last - first + 1, replacement);
    }

    // replave should, must, may with @wis_should, @wis_must, @wis_may
    // skip replacing in brief documentation
    size_t start_replacing = 0;
    if (doc.find("///<") != std::string::npos || doc.find("/// @brief") != std::string::npos) {
        start_replacing = doc.length();
    } else {
        size_t brief_start = doc.find("@brief");
        if (brief_start != std::string::npos) {
            size_t brief_end = doc.find("\n * \n * ", brief_start);
            if (brief_end == std::string::npos) {
                brief_end = doc.find("\n * \n", brief_start);
            }
            if (brief_end != std::string::npos) {
                start_replacing = brief_end;
            } else {
                start_replacing = doc.length();
            }
        }
    }

    if (start_replacing < doc.length()) {
        std::string big_doc = doc.substr(start_replacing);
        ReplaceAll(big_doc, " should not", " @wis_shouldnot ");
        ReplaceAll(big_doc, " must not", " @wis_mustnot");
        ReplaceAll(big_doc, " may not", " @wis_maynot");

        ReplaceAll(big_doc, " should ", " @wis_should ");
        ReplaceAll(big_doc, " must ", " @wis_must ");
        ReplaceAll(big_doc, " may ", " @wis_may ");

        doc.replace(start_replacing, std::string::npos, big_doc);
    }
    return doc;
}
std::string Generator::FinalizeCPPDocumentation(std::string doc, std::string_view this_type, Backend backend)
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

        switch (GetType(this_type_view)) {
        case TypeKind::Base:
            replacement = GetCFullTypename(this_type_view, backend);
            break;
        case TypeKind::None:
            break;
        case TypeKind::Enum: {
            auto& x = enum_map.at(this_type_view);
            auto evalue = x.HasValue(value);
            replacement = evalue ? std::format("`{}::{}`", GetCPPFullTypename(x.name, backend), evalue->name)
                                 : GetCPPFullTypename(x.name, backend);
            break;
        }
        case TypeKind::Bitmask: {
            auto& b = bitmask_map.at(this_type_view);
            auto evalue = b.HasValue(value);
            replacement = evalue ? std::format("`{}::{}`", GetCPPFullTypename(b.name, backend), evalue->name)
                                 : GetCPPFullTypename(b.name, backend);
            break;
        }
        case TypeKind::Struct: {
            auto& s = struct_map.at(this_type_view);
            auto member = s.HasValue(value);
            replacement = member ? std::format("`{}::{}`", GetCPPFullTypename(s.name, backend), member->name)
                                 : GetCPPFullTypename(s.name, backend);
            break;
        }
        case TypeKind::Variant: {
            auto& v = variant_map.at(this_type_view);
            auto m = v.HasValue(value);
            replacement = m ? std::format("`{}::{}`", GetCPPFullTypename(v.name, backend), m->name)
                            : GetCPPFullTypename(v.name, backend);
            break;
        }
        case TypeKind::FuncPointer: {
            auto& d = delegate_map.at(this_type_view);
            auto m = d.HasValue(value);
            replacement = m ? std::format("`{}::{}`", GetCPPFullTypename(d.name, backend), m->name)
                            : GetCPPFullTypename(d.name, backend);
            break;
        }
        case TypeKind::Handle: {
            auto& h = handle_map.at(this_type_view);
            if (value.empty()) {
                replacement = GetCPPFullTypename(h.name, backend);
            } else {
                // member function
                FunctionKey key = MakeFunctionKey(type, value);
                auto f = function_map.find(key);
                if (f != function_map.end()) {
                    replacement = GetCPPFullFunctionName(key);
                }
            }
            break;
        }
        case TypeKind::View: {
            replacement = GetCPPFullTypename(this_type_view, backend);
        }
        }
        doc.replace(first, last - first + 1, replacement);
    }

    // replave should, must, may with @wis_should, @wis_must, @wis_may
    // skip replacing in brief documentation
    size_t start_replacing = 0;
    if (doc.find("///<") != std::string::npos || doc.find("/// @brief") != std::string::npos) {
        start_replacing = doc.length();
    } else {
        size_t brief_start = doc.find("@brief");
        if (brief_start != std::string::npos) {
            size_t brief_end = doc.find("\n * \n * ", brief_start);
            if (brief_end == std::string::npos) {
                brief_end = doc.find("\n * \n", brief_start);
            }
            if (brief_end != std::string::npos) {
                start_replacing = brief_end;
            } else {
                start_replacing = doc.length();
            }
        }
    }

    if (start_replacing < doc.length()) {
        std::string big_doc = doc.substr(start_replacing);
        ReplaceAll(big_doc, " should not", " @wis_shouldnot ");
        ReplaceAll(big_doc, " must not", " @wis_mustnot");
        ReplaceAll(big_doc, " may not", " @wis_maynot");

        ReplaceAll(big_doc, " should ", " @wis_should ");
        ReplaceAll(big_doc, " must ", " @wis_must ");
        ReplaceAll(big_doc, " may ", " @wis_may ");

        doc.replace(start_replacing, std::string::npos, big_doc);
    }
    return doc;
}

std::string Generator::GetSpecificationCode(
    std::string_view c_code,
    std::string_view c_impl_code,
    std::string_view cpp_code,
    std::string_view cpp_impl_code
)
{
    std::string template_content_c;
    if (!c_code.empty()) {
        template_content_c = std::format(" C Version:\n```c\n{}```\n", c_code);
        if (!c_impl_code.empty()) {
            // append a details section
            template_content_c += std::format(
                "<details>\n<summary>C Implementation Specific Version:</summary>\n```c\n{}```\n</details>\n",
                c_impl_code
            );
        }
    }

    std::string template_content_cpp;
    if (!cpp_code.empty()) {
        template_content_cpp = std::format("C++ Version:\n```cpp\nnamespace wis{{\n{}}}\n```\n", cpp_code);
        if (!cpp_impl_code.empty()) {
            // append a details section
            template_content_cpp += std::format(
                "<details>\n<summary>C++ Implementation Specific Version:</summary>\n```cpp\nnamespace "
                "wis{{\n{}}}\n```\n</details>\n",
                cpp_impl_code
            );
        }
    }

    std::string output = std::format(" * {}\n{}", template_content_c, template_content_cpp);
    ReplaceAll(output, "\n", "\n * ");
    return output;
}

Backend Generator::ParseBackend(std::string_view backend) noexcept
{
    if (backend == "dx" || backend == "DX12") {
        return Backend::DX12;
    }
    if (backend == "vk" || backend == "VK") {
        return Backend::Vulkan;
    }
    return Backend::Any;
}

ImplOs Generator::GetImplOs(std::string_view os) noexcept
{
    // tokenize by comma
    for (auto&& tk : std::views::split(os, std::string_view{","})) {
        // trim
        std::string_view tk_view{tk.begin(), tk.end()};
        if (tk_view == "windows") {
            return ImplOs::Windows;
        }
        if (tk_view == "linux") {
            return ImplOs::Linux;
        }
    }
    return ImplOs::None;
}

Extends Generator::GetExtends(std::string_view extends_str) noexcept
{
    if (extends_str.empty()) {
        return Extends::None;
    }
    if (extends_str == "Instance") {
        return Extends::Instance;
    }
    if (extends_str == "Device") {
        return Extends::Device;
    }
    return Extends::None;
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
    auto end = str.rfind('}');
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
        type = caption.substr(0, colon);
    }

    return {type, value, pos, end};
}

std::string Generator::MakeVersionString(std::string_view version, bool newline)
{
    return version.empty() ? "" : std::format("Provided by Wisdom {}.{}", version, newline ? "\n" : " ");
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
    for (auto&& tk : std::views::split(mod_str, std::string_view{","})) {
        // trim
        std::string_view tk_view{tk.begin(), tk.end()};
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

template <typename Kty>
void Deduplicate(std::vector<Kty>& dependency)
{
    // deduplicate handles
    std::unordered_set<Kty> seen;
    // Use the erase-remove idiom with a custom predicate
    auto new_end = std::remove_if(dependency.begin(), dependency.end(), [&](auto sv) {
        // If insert fails, the element is a duplicate
        return !seen.insert(sv).second;
    });

    dependency.erase(new_end, dependency.end());
}

std::string Generator::GetRefs(std::string_view for_type)
{
    auto it = dependency_tree.find(for_type);
    if (it == dependency_tree.end()) {
        return "";
    }
    auto& xrefs = it->second;

    std::size_t ref_count = 0;
    static constexpr std::size_t max_ref_count = 10;

    auto type = GetType(for_type);

    // Gather references
    std::string refs;

    // First go handles
    if (!xrefs.handles.empty()) {
        refs += "Handles:\n";
    }
    Deduplicate(xrefs.handles);

    for (auto& h : xrefs.handles) {
        refs += GetCFullTypename(h, Backend::Any);
        refs += ", ";
    }
    if (!xrefs.handles.empty()) {
        refs.pop_back(); // remove last space
        refs.pop_back(); // remove last comma
    }

    // Then go structs
    if (!xrefs.structs.empty()) {
        if (!refs.empty()) {
            refs += "\n";
        }
        refs += "Structs:\n";
    }
    Deduplicate(xrefs.structs);
    for (auto& s : xrefs.structs) {
        refs += GetCFullTypename(s, Backend::Any);
        refs += ", ";
    }
    if (!xrefs.structs.empty()) {
        refs.pop_back(); // remove last space
        refs.pop_back(); // remove last comma
    }

    // last go functions
    if (!xrefs.functions.empty()) {
        if (!refs.empty()) {
            refs += "\n";
        }
        refs += "Functions:\n";
    }
    Deduplicate(xrefs.functions);
    for (auto& f : xrefs.functions) {
        refs += GetCFullFunctionName(f);
        refs += ", ";
    }
    if (!xrefs.functions.empty()) {
        refs.pop_back(); // remove last space
        refs.pop_back(); // remove last comma
    }

    if (!refs.empty()) {
        refs = std::format(" * @see {}\n", refs);
    }
    return refs;
}

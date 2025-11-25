#include "generator.hpp"
#include <fstream>

//-----------------------------------------------------------------------------
#define CHECKED_CALL(call)                                           \
    if (auto err = (call); err != tinyxml2::XMLError::XML_SUCCESS) { \
        return err;                                                  \
    }

//-----------------------------------------------------------------------------

tinyxml2::XMLError Generator::ParseFile(std::filesystem::path file)
{
    // open and parse XML file
    auto absolute = std::filesystem::absolute(file);
    auto& doc = documents[absolute];
    CHECKED_CALL(doc.LoadFile(absolute.string().c_str()));
    return ParseFile(doc);
}

void Generator::WriteMainAPI()
{
    std::filesystem::path cpp_output_path = main_output_dir;
    std::filesystem::path cpp_output_path_api = cpp_output_path / "generated/api";
    WriteCAPI(cpp_output_path_api / "api.h");
}

void Generator::WriteMainAPIDoc()
{
    std::filesystem::path doc_output_path = doc_output_dir;
    std::filesystem::path enum_output_path = doc_output_path / "wisdom/enum";

    MakeEnumDocumentation(enum_output_path);
}

//-----------------------------------------------------------------------------
tinyxml2::XMLError Generator::ParseFile(tinyxml2::XMLDocument& doc)
{
    auto* root = doc.FirstChildElement("registry");
    if (!root) {
        return tinyxml2::XMLError::XML_ERROR_PARSING_ELEMENT;
    }

    if (auto* include = root->FirstChildElement("includes")) {
        CHECKED_CALL(ParseIncludes(include));
    }

    if (auto* handles = root->FirstChildElement("handles")) {
        // ParseHandles(handles);
    }

    if (auto* types = root->FirstChildElement("types")) {
        ParseTypes(types);
    }

    if (auto* funcs = root->FirstChildElement("functions")) {
        // ParseFunctions(funcs);
    }

    if (auto* exts = root->FirstChildElement("extensions")) {
        // ParseExtensions(exts);
    }
    return tinyxml2::XMLError::XML_SUCCESS;
}

tinyxml2::XMLError Generator::ParseIncludes(tinyxml2::XMLElement* includes)
{
    for (auto* include = includes->FirstChildElement("include"); include;
         include = include->NextSiblingElement("include")) {
        auto file = include->GetText();
        auto rpath = std::filesystem::path(INPUT_FILE).parent_path() / file;
        auto absolute = std::filesystem::absolute(rpath);

        if (std::filesystem::exists(absolute) && !documents.contains(absolute)) {
            auto& doc = documents[absolute];
            CHECKED_CALL(doc.LoadFile(absolute.string().c_str()));
            CHECKED_CALL(ParseFile(doc));
        }
    }
    return tinyxml2::XMLError::XML_SUCCESS;
}

tinyxml2::XMLError Generator::ParseTypes(tinyxml2::XMLElement* types, std::string_view extension)
{
    for (auto* type = types->FirstChildElement("type"); type;
         type = type->NextSiblingElement("type")) {
        auto category = type->FindAttribute("category")->Value();
        if (std::string_view(category) == "struct") {
            // ParseStruct(*type);
        } else if (std::string_view(category) == "enum") {
            CHECKED_CALL(ParseEnum(type));
        } else if (std::string_view(category) == "bitmask") {
            // ParseBitmask(*type);
        } else if (std::string_view(category) == "delegate") {
            // ParseDelegate(type);
        } else if (std::string_view(category) == "variant") {
            // ParseVariant(*type, extension);
        }
    }
    return tinyxml2::XMLError::XML_SUCCESS;
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
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
)";

    file << "\n//==============================================================\n"
            "// Enums\n"
            "//==============================================================\n\n";

    // Write typedefs
    for (auto& [enum_name, enum_def] : enum_map) {
        auto full_name = GetCFullTypename(enum_def.name, "");
        file << wis::format("typedef enum {} {};\n", full_name, full_name);
    }

    // Write enums
    for (auto& enum_name : enums_in_order) {
        auto& enum_def = enum_map[enum_name];
        file << MakeCEnum(enum_def);
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

// Helpers
inline std::string Generator::GetCFullTypename(std::string_view type, std::string_view impl)
{
    if (type.empty()) {
        return "";
    }

    if (auto it = standard_types.find(type); it != standard_types.end()) {
        return std::string(it->second);
    }

    if (auto it = enum_map.find(type); it != enum_map.end()) {
        return "Wis" + std::string(type);
    }

    // if (auto it = bitmask_map.find(type); it != bitmask_map.end()) {
    //     return "Wis" + std::string(type);
    // }

    // if (auto it = variant_map.find(type); it != variant_map.end()) {
    //     return std::string(impl) + std::string(type);
    // }

    // if (auto it = struct_map.find(type); it != struct_map.end()) {
    //     return "Wis" + std::string(type);
    // }

    // if (auto it = handle_map.find(type); it != handle_map.end()) {
    //     return std::string(impl) + std::string(type);
    // }
    // if (auto it = extension_map.find(type); it != extension_map.end()) {
    //     return std::string(impl) + std::string(type);
    // }

    // if (auto it = delegate_map.find(type); it != delegate_map.end()) {
    //     return std::string(type);
    // }

    // if (auto it = function_map.find(std::string(type)); it != function_map.end()) {
    //     return std::string(impl) + std::string(type);
    // }

    return "";
}
std::string Generator::FinalizeCDocumentation(std::string doc, std::string_view this_type, std::string_view impl)
{
    if (doc.empty()) {
        return doc;
    }

    size_t pos = 0;
    std::string_view this_type_view = this_type;

    while (true) {
        auto&& [type, value, first, last] = FindInlineType(doc, pos);
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
            replacement = evalue ? wis::format("Wis{}{}", x->second.name, evalue->name)
                                 : GetCFullTypename(x->second.name, impl);

        } /* else if (auto y = bitmask_map.find(this_type_view); y != bitmask_map.end()) {
             auto evalue = y->second.HasValue(value);
             replacement = evalue ? wis::format("{}{}{}", y->second.name, impls[+evalue->impl], evalue->name)
                                  : GetCFullTypename(y->second.name, impl);
         } else if (auto z = struct_map.find(this_type_view); z != struct_map.end()) {
             auto member = z->second.HasValue(value);
             replacement = member ? wis::format("{}::{}", GetCFullTypename(z->second.name, impl), member->name)
                                  : GetCFullTypename(z->second.name, impl);
         } else if (auto d = delegate_map.find(this_type_view); d != delegate_map.end()) {
             auto member = d->second.HasValue(value);
             replacement = member ? wis::format("{}::{}", GetCFullTypename(d->second.name, impl), member->name)
                                  : GetCFullTypename(d->second.name, impl);
         } else if (auto h = handle_map.find(this_type_view); h != handle_map.end()) {
             replacement = GetCFullTypename(h->second.name, impl);
         } else if (auto f = function_map.find(std::string(this_type_view)); f != function_map.end()) {
             auto member = f->second.HasValue(value);
             replacement = member ? wis::format("{}({})", GetCFullTypename(f->second.name, impl), member->name)
                                  : GetCFullTypename(f->second.name, impl);
         }*/

        pos = last;
        doc.replace(first, last - first + 1, replacement);
    }
    return doc;
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

InlineTypeInfo Generator::FindInlineType(std::string_view str, size_t initial)
{
    // Find caption enclosed in { }
    auto pos = str.find('{', initial);
    if (pos == std::string_view::npos) {
        return {};
    }

    auto end = str.find('}', pos);
    auto end_n = str.find('\n', pos);
    if (end == std::string_view::npos || end_n < end) {
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

#pragma once
#include <tinyxml2.h>
#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <span>
#include <array>
#include <optional>
#include <bitset>

#include "types.h"
#include "../src/include/wisdom/bridge/format.hpp"

class Generator
{
    static constexpr std::string_view main_output_dir = CPP_OUTPUT_DIR;
    static constexpr std::string_view doc_output_dir = DOC_OUTPUT_DIR;
    static constexpr inline std::array<std::string_view, 5> impls{
        "",
        "DX12",
        "VK"
    };

public:
    Generator() = default;

public:
    void ParseFile(std::filesystem::path file);
    void WriteMainAPI();
    void WriteMainAPIDoc();
    std::span<const std::filesystem::path> GetFiles() const { return files; }

public:
    void ParseIncludes(tinyxml2::XMLElement* includes);
    void ParseFile(tinyxml2::XMLDocument& doc);
    void ParseTypes(tinyxml2::XMLElement* types);
    void ParseEnum(tinyxml2::XMLElement* type);
    void ParseStruct(tinyxml2::XMLElement* type);
    void ParseHandles(tinyxml2::XMLElement* handles);
    void ParseVariant(tinyxml2::XMLElement* type);
    // tinyxml2::XMLError ParseBitmask(tinyxml2::XMLElement* type);

    // Make
    std::string MakeCEnum(const WisEnum& s, DocKind kind = DocKind::Full);
    std::string MakeCStruct(const WisStruct& s, DocKind kind = DocKind::Full);
    std::string MakeCVariant(const WisStruct& s, std::string_view impl = "", DocKind kind = DocKind::Full);
    std::string MakeCHandle(const WisHandle& s, std::string_view impl = "", DocKind kind = DocKind::Full);


    std::string MakeEnumDescription(const WisEnum& s);
    std::string MakeStructDescription(const WisStruct& s);
    std::string MakeVariantDescription(const WisStruct& s);
    std::string MakeCMemberDeclaration(const WisStructMember& member, size_t align_width, std::string_view impl = "");
    void TryMakeRef(std::string_view type, std::string_view from);

    // Write
    void WriteCAPI(std::filesystem::path path);
    void WriteCDependentAPI(std::filesystem::path path);
    void WriteEnumDocumentation(std::filesystem::path enum_output_path);
    void WriteStructDocumentation(std::filesystem::path struct_output_path);
    void WriteVariantDocumentation(std::filesystem::path struct_output_path);
    void WriteHandleDocumentation(std::filesystem::path handle_output_path);
    void WriteDocumentation(std::filesystem::path doc_output_path,
                            std::string_view doc_template,
                            std::string_view object_name,
                            std::string_view code,
                            std::string_view desc,
                            std::string_view refs);

    // Helpers
    std::string GetCFullTypename(std::string_view type, std::string_view impl = "");
    std::string FinalizeCDocumentation(std::string doc, std::string_view this_type, std::string_view impl = "");
    std::string GetMemberTypeString(const WisStructMember& member, std::string_view impl = "");

    TypeKind GetType(std::string_view type_name) const noexcept;
    std::string GetRefs(std::string_view for_type);

    static ImplementedFor ImplCode(std::string_view impl) noexcept;
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    static InlineTypeInfo FindInlineType(std::string_view str, size_t initial);
    static std::string MakeVersionString(std::string_view version, bool newline = false);
    static std::string MakeSnakeCase(std::string_view str);
    static Modifier GetModifiers(std::string_view mod_str) noexcept;

public:
    template<typename T, typename V>
    std::string MakeCValueDocumentation(const T& type, const V& value, std::string_view value_decl, DocKind kind)
    {
        std::string version_info;
        if constexpr (requires { value.version; }) {
            version_info = MakeVersionString(value.version);
        }

        if (kind == DocKind::VersionOnly) {
            if constexpr (requires { value.version; }) {
                if (value.version.empty()) {
                    return wis::format("{}\n", value_decl);
                }
                return wis::format("// {}{}\n", version_info, value_decl);
            }
            return wis::format("{}\n", value_decl);
        }

        auto doc = value.doc;
        auto type_name = type.name;

        std::string documentation;
        bool pre_doc = false;
        if (!doc.empty()) {
            if (doc.find('\n') != std::string_view::npos) {
                pre_doc = true;
                documentation = wis::format("/**\n@brief {}\n{}\n*/", version_info, doc);
                ReplaceAll(documentation, "\n", "\n * ");
            } else {
                documentation = wis::format(" ///< {}{}", version_info, doc);
            }
            documentation = FinalizeCDocumentation(documentation, type_name);
        }
        return pre_doc
                ? wis::format("    {}\n    {}\n", documentation, value_decl)
                : wis::format("{}{}\n", value_decl, documentation);
    }

    template<typename T>
    std::string MakeTypeDocumentation(const T& type, DocKind kind)
    {
        std::string version_info = MakeVersionString(type.version);
        if (!type.doc.empty() && kind == DocKind::Full) {
            std::string documentation = wis::format("/**\n@brief {}{}\n\n", version_info, type.doc);
            if constexpr (requires { type.doc_translates; }) {
                documentation += type.doc_translates;
            }
            documentation += "\n*/";

            ReplaceAll(documentation, "\n", "\n * ");
            return FinalizeCDocumentation(documentation, type.name);
        }
        return wis::format("// {}", version_info);
    }
    static constexpr std::string_view GetImplString(ImplementedFor impl) noexcept
    {
        switch (impl) {
        case ImplementedFor::Both:
            return "";
        case ImplementedFor::DX12:
            return "DX12";
        case ImplementedFor::Vulkan:
            return "VK";
        default:
            return "";
        }
    }

private:
    std::unordered_map<std::filesystem::path, tinyxml2::XMLDocument> documents;

    std::unordered_map<std::string_view, WisEnum> enum_map;
    std::unordered_map<std::string_view, WisStruct> struct_map;
    std::unordered_map<std::string_view, WisStruct> variant_map;
    std::unordered_map<std::string_view, WisHandle> handle_map;

    std::unordered_map<std::string_view, Dependencies> dependency_tree;

    // Ordered members
    std::vector<std::string_view> enums_in_order;
    std::vector<std::string_view> structs_in_order;
    std::vector<std::string_view> variants_in_order;
    std::vector<std::string_view> handles_in_order;
    std::vector<std::filesystem::path> files;

    // Standard type translations
    const std::unordered_map<std::string_view, std::string_view> standard_types{
        { "bool", "bool" },
        { "void", "void" },
        { "u8", "uint8_t" },
        { "u16", "uint16_t" },
        { "u32", "uint32_t" },
        { "u64", "uint64_t" },
        { "i8", "int8_t" },
        { "i16", "int16_t" },
        { "i32", "int32_t" },
        { "i64", "int64_t" },

        { "f32", "float" },
        { "f64", "double" },

        { "char", "char" },
        { "u8string", "const char" },
        { "u16string", "const char16_t" },
        { "u32string", "const char32_t" },
    };
};

#pragma once
#include <tinyxml2.h>
#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <span>
#include <array>
#include <optional>
#include <bitset>

#include "../src/include/wisdom/bridge/format.hpp"

namespace tinyxml2 {
class XMLDocument;
} // namespace tinyxml2

enum class DocKind {
    Full,
    VersionOnly,
};

enum class TypeKind {
    Base,
    Struct,
    Union,
    Enum,
    Bitmask,
    Handle,
    FuncPointer,
    Alias,
};
enum ImplementedFor {
    Both,
    DX12,
    Vulkan,
};

struct InlineTypeInfo {
    std::string_view type;
    std::string_view value;
    std::size_t pos;
    std::size_t after;
};

struct Type {
    std::string_view name;
    TypeKind kind;
};

struct WisConvert {
    std::string_view value;
    bool direct = false;
};
struct WisEnumValue {
    std::string_view name;
    std::string_view doc;
    std::string_view version;
    std::array<std::string_view, 3> converts;
    int64_t value = 0;
};
struct WisEnum {
    std::string_view name;
    std::string_view type;
    std::string_view doc;
    std::string_view version;
    std::string doc_translates;
    std::vector<WisEnumValue> values;
    std::array<WisConvert, 3> conversion_type;

public:
    std::optional<WisEnumValue> HasValue(std::string_view name) const noexcept
    {
        auto enum_value = std::find_if(values.begin(), values.end(), [&](auto& v) {
            return v.name == name;
        });
        return enum_value != values.end() ? std::optional<WisEnumValue>{ *enum_value } : std::nullopt;
    }
};

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
    tinyxml2::XMLError ParseFile(std::filesystem::path file);
    void WriteMainAPI();
    void WriteMainAPIDoc();
    std::span<const std::filesystem::path> GetFiles() const { return files; }

public:
    tinyxml2::XMLError ParseIncludes(tinyxml2::XMLElement* includes);
    tinyxml2::XMLError ParseFile(tinyxml2::XMLDocument& doc);
    tinyxml2::XMLError ParseTypes(tinyxml2::XMLElement* types, std::string_view extension = "");
    tinyxml2::XMLError ParseEnum(tinyxml2::XMLElement* type);

    // Make
    std::string MakeCEnum(const WisEnum& s, DocKind kind = DocKind::Full);

    void MakeEnumDocumentation(std::filesystem::path enum_output_path);
    std::string MakeEnumDescription(const WisEnum& s);

    // Write
    void WriteCAPI(std::filesystem::path path);

    // Helpers
    std::string GetCFullTypename(std::string_view type, std::string_view impl);
    std::string FinalizeCDocumentation(std::string doc, std::string_view this_type, std::string_view impl = "");

    static ImplementedFor ImplCode(std::string_view impl) noexcept;
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    static InlineTypeInfo FindInlineType(std::string_view str, size_t initial);
    static std::string MakeVersionString(std::string_view version, bool newline = false);
    static std::string MakeSnakeCase(std::string_view str);

public:
    template<typename T, typename V>
    std::string MakeCValueDocumentation(const T& type, const V& value, std::string_view value_decl, DocKind kind)
    {
        if (kind == DocKind::VersionOnly) {
            if (value.version.empty()) {
                return wis::format("{}\n", value_decl);
            }
            return wis::format("// {}{}\n", MakeVersionString(value.version, true), value_decl);
        }

        auto doc = value.doc;
        auto type_name = type.name;

        std::string documentation;
        bool pre_doc = false;
        if (!doc.empty()) {
            if (doc.find('\n') != std::string_view::npos) {
                pre_doc = true;
                documentation = wis::format("/**\n@brief {}{}\n*/", MakeVersionString(value.version, true), doc);
                ReplaceAll(documentation, "\n", "\n * ");
            } else {
                documentation = wis::format(" ///< {}{}", MakeVersionString(value.version, false), doc);
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
            std::string documentation = wis::format("/**\n@brief {}{}\n\n{}*/", version_info, type.doc, type.doc_translates);
            ReplaceAll(documentation, "\n", "\n * ");
            return FinalizeCDocumentation(documentation, type.name);
        }
        return wis::format("// {}", version_info);
    }

private:
    std::unordered_map<std::filesystem::path, tinyxml2::XMLDocument> documents;

    std::unordered_map<std::string_view, WisEnum> enum_map;

    std::unordered_map<std::string_view, Type> dependency_tree;

    // Ordered members
    std::vector<std::string_view> enums_in_order;
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

        { "u8string", "const char" },
        { "u16string", "const char16_t" },
        { "u32string", "const char32_t" },
    };
};

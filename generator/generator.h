#pragma once
#include <tinyxml2.h>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <span>
#include <array>

static constexpr std::string_view documentation_header = R"(/** \mainpage Wisdom API Documentation

<b>Version {}</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom))";

enum ImplementedFor {
    None = 0,
    DX12 = 1,
    Vulkan = 2,
    Both = 3,
    Unspecified = Both | 4
};
struct WisEnumValue {
    std::string_view name;
    std::string_view doc;
    int64_t value;
    ImplementedFor impl = ImplementedFor::Both;
};
struct WisEnum {
    std::string_view name;
    std::string_view type;
    std::string_view doc;
    std::string doc_translates;
    std::vector<WisEnumValue> values;

    std::optional<WisEnumValue> HasValue(std::string_view name) const noexcept
    {
        if (name.empty())
            return {};

        auto enum_value = std::find_if(values.begin(), values.end(), [&](auto& v) {
            return v.name == name;
        });
        return *enum_value;
    }
};

struct WisBitmaskValue {
    std::string_view name;
    std::string_view doc;
    enum class Type {
        Bit,
        Value
    } type = Type::Value;
    uint64_t value = 0;
    uint32_t bit = 0;
    ImplementedFor impl = ImplementedFor::Both;
};
struct WisBitmask {
    std::string_view name;
    std::string_view type;
    std::string_view doc;
    std::string doc_translates;
    std::vector<WisBitmaskValue> values;

    std::optional<WisBitmaskValue> HasValue(std::string_view name) const noexcept
    {
        if (name.empty())
            return {};

        auto enum_value = std::find_if(values.begin(), values.end(), [&](auto& v) {
            return v.name == name;
        });
        return *enum_value;
    }
};

struct WisStructMember {
    std::string_view name;
    std::string_view type;
    std::string_view array_size;
    std::string_view modifier;
    std::string_view default_value;
    std::string_view doc;
};
struct WisStruct {
    std::string_view name;
    std::string_view doc;
    std::vector<WisStructMember> members;
    std::optional<WisStructMember> HasValue(std::string_view name) const noexcept
    {
        if (name.empty())
            return {};

        auto enum_value = std::find_if(members.begin(), members.end(), [&](auto& v) {
            return v.name == name;
        });
        return *enum_value;
    }
};
struct WisVariantImpl {
    ImplementedFor impl = ImplementedFor::Both;
    std::vector<WisStructMember> members;
};
struct WisVariant {
    std::string_view name;
    std::string_view doc;
    std::string_view this_type;
    std::vector<WisVariantImpl> impls;
};

static constexpr std::string_view ResultDoc = "Result with {Status::Ok} on success.\nError in {Result::error} otherwise.";

enum class ReplaceTypeFor {
    None,
    C,
    CPP
};
enum class TypeInfo {
    None,
    Result,
    Regular,
    Struct,
    Enum,
    Bitmask,
    Handle,
    Delegate,
    String,
    View,
    Variant,
    ExtHandle
};
struct ReplacedParameter {
    ReplaceTypeFor replace_for = ReplaceTypeFor::None;

    TypeInfo type_info = TypeInfo::None;
    std::string_view type;
    std::string_view name;
    std::string_view modifier;
    std::string_view default_value;
    std::string_view doc;
};
struct WisFunctionParameter {
    std::optional<ReplacedParameter> replaced;

    TypeInfo type_info = TypeInfo::None;
    std::string_view type;
    std::string_view name;
    std::string_view modifier;
    std::string_view default_value;
    std::string_view doc;
};
struct WisReturnType {
    TypeInfo type_info = TypeInfo::None;
    bool has_result = false;
    std::string_view type;
    std::string_view doc;
    std::string_view opt_name;
    std::string_view modifier;
};
struct WisFunction {
    std::string_view name;
    std::string_view this_type;
    std::string_view doc;
    TypeInfo this_type_info = TypeInfo::None;

    WisReturnType return_type;
    std::vector<WisFunctionParameter> parameters;
    ReplaceTypeFor implemented_for = ReplaceTypeFor::None;
    bool custom_impl = false;

    std::optional<WisFunctionParameter> HasValue(std::string_view name) const noexcept
    {
        if (name.empty())
            return {};
        auto enum_value = std::find_if(parameters.begin(), parameters.end(), [&](auto& v) {
            return v.name == name;
        });
        return *enum_value;
    }
};

struct WisHandle {
    std::string_view name;
    std::string_view doc;

    std::vector<std::string_view> functions;
    std::array<std::string_view, 2> files; // first is DX12, second is Vulkan

    void AddFile(std::string_view file, ImplementedFor impl) noexcept
    {
        if (impl & ImplementedFor::DX12)
            files[0] = file;
        if (impl & ImplementedFor::Vulkan)
            files[1] = file;
        if (impl & ImplementedFor::Both) {
            files[0] = file;
            files[1] = file;
        }
    }

    std::string_view GetFile(ImplementedFor impl) const noexcept
    {
        if (impl & ImplementedFor::DX12)
            return files[0];
        if (impl & ImplementedFor::Vulkan)
            return files[1];
        return "";
    }

    std::optional<const WisFunction> HasValue(std::string_view name,
                                              const std::unordered_map<std::string_view, WisFunction>& function_map) const noexcept
    {
        if (name.empty())
            return {};

        auto enum_value = std::find_if(functions.begin(), functions.end(), [&](auto& v) {
            return v == name;
        });

        if (enum_value == functions.end())
            return {};

        return function_map.at(*enum_value);
    }
};

enum class ExtensionType {
    None,
    Factory,
    Device
};
struct WisExtension : WisHandle {
    ExtensionType type = ExtensionType::None;
    std::string_view ext_folder;
};

struct WisConversion {
    std::string value;
    ImplementedFor impl = ImplementedFor::None;
};

class Generator
{
    static constexpr std::string_view output_dir = OUTPUT_DIR;
    static constexpr std::string_view cpp_output_dir = CPP_OUTPUT_DIR;
    static constexpr std::string_view ext_dir = CPP_OUTPUT_DIR "/../../extensions";
    static constexpr inline std::array<std::string_view, 5> impls{
        "",
        "DX12",
        "VK",
        "",
        ""
    };

public:
    Generator(tinyxml2::XMLDocument& doc);

public:
    std::span<const std::filesystem::path> GetFiles() const noexcept
    {
        return files;
    }

public:
    int GenerateCAPI();
    int GenerateCPPAPI();
    int GenerateCPPInlineDoc();
    std::tuple<std::string, std::string, std::string> GenerateCTypes();
    std::tuple<std::string, std::string, std::string> GenerateCTypedefs();
    std::string GenerateCPPTypes();
    std::string GenerateCPPTypedefs();
    std::string GenerateCPPPlatformTypedefs(std::string_view impl);
    std::string GenerateCPPExportHeader();
    std::string GenerateCPPPlatformExportHeader(std::string_view impl);

    void ParseFile(tinyxml2::XMLDocument& doc);
    void ParseTypes(tinyxml2::XMLElement* types);
    void ParseIncludes(tinyxml2::XMLElement* handles);
    void ParseHandles(tinyxml2::XMLElement* handles);
    void ParseFunctions(tinyxml2::XMLElement* functions);
    void ParseExtensions(tinyxml2::XMLElement* extensions);

    void ParseStruct(tinyxml2::XMLElement& type);
    void ParseEnum(tinyxml2::XMLElement& type);
    void ParseBitmask(tinyxml2::XMLElement& type);
    void ParseDelegate(tinyxml2::XMLElement* type);
    void ParseVariant(tinyxml2::XMLElement& type);

    std::string FinalizeCDocumentation(std::string doc, std::string_view this_type, std::string_view impl = "");
    std::string FinalizeCPPDocumentation(std::string doc, std::string_view this_type, std::string_view impl = "");
    std::string MakeCValueDocumentation(std::string value, std::string_view doc, std::string_view type_name);
    std::string MakeCPPValueDocumentation(std::string value, std::string_view doc, std::string_view type_name);

    std::string MakeCStruct(const WisStruct& s);
    std::pair<std::string, std::string> MakeCVariant(const WisVariant& s);
    std::string MakeCPPVariant(const WisVariant& s, ImplementedFor impl);
    std::string MakeCEnum(const WisEnum& s);
    std::string MakeCBitmask(const WisBitmask& s);
    std::string MakeCPPStruct(const WisStruct& s);
    std::string MakeCPPEnum(const WisEnum& s);
    std::string MakeCPPBitmask(const WisBitmask& s);

#pragma region C API
    // Function generation
    std::string MakeCFunctionGenericDecl(const WisFunction& func, std::string_view impl);
    std::string MakeCFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl = "WISDOM_API", bool doc = true);
    std::string MakeCFunctionDecl(const WisFunction& func, std::string_view impl, std::string_view pre_decl = "WISDOM_API");
    std::string MakeCFunctionCall(const WisFunction& func, std::string_view prefix, std::string_view impl);
    std::string MakeCFunctionImpl(const WisFunction& func, std::string_view prefix, std::string_view impl);
    std::string MakeCDelegate(const WisFunction& s);

    // Handle generation
    std::string MakeCHandle(const WisHandle& s, std::string_view impl);
    std::string MakeCHandleGeneric(const WisHandle& s, std::string_view impl);
    std::string MakeCHandleMethods(const WisHandle& s, std::string_view impl);
    std::string MakeCHandleMethodsGeneric(const WisHandle& s, std::string_view impl);
    std::string MakeCHandleMethodImpls(const WisHandle& s, std::string_view impl);

    // Extension generation
    std::string MakeCExtensionHeader(const WisExtension& s);
    std::string MakeCExtensionImpl(const WisExtension& s, std::string_view impl);
    std::string MakeCExtensionMap(std::string_view impl);

    std::string MakeCVariantGeneric(const WisVariant& s, std::string_view impl);
#pragma endregion

#pragma region CPP API
    // Function generation
    std::string MakeCPPFunctionGenericDecl(const WisFunction& func, std::string_view impl);
    std::string MakeCPPFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl = "WISDOM_API", bool doc = true, bool impl_on_fdecl = true);
    std::string MakeCPPFunctionDecl(const WisFunction& func, std::string_view impl, std::string_view pre_decl = "WISDOM_API");
    std::string MakeCPPFunctionCall(const WisFunction& func, std::string_view impl);
    std::string MakeCPPDelegate(const WisFunction& s);

    // Handle generation
    std::string MakeCPPHandle(const WisHandle& s, std::string_view impl);
#pragma endregion

    std::string GetCFullTypename(std::string_view type, std::string_view impl = "");
    std::string GetCPPFullTypename(std::string_view type, std::string_view impl = "");
    std::string GetCFullArg(const WisFunctionParameter& arg, std::string_view impl, bool only_type = false);
    std::string GetCPPFullArg(const WisFunctionParameter& arg, std::string_view impl, bool only_type = false);
    std::string ConvertFromCType(const WisFunctionParameter& arg, std::string_view impl);
    std::string ConvertToCType(const WisFunctionParameter& arg, std::string_view impl);

    WisReturnType ParseFunctionReturn(tinyxml2::XMLElement* func);
    std::vector<WisFunctionParameter> ParseFunctionArgs(tinyxml2::XMLElement* func);

    ImplementedFor GetImplementedFor(std::string_view type);
    TypeInfo GetTypeInfo(std::string_view type);

private:
    std::unordered_map<std::filesystem::path, tinyxml2::XMLDocument> includes;
    std::unordered_map<std::filesystem::path, std::string> file_contents;
    std::vector<std::filesystem::path> files;

    std::vector<WisStruct*> structs;
    std::vector<WisFunction*> functions; // order preservation
    std::vector<WisFunction*> cpp_funcs;
    std::vector<WisFunction*> c_funcs;

    std::vector<std::string> function_impl;
    std::vector<std::string> function_impl_dx;
    std::vector<std::string> function_impl_vk;

    std::vector<std::string> function_decls;
    std::vector<std::string> function_decls_dx;
    std::vector<std::string> function_decls_vk;

    std::vector<std::string> cpp_type_traits;
    std::vector<WisConversion> cpp_conversion;
    std::vector<WisVariant*> variants;

    std::unordered_map<std::string_view, WisHandle> handle_map;
    std::unordered_map<std::string_view, WisFunction> delegate_map;
    std::unordered_map<std::string_view, WisStruct> struct_map;
    std::unordered_map<std::string_view, WisVariant> variant_map;
    std::unordered_map<std::string_view, WisEnum> enum_map;
    std::unordered_map<std::string_view, WisBitmask> bitmask_map;
    std::unordered_map<std::string_view, WisExtension> extension_map;
    std::unordered_map<std::string, WisFunction> function_map;

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

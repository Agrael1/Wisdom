#pragma once
#include <tinyxml2.h>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <span>
#include <array>

enum ImplementedFor {
    None = 0,
    DX12 = 1,
    Vulkan = 2,
    Both = 3,
    Unspecified = Both | 4
};
struct WisEnumValue {
    std::string_view name;
    int64_t value;
    ImplementedFor impl = ImplementedFor::Both;
};
struct WisEnum {
    std::string_view name;
    std::string_view type;
    std::vector<WisEnumValue> values;
};

struct WisBitmaskValue {
    std::string_view name;
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
    std::vector<WisBitmaskValue> values;
};

struct WisStructMember {
    std::string_view name;
    std::string_view type;
    std::string_view array_size;
    std::string_view modifier;
    std::string_view default_value;
};
struct WisStruct {
    std::string_view name;
    std::vector<WisStructMember> members;
};
struct WisVariantImpl {
    ImplementedFor impl = ImplementedFor::Both;
    std::vector<WisStructMember> members;
};
struct WisVariant {
    std::string_view name;
    std::string_view this_type;
    std::vector<WisVariantImpl> impls;
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
};
struct WisFunctionParameter {
    TypeInfo type_info = TypeInfo::None;
    std::string_view type;
    std::string_view name;
    std::string_view modifier;
    std::string_view default_value;
};
struct WisReturnType {
    TypeInfo type_info = TypeInfo::None;
    std::string_view type;
    std::string_view opt_name;
    std::string_view modifier;
};
struct WisFunction {
    std::string_view name;
    std::string_view this_type;
    TypeInfo this_type_info = TypeInfo::None;
    std::vector<WisReturnType> return_types;
    std::vector<WisFunctionParameter> parameters;
    ImplementedFor impl = ImplementedFor::Unspecified;
};

struct WisHandle {
    std::string_view name;
    ImplementedFor impl = ImplementedFor::Both;
};

class Generator
{
    static constexpr std::string_view output_dir = OUTPUT_DIR;
    static constexpr std::string_view cpp_output_dir = CPP_OUTPUT_DIR;
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
    int GenerateCAPI();
    int GenerateCPPAPI();
    std::string GenerateCTypes();
    std::string GenerateCPPTypes();
    std::string GenerateCTypedefs();
    std::string GenerateCPPTypedefs();
    std::string GenerateCPPPlatformTypedefs(std::string_view impl);
    std::string MakeCPPPlatformFunc(WisFunction& func, std::string_view impl);
    std::string GenerateCPPExportHeader();

    void ParseTypes(tinyxml2::XMLElement* types);

    void ParseHandles(tinyxml2::XMLElement* handles);
    void ParseFunctions(tinyxml2::XMLElement* functions);

    void ParseStruct(tinyxml2::XMLElement& type);
    void ParseEnum(tinyxml2::XMLElement& type);
    void ParseBitmask(tinyxml2::XMLElement& type);
    void ParseDelegate(tinyxml2::XMLElement* type);
    void ParseVariant(tinyxml2::XMLElement& type);

    std::string MakeCStruct(const WisStruct& s);
    std::string MakeCVariant(const WisVariant& s);
    std::string MakeCPPVariant(const WisVariant& s, ImplementedFor impl);
    std::string MakeCEnum(const WisEnum& s);
    std::string MakeCBitmask(const WisBitmask& s);
    std::string MakeCPPStruct(const WisStruct& s);
    std::string MakeCPPEnum(const WisEnum& s);
    std::string MakeCPPBitmask(const WisBitmask& s);
    std::string MakeCPPDelegate(const WisFunction& s);

    std::string MakeHandle(const WisHandle& s);
    std::string MakeFunctionDecl(const WisFunction& s);
    std::string MakeDelegate(const WisFunction& s);
    std::string MakeFunctionImpl(const WisFunction& func, std::string_view decl, std::string_view impl);

    std::string GetCFullTypename(std::string_view type, std::string_view impl = "");
    std::string GetCPPFullTypename(std::string_view type, std::string_view impl = "");
    std::string GetCFullArg(const WisFunctionParameter& arg, std::string_view impl);
    std::string GetCPPFullArg(const WisFunctionParameter& arg, std::string_view impl);

    std::vector<WisReturnType> ParseFunctionReturn(tinyxml2::XMLElement* func);
    std::vector<WisFunctionParameter> ParseFunctionArgs(tinyxml2::XMLElement* func);

    ImplementedFor GetImplementedFor(std::string_view type);
    TypeInfo GetTypeInfo(std::string_view type);

private:
    std::vector<WisStruct*> structs;
    std::vector<WisFunction> functions;
    std::vector<std::string> function_impl;
    std::vector<std::string> function_decls;

    std::vector<std::string> cpp_type_traits;
    std::vector<WisVariant*> variants;

    std::unordered_map<std::string_view, WisHandle> handle_map;
    std::unordered_map<std::string_view, WisFunction> delegate_map;
    std::unordered_map<std::string_view, WisStruct> struct_map;
    std::unordered_map<std::string_view, WisVariant> variant_map;
    std::unordered_map<std::string_view, WisEnum> enum_map;
    std::unordered_map<std::string_view, WisBitmask> bitmask_map;

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

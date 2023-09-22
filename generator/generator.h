#pragma once
#define WIS_EXPORT
#include <tinyxml2.h>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

struct WisStructMember {
    std::string name;
    std::string type;
    std::string array_size;
    std::string default_value;
};
struct WisStruct {
    std::string name;
    std::vector<WisStructMember> members;
};

struct WisEnumValue {
    std::string name;
    uint32_t value;
};
struct WisEnum {
    std::string name;
    std::string type;
    std::vector<WisEnumValue> values;
};

struct WisBitmaskValue {
    std::string name;
    enum class Type {
        Bit,
        Value
    } type = Type::Value;
    uint32_t value = 0;
    uint32_t bit = 0;
};
struct WisBitmask {
    std::string name;
    uint32_t size = 0;
    std::vector<WisBitmaskValue> values;
};

struct WisFunctionParameter {
    std::string type;
    std::string name;
    std::string array_size;
    std::string default_value;
};
struct WisFunction {
    std::string name;
    std::string return_type;
    std::string this_type;
    std::vector<WisFunctionParameter> parameters;
};

enum ImplementedFor {
    None = 0,
    DX12 = 1,
    Vulkan = 2,
    Both = 3
};

enum class TypeInfo {
    None,
    Regular,
    Struct,
    Enum,
    Handle,
};

struct WisHandle {
    std::string name;
    ImplementedFor impl = ImplementedFor::Both;
};

using ResolvedType = std::pair<TypeInfo, std::string>;

class Generator
{
    static constexpr std::string_view input_file = INPUT_FILE;
    static constexpr std::string_view output_dir = OUTPUT_DIR;
    static constexpr std::string_view cpp_output_dir = CPP_OUTPUT_DIR;

public:
    Generator() = default;

public:
    int GenerateCAPI(std::filesystem::path file = input_file);
    std::string GenerateCTypes();
    std::string GenerateCPPTypes();
    std::string GenerateCTypedefs();
    std::string GenerateCPPTypedefs();

    void ParseTypes(tinyxml2::XMLElement* types);
    void ParseHandles(tinyxml2::XMLElement* handles);
    void ParseFunctions(tinyxml2::XMLElement* functions);

    void ParseStruct(tinyxml2::XMLElement* type);
    void ParseEnum(tinyxml2::XMLElement* type);
    void ParseBitmask(tinyxml2::XMLElement* type);

    std::string MakeCStruct(const WisStruct& s);
    std::string MakeCEnum(const WisEnum& s);
    std::string MakeCBitmask(const WisBitmask& s);
    std::string MakeCPPStruct(const WisStruct& s);
    std::string MakeCPPEnum(const WisEnum& s);
    std::string MakeCPPBitmask(const WisBitmask& s);

    std::string MakeHandle(const WisHandle& s);
    std::string MakeFunctionDecl(const WisFunction& s);
    ResolvedType ResolveType(const std::string& type);

    std::string MakeFunctionImpl(const WisFunction& func, std::string_view func_decl, std::string_view impl);
private:
    tinyxml2::XMLDocument doc;

    std::vector<WisStruct> structs;
    std::vector<WisEnum> enums;
    std::vector<WisBitmask> bitmasks;
    std::vector<WisHandle> handles;
    std::vector<WisFunction> functions;
    std::vector<std::string> function_impl;

    std::vector<std::string> cpp_type_traits;

    std::unordered_map<std::string, WisStruct*> struct_map;
    std::unordered_map<std::string, WisHandle*> handle_map;

    const std::unordered_map<std::string_view, std::string_view> standard_types{
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

        { "u8string", "const char8_t" },
        { "u16string", "const char16_t" },
        { "u32string", "const char32_t" },
    };
};
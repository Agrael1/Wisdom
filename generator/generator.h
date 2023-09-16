#pragma once
#define WIS_EXPORT
#include <tinyxml2.h>
#include <string_view>
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

class Generator
{
    static constexpr std::string_view input_file = INPUT_FILE;
    static constexpr std::string_view output_dir = OUTPUT_DIR;

public:
    Generator() = default;

public:
    int GenerateCAPI(std::filesystem::path file = input_file);
    std::string GenerateCTypes();
    std::string GenerateCTypedefs();

    void ParseTypes(tinyxml2::XMLElement* types);
    void ParseStruct(tinyxml2::XMLElement* type);
    void ParseEnum(tinyxml2::XMLElement* type);
    void ParseBitmask(tinyxml2::XMLElement* type);

    std::string MakeCStruct(const WisStruct& s);
    std::string MakeCEnum(const WisEnum& s);
    std::string MakeCBitmask(const WisBitmask& s);

    std::string MakeHandle(tinyxml2::XMLElement* type);
    std::string MakeFunction(tinyxml2::XMLElement* type);

private:
    tinyxml2::XMLDocument doc;

    std::vector<WisStruct> structs;
    std::vector<WisEnum> enums;
    std::vector<WisBitmask> bitmasks;

    const std::unordered_map<std::string, std::string> standard_types{
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
    std::string output;
};
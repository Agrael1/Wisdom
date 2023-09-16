#pragma once
#define WIS_EXPORT
#include <tinyxml2.h>
#include <string_view>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

class Generator
{
    static constexpr std::string_view output_file = OUTPUT_FILE;

public:
    Generator() = default;

public:
    int GenerateAPI(std::filesystem::path file = OUTPUT_FILE);
    int GenerateTypes(tinyxml2::XMLElement* types);
    std::string MakeStruct(tinyxml2::XMLElement* type);
    std::string MakeEnum(tinyxml2::XMLElement* type);
    std::string MakeBitmask(tinyxml2::XMLElement* type);
    std::string MakeHandle(tinyxml2::XMLElement* type);
    std::string MakeFunction(tinyxml2::XMLElement* type);

private:
    tinyxml2::XMLDocument doc;
    std::unordered_set<std::string> structs;
    std::unordered_set<std::string> enums;
    std::unordered_set<std::string> bitmasks;
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
#include "generator.h"
#include "../wisdom/include/wisdom/bridge/format.h"

using namespace tinyxml2;

int Generator::GenerateCAPI(std::filesystem::path file)
{
    structs.clear();
    enums.clear();
    bitmasks.clear();

    if (doc.LoadFile(file.string().c_str()) != XML_SUCCESS)
        return 1;

    auto* root = doc.FirstChildElement("registry");
    if (!root)
        return 1;

    auto* types = root->FirstChildElement("types");
    ParseTypes(types);
    output = GenerateCTypes();
    return 0;
}

std::string Generator::GenerateCTypes()
{
    std::string c_types = GenerateCTypedefs();
    for (auto& s : structs) {
        c_types += MakeCStruct(s);
    }
    for (auto& s : enums) {
        c_types += MakeCEnum(s);
    }
    for (auto& s : bitmasks) {
        c_types += MakeCBitmask(s);
    }
    return c_types;
}

std::string Generator::GenerateCTypedefs()
{
    std::string c_types;
    for (auto& s : structs) {
        c_types += std::format("typedef struct Wis{} Wis{};\n", s.name, s.name);
    }
    for (auto& s : enums) {
        c_types += std::format("typedef enum Wis{} Wis{};\n", s.name, s.name);
    }
    for (auto& s : bitmasks) {
        c_types += std::format("typedef enum Wis{} Wis{};\n", s.name, s.name);
    }
    return c_types;
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
        }
    }
}

void Generator::ParseStruct(tinyxml2::XMLElement* type)
{
    auto& ref = structs.emplace_back();
    auto name = type->FindAttribute("name")->Value();
    ref.name = name;

    for (auto* member = type->FirstChildElement("member"); member; member = member->NextSiblingElement("member")) {
        auto& m = ref.members.emplace_back();

        auto* type = member->FindAttribute("type")->Value();
        auto* name = member->FindAttribute("name")->Value();

        auto* arr = member->FindAttribute("array");

        m.type = type;
        m.name = name;
        if (auto* arr = member->FindAttribute("array")) {
            m.array_size = arr->Value();
        }
        if (auto* def = member->FindAttribute("default")) {
            m.default_value = def->Value();
        }
    }
}

void Generator::ParseEnum(tinyxml2::XMLElement* type)
{
    auto& ref = enums.emplace_back();
    auto name = type->FindAttribute("name")->Value();
    ref.name = name;

    if (auto* size = type->FindAttribute("type"))
        ref.type = size->Value();

    for (auto* member = type->FirstChildElement("value"); member; member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        auto* name = member->FindAttribute("name")->Value();
        auto* value = member->FindAttribute("value")->Value();

        m.name = name;
        m.value = std::stoul(value);
    }
}

void Generator::ParseBitmask(tinyxml2::XMLElement* type)
{
    auto& ref = bitmasks.emplace_back();
    auto name = type->FindAttribute("name")->Value();
    ref.name = name;

    if (auto* size = type->FindAttribute("size"))
        ref.size = std::stoul(size->Value());
    else
        ref.size = 32;

    for (auto* member = type->FirstChildElement("value"); member; member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        auto* name = member->FindAttribute("name")->Value();
        m.name = name;

        auto* value = member->FindAttribute("value");
        auto* bit = member->FindAttribute("bit");

        if (value) {
            m.value = std::stoul(value->Value());
            m.type = WisBitmaskValue::Type::Value;
            continue;
        }

        m.bit = std::stoul(bit->Value());
        m.type = WisBitmaskValue::Type::Bit;
    }
}

std::string MakeCArray(std::string_view type, std::string_view name, std::string_view arr_len)
{
    return wis::format("    {} {}[{}];\n", type, name, arr_len);
}

std::string Generator::MakeCStruct(const WisStruct& s)
{
    auto st_decl = wis::format(
            "struct Wis{}{{\n", s.name);

    for (auto& m : s.members) {

        std::string res_type;
        if (auto it = standard_types.find(m.type); it != standard_types.end())
            res_type = it->second;
        else
            res_type = "Wis" + m.type;

        if (m.array_size.empty()) {
            st_decl += wis::format("    {} {};\n", res_type, m.name);
        } else {
            st_decl += MakeCArray(res_type, m.name, m.array_size);
        }
    }

    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCEnum(const WisEnum& s)
{
    std::string st_decl = !s.type.empty() ? wis::format("enum Wis{} : {}{{\n", s.name, standard_types.at(s.type)) : wis::format("enum Wis{}{{\n", s.name);

    for (auto& m : s.values) {
        st_decl += wis::format("    Wis{}{} = {},\n", s.name, m.name, m.value);
    }
    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCBitmask(const WisBitmask& s)
{
    std::string st_decl = s.size ? wis::format("enum Wis{} : uint{}_t {{\n", s.name, s.size) : wis::format("enum Wis{}{{\n", s.name);

    for (auto& m : s.values) {
        if (m.type == WisBitmaskValue::Type::Value) {
            st_decl += wis::format("    Wis{}{} = 0x{:X},\n", s.name, m.name, m.value);
        } else {
            st_decl += wis::format("    Wis{}{} = 1 << {},\n", s.name, m.name, m.bit);
        }
    }
    st_decl += wis::format("    Wis{}Max = 0x{:X};\n", s.name, (1ull << s.size) - 1);

    st_decl += "};\n\n";
    return st_decl;
}

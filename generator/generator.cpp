#include "generator.h"
#include "../wisdom/include/wisdom/bridge/format.h"

using namespace tinyxml2;

int Generator::GenerateAPI(std::filesystem::path file)
{
    if (doc.LoadFile(file.string().c_str()) != XML_SUCCESS)
        return 1;

    auto* root = doc.FirstChildElement("registry");
    if (!root)
        return 1;

    auto* types = root->FirstChildElement("types");
    if (GenerateTypes(types))
        return 1;

    return 0;
}

int Generator::GenerateTypes(tinyxml2::XMLElement* types)
{
    if (!types)
        return 1;

    for (auto* type = types->FirstChildElement("type"); type; type = type->NextSiblingElement("type")) {
        auto category = type->FindAttribute("category")->Value();
        if (std::string_view(category) == "struct") {
            std::string x = MakeStruct(type);
            output += x;
        } else if (std::string_view(category) == "enum") {
            std::string x = MakeEnum(type);
            output += x;
        } else if (std::string_view(category) == "bitmask") {
            std::string x = MakeBitmask(type);
            output += x;
        }
    }

    return 0;
}

std::string MakeArray(std::string_view type, std::string_view name, std::string_view arr_len)
{
    return wis::format("    std::array<{}, {}> {};\n", type, arr_len, name);
}

std::string Generator::MakeStruct(tinyxml2::XMLElement* type)
{
    auto name = type->FindAttribute("name")->Value();
    auto st_decl = wis::format(
            "struct {}{{\n", name);

    for (auto* member = type->FirstChildElement("member"); member; member = member->NextSiblingElement("member")) {
        auto* type = member->FindAttribute("type")->Value();
        auto* name = member->FindAttribute("name")->Value();
        auto* arr = member->FindAttribute("array");

        std::string_view type_sv = type;
        if (auto it = standard_types.find(type); it != standard_types.end()) {
            type_sv = it->second;
        }

        if (arr) {
            st_decl += MakeArray(type_sv, name, arr->Value());
        } else {
            st_decl += wis::format("    {} {};\n", type_sv, name);
        }
    }
    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeEnum(tinyxml2::XMLElement* type)
{
    return {};
}

std::string Generator::MakeBitmask(tinyxml2::XMLElement* type)
{
    return std::string();
}

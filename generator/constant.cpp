#include "generator.hpp"
#include <fstream>

//----------------------------------------------------------------------------------------------------------------------
static inline constexpr char template_constant[] =
        R"(/**
 * @page {0}
 * @ingroup Constants
 *
 *
 * @section {0}_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * {1}
 * \endcond
 *
 * @section {0}_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */)";

//----------------------------------------------------------------------------------------------------------------------
void Generator::ParseConstants(tinyxml2::XMLElement* constants)
{
    for (auto* val = constants->FirstChildElement("value"); val;
         val       = val->NextSiblingElement("value")) {
        auto  name = val->FindAttribute("name")->Value();
        auto& ref  = constant_map[name];
        module_map[active_module_name].constants_in_order.emplace_back(name);

        ref.name  = name;
        ref.type  = val->FindAttribute("type")->Value();
        ref.value = val->FindAttribute("value")->Value();

        if (auto* doc = val->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }
        if (auto* version = val->FindAttribute("version")) {
            ref.version = version->Value();
        }
        if (auto* mod = val->FindAttribute("mod")) {
            ref.modifier = GetModifiers(mod->Value());
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCConstant(const WisConstant& c, DocKind kind)
{
    std::string type_str = GetCFullTypename(c.type, Backend::Any);
    std::string mod_str;
    if ((c.modifier & Modifier::Pointer) != Modifier::None) {
        mod_str += "*";
    }
    if ((c.modifier & Modifier::Const) != Modifier::None) {
        type_str = "const " + type_str;
    }

    std::string define_name = "WIS_" + MakeUpperSnakeCase(c.name);
    std::string st_decl     = wis::format("#define {} (({}{}){})\n", define_name, type_str, mod_str, c.value);

    if (!c.doc.empty() && kind == DocKind::Full) {
        std::string version_info  = MakeVersionString(c.version);
        std::string documentation = wis::format("/// @brief {}{}\n", version_info, c.doc);
        documentation             = FinalizeCDocumentation(documentation, c.name);
        st_decl                   = documentation + st_decl;
    }
    return st_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCPPConstant(const WisConstant& c, DocKind kind)
{
    std::string type_str = GetCPPFullTypename(c.type, Backend::Any);
    std::string mod_str;
    if ((c.modifier & Modifier::Pointer) != Modifier::None) {
        mod_str += "*";
    }
    if ((c.modifier & Modifier::Const) != Modifier::None) {
        type_str = "const " + type_str;
    }

    std::string st_decl = wis::format("static constexpr {}{} {} = {};\n", type_str, mod_str, c.name, c.value);

    if (!c.doc.empty() && kind == DocKind::Full) {
        std::string version_info  = MakeVersionString(c.version);
        std::string documentation = wis::format("/// @brief {}{}\n", version_info, c.doc);
        documentation             = FinalizeCPPDocumentation(documentation, c.name);
        st_decl                   = documentation + st_decl;
    }
    return st_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeConstantDescription(const WisConstant& c)
{
    std::string description;
    if (!c.doc.empty()) {
        description += std::string(c.doc) + "\n\n";
    }

    std::string type_str = GetCFullTypename(c.type, Backend::Any);
    description += wis::format("Type: `{}`\n", type_str);
    description += wis::format("Value: `{}`\n", c.value);
    return description;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeUpperSnakeCase(std::string_view str)
{
    std::string result;
    for (char ch : str) {
        if (std::isupper(static_cast<unsigned char>(ch))) {
            if (!result.empty()) {
                result += '_';
            }
        }
        result += std::toupper(static_cast<unsigned char>(ch));
    }
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
void Generator::WriteConstantDocumentation(std::filesystem::path const_output_path)
{
    std::filesystem::create_directories(const_output_path);
    std::filesystem::path const_file_path = const_output_path / "constants.h";

    std::string all_c_code;
    std::string all_cpp_code;

    auto& constant_names = module_map.at(active_module_name).constants_in_order;
    for (auto& const_name : constant_names) {
        auto& const_ref = constant_map[const_name];
        all_c_code += MakeCConstant(const_ref, DocKind::VersionOnly);
        all_cpp_code += MakeCPPConstant(const_ref, DocKind::VersionOnly);
    }

    std::string const_template_content = GetSpecificationCode(all_c_code, "", all_cpp_code, "");

    WriteDocumentation(const_file_path,
                       template_constant,
                       "Constants",
                       const_template_content,
                       empty_doc,
                       empty_doc,
                       empty_doc);
}

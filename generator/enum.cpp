#include "generator.hpp"
#include <fstream>

//-----------------------------------------------------------------------------
static inline constexpr char template_enum[] =
        R"(/**
 * @defgroup {0} {0}
 * @ingroup Enumerations
 *
 * @section {0}_name Name
 * <hr>
 * 
 * @section {0}_spec C Specification
 * <hr>
 * 
 * \cond WIS_GEN_CODE
 * {1}
 * \endcond
 * 
 * @section {0}_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * {2}
 * \endcond
 * 
 * 
 * @section {0}_see_also See Also
 * <hr>
 */)";

//-----------------------------------------------------------------------------
tinyxml2::XMLError Generator::ParseEnum(tinyxml2::XMLElement* type)
{
    std::unordered_map<std::string_view, std::string> cvts;

    // Local documentation
    std::string documentation;
    std::string impl_doc;

    auto name = type->FindAttribute("name")->Value();
    enums_in_order.push_back(name);
    auto& ref = enum_map[name];
    ref.name = name;

    if (auto* size = type->FindAttribute("type")) {
        ref.type = size->Value();
    }

    if (auto* size = type->FindAttribute("doc")) {
        ref.doc = size->Value();
    }

    if (auto* size = type->FindAttribute("version")) {
        ref.version = size->Value();
    } else {
        throw std::runtime_error(wis::format("Enum {} is missing version attribute.", name));
    }

    for (auto* impl_type = type->FirstChildElement("impl_type"); impl_type;
         impl_type = impl_type->NextSiblingElement("impl_type")) {
        auto impl_for = impl_type->FindAttribute("for")->Value();
        auto impl_for_code = ImplCode(impl_for);
        auto impl_name = impl_type->FindAttribute("name")->Value();

        ref.doc_translates += wis::format("Translates to {} for {} implementation.\n", impl_name, impl_for);

        std::string_view def_value = "{}";
        if (auto xdefault = impl_type->FindAttribute("default")) {
            def_value = xdefault->Value();
        }

        if (auto direct = impl_type->FindAttribute("direct")) {
            ref.conversion_type[impl_for_code].direct = true;
            continue;
        }

        ref.conversion_type[static_cast<size_t>(impl_for_code)] = WisConvert{ impl_name, false };
    }

    for (auto* member = type->FirstChildElement("value"); member;
         member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        m.name = member->FindAttribute("name")->Value();
        m.value = std::stoll(member->FindAttribute("value")->Value());
        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }

        if (auto* size = member->FindAttribute("version")) {
            m.version = size->Value();
        }

        for (auto* impl = member->FirstChildElement("impl"); impl;
             impl = impl->NextSiblingElement("impl")) {
            auto impl_name = impl->FindAttribute("name")->Value();
            auto value = impl->FindAttribute("value")->Value();

            auto impl_for_code = ImplCode(impl_name);
            m.converts[impl_for_code] = value;
        }
    }
    return tinyxml2::XMLError::XML_SUCCESS;
}

//-----------------------------------------------------------------------------

std::string Generator::MakeCEnum(const WisEnum& s, DocKind kind)
{
    auto full_name = GetCFullTypename(s.name, "");
    std::string st_decl = wis::format("enum {} {{\n", full_name);

    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (auto& m : s.values) {
        st_decl += MakeCValueDocumentation(s, m, wis::format("    Wis{}{} = {},", s.name, m.name, m.value), kind);
    }

    st_decl += "};\n\n";
    return st_decl;
}

//-----------------------------------------------------------------------------

void Generator::MakeEnumDocumentation(std::filesystem::path enum_output_path)
{
    for (auto& enum_name : enums_in_order) {

        // Make a folder for enums starting with this letter
        std::filesystem::create_directories(enum_output_path);
        std::filesystem::path enum_file_path = enum_output_path / wis::format("{}_enum.h", MakeSnakeCase(enum_name));

        bool file_exists = std::filesystem::exists(enum_file_path);

        // If file exists, only edit the generated code section, else create new file
        std::fstream enum_file{ enum_file_path, file_exists ? std::ios::in | std::ios::out : std::ios::out };
        auto& enum_ref = enum_map[enum_name];

        std::string enum_template_content = wis::format(" * ```c\n{}```\n", MakeCEnum(enum_ref, DocKind::VersionOnly));
        std::string enum_description = wis::format(" * {}", MakeEnumDescription(enum_ref));
        ReplaceAll(enum_template_content, "\n", "\n * ");
        ReplaceAll(enum_description, "\n", "\n * ");
        enum_description = FinalizeCDocumentation(enum_description, enum_name);

        if (!file_exists) {
            std::string xenum = wis::format(template_enum,
                                            enum_name,
                                            enum_template_content,
                                            enum_description);

            enum_file << FinalizeCDocumentation(xenum, enum_name);
            enum_file.close();
            continue;
        }

        // Otherwise, we would need to parse the existing file and replace the generated section
        // Read entire file content
        std::string existing_content((std::istreambuf_iterator<char>(enum_file)),
                                     std::istreambuf_iterator<char>());
        enum_file.close();
        // Find the generated section
        size_t gen_start = existing_content.find(R"(\cond WIS_GEN_CODE)");
        size_t gen_end = existing_content.find(R"(\endcond)");
        if (gen_start == std::string::npos || gen_end == std::string::npos || gen_end <= gen_start) {
            throw std::runtime_error(wis::format("Generated section not found or malformed in {}", enum_file_path.string()));
        }

        // Find the
        size_t desc_start = existing_content.find(R"(\cond WIS_GEN_DESC)");
        size_t desc_end = existing_content.find(R"(\endcond)", desc_start);
        if (desc_start == std::string::npos || desc_end == std::string::npos || desc_end <= desc_start) {
            throw std::runtime_error(wis::format("Description section not found or malformed in {}", enum_file_path.string()));
        }

        // Replace the description section
        std::string updated_content = existing_content.substr(0, desc_start) + "\\cond WIS_GEN_DESC\n" + enum_description + existing_content.substr(desc_end);
        existing_content = updated_content;

        // Replace the generated section
        std::string new_content = existing_content.substr(0, gen_start) + "\\cond WIS_GEN_CODE\n" + enum_template_content + existing_content.substr(gen_end);
        new_content = FinalizeCDocumentation(new_content, enum_name);

        // Write back to file
        std::ofstream enum_file_out{ enum_file_path, std::ios::trunc };
        enum_file_out << new_content;
        enum_file_out.close();
    }
}
std::string Generator::MakeEnumDescription(const WisEnum& s)
{
    std::string description;
    if (!s.doc.empty()) {
        description += std::string(s.doc) + "\n\n";
    }
    description += "Values:\n";
    for (auto& m : s.values) {
        description += wis::format("- `Wis{}{} = {}`: {}\n", s.name, m.name, m.value, m.doc);
    }
    return description;
}
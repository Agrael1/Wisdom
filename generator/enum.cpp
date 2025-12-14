#include "generator.hpp"
#include <fstream>

//-----------------------------------------------------------------------------
static inline constexpr char template_enum[] =
    R"(/**
 * @struct {0} {0}
 * @ingroup Enumerations
 *
 * @section {0}_spec Specification
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
 *
 * \cond WIS_GEN_REFS
 * RESERVED
 * \endcond
 */)";

//-----------------------------------------------------------------------------
void Generator::ParseEnum(tinyxml2::XMLElement* type)
{
    std::unordered_map<std::string_view, std::string> cvts;

    // Local documentation
    std::string documentation;
    std::string impl_doc;

    auto name = type->FindAttribute("name")->Value();
    enums_in_order.push_back(name);
    auto& ref = enum_map[name];
    ref.name  = name;

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
            impl_type       = impl_type->NextSiblingElement("impl_type")) {
        auto impl_for      = impl_type->FindAttribute("for")->Value();
        auto impl_for_code = ImplCode(impl_for);
        auto impl_name     = impl_type->FindAttribute("name")->Value();

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
            member       = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        m.name  = member->FindAttribute("name")->Value();
        m.value = std::stoll(member->FindAttribute("value")->Value());
        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }

        if (auto* size = member->FindAttribute("version")) {
            m.version = size->Value();
        }

        for (auto* impl = member->FirstChildElement("impl"); impl;
                impl       = impl->NextSiblingElement("impl")) {
            auto impl_name = impl->FindAttribute("name")->Value();
            auto value     = impl->FindAttribute("value")->Value();

            auto impl_for_code        = ImplCode(impl_name);
            m.converts[impl_for_code] = value;
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCEnum(const WisEnum& s, DocKind kind)
{
    auto        full_name = GetCFullTypename(s.name, "");
    std::string st_decl   = wis::format("typedef enum {} {{\n", full_name);

    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (auto& m : s.values) {
        st_decl += MakeValueDocumentation(s, m, wis::format("    Wis{}{} = {},", s.name, m.name, m.value), kind);
    }

    st_decl += wis::format("}} {};\n", full_name);
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPEnum(const WisEnum& s, DocKind kind)
{
    std::string st_decl = wis::format("enum class {} {{\n", s.name);

    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (auto& m : s.values) {
        st_decl += MakeValueDocumentation<Lang::CPP>(s, m, wis::format("    {} = {},", m.name, m.value), kind);
    }

    st_decl += "};\n";
    return st_decl;
}

//-----------------------------------------------------------------------------
void Generator::WriteEnumDocumentation(std::filesystem::path enum_output_path)
{
    std::filesystem::create_directories(enum_output_path);
    for (auto& enum_name : enums_in_order) {
        // Make a folder for enums starting with this letter
        std::filesystem::path enum_file_path = enum_output_path / wis::format("{}_enum.h", MakeSnakeCase(enum_name));
        auto&                 enum_ref       = enum_map[enum_name];

        std::string enum_template_content = wis::format(" * C version:\n```c\n{}```\n"
                                            "C++ version:\n```cpp\nnamespace wis{{\n{}}}\n```\n",
                                            MakeCEnum(enum_ref, DocKind::VersionOnly),
                                            MakeCPPEnum(enum_ref, DocKind::VersionOnly));
        std::string enum_description      = wis::format(" * {}", MakeEnumDescription(enum_ref));
        std::string enum_refs             = GetRefs(enum_name);
        ReplaceAll(enum_template_content, "\n", "\n * ");
        ReplaceAll(enum_description, "\n", "\n * ");
        ReplaceAll(enum_refs, "\n", "\n * ");

        enum_description = FinalizeCDocumentation(enum_description, enum_name);

        WriteDocumentation(enum_file_path,
                           template_enum,
                           GetCFullTypename(enum_name, ""),
                           enum_template_content,
                           empty_doc,
                           enum_description,
                           enum_refs);
    }
}

//-----------------------------------------------------------------------------
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
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
    auto name = type->FindAttribute("name")->Value();
    enums_in_order.push_back(name);
    module_map[active_module_name].enums_in_order.push_back(name);
    type_map[name] = TypeKind::Enum;
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
        auto backend       = ParseBackend(impl_for);
        auto impl_name     = impl_type->FindAttribute("name")->Value();

        std::string_view def_value;
        if (auto xdefault = impl_type->FindAttribute("default")) {
            def_value = xdefault->Value();
        }

        if (auto direct = impl_type->FindAttribute("direct")) {
            ref.conversion_type[static_cast<size_t>(backend)] = WisConvert{ impl_name, def_value, true };
            continue;
        }

        ref.conversion_type[static_cast<size_t>(backend)] = WisConvert{ impl_name, def_value, false };
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

            auto backend = ParseBackend(impl_name);
            m.converts[static_cast<size_t>(backend)] = value;
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCEnum(const WisEnum& s, DocKind kind)
{
    auto        full_name = GetCFullTypename(s.name, Backend::Any);
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
    auto module_it = module_map.find(active_module_name);
    auto& enum_names = module_it != module_map.end() ? module_it->second.enums_in_order : enums_in_order;
    for (auto& enum_name : enum_names) {
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
                           GetCFullTypename(enum_name, Backend::Any),
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
    static constexpr std::array<std::string_view, 3> impl_names{
        "Common",
        "DirectX 12",
        "Vulkan",
    };

    std::string translates    = "\\note Translates to ";
    bool        has_translate = false;
    for (size_t i = 1; i < s.conversion_type.size(); ++i) {
        auto& cvt = s.conversion_type[i];
        if (cvt.value.empty()) {
            continue;
        }

        translates += wis::format("{} `{}` for {} implementation", has_translate ? ", and" : "", cvt.value, impl_names[i]);
        has_translate = true;
    }
    if (has_translate) {
        description += translates + ".\n\n";
    }

    description += "Values:\n";
    for (auto& m : s.values) {
        description += wis::format("- `Wis{}{} = {}`: {}\n", s.name, m.name, m.value, m.doc);
    }
    return description;
}

std::string Generator::MakeEnumConverter(const WisEnum& s, Backend backend)
{
    std::string converters;
    auto        backend_tag = GetBackendTag(backend);
    auto&       cvt     = s.conversion_type[static_cast<size_t>(backend)];
    if (cvt.value.empty()) {
        return converters;
    }
    if (cvt.direct) {
        converters = wis::format("constexpr inline {} convert_{}({} value) noexcept {{\n    return static_cast<{}>(value);\n}}\n\n",
                                 cvt.value,
                                 backend_tag,
                                 GetCFullTypename(s.name, backend),
                                 cvt.value);
    } else {
        converters = wis::format("constexpr inline {} convert_{}({} value) noexcept {{\n    switch(value) {{\n",
                                 cvt.value,
                                 backend_tag,
                                 GetCFullTypename(s.name, backend));
        for (auto& m : s.values) {
            auto convert_value = m.converts[static_cast<size_t>(backend)];
            if (convert_value.empty()) {
                continue;
            }
            converters += wis::format("    case {}: return {};\n",
                                      wis::format("{}{}",
                                                   GetCFullTypename(s.name, backend),
                                                  m.name),
                                      convert_value);
        }

        if (!cvt.default_value.empty()) {
            converters += wis::format("    default: return {};\n    }}\n}}\n\n", cvt.default_value);
        } else {
            converters += wis::format("    default: return static_cast<{}>(0);\n    }}\n}}\n\n",
                                      cvt.value);
        }
    }
    return converters;
}

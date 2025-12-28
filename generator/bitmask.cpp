#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char template_bitmask[] =
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
void Generator::ParseBitmask(tinyxml2::XMLElement* type)
{
    auto  name = type->FindAttribute("name")->Value();
    auto& ref  = bitmask_map[name];
    bitmasks_in_order.push_back(name);
    ref.name = name;

    // Unused currently, but keep for u64 flags
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

        if (auto direct = impl_type->FindAttribute("direct")) {
            ref.conversion_type[static_cast<size_t>(impl_for_code)] = WisConvert{ impl_name, true };
            continue;
        }
        ref.conversion_type[static_cast<size_t>(impl_for_code)] = WisConvert{ impl_name, false };
    }

    for (auto* member = type->FirstChildElement("value"); member;
         member       = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        m.name      = member->FindAttribute("name")->Value();
        auto* value = member->FindAttribute("value");
        auto* bit   = member->FindAttribute("bit");

        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }

        if (value) {
            m.value_or_bit = std::stoull(value->Value());
            m.is_bit       = false;
            continue;
        }

        if (auto* size = member->FindAttribute("version")) {
            m.version = size->Value();
        }

        m.value_or_bit = std::stoul(bit->Value());
        m.is_bit       = true;

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
std::string Generator::MakeCBitmask(const WisBitmask& s, DocKind kind)
{
    auto        full_name = GetCFullTypename(s.name, "");
    std::string st_decl   = wis::format("typedef enum {} {{\n", full_name);

    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (auto& m : s.values) {
        if (m.is_bit) {
            st_decl += MakeValueDocumentation(s, m, wis::format("    Wis{}{} = (1 << {}),", s.name, m.name, m.value_or_bit), kind);
            continue;
        }
        st_decl += MakeValueDocumentation(s, m, wis::format("    Wis{}{} = {},", s.name, m.name, m.value_or_bit), kind);
    }

    st_decl += wis::format("}} {};\n", full_name);
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPBitmask(const WisBitmask& s, DocKind kind)
{
    std::string st_decl = wis::format("enum class {} : uint32_t {{\n", s.name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }
    for (auto& m : s.values) {
        if (m.is_bit) {
            st_decl += MakeValueDocumentation<Lang::CPP>(s, m, wis::format("    {} = (1 << {}),", m.name, m.value_or_bit), kind);
            continue;
        }
        st_decl += MakeValueDocumentation<Lang::CPP>(s, m, wis::format("    {} = {},", m.name, m.value_or_bit), kind);
    }
    st_decl += "};\n";
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeBitmaskDescription(const WisBitmask& s)
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
        if (has_translate) {
            translates += ", ";
        }
        translates += wis::format("{} as {}", impl_names[i], cvt.value);
        has_translate = true;
    }
    if (has_translate) {
        description += translates + ".\n";
    }
    description += "Values:\n";
    for (auto& m : s.values) {
        if (m.is_bit) {
            description += wis::format("- `Wis{}{} = (1 << {})`: {}\n", s.name, m.name, m.value_or_bit, m.doc);
            continue;
        }
        description += wis::format("- `Wis{}{} = {}`: {}\n", s.name, m.name, m.value_or_bit, m.doc);
    }
    return description;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeBitmaskConverter(const WisBitmask& s, std::string_view impl)
{
    std::string converters;
    auto        impl_code = ImplCode(impl);
    auto&       cvt       = s.conversion_type[static_cast<size_t>(impl_code)];
    if (cvt.value.empty()) {
        return converters;
    }
    if (cvt.direct) {
        converters = wis::format("inline {} convert_{}({} value) noexcept {{\n    return static_cast<{}>(value);\n}}\n\n",
                                 cvt.value,
                                 impl,
                                 GetCFullTypename(s.name, impl),
                                 cvt.value);
    } else {
        converters = wis::format("inline {} convert_{}({} value) noexcept {{\n",
                                 cvt.value,
                                 impl,
                                 GetCFullTypename(s.name, impl));

        // Start with default value
        converters += wis::format("    {} result = static_cast<{}>(0);\n",
                                  cvt.value,
                                  cvt.value);

        for (auto& m : s.values) {
            auto convert_value = m.converts[static_cast<size_t>(impl_code)];
            if (convert_value.empty()) {
                continue;
            }
            converters += wis::format("    if (value & {}{}) {{ result |= {}; }}\n",
                                      GetCFullTypename(s.name, impl),
                                      m.name,
                                      convert_value);
        }
        converters += wis::format("    return result;\n}}\n\n");
    }
    return converters;
}

//-----------------------------------------------------------------------------
void Generator::WriteBitmaskDocumentation(std::filesystem::path enum_output_path)
{
    std::filesystem::create_directories(enum_output_path);
    for (auto& enum_name : bitmasks_in_order) {
        // Make a folder for enums starting with this letter
        std::filesystem::path enum_file_path = enum_output_path / wis::format("{}_enum.h", MakeSnakeCase(enum_name));
        auto&                 enum_ref       = bitmask_map[enum_name];

        std::string enum_template_content = wis::format(" * C version:\n```c\n{}```\n"
                                                        "C++ version:\n```cpp\nnamespace wis{{\n{}}}\n```\n",
                                                        MakeCBitmask(enum_ref, DocKind::VersionOnly),
                                                        MakeCPPBitmask(enum_ref, DocKind::VersionOnly));
        std::string enum_description      = wis::format(" * {}", MakeBitmaskDescription(enum_ref));
        std::string enum_refs             = GetRefs(enum_name);
        ReplaceAll(enum_template_content, "\n", "\n * ");
        ReplaceAll(enum_description, "\n", "\n * ");
        ReplaceAll(enum_refs, "\n", "\n * ");

        enum_description = FinalizeCDocumentation(enum_description, enum_name);

        WriteDocumentation(enum_file_path,
                           template_bitmask,
                           GetCFullTypename(enum_name, ""),
                           enum_template_content,
                           empty_doc,
                           enum_description,
                           enum_refs);
    }
}
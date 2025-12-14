#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char template_struct[] =
        R"(/**
 * @struct {0}
 * @ingroup Structures
 *
 * 
 * @section {0}_spec Specification
 * <hr>
 * 
 * \cond WIS_GEN_CODE
 * {1}
 * \endcond
 * 
 * @section {0}_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * {2}
 * \endcond
 *
 * @section {0}_descr Description
 * <hr>
 * 
 * \cond WIS_GEN_WIS_IDS 
 * \endcond
 *
 * @section {0}_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */)";

//-----------------------------------------------------------------------------
void Generator::ParseStruct(tinyxml2::XMLElement* type)
{
    auto  name = type->FindAttribute("name")->Value();
    auto& ref  = struct_map[name];
    structs_in_order.emplace_back(name);
    ref.name = name;

    if (auto* size = type->FindAttribute("doc")) {
        ref.doc = size->Value();
    }

    if (auto* size = type->FindAttribute("version")) {
        ref.version = size->Value();
    } else {
        throw std::runtime_error(wis::format("Struct {} is missing version attribute.", name));
    }

    if (auto* mod = type->FindAttribute("mod")) {
        ref.modifier = GetModifiers(mod->Value());
    }

    for (auto* member = type->FirstChildElement("member"); member;
         member       = member->NextSiblingElement("member")) {
        auto& m = ref.members.emplace_back();

        auto* type = member->FindAttribute("type")->Value();
        auto* name = member->FindAttribute("name")->Value();

        auto* arr = member->FindAttribute("array");

        // Ref the type
        TryMakeRef(type, ref.name);

        m.type = type;
        m.name = name;
        if (auto* arr = member->FindAttribute("array")) {
            m.array_size = arr->Value();
        }
        if (auto* def = member->FindAttribute("default")) {
            m.default_value = def->Value();
        }
        if (auto* mod = member->FindAttribute("mod")) {
            m.modifier = GetModifiers(mod->Value());
        }
        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCStruct(const WisStruct& s, DocKind kind)
{
    auto        full_name = GetCFullTypename(s.name, "");
    std::string st_decl   = wis::format("typedef struct {} {} {{\n", s.modifier & Modifier::Nodiscard ? "WIS_NODISCARD" : "", full_name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    // Calculate maximum type length for alignment
    size_t max_type_length = 0;
    for (auto& m : s.members) {
        size_t type_length = GetMemberTypeString(m).length();
        max_type_length    = std::max(max_type_length, type_length);
    }

    for (auto& m : s.members) {
        st_decl += MakeValueDocumentation(s, m, MakeCMemberDeclaration(m, max_type_length), kind);
    }
    st_decl += wis::format("}} {};\n\n", full_name);
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPStruct(const WisStruct& s, DocKind kind)
{
    std::string st_decl = wis::format("struct {} {} {{\n", s.modifier & Modifier::Nodiscard ? "WIS_NODISCARD" : "", s.name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    // Calculate maximum type length for alignment
    size_t max_type_length = 0;
    for (auto& m : s.members) {
        size_t type_length = GetMemberTypeString<Lang::CPP>(m).length();
        max_type_length    = std::max(max_type_length, type_length);
    }

    bool prev_span = false;
    for (auto& m : s.members) {
        if (prev_span) {
            prev_span = false;
            continue;
        }

        st_decl += MakeValueDocumentation<Lang::CPP>(s, m, MakeCPPMemberDeclaration(m, max_type_length, ""), kind);
        prev_span = m.modifier & Modifier::Span;
    }
    st_decl += "};\n";
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCMemberDeclaration(const WisStructMember& member, size_t align_width, std::string_view impl)
{
    std::string type_string = GetMemberTypeString(member, impl);
    std::string array_modifier;

    if (!member.array_size.empty()) {
        array_modifier = wis::format("[{}]", member.array_size);
    }

    // Pad the type string to align_width
    size_t      padding     = align_width > type_string.length() ? align_width - type_string.length() : 0;
    std::string padded_type = type_string + std::string(padding, ' ');

    return std::format("    {} {}{};", padded_type, member.name, array_modifier);
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPMemberDeclaration(const WisStructMember& member, size_t align_width, std::string_view impl)
{
    std::string type_string = GetMemberTypeString<Lang::CPP>(member, impl);

    if (!member.array_size.empty()) {
        type_string = wis::format("std::array<{}, {}>", type_string, member.array_size);
    }

    // Pad the type string to align_width
    size_t      padding     = align_width > type_string.length() ? align_width - type_string.length() : 0;
    std::string padded_type = type_string + std::string(padding, ' ');

    return std::format("    {} {};", padded_type, member.name);
}

//-----------------------------------------------------------------------------
std::string Generator::MakeStructDescription(const WisStruct& s)
{
    std::string description;
    for (auto& m : s.members) {
        description += wis::format("- `{}` {}\n", m.name, m.doc.empty() ? "No description." : m.doc);
    }
    return description;
}

//-----------------------------------------------------------------------------
void Generator::WriteStructDocumentation(std::filesystem::path struct_output_path)
{
    std::filesystem::create_directories(struct_output_path);
    for (const auto& struct_name : structs_in_order) {
        // Make a folder for enums starting with this letter
        std::filesystem::path struct_file_path = struct_output_path / wis::format("{}_struct.h", MakeSnakeCase(struct_name));
        auto&                 struct_ref       = struct_map[struct_name];

        std::string struct_template_content = wis::format(" * C version:\n```c\n{}```\n"
                                                          "C++ version:\n```cpp\nnamespace wis{{\n{}}}\n```\n",
                                                          MakeCStruct(struct_ref, DocKind::VersionOnly),
                                                          MakeCPPStruct(struct_ref, DocKind::VersionOnly));

        std::string struct_description = wis::format(" * {}", MakeStructDescription(struct_ref));
        std::string struct_refs        = GetRefs(struct_name);
        std::string vuids              = MakeValidationForType(struct_name);

        ReplaceAll(struct_template_content, "\n", "\n * ");
        ReplaceAll(struct_description, "\n", "\n * ");
        ReplaceAll(struct_refs, "\n", "\n * ");
        struct_description = FinalizeCDocumentation(struct_description, struct_name);

        WriteDocumentation(struct_file_path,
                           template_struct,
                           GetCFullTypename(struct_name, ""),
                           struct_template_content,
                           vuids,
                           struct_description,
                           struct_refs);
    }
}
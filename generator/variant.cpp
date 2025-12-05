#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char template_struct[] =
        R"(/**
 * @struct {0}
 * @ingroup Structures
 *
 * 
 * @section {0}_spec C Specification
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
void Generator::ParseVariant(tinyxml2::XMLElement* type)
{
    auto  name = type->FindAttribute("name")->Value();
    auto& ref  = variant_map[name];
    variants_in_order.emplace_back(name);
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
std::string Generator::MakeCVariant(const WisStruct& s, std::string_view impl, DocKind kind)
{
    ImplementedFor impl_code = ImplCode(impl);
    auto           full_name = GetCFullTypename(s.name, GetImplString(impl_code));
    std::string    st_decl   = wis::format("typedef struct {} {} {{\n", s.modifier & Modifier::Nodiscard ? "WIS_NODISCARD" : "", full_name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    // Calculate maximum type length for alignment
    size_t max_type_length = 0;
    for (auto& m : s.members) {
        size_t type_length = GetMemberTypeString(m, impl).length();
        max_type_length    = std::max(max_type_length, type_length);
    }

    for (auto& m : s.members) {
        st_decl += MakeCValueDocumentation(s, m, MakeCMemberDeclaration(m, max_type_length, impl), kind);
    }
    st_decl += wis::format("}} {};\n\n", full_name);
    return st_decl;
}

std::string Generator::MakeVariantDescription(const WisStruct& s)
{
    std::string description;
    for (auto& m : s.members) {
        description += wis::format("- `{}` {}\n", m.name, m.doc.empty() ? "No description." : m.doc);
    }
    return description;
}

//-----------------------------------------------------------------------------
void Generator::WriteVariantDocumentation(std::filesystem::path struct_output_path)
{
    std::filesystem::create_directories(struct_output_path);
    for (const auto& variant_name : variants_in_order) {
        // Make a folder for enums starting with this letter
        std::filesystem::path variant_file_path = struct_output_path / wis::format("{}_struct.h", MakeSnakeCase(variant_name));
        auto&                 variant_ref       = variant_map[variant_name];

        std::string vk_code      = MakeCVariant(variant_ref, "vk", DocKind::VersionOnly);
        std::string dx_code      = MakeCVariant(variant_ref, "dx", DocKind::VersionOnly);
        std::string regular_code = MakeCVariant(variant_ref, "", DocKind::VersionOnly);

        std::string variant_template_content = wis::format(" * General Version:\n```c\n{}```\nVulkan Version:\n```c\n{}```\nDX12 Version:\n```c\n{}```\n", regular_code, vk_code, dx_code);
        std::string variant_description      = wis::format(" * {}", MakeVariantDescription(variant_ref));
        std::string variant_refs             = GetRefs(variant_name);
        std::string vuids                    = MakeValidationForType(variant_name);

        ReplaceAll(variant_template_content, "\n", "\n * ");
        ReplaceAll(variant_description, "\n", "\n * ");
        ReplaceAll(variant_refs, "\n", "\n * ");
        variant_description = FinalizeCDocumentation(variant_description, variant_name);

        WriteDocumentation(variant_file_path,
                           template_struct,
                           GetCFullTypename(variant_name, ""),
                           variant_template_content,
                           vuids,
                           variant_description,
                           variant_refs);
    }
}
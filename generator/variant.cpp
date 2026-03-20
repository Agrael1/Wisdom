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
void Generator::ParseVariant(tinyxml2::XMLElement* type)
{
    auto  name = type->FindAttribute("name")->Value();
    auto& ref  = variant_map[name];
    variants_in_order.emplace_back(name);
    module_map[active_module_name].variants_in_order.emplace_back(name);
    type_map[name] = TypeKind::Variant;
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
        ref.FilterBackend(GetTypeBackendSupport(type));

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
std::string Generator::MakeCVariant(const WisStruct& s, Backend backend, DocKind kind)
{
    auto    impl_suffix = GetBackendSuffix(backend);
    auto           full_name   = GetCFullTypename(s.name, backend);
    std::string    st_decl   = wis::format("typedef struct {}{} {{\n", s.modifier & Modifier::Nodiscard ? "WIS_NODISCARD " : "", full_name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    // Calculate maximum type length for alignment
    size_t max_type_length = 0;
    for (auto& m : s.members) {
        size_t type_length = GetMemberTypeString(m, backend).length();
        max_type_length    = std::max(max_type_length, type_length);
    }

    for (auto& m : s.members) {
        st_decl += MakeValueDocumentation(s, m, MakeCMemberDeclaration(m, max_type_length, backend), kind);
    }
    st_decl += wis::format("}} {};\n", full_name);
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPVariant(const WisStruct& s, Backend backend, DocKind kind)
{
    if (s.modifier & Modifier::COnly) {
        return "";
    }

    auto    impl_suffix = GetBackendSuffix(backend);
    std::string    st_decl   = wis::format("struct {}{}{} {{\n",
                                      s.modifier & Modifier::Nodiscard ? "WIS_NODISCARD " : "",
                                      impl_suffix,
                                      s.name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    // Calculate maximum type length for alignment
    size_t max_type_length = 0;
    for (auto& m : s.members) {
        size_t type_length = GetMemberTypeString<Lang::CPP>(m, backend).length();
        max_type_length    = std::max(max_type_length, type_length);
    }

    bool prev_span = false;
    for (auto& m : s.members) {
        if (prev_span) {
            prev_span = false;
            continue;
        }

        st_decl += MakeValueDocumentation<Lang::CPP>(s, m, MakeCPPMemberDeclaration(m, max_type_length, backend), kind);
        prev_span = m.modifier & Modifier::Span;
    }
    st_decl += "};\n";
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
    auto module_it = module_map.find(active_module_name);
    auto& variant_names = module_it != module_map.end() ? module_it->second.variants_in_order : variants_in_order;
    for (const auto& variant_name : variant_names) {
        // Make a folder for enums starting with this letter
        std::filesystem::path variant_file_path = struct_output_path / wis::format("{}_struct.h", MakeSnakeCase(variant_name));
        auto&                 variant_ref       = variant_map[variant_name];

        auto supports_vk = has(variant_ref.backend, Backend::Vulkan);
        auto supports_dx = has(variant_ref.backend, Backend::DX12);

        std::string vk_code      = supports_vk ? MakeCVariant(variant_ref, Backend::Vulkan, DocKind::VersionOnly) : "";
        std::string dx_code      = supports_dx ? MakeCVariant(variant_ref, Backend::DX12, DocKind::VersionOnly) : "";
        std::string regular_code = supports_vk && supports_dx ? MakeCVariant(variant_ref, Backend::Any, DocKind::VersionOnly) : "";

        std::string c_code     = regular_code;
        std::string cimpl_code = supports_vk && supports_dx
                ? (vk_code + '\n' + dx_code)
                : "";
        if (c_code.empty()) {
            c_code = !vk_code.empty() ? vk_code : dx_code;
        }

        std::string vk_cpp = variant_ref.modifier & Modifier::COnly || !supports_vk
                ? ""
                : MakeCPPVariant(variant_ref, Backend::Vulkan, DocKind::VersionOnly);
        std::string dx_cpp = variant_ref.modifier & Modifier::COnly || !supports_dx
                ? ""
                : MakeCPPVariant(variant_ref, Backend::DX12, DocKind::VersionOnly);
        std::string regular_code_cpp = variant_ref.modifier & Modifier::COnly || !(supports_vk && supports_dx)
                ? ""
                : MakeCPPVariant(variant_ref, Backend::Any, DocKind::VersionOnly);
        std::string cpp_code      = regular_code_cpp;
        std::string cimpl_code_cpp = variant_ref.modifier & Modifier::COnly || !(supports_vk && supports_dx)
                ? ""
                : vk_cpp + '\n' + dx_cpp;
        if (cpp_code.empty()) {
            cpp_code = !vk_cpp.empty() ? vk_cpp : dx_cpp;
        }

        std::string variant_template_content = GetSpecificationCode(c_code, cimpl_code, cpp_code, cimpl_code_cpp);

        std::string variant_description = wis::format(" * {}", MakeVariantDescription(variant_ref));
        std::string variant_refs        = GetRefs(variant_name);
        std::string vuids               = MakeValidationForType(variant_name);

        ReplaceAll(variant_description, "\n", "\n * ");
        ReplaceAll(variant_refs, "\n", "\n * ");
        variant_description = FinalizeCDocumentation(variant_description, variant_name);

        WriteDocumentation(variant_file_path,
                           template_struct,
                           GetCFullTypename(variant_name, Backend::Any),
                           variant_template_content,
                           vuids,
                           variant_description,
                           variant_refs);
    }
}

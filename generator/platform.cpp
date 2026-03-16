#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char template_platform[] =
        R"(/**
 * @struct {0}
 * @ingroup Handles
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
 */)"; // TODO: better doc for platforms

static auto to_upper(std::string_view str)
{
    std::string result;
    result.reserve(str.size());
    for (char c : str) {
        result += std::toupper(c);
    }
    return result;
};

//-----------------------------------------------------------------------------
void Generator::ParsePlatforms(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("platform"); type;
         type       = type->NextSiblingElement("platform")) {

        auto  name    = type->FindAttribute("name")->Value();
        auto  version = type->FindAttribute("version")->Value();
        auto& ref     = platform_map[name];
        platforms_in_order.emplace_back(name);

        ref.name    = name;
        ref.version = version;

        if (auto* doc = type->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        // Parse implementations
        if (auto* backend = type->FindAttribute("backend")) {
            ref.impl = ImplCode(backend->Value());
        }

        if (auto* os = type->FindAttribute("os")) {
            ref.os = ImplOs(os->Value());
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPlatform(const WisPlatform& p, DocKind kind)
{
    auto upper_name = to_upper(p.name);
    std::string st_decl = wis::format("#ifdef WIS_USE_PLATFORM_{}\n\n", upper_name);
    if (!p.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(p, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (const auto& struct_name : p.structs_in_order) {
        auto& struct_ref = struct_map[struct_name];
        st_decl += MakeCStruct(struct_ref, kind);
        st_decl += "\n";
    }

    st_decl += wis::format("#endif // WIS_USE_PLATFORM_{}\n\n", upper_name);

    return st_decl;
}

std::string Generator::MakeCPPPlatform(const WisPlatform& p, DocKind kind)
{
    auto        upper_name = to_upper(p.name);
    std::string st_decl    = wis::format("#ifdef WIS_USE_PLATFORM_{}\n\n", upper_name);
    if (!p.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(p, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (const auto& struct_name : p.structs_in_order) {
        auto& struct_ref = struct_map[struct_name];
        st_decl += MakeCPPStruct(struct_ref, kind);
        st_decl += "\n";
    }

    st_decl += wis::format("#endif // WIS_USE_PLATFORM_{}\n\n", upper_name);

    return st_decl;
}
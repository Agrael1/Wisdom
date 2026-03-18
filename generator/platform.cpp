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
            ref.os = GetImplOs(os->Value());
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPlatform(const WisPlatform& p, DocKind kind)
{
    auto        upper_name = to_upper(p.name);
    std::string st_decl;
    if (!p.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(p, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (const auto& struct_name : p.structs_in_order) {
        auto& struct_ref = struct_map[struct_name];
        st_decl += MakeCStruct(struct_ref, kind);
        st_decl += "\n";
    }

    // dependent API elements (handles, functions)

    if (p.impl == ImplementedFor::Both || p.impl == ImplementedFor::DX12) {
        st_decl += wis::format("#if defined(WISDOM_DX12)\n\n");

        for (const auto& handle_name : p.handles_in_order) {
            auto& handle_ref = handle_map[handle_name];
            st_decl += MakeCHandle(handle_ref, "dx", kind);
            st_decl += "\n";
        }
        for (const auto& func_name : p.functions_in_order) {
            auto& func_ref = function_map[func_name];
            st_decl += MakeCFunctionDecl(func_ref, "dx", "WISDOM_PLATFORM_API ", kind);
            st_decl += "\n";
        }

        st_decl += wis::format("#endif // defined(WISDOM_DX12)\n\n");
    }

    if (p.impl == ImplementedFor::Both || p.impl == ImplementedFor::Vulkan) {
        st_decl += wis::format("#if defined(WISDOM_VULKAN)\n\n");

        for (const auto& handle_name : p.handles_in_order) {
            auto& handle_ref = handle_map[handle_name];
            st_decl += MakeCHandle(handle_ref, "vk", kind);
            st_decl += "\n";
        }
        for (const auto& func_name : p.functions_in_order) {
            auto& func_ref = function_map[func_name];
            st_decl += MakeCFunctionDecl(func_ref, "vk", "WISDOM_PLATFORM_API ", kind);
            st_decl += "\n";
        }

        st_decl += wis::format("#endif // defined(WISDOM_VULKAN)\n\n");
    }

    return st_decl;
}

std::string Generator::MakeCPPPlatform(const WisPlatform& p, DocKind kind)
{
    auto        upper_name = to_upper(p.name);
    std::string st_decl;
    if (!p.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(p, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    for (const auto& struct_name : p.structs_in_order) {
        auto& struct_ref = struct_map[struct_name];
        st_decl += MakeCPPStruct(struct_ref, kind);
        st_decl += "\n";
    }

    // dependent API elements (handles, functions)

    if (p.impl == ImplementedFor::Both || p.impl == ImplementedFor::DX12) {
        st_decl += wis::format("#if defined(WISDOM_DX12)\n\n");
        for (const auto& handle_name : p.handles_in_order) {
            auto& handle_ref = handle_map[handle_name];
            st_decl += MakeCPPHandle(handle_ref, "dx", kind);
            st_decl += "\n";
        }

        st_decl += wis::format("#endif // defined(WISDOM_DX12)\n\n");
    }

    if (p.impl == ImplementedFor::Both || p.impl == ImplementedFor::Vulkan) {
        st_decl += wis::format("#if defined(WISDOM_VULKAN)\n\n");
        for (const auto& handle_name : p.handles_in_order) {
            auto& handle_ref = handle_map[handle_name];
            st_decl += MakeCPPHandle(handle_ref, "vk", kind);
            st_decl += "\n";
        }

        st_decl += wis::format("#endif // defined(WISDOM_VULKAN)\n\n");
    }

    return st_decl;
}

std::string Generator::MakeCIndependentPlatform(const WisPlatform& p, std::string_view impl, DocKind kind)
{
    if (p.impl != ImplementedFor::Both && p.impl != ImplCode(impl)) {
        return "";
    }

    auto        upper_name = to_upper(p.name);
    std::string st_decl;

    // handles
    for (auto& handle_name : p.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        st_decl += wis::format("typedef struct {} {};\n", GetCFullTypename(handle_def.name, impl), GetCFullTypename(handle_def.name));
    }

    // functions
    for (auto& func_name : p.functions_in_order) {
        auto& func_def = function_map[func_name];
        st_decl += wis::format("#define {} {}\n",
                               wis::format("wis{}{}", func_def.modifier & (Destroy | Construct) ? "" : func_def.this_type, func_def.name),
                               wis::format("wis{}{}{}", impl, func_def.modifier & (Destroy | Construct) ? "" : func_def.this_type, func_def.name));
    }

    return st_decl;
}

std::string Generator::MakeCPPIndependentPlatform(const WisPlatform& p, std::string_view impl, DocKind kind)
{
    if (p.impl != ImplementedFor::Both && p.impl != ImplCode(impl)) {
        return "";
    }
    auto        upper_name = to_upper(p.name);
    std::string st_decl;

    for (auto& handle_name : p.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        st_decl += wis::format("using {} = {};\n", handle_def.name, GetCPPFullTypename(handle_def.name, impl));
    }

    return st_decl;
}

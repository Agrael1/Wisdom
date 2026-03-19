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
std::string Generator::MakeCPlatform(const WisModule& p, DocKind kind)
{
    std::string st_decl;

    for (const auto& struct_name : p.structs_in_order) {
        auto& struct_ref = struct_map[struct_name];
        st_decl += MakeCStruct(struct_ref, kind);
        st_decl += "\n";
    }

    // dependent API elements (handles, functions)

    st_decl += wis::format("#if defined(WISDOM_DX12)\n\n");

    for (const auto& handle_name : p.handles_in_order) {
        auto& handle_ref = handle_map[handle_name];
        if (handle_ref.GetSize(Backend::DX12) > 0) {
            st_decl += MakeCHandle(handle_ref, Backend::DX12, kind);
            st_decl += "\n";
        }
    }
    for (const auto& func_name : p.functions_in_order) {
        auto& func_ref = function_map[func_name];
        if (func_ref.this_type.empty() || handle_map[func_ref.this_type].GetSize(Backend::DX12) > 0) {
            st_decl += MakeCFunctionDecl(func_ref, Backend::DX12, "WISDOM_PLATFORM_API ", kind);
            st_decl += "\n";
        }
    }
    st_decl += wis::format("#endif // defined(WISDOM_DX12)\n\n");

    st_decl += wis::format("#if defined(WISDOM_VULKAN)\n\n");

    for (const auto& handle_name : p.handles_in_order) {
        auto& handle_ref = handle_map[handle_name];
        if (handle_ref.GetSize(Backend::Vulkan) > 0) {
            st_decl += MakeCHandle(handle_ref, Backend::Vulkan, kind);
            st_decl += "\n";
        }
    }
    for (const auto& func_name : p.functions_in_order) {
        auto& func_ref = function_map[func_name];
        if (func_ref.this_type.empty() || handle_map[func_ref.this_type].GetSize(Backend::Vulkan) > 0) {
            st_decl += MakeCFunctionDecl(func_ref, Backend::Vulkan, "WISDOM_PLATFORM_API ", kind);
            st_decl += "\n";
        }
    }
    st_decl += wis::format("#endif // defined(WISDOM_VULKAN)\n\n");

    return st_decl;
}

std::string Generator::MakeCPPPlatform(const WisModule& p, DocKind kind)
{
    std::string st_decl;

    for (const auto& struct_name : p.structs_in_order) {
        auto& struct_ref = struct_map[struct_name];
        st_decl += MakeCPPStruct(struct_ref, kind);
        st_decl += "\n";
    }

    // dependent API elements (handles, functions)

    st_decl += wis::format("#if defined(WISDOM_DX12)\n\n");
    for (const auto& handle_name : p.handles_in_order) {
        auto& handle_ref = handle_map[handle_name];
        if (handle_ref.GetSize(Backend::DX12) > 0) {
            st_decl += MakeCPPHandle(handle_ref, Backend::DX12, kind);
            st_decl += "\n";
        }
    }
    st_decl += wis::format("#endif // defined(WISDOM_DX12)\n\n");

    st_decl += wis::format("#if defined(WISDOM_VULKAN)\n\n");
    for (const auto& handle_name : p.handles_in_order) {
        auto& handle_ref = handle_map[handle_name];
        if (handle_ref.GetSize(Backend::Vulkan) > 0) {
            st_decl += MakeCPPHandle(handle_ref, Backend::Vulkan, kind);
            st_decl += "\n";
        }
    }
    st_decl += wis::format("#endif // defined(WISDOM_VULKAN)\n\n");

    return st_decl;
}

std::string Generator::MakeCIndependentPlatform(const WisModule& p, Backend backend, DocKind kind)
{
    (void)kind;
    std::string st_decl;

    // handles
    for (auto& handle_name : p.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetSize(backend) > 0) {
            st_decl += wis::format("typedef struct {} {};\n", GetCFullTypename(handle_def.name, backend), GetCFullTypename(handle_def.name));
        }
    }

    // functions
    for (auto& func_name : p.functions_in_order) {
        auto& func_def = function_map[func_name];
        if (func_def.this_type.empty() || handle_map[func_def.this_type].GetSize(backend) > 0) {
            st_decl += wis::format("#define {} {}\n",
                                   wis::format("wis{}{}", func_def.modifier & (Destroy | Construct) ? "" : func_def.this_type, func_def.name),
                                   wis::format("wis{}{}{}", GetBackendSuffix(backend), func_def.modifier & (Destroy | Construct) ? "" : func_def.this_type, func_def.name));
        }
    }

    return st_decl;
}

std::string Generator::MakeCPPIndependentPlatform(const WisModule& p, Backend backend, DocKind kind)
{
    (void)kind;
    std::string st_decl;

    for (auto& handle_name : p.handles_in_order) {
        auto& handle_def = handle_map[handle_name];
        if (handle_def.GetSize(backend) > 0) {
            st_decl += wis::format("using {} = {};\n", handle_def.name, GetCPPFullTypename(handle_def.name, backend));
        }
    }

    return st_decl;
}

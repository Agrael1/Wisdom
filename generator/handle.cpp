#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char template_handle[] =
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
 * @section {0}_descr Description
 * <hr>
 *
 * @section {0}_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */)";

//-----------------------------------------------------------------------------
void Generator::ParseHandles(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("handle"); type;
         type       = type->NextSiblingElement("handle")) {

        auto  name    = type->FindAttribute("name")->Value();
        auto  version = type->FindAttribute("version")->Value();
        auto& ref     = handle_map[name];
        handles_in_order.emplace_back(name);

        ref.name    = name;
        ref.version = version;

        if (auto* doc = type->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        // Add destroy function
        std::string destr_name   = "Destroy" + std::string(name);
        std::string destr_doc    = "Destroys a {" + std::string(name) + "::} handle.";

        auto& xdestroy = destructors.emplace_back(destr_name + destr_doc);

        std::string_view xdestroy_name   = std::string_view(xdestroy.c_str(), destr_name.size());
        std::string_view xdestroy_doc  = std::string_view(xdestroy_name.data() + destr_name.size(), destr_doc.size());

        auto& destroy     = function_map[xdestroy_name];
        destroy.name      = xdestroy_name;
        destroy.this_type = name;
        destroy.modifier  = Modifier::Destroy;
        destroy.version   = version;
        destroy.doc       = xdestroy_doc;
        ref.functions.emplace_back(destroy.name);
        functions_in_order.emplace_back(destroy.name);
        dependency_tree[name].dependencies.emplace_back(destroy.name);


        // Parse implementations
        for (auto* impl = type->FirstChildElement("impl"); impl;
             impl       = impl->NextSiblingElement("impl")) {
            auto impl_for      = impl->FindAttribute("for")->Value();
            auto impl_for_code = ImplCode(impl_for);
            auto impl_file     = impl->FindAttribute("include")->Value();

            uint32_t size = impl->UnsignedAttribute("size", 0);
            if (impl_for_code == ImplementedFor::DX12) {
                ref.sizes[0] = size;
            } else if (impl_for_code == ImplementedFor::Vulkan) {
                ref.sizes[1] = size;
            }
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCHandle(const WisHandle& s, std::string_view impl, DocKind kind)
{
    ImplementedFor impl_code = ImplCode(impl);
    auto           full_name = GetCFullTypename(s.name, GetImplString(impl_code));
    std::string    st_decl   = wis::format("WIS_DEFINE_HANDLE({},{});\n", full_name, s.GetSize(impl_code));
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }
    return st_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPHandle(const WisHandle& s, std::string_view impl, DocKind kind)
{
    ImplementedFor impl_code   = ImplCode(impl);
    auto           impl_string = GetImplString(impl_code);
    auto           full_name   = GetCFullTypename(s.name, impl_string);

    std::string deleter = wis::format("struct {}{}Deleter {{\n    "
                                      "void operator()({}* handle) noexcept {{\n        ",
                                      impl_string,
                                      s.name,
                                      full_name);

    std::string st_decl = wis::format("class {}{} : public wis::impl::Implements<wis::impl::{}{}Impl,{}, wis::{}{}Deleter>{{\npublic:\n",
                                      impl_string,
                                      s.name,
                                      impl_string,
                                      s.name,
                                      full_name,
                                      impl_string,
                                      s.name);
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }

    // Use constructor from base
    st_decl += "    using ImplType::ImplType;\n";
    st_decl += "public:\n";

    // Add all the functions
    for (const auto& func_name : s.functions) {
        auto& func_ref = function_map[func_name];
        if (func_ref.modifier & Modifier::Destroy) {
            deleter += wis::format("            ::{}(handle);\n",
                                   GetCFullTypename(func_ref.name, impl_string));
            continue;
        }

        st_decl += MakeCPPFunctionImpl(func_ref, impl, "inline", kind);
    }
    deleter += "    }\n};\n";

    st_decl += "};\n";
    return deleter + st_decl;
}

//-----------------------------------------------------------------------------
void Generator::WriteHandleDocumentation(std::filesystem::path handle_output_path)
{
    for (const auto& handle_name : handles_in_order) {
        // Make a folder for enums starting with this letter
        std::filesystem::create_directories(handle_output_path);
        std::filesystem::path handle_file_path = handle_output_path / wis::format("{}_handle.h", MakeSnakeCase(handle_name));
        auto&                 handle_ref       = handle_map[handle_name];

        std::string vk_code = MakeCHandle(handle_ref, "vk", DocKind::VersionOnly);
        std::string dx_code = MakeCHandle(handle_ref, "dx", DocKind::VersionOnly);

        std::string handle_template_content = wis::format(" * Vulkan Version:\n```c\n{}```\nDX12 Version:\n```c\n{}```\n", vk_code, dx_code);
        std::string handle_refs             = GetRefs(handle_name);
        ReplaceAll(handle_template_content, "\n", "\n * ");
        ReplaceAll(handle_refs, "\n", "\n * ");

        WriteDocumentation(handle_file_path,
                           template_handle,
                           GetCFullTypename(handle_name, ""),
                           handle_template_content,
                           empty_doc,
                           empty_doc,
                           handle_refs);
    }
}
#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char template_handle[] =
        R"(/**
 * @struct {0}
 * @ingroup Handles
 *
 * 
 * @section {0}_spec C Specification
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

void Generator::ParseHandles(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("handle"); type;
         type = type->NextSiblingElement("handle")) {

        auto name = type->FindAttribute("name")->Value();
        auto version = type->FindAttribute("version")->Value();
        auto& ref = handle_map[name];
        handles_in_order.emplace_back(name);

        ref.name = name;
        ref.version = version;

        if (auto* doc = type->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        // Parse implementations
        for (auto* impl = type->FirstChildElement("impl"); impl;
             impl = impl->NextSiblingElement("impl")) {
            auto impl_for = impl->FindAttribute("for")->Value();
            auto impl_for_code = ImplCode(impl_for);
            auto impl_file = impl->FindAttribute("include")->Value();

            uint32_t size = impl->UnsignedAttribute("size", 0);
            if (impl_for_code == ImplementedFor::DX12) {
                ref.sizes[0] = size;
            } else if (impl_for_code == ImplementedFor::Vulkan) {
                ref.sizes[1] = size;
            }
        }
    }
}

std::string Generator::MakeCHandle(const WisHandle& s, std::string_view impl, DocKind kind)
{
    ImplementedFor impl_code = ImplCode(impl);
    auto full_name = GetCFullTypename(s.name, GetImplString(impl_code));
    std::string st_decl = wis::format("WIS_DEFINE_HANDLE({},{});\n", full_name, s.GetSize(impl_code));
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl = wis::format("{}\n{}", xdoc, st_decl);
    }
    return st_decl;
}

//-----------------------------------------------------------------------------
void Generator::WriteHandleDocumentation(std::filesystem::path handle_output_path)
{
    for (const auto& handle_name : handles_in_order) {
        // Make a folder for enums starting with this letter
        std::filesystem::create_directories(handle_output_path);
        std::filesystem::path handle_file_path = handle_output_path / wis::format("{}_handle.h", MakeSnakeCase(handle_name));
        auto& handle_ref = handle_map[handle_name];

        std::string vk_code = MakeCHandle(handle_ref, "vk", DocKind::VersionOnly);
        std::string dx_code = MakeCHandle(handle_ref, "dx", DocKind::VersionOnly);

        std::string handle_template_content = wis::format(" * Vulkan Version:\n```c\n{}```\nDX12 Version:\n```c\n{}```\n", vk_code, dx_code);
        std::string handle_refs = GetRefs(handle_name);
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
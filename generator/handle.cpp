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

        if (auto* platform = type->FindAttribute("platform")) {
            ref.platform = platform->Value();
            platform_map[ref.platform].handles_in_order.emplace_back(name);
        }

        if (auto* extends = type->FindAttribute("extends")) {
            ref.extends = GetExtends(extends->Value());
        }

        // Add destroy function
        std::string destr_name = "Destroy" + std::string(name);
        std::string destr_doc  = "Destroys a {" + std::string(name) + "::} handle.";

        auto& xdestroy = destructors.emplace_back(destr_name + destr_doc);

        std::string_view xdestroy_name = std::string_view(xdestroy.c_str(), destr_name.size());
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

        if (!ref.platform.empty()) {
            auto& platform = platform_map[ref.platform];
            platform.functions_in_order.emplace_back(destroy.name);
        }

        // Parse implementations
        for (auto* impl = type->FirstChildElement("impl"); impl;
             impl       = impl->NextSiblingElement("impl")) {
            auto impl_for      = impl->FindAttribute("for")->Value();
            auto impl_for_code = ImplCode(impl_for);

            uint32_t size = impl->UnsignedAttribute("size", 0);
            if (impl_for_code == ImplementedFor::DX12) {
                ref.sizes[0] = size;
            } else if (impl_for_code == ImplementedFor::Vulkan) {
                ref.sizes[1] = size;
            }
        }

        // view sizes
        bool has_view = false;
        for (auto* impl = type->FirstChildElement("view"); impl;
             impl       = impl->NextSiblingElement("view")) {
            has_view      = true;
            auto impl_for = impl->FindAttribute("for");
            if (!impl_for) {
                // if "for" attribute is missing, we can assume it's for both
                uint32_t size = impl->UnsignedAttribute("size", 0);
                for (size_t i = 0; i < ref.view_sizes.size(); ++i) {
                    ref.view_sizes[i] = size;
                }
                break;
            }

            auto impl_for_code = ImplCode(impl_for->Value());

            uint32_t size = impl->UnsignedAttribute("size", 0);
            if (impl_for_code == ImplementedFor::DX12) {
                ref.sizes[0] = size;
            } else if (impl_for_code == ImplementedFor::Vulkan) {
                ref.sizes[1] = size;
            }
        }

        if (has_view) {
            views_in_order.emplace_back(name);
            view_set.insert(name);
        }
    }
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCHandle(const WisHandle& s, std::string_view impl, DocKind kind)
{
    ImplementedFor impl_code = ImplCode(impl);
    auto           impl_string = GetImplString(impl_code);

    auto extends_macro = s.extends == Extends::None 
        ? std::string("WIS_DEFINE_HANDLE") 
        : (s.extends == Extends::Instance 
            ? wis::format("WIS_DEFINE_{}_INSTANCE_EXT_HANDLE", impl_string) 
            : wis::format("WIS_DEFINE_{}_DEVICE_EXT_HANDLE", impl_string));

    auto           full_name   = GetCFullTypename(s.name, impl_string);

    std::string st_decl = wis::format("{}({},{});\n", extends_macro, full_name, s.GetSize(impl_code));
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }
    if (s.GetViewSize(impl_code) > 0) {
        std::string view_decl = wis::format("WIS_DEFINE_HANDLE_VIEW({},{});\n", full_name, s.GetViewSize(impl_code));
        st_decl += view_decl;
    }

    if (kind == DocKind::Full && s.GetViewSize(impl_code) > 0) {
        // Add view extraction function
        st_decl += wis::format("\nstatic inline {}View wisGet{}{}View(const {}* handle){{\n", full_name, impl_string, s.name, full_name);
        st_decl += wis::format("    {}View v;\n", full_name);
        st_decl += "    memcpy(&v, handle, sizeof(v));\n"
                   "    return v;\n}\n";
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

    if (s.GetViewSize(impl_code) > 0) {
        // Strict aliasing rules prevent us from doing a simple cast, so we have to memcpy the data to a new view struct
        st_decl += wis::format(
                "    WIS_NODISCARD {}{}View GetView() const noexcept {{\n"
                "        {}{}View v;\n"
                "        std::memcpy(&v, &_impl_storage, sizeof(v));\n"
                "        return v;\n"
                "    }}\n",
                impl_string,
                s.name,
                impl_string,
                s.name);

        // add conversion operator to view
        st_decl += wis::format(
                "    WIS_NODISCARD operator {}{}View() const noexcept {{\n"
                "        return GetView();\n"
                "    }}\n",
                impl_string,
                s.name);
    }

    // Add all the functions
    for (const auto& func_name : s.functions) {
        auto& func_ref = function_map[func_name];
        if (func_ref.modifier & Modifier::Destroy) {
            deleter += wis::format("            ::{}(handle);\n",
                                   GetCFullTypename(func_ref.name, impl_string));
            continue;
        }

        st_decl += MakeCPPFunctionImpl(func_ref, impl, "inline ", kind, ProtoType::ClassMember);
    }
    deleter += "    }\n};\n";

    st_decl += "};\n";
    return deleter + st_decl;
}

std::string Generator::MakeCPPView(const WisHandle& s, std::string_view impl, DocKind kind)
{
    ImplementedFor impl_code   = ImplCode(impl);
    auto           impl_string = GetImplString(impl_code);
    auto           full_name   = GetCFullTypename(s.name, impl_string);

    std::string view_decl;
    if (s.GetViewSize(impl_code) > 0) {
        view_decl = wis::format("using {}{}View = {}View;\n", impl_string, s.name, full_name);
    }
    return view_decl;
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

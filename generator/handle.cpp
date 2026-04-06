#include "generator.hpp"

//----------------------------------------------------------------------------------------------------------------------
static inline constexpr char template_handle[] =
    R"(/**
 * @struct {0}
 * @ingroup Handles {3}
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

//----------------------------------------------------------------------------------------------------------------------
void Generator::ParseHandles(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("handle"); type;
            type       = type->NextSiblingElement("handle")) {

        auto  name    = type->FindAttribute("name")->Value();
        auto  version = type->FindAttribute("version")->Value();
        auto& ref     = handle_map[name];
        module_map[active_module_name].handles_in_order.emplace_back(name);
        type_map[name] = TypeKind::Handle;

        ref.name    = name;
        ref.version = version;

        if (auto* doc = type->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        if (auto* extends = type->FindAttribute("extends")) {
            ref.extends = GetExtends(extends->Value());
        }

        // Parse implementations
        for (auto* impl = type->FirstChildElement("impl"); impl;
                impl       = impl->NextSiblingElement("impl")) {
            auto impl_for = impl->FindAttribute("for")->Value();
            auto backend  = ParseBackend(impl_for);

            uint32_t size = impl->UnsignedAttribute("size", 0);
            if (backend == Backend::DX12) {
                ref.sizes[0] = size;
            } else if (backend == Backend::Vulkan) {
                ref.sizes[1] = size;
            }
        }

        // Add destroy function
        std::string destr_name = "Destroy" + std::string(name);
        std::string destr_doc  = "Destroys a {" + std::string(name) + "::} handle.";

        auto&       kref        = ref.functions.emplace_back(std::move(destr_name));
        FunctionKey destroy_key = MakeFunctionKey(name, kref);

        auto& destroy     = function_map[destroy_key];
        destroy.name      = kref;
        destroy.this_type = name;
        destroy.modifier  = Modifier::Destroy;
        destroy.version   = version;
        destroy.doc       = destr_doc;
        destroy.FilterBackend(ref.GetBackend());
        type_map[kref] = TypeKind::Function;
        module_map[active_module_name].functions_in_order.emplace_back(destroy_key);
        dependency_tree[name].functions.emplace_back(destroy_key);

        // if the handle is an extension, add create function as well
        if (ref.extends != Extends::None) {
            std::string create_name = "Init" + std::string(name);
            std::string create_doc  = "Initializes a {" + std::string(name) + "::} handle.";

            auto&       iref       = ref.functions.emplace_back(std::move(create_name));
            FunctionKey create_key = MakeFunctionKey(name, iref);

            auto& create     = function_map[create_key];
            create.name      = iref;
            create.this_type = name;
            create.modifier  = Modifier::Construct;
            create.version   = version;
            create.doc       = create_doc;
            create.FilterBackend(ref.GetBackend());
            type_map[iref] = TypeKind::Function;
            module_map[active_module_name].functions_in_order.emplace_back(create_key);
            dependency_tree[name].functions.emplace_back(create_key);
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

            auto backend = ParseBackend(impl_for->Value());

            uint32_t size = impl->UnsignedAttribute("size", 0);
            if (backend == Backend::DX12) {
                ref.sizes[0] = size;
            } else if (backend == Backend::Vulkan) {
                ref.sizes[1] = size;
            }
        }

        if (has_view) {
            module_map[active_module_name].views_in_order.emplace_back(name);
            view_set.insert(name);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCHandle(const WisHandle& s, Backend backend, DocKind kind)
{
    auto impl_string = GetBackendSuffix(backend);

    auto extends_macro = s.extends == Extends::None
                         ? std::string("WIS_DEFINE_HANDLE")
                         : (s.extends == Extends::Instance
                            ? wis::format("WIS_DEFINE_{}_INSTANCE_EXT_HANDLE", impl_string)
                            : wis::format("WIS_DEFINE_{}_DEVICE_EXT_HANDLE", impl_string));

    auto full_name = GetCFullTypename(s.name, backend);

    std::string st_decl = wis::format("{}({},{});\n", extends_macro, full_name, s.GetSize(backend));
    if (!s.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(s, kind);
        st_decl          = wis::format("{}\n{}", xdoc, st_decl);
    }
    if (s.GetViewSize(backend) > 0) {
        std::string view_decl = wis::format("WIS_DEFINE_HANDLE_VIEW({},{});\n", full_name, s.GetViewSize(backend));
        st_decl += view_decl;
    }

    if (kind == DocKind::Full && s.GetViewSize(backend) > 0) {
        // Add view extraction function
        st_decl += wis::format("\nstatic inline {}View wisGet{}{}View(const {}* handle){{\n", full_name, impl_string, s.name, full_name);
        st_decl += wis::format("    {}View v;\n", full_name);
        st_decl += "    memcpy(&v, handle, sizeof(v));\n"
                   "    return v;\n}\n";
    }

    return st_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCPPHandle(const WisHandle& s, Backend backend, DocKind kind)
{
    auto impl_string = GetBackendSuffix(backend);
    auto full_name   = GetCFullTypename(s.name, backend);

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

    std::string ctor_decl;
    // Use constructor from base
    if (s.extends != Extends::None) {
        ctor_decl += wis::format("{}{}() noexcept\n:ImplType(std::in_place)\n{{\n    ",
                                 impl_string,
                                 s.name);
    } else {
        ctor_decl += "    using ImplType::ImplType;\n";
    }
    std::string st_decl2 = "public:\n";

    if (s.GetViewSize(backend) > 0) {
        // Strict aliasing rules prevent us from doing a simple cast, so we have to memcpy the data to a new view struct
        st_decl2 += wis::format(
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
        st_decl2 += wis::format(
                        "    WIS_NODISCARD operator {}{}View() const noexcept {{\n"
                        "        return GetView();\n"
                        "    }}\n",
                        impl_string,
                        s.name);
    }

    // Add all the functions
    for (const auto& func_name : s.functions) {
        FunctionKey func_key{ s.name, func_name };
        auto&       func_ref = function_map[func_key];
        auto        c_name   = wis::format("wis{}{}{}", impl_string, func_ref.modifier & (Destroy | Construct) ? "" : func_ref.this_type, func_ref.name);
        if (func_ref.modifier & Modifier::Destroy) {
            deleter += wis::format("            ::{}(handle);\n",
                                   c_name);
            continue;
        }
        if (func_ref.modifier & Modifier::Construct) {
            ctor_decl += wis::format("        ::{}(GetStorage());\n    }}\n",
                                     c_name);
            continue;
        }

        st_decl2 += MakeCPPFunctionImpl(func_ref, backend, "inline ", kind, ProtoType::ClassMember);
    }

    if (s.extends != Extends::None) {
        auto header = s.extends == Extends::Instance
                      ? GetCPPFullTypename("InstanceExtensionHeader", backend)
                      : GetCPPFullTypename("DeviceExtensionHeader", backend);
        ctor_decl += wis::format("        // Operator & overload\n"
                                 "{}* operator&() noexcept {{\n"
                                 "    return &GetMutableInternal().header;\n"
                                 "}}\n",
                                 header);
    }

    deleter += "    }\n};\n";

    st_decl2 += "};\n";
    return deleter + st_decl + ctor_decl + st_decl2;
}

std::string Generator::MakeCPPView(const WisHandle& s, Backend backend, DocKind kind)
{
    auto impl_string = GetBackendSuffix(backend);
    auto full_name   = GetCFullTypename(s.name, backend);

    std::string view_decl;
    if (s.GetViewSize(backend) > 0) {
        view_decl = wis::format("using {}{}View = {}View;\n", impl_string, s.name, full_name);
    }
    return view_decl;
}

//----------------------------------------------------------------------------------------------------------------------
void Generator::WriteHandleDocumentation(std::filesystem::path handle_output_path)
{
    auto& handle_names = module_map.at(active_module_name).handles_in_order;
    for (const auto& handle_name : handle_names) {
        auto backend = handle_map[handle_name].GetBackend();

        // Make a folder for enums starting with this letter
        std::filesystem::create_directories(handle_output_path);
        std::filesystem::path handle_file_path = handle_output_path / wis::format("{}_handle.h", MakeSnakeCase(handle_name));
        auto&                 handle_ref       = handle_map[handle_name];

        std::string vk_code;
        std::string dx_code;
        if (has(backend, Backend::Vulkan)) {
            vk_code = MakeCHandle(handle_ref, Backend::Vulkan, DocKind::VersionOnly);
            vk_code = wis::format(" Vulkan Version:\n```c\n{}```\n", vk_code);
        }
        if (has(backend, Backend::DX12)) {
            dx_code = MakeCHandle(handle_ref, Backend::DX12, DocKind::VersionOnly);
            dx_code = wis::format(" DX12 Version:\n```c\n{}```\n", dx_code);
        }

        std::string handle_template_content = " * " + vk_code + dx_code;
        std::string handle_refs             = GetRefs(handle_name);
        ReplaceAll(handle_template_content, "\n", "\n * ");
        ReplaceAll(handle_refs, "\n", "\n * ");

        WriteDocumentation(handle_file_path,
                           template_handle,
                           GetCFullTypename(handle_name, Backend::Any),
                           handle_template_content,
                           empty_doc,
                           empty_doc,
                           handle_refs);
    }
}

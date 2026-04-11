#include "generator.hpp"

//----------------------------------------------------------------------------------------------------------------------
static inline constexpr char function_doc_template[] =
    R"(/**
 * @struct {0}
 * @ingroup Functions {3}
 *
 *
 * @section {0}_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * {1}
 * \endcond
 *
 * @section {0}_memb Parameters
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

//----------------------------------------------------------------------------------------------------------------------
void Generator::ParseFunctions(tinyxml2::XMLElement* type)
{
    for (auto* func = type->FirstChildElement("func"); func; func = func->NextSiblingElement("func")) {

        auto name = func->FindAttribute("name")->Value();
        auto* this_type = func->FindAttribute("for");

        auto key = MakeFunctionKey(this_type ? this_type->Value() : "", name);

        auto& ref = function_map[key];
        module_map[active_module_name].functions_in_order.emplace_back(key);
        type_map[name] = TypeKind::Function;
        ref.name = name;

        if (auto* version = func->FindAttribute("version")) {
            ref.version = version->Value();
        } else {
            throw std::runtime_error(wis::format("Function {} is missing version attribute.", name));
        }

        if (this_type) {
            ref.this_type = this_type->Value();
            auto& handle = handle_map[ref.this_type];
            handle.functions.emplace_back(std::string(name));
            ref.FilterBackend(handle.GetBackend());
            TryMakeRef(ref.this_type, key);
        } else {
            module_map[active_module_name].free_functions_in_order.emplace_back(name);
        }

        if (auto* doc = func->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        if (auto* mod = func->FindAttribute("mod")) {
            ref.modifier = GetModifiers(mod->Value());
        }

        // Parse return type
        auto* return_type = func->FirstChildElement("ret");
        if (return_type) {
            ref.return_type.type = return_type->FindAttribute("type")->Value();
            if (ref.return_type.type == "Result") {
                ref.return_type.has_result = true;
                ref.return_type.type = "";
            } else {
                // get backend support
                ref.FilterBackend(GetTypeBackendSupport(ref.return_type.type));
            }

            if (auto* doc = return_type->FindAttribute("doc")) {
                ref.return_type.doc = doc->Value();
            }

            if (auto* name_attr = return_type->FindAttribute("name")) {
                ref.return_type.opt_name = name_attr->Value();
            }

            if (auto* mod = return_type->FindAttribute("mod")) {
                ref.return_type.modifier = GetModifiers(mod->Value());
            }

            if (auto* has_result = return_type->FindAttribute("result")) {
                ref.return_type.has_result = std::string_view(has_result->Value()) == "true";
            }
            TryMakeRef(ref.return_type.type, key);
        }

        // Parse parameters
        for (auto* param = func->FirstChildElement("arg"); param; param = param->NextSiblingElement("arg")) {

            auto& p = ref.parameters.emplace_back();
            p.type = param->FindAttribute("type")->Value();

            if (auto* name_attr = param->FindAttribute("name")) {
                p.name = name_attr->Value();
            } else {
                throw std::runtime_error(wis::format("Function {} has a parameter with no name.", name));
            }
            if (auto* def = param->FindAttribute("default")) {
                p.default_value = def->Value();
            }
            if (auto* mod = param->FindAttribute("mod")) {
                p.modifier = GetModifiers(mod->Value());
            }
            if (auto* doc = param->FindAttribute("doc")) {
                p.doc = doc->Value();
            }
            ref.FilterBackend(GetTypeBackendSupport(p.type));
            TryMakeRef(p.type, key);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Generator::ParseDelegate(tinyxml2::XMLElement* func)
{
    auto name = func->FindAttribute("name")->Value();
    auto& ref = delegate_map[name];
    module_map[active_module_name].delegates_in_order.push_back(name);
    type_map[name] = TypeKind::FuncPointer;
    ref.name = name;

    if (auto* version = func->FindAttribute("version")) {
        ref.version = version->Value();
    } else {
        throw std::runtime_error(wis::format("Delegate {} is missing version attribute.", name));
    }

    if (auto* doc = func->FindAttribute("doc")) {
        ref.doc = doc->Value();
    }

    if (auto* mod = func->FindAttribute("mod")) {
        ref.modifier = GetModifiers(mod->Value());
    }

    // Parse parameters
    for (auto* param = func->FirstChildElement("arg"); param; param = param->NextSiblingElement("arg")) {

        auto& p = ref.parameters.emplace_back();
        p.type = param->FindAttribute("type")->Value();

        if (auto* name_attr = param->FindAttribute("name")) {
            p.name = name_attr->Value();
        } else {
            throw std::runtime_error(wis::format("Function {} has a parameter with no name.", name));
        }
        if (auto* def = param->FindAttribute("default")) {
            p.default_value = def->Value();
        }
        if (auto* mod = param->FindAttribute("mod")) {
            p.modifier = GetModifiers(mod->Value());
        }
        if (auto* doc = param->FindAttribute("doc")) {
            p.doc = doc->Value();
        }
        TryMakeRef(p.type, ref.name);
    }
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCFunctionProto(
    const WisFunction& func,
    Backend backend,
    std::string_view pre_decl,
    DocKind kind
)
{
    auto re_impl = GetBackendSuffix(backend);

    std::string full_return_type;
    std::string post_return;
    std::string function_full_name = wis::format("wis{}{}{}", re_impl, func.IsCD() ? "" : func.this_type, func.name);
    size_t post_return_length = 0;

    if (func.return_type.IsVoid()) {
        full_return_type = "void";
    } else if (func.return_type.IsResultOnly()) {
        full_return_type = GetCFullTypename("Result", Backend::Any);
    } else if (func.return_type.has_result) {
        full_return_type = GetCFullTypename("Result", Backend::Any);
        std::string arg_name = func.return_type.opt_name.empty()
                                 ? wis::format("out_{}", MakeSnakeCase(func.return_type.type))
                                 : std::string(func.return_type.opt_name);

        std::string prefix = "";
        size_t length = full_return_type.size() + 1 + pre_decl.size() + 1 + function_full_name.size();

        if (func.parameters.size() > 0) {
            prefix = ",\n" + std::string(length, ' ');
        }

        std::string type_str = GetMemberTypeString(func.return_type, backend);
        post_return = wis::format("{}{}*{{}}{}", prefix, type_str, arg_name);
        post_return_length = type_str.size();
    } else {
        full_return_type = GetMemberTypeString(func.return_type, backend);
    }

    // This type
    std::string this_arg;
    std::size_t length_this = 0;
    if (!func.this_type.empty()) {
        WisFunctionParameter this_param;
        this_param.type = func.this_type;
        this_param.name = "self";
        this_param.modifier = Modifier(Modifier::Pointer | func.modifier & Modifier::Const);

        auto full_this_type = GetMemberTypeString(this_param, backend);
        this_arg = wis::format("{} {}", full_this_type, this_param.name);
        if (func.parameters.size() > 0) {
            this_arg += ",\n";
        }
        length_this = full_this_type.size();
    }

    size_t length = full_return_type.size() + 1 + pre_decl.size() + 1 + function_full_name.size();
    size_t max_arg_length = std::max(post_return_length, length_this);

    for (size_t i = 0; i < func.parameters.size(); ++i) {
        const auto& p = func.parameters[i];
        std::string type_str = GetMemberTypeString(p, backend);
        max_arg_length = std::max(max_arg_length, type_str.length());
    }

    ReplaceAll(post_return, "{}", std::string(max_arg_length - post_return_length, ' '));

    std::string params;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        const auto& p = func.parameters[i];
        std::string prefix_spaces;
        if (i > 0 || !func.this_type.empty()) {
            prefix_spaces = std::string(length, ' ');
        }

        std::string type_str = GetMemberTypeString(p, backend);
        std::string padding;
        size_t pad_length = max_arg_length > type_str.length() ? max_arg_length - type_str.length() : 0;
        padding = std::string(pad_length, ' ');

        params += wis::format("{}{}{} {}", prefix_spaces, type_str, padding, p.name);
        if (i < func.parameters.size() - 1) {
            params += ",\n";
        }
        max_arg_length = std::max(max_arg_length, type_str.length());
    }

    return wis::format(
        "{}{} {}({}{}{});\n",
        pre_decl,
        full_return_type,
        function_full_name,
        this_arg,
        params.empty() && !post_return.empty() ? ",\n" : params.c_str(),
        post_return
    );
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCPPFunctionProto(
    const WisFunction& func,
    Backend backend,
    std::string_view pre_decl,
    DocKind kind,
    ProtoType type
)
{
    // Inverted situation for C++
    // The return type is always direct, and the out parameter is used for result
    // Expected will be implemented later

    auto re_impl = GetBackendSuffix(backend);
    auto type_prefix = type != ProtoType::Universal ? re_impl : "";
    auto func_prefix = type != ProtoType::Prefixed ? "" : re_impl;
    std::string xclass_code;
    if (!func.this_type.empty() && kind != DocKind::Full) {
        xclass_code = wis::format("{}::", func.this_type);
    }

    std::string full_return_type;
    std::string post_return;

    size_t post_return_length = 0;

    auto ret = func.return_type.GetKind();
    switch (ret) {
    case Void:
        full_return_type = "void";
        break;
    case Direct:
        full_return_type = GetMemberTypeString<Lang::CPP>(
            func.return_type,
            type != ProtoType::Universal ? backend : Backend::Any
        );
        break;
    case ResultOnly:
        full_return_type = "wis::Result";
        break;
    case ResultAndValue:
        full_return_type = GetMemberTypeString<Lang::CPP>(
            func.return_type,
            type != ProtoType::Universal ? backend : Backend::Any
        );
        // Add out parameter for result
        {
            std::string prefix = "";
            size_t length = full_return_type.size() + 1 + pre_decl.size() + 1 + func.name.size() + func_prefix.size()
                          + xclass_code.size();
            if (func.parameters.size() > 0) {
                prefix = ",\n" + std::string(length, ' ');
            }
            std::string type_str = "wis::Result&";
            std::string arg_name = "out_result";
            post_return = wis::format("{}{} {{}}{}", prefix, type_str, arg_name);
            post_return_length = type_str.size();
        }
        break;
    default:
        break;
    }

    size_t length = full_return_type.size() + 1 + pre_decl.size() + 1 + func.name.size() + func_prefix.size()
                  + xclass_code.size();
    size_t max_arg_length = post_return_length;

    // account for spans
    bool last_was_span = false;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        if (last_was_span) {
            last_was_span = false;
            continue;
        }

        const auto& p = func.parameters[i];

        std::string type_str = GetMemberTypeString<Lang::CPP>(p, type != ProtoType::Universal ? backend : Backend::Any);
        max_arg_length = std::max(max_arg_length, type_str.length());
    }
    last_was_span = false;

    ReplaceAll(post_return, "{}", std::string(max_arg_length - post_return_length, ' '));

    std::string params;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        if (last_was_span) {
            last_was_span = false;
            continue;
        }

        const auto& p = func.parameters[i];
        if (p.modifier & Modifier::Span) {
            last_was_span = true;
        }
        std::string prefix_spaces;
        if (i > 0) {
            prefix_spaces = std::string(length, ' ');
        }

        std::string type_str = GetMemberTypeString<Lang::CPP>(p, type != ProtoType::Universal ? backend : Backend::Any);
        std::string padding;
        size_t pad_length = max_arg_length > type_str.length() ? max_arg_length - type_str.length() : 0;
        padding = std::string(pad_length, ' ');

        params += wis::format("{}{}{} {}", prefix_spaces, type_str, padding, p.name);

        // edge case for spans - if last argument was a span, skip the next one (the size)
        // That means we need to check if i<func.parameters.size()-2 for the comma
        bool span_last = (p.modifier & Modifier::Span) != 0 && (i == func.parameters.size() - 2);

        if (i < func.parameters.size() - 1 && !span_last) {
            params += ",\n";
        }
        max_arg_length = std::max(max_arg_length, type_str.length());
    }

    return wis::format(
        "{}{} {}{}{}({}{}){} noexcept;\n",
        pre_decl,
        full_return_type,
        func_prefix,
        xclass_code,
        func.name,
        params,
        post_return,
        func.modifier & Modifier::Const ? " const" : ""
    );
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCFunctionDecl(
    const WisFunction& func,
    Backend backend,
    std::string_view pre_decl,
    DocKind kind
)
{
    std::string func_decl = MakeCFunctionProto(func, backend, pre_decl, kind);
    if (!func.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(func, kind);
        func_decl = wis::format("{}\n{}", xdoc, func_decl);
    }
    return func_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCDelegate(const WisFunction& func, DocKind kind)
{
    std::string params;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        const auto& p = func.parameters[i];
        std::string type_str = GetMemberTypeString(p, Backend::Any);
        params += wis::format("{} {}", type_str, p.name);
        if (i < func.parameters.size() - 1) {
            params += ", ";
        }
    }
    std::string delegate_decl = wis::format("typedef void (*Wis{})({});\n", func.name, params);
    if (!func.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(func, kind);
        delegate_decl = wis::format("{}\n{}", xdoc, delegate_decl);
    }
    return delegate_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCPPFunctionImpl(
    const WisFunction& func,
    Backend backend,
    std::string_view pre_decl,
    DocKind kind,
    ProtoType type
)
{
    std::string add_decl;
    if (func.return_type.IsRV() || func.return_type.IsDirect()) {
        add_decl = wis::format("{} {}", "WIS_NODISCARD", pre_decl);
    }

    std::string func_decl = MakeCPPFunctionProto(func, backend, add_decl.empty() ? pre_decl : add_decl, kind, type);
    if (!func.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(func, kind);
        func_decl = wis::format("{}\n{}", xdoc, func_decl);
    }
    if (kind != DocKind::Full) {
        return func_decl;
    }

    auto re_impl = GetBackendSuffix(backend);
    auto c_name = wis::format("wis{}{}{}", re_impl, func.IsCD() ? "" : func.this_type, func.name);

    // Convert args and call C function
    std::string body = "{\n";

    constexpr static std::string_view arg_prefix = ",\n    ";
    auto set_params = [&]() {
        for (size_t i = 0; i < func.parameters.size(); ++i) {
            auto& p = func.parameters[i];

            if (p.modifier & Modifier::Span) {
                body += wis::format(
                    "reinterpret_cast<{}>({}.data()), {}.size()",
                    GetMemberTypeString<Lang::C>(p, backend),
                    p.name,
                    p.name
                );
                i++; // skip next parameter (the size)
                if (i < func.parameters.size() - 1) {
                    body += arg_prefix;
                }
                continue;
            }

            switch (GetType(p.type)) {
            case TypeKind::Enum:
            case TypeKind::Bitmask:
                body += wis::format("static_cast<{}>({})", GetMemberTypeString<Lang::C>(p, backend), p.name);
                break;
            case TypeKind::None:
            case TypeKind::View:
            case TypeKind::Base:
                body += p.name;
                break;
            default:
                if (p.modifier & Modifier::Reference) {
                    body += wis::format("reinterpret_cast<{}>(&{})", GetMemberTypeString<Lang::C>(p, backend), p.name);
                    break;
                }
                body += wis::format("reinterpret_cast<{}>({})", GetMemberTypeString<Lang::C>(p, backend), p.name);
                break;
            }

            if (i < func.parameters.size() - 1) {
                body += arg_prefix;
            }
        }
    };

    switch (func.return_type.GetKind()) {
    case ReturnTypeKind::ResultAndValue: {
        auto ret_value_name = func.return_type.opt_name.empty()
                                ? wis::format("out_{}", MakeSnakeCase(func.return_type.type))
                                : std::string(func.return_type.opt_name);

        // Prepare out parameter
        body += wis::format("    {} {};\n", GetMemberTypeString<Lang::CPP>(func.return_type, backend), ret_value_name);

        body += wis::format(
            "    const WisResult wis_result = ::{}({}",
            c_name,
            func.this_type.empty() ? "" : "&_impl_storage"
        );

        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }

        set_params();

        auto ret_type = GetType(func.return_type.type);

        if (ret_type == TypeKind::Handle) {
            body += wis::format(", {}.GetStorage());\n", ret_value_name);
        } else {
            body += wis::format(
                ", reinterpret_cast<{}*>(&{}));\n",
                GetMemberTypeString<Lang::C>(func.return_type, backend),
                ret_value_name
            );
        }
        body += "    out_result = wis::Result{ static_cast<wis::Status>(wis_result.status), wis_result.platform_code, "
                "wis_result.error };\n";
        body += wis::format("    return {};\n", ret_value_name);
    } break;
    case ReturnTypeKind::ResultOnly: {
        body += wis::format(
            "    const WisResult wis_result = ::{}({}",
            c_name,
            func.this_type.empty() ? "" : "&_impl_storage"
        );
        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }
        set_params();
        body += ");\n";
        body += "    return wis::Result{ static_cast<wis::Status>(wis_result.status), wis_result.platform_code, "
                "wis_result.error };\n";
    } break;
    case ReturnTypeKind::Direct: {
        auto ret_type = GetType(func.return_type.type);
        std::string return_cast;
        switch (ret_type) {
        case TypeKind::Base:
            break;
        case TypeKind::Enum:
        case TypeKind::Bitmask:
            return_cast = wis::format("static_cast<{}>", GetMemberTypeString<Lang::CPP>(func.return_type, backend));
            break;
        case TypeKind::Handle:
            throw std::runtime_error(
                wis::format("Function {} return type cannot be a handle in direct return.", func.name)
            );
            break;
        default:
            return_cast = wis::format(
                "reinterpret_cast<{}>",
                GetMemberTypeString<Lang::CPP>(func.return_type, backend)
            );
            break;
        }

        body += wis::format(
            "    return {}(::{}({}",
            return_cast,
            c_name,
            func.this_type.empty() ? "" : "&_impl_storage"
        );
        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }
        set_params();
        body += "));\n";
    } break;
    case ReturnTypeKind::Void: {
        body += wis::format("    ::{}({}", c_name, func.this_type.empty() ? "" : "&_impl_storage");
        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }
        set_params();
        body += ");\n";
    } break;
    default:
        break;
    }

    body += "}\n";

    // Remove the semicolon from declaration and add body
    func_decl.pop_back();
    func_decl.pop_back();
    func_decl += " " + body;
    return func_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeCPPDelegate(const WisFunction& func, DocKind kind)
{
    std::string params;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        const auto& p = func.parameters[i];
        std::string type_str = GetMemberTypeString<Lang::CPP>(p, Backend::Any);
        params += wis::format("{} {}", type_str, p.name);
        if (i < func.parameters.size() - 1) {
            params += ", ";
        }
    }
    std::string delegate_decl = wis::format("using {} = void (*)({});\n", func.name, params);
    if (!func.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(func, kind);
        delegate_decl = wis::format("{}\n{}", xdoc, delegate_decl);
    }
    return delegate_decl;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeFunctionDescription(const WisFunction& s)
{
    std::string description = " * ";
    if (!s.this_type.empty()) {
        if (s.modifier & Modifier::Construct) {
            description += wis::format(
                "- **this** `self` is a pointer to uninitialized {{{}::}} instance memory. It will be initialized by "
                "this function.\n",
                s.this_type
            );

            // There must also be a note about the destroy function in the description
            description += wis::format("**note** The corresponding destroy function is `wisDestroy{}`.\n", s.this_type);
        } else {
            description += wis::format(
                "- **this** `self` self is a pointer to the valid {{{}::}} instance.\n",
                s.this_type
            );
        }
    }

    for (auto& p : s.parameters) {
        description += wis::format("- `{}` {}\n", p.name, p.doc.empty() ? "No description." : p.doc);
    }

    switch (s.return_type.GetKind()) {
    case ReturnTypeKind::Direct:
        description += wis::format(
            "\n- **return** {}\n",
            s.return_type.doc.empty() ? "No description." : s.return_type.doc
        );
        break;
    case ReturnTypeKind::ResultOnly:
        description += wis::format("\n- **return** denoting the outcome of operation.\n");
        break;
    case ReturnTypeKind::ResultAndValue: {
        std::string arg_name = s.return_type.opt_name.empty() ? wis::format("out_{}", MakeSnakeCase(s.return_type.type))
                                                              : std::string(s.return_type.opt_name);
        description += wis::format(
            "- `{}` {}\n",
            s.return_type.opt_name.empty() ? "value" : s.return_type.opt_name,
            s.return_type.doc.empty() ? "No description." : s.return_type.doc
        );
        description += wis::format("\n- **return** denoting the outcome of operation.\n");
        break;
    }
    default:
        break;
    }

    return description;
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeDelegateDescription(const WisFunction& s)
{
    std::string description = " * ";
    for (auto& p : s.parameters) {
        description += wis::format("- `{}` {}\n", p.name, p.doc.empty() ? "No description." : p.doc);
    }
    return description;
}

//----------------------------------------------------------------------------------------------------------------------
void Generator::WriteFunctionDocumentation(std::filesystem::path func_output_path)
{
    std::filesystem::create_directories(func_output_path);
    auto& function_names = module_map.at(active_module_name).functions_in_order;
    for (auto& func_name : function_names) {
        auto& func_def = function_map[func_name];
        std::string full_func_name = wis::format(
            "wis{}{}",
            func_def.modifier & (Destroy | Construct) ? "" : func_def.this_type,
            func_def.name
        );
        auto func_doc_path = func_output_path / wis::format("{}_function.h", MakeSnakeCase(full_func_name.substr(3)));

        auto supports_vk = has(func_def.backend, Backend::Vulkan);
        auto supports_dx = has(func_def.backend, Backend::DX12);

        std::string vk_code = supports_vk ? MakeCFunctionDecl(func_def, Backend::Vulkan, "", DocKind::VersionOnly) : "";
        std::string dx_code = supports_dx ? MakeCFunctionDecl(func_def, Backend::DX12, "", DocKind::VersionOnly) : "";
        std::string regular_code = MakeCFunctionDecl(func_def, Backend::Any, "", DocKind::VersionOnly);
        std::string c_code = regular_code;
        std::string cimpl_code = (vk_code + '\n' + dx_code);
        if (c_code.empty()) {
            c_code = !vk_code.empty() ? vk_code : dx_code;
        }

        std::string vk_code_cpp = func_def.modifier & Modifier::Destroy || !supports_vk
                                    ? ""
                                    : MakeCPPFunctionImpl(func_def, Backend::Vulkan, "", DocKind::VersionOnly);
        std::string dx_code_cpp = func_def.modifier & Modifier::Destroy || !supports_dx
                                    ? ""
                                    : MakeCPPFunctionImpl(func_def, Backend::DX12, "", DocKind::VersionOnly);
        std::string regular_code_cpp = func_def.modifier & Modifier::Destroy || !(supports_vk && supports_dx)
                                         ? ""
                                         : MakeCPPFunctionImpl(func_def, Backend::Any, "", DocKind::VersionOnly);
        std::string cpp_code = regular_code_cpp;
        std::string cpp_impl_code = func_def.modifier & Modifier::Destroy || !(supports_vk && supports_dx)
                                      ? ""
                                      : vk_code_cpp + '\n' + dx_code_cpp;
        if (cpp_code.empty()) {
            cpp_code = !vk_code_cpp.empty() ? vk_code_cpp : dx_code_cpp;
        }

        std::string func_template_content = GetSpecificationCode(c_code, cimpl_code, cpp_code, cpp_impl_code);

        std::string func_description = MakeFunctionDescription(func_def);
        std::string func_refs = GetRefs(func_def.name);
        std::string vuids = MakeValidationForType(func_def.name);

        ReplaceAll(func_description, "\n", "\n * ");
        ReplaceAll(func_refs, "\n", "\n * ");
        func_description = FinalizeCDocumentation(func_description, func_def.name);

        WriteDocumentation(
            func_doc_path,
            function_doc_template,
            full_func_name,
            func_template_content,
            vuids,
            func_description,
            func_refs
        );
    }
}

void Generator::WriteDelegateDocumentation(std::filesystem::path func_output_path)
{
    std::filesystem::create_directories(func_output_path);
    for (auto& delegate_name : module_map.at(active_module_name).delegates_in_order) {
        auto full_delegate_name = GetCFullTypename(delegate_name, Backend::Any);
        auto delegate_doc_path = func_output_path
                               / wis::format("{}_delegate.h", MakeSnakeCase(full_delegate_name.substr(3)));
        auto& delegate_def = delegate_map[delegate_name];

        std::string regular_code = MakeCDelegate(delegate_def, DocKind::VersionOnly);
        std::string regular_code_cpp = MakeCPPDelegate(delegate_def, DocKind::VersionOnly);
        std::string delegate_template_content = GetSpecificationCode(regular_code, "", regular_code_cpp, "");

        std::string delegate_description = MakeDelegateDescription(delegate_def);
        std::string delegate_refs = GetRefs(delegate_def.name);
        std::string vuids = MakeValidationForType(delegate_def.name);
        ReplaceAll(delegate_description, "\n", "\n * ");
        ReplaceAll(delegate_refs, "\n", "\n * ");
        delegate_description = FinalizeCDocumentation(delegate_description, delegate_name);
        WriteDocumentation(
            delegate_doc_path,
            function_doc_template,
            GetCFullTypename(delegate_name, Backend::Any),
            delegate_template_content,
            vuids,
            delegate_description,
            delegate_refs
        );
    }
}

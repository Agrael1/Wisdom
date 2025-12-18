#include "generator.hpp"

//-----------------------------------------------------------------------------
static inline constexpr char function_doc_template[] =
        R"(/**
 * @struct {0}
 * @ingroup Functions
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

//-----------------------------------------------------------------------------
void Generator::ParseFunctions(tinyxml2::XMLElement* type)
{
    for (auto* func = type->FirstChildElement("func"); func;
         func       = func->NextSiblingElement("func")) {

        auto  name = func->FindAttribute("name")->Value();
        auto& ref  = function_map[name];
        functions_in_order.emplace_back(name);
        ref.name = name;

        if (auto* version = func->FindAttribute("version")) {
            ref.version = version->Value();
        } else {
            throw std::runtime_error(wis::format("Function {} is missing version attribute.", name));
        }

        auto* this_type = func->FindAttribute("for");
        if (this_type) {
            ref.this_type = this_type->Value();
            auto& handle  = handle_map[ref.this_type];
            handle.functions.emplace_back(name);
            TryMakeRef(ref.this_type, ref.name);
        } else {
            free_functions_in_order.emplace_back(name);
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
            TryMakeRef(ref.return_type.type, ref.name);
        }

        // Parse parameters
        for (auto* param = func->FirstChildElement("arg"); param;
             param       = param->NextSiblingElement("arg")) {

            auto& p = ref.parameters.emplace_back();
            p.type  = param->FindAttribute("type")->Value();

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
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl, DocKind kind)
{
    ImplementedFor impl_code = ImplCode(impl);
    auto           re_impl   = GetImplString(impl_code);

    std::string full_return_type;
    std::string post_return;
    std::string function_full_name = wis::format("wis{}{}{}", re_impl, func.name.starts_with("Destroy") ? "" : func.this_type, func.name);
    size_t      post_return_length = 0;

    if (func.return_type.IsVoid()) {
        full_return_type = "void";
    } else if (func.return_type.has_result) {
        full_return_type     = GetCFullTypename("Result", "");
        std::string arg_name = func.return_type.opt_name.empty()
                ? wis::format("out_{}", MakeSnakeCase(func.return_type.type))
                : std::string(func.return_type.opt_name);

        std::string prefix = "";
        size_t      length = full_return_type.size() + 1 + pre_decl.size() + 1 + function_full_name.size();

        if (func.parameters.size() > 0) {
            prefix = ",\n" + std::string(length, ' ');
        }

        std::string type_str = GetMemberTypeString(func.return_type, re_impl);
        post_return          = wis::format("{}{}*{{}}{}",
                                  prefix,
                                  type_str,
                                  arg_name);
        post_return_length   = type_str.size();
    } else {
        full_return_type = GetMemberTypeString(func.return_type, re_impl);
    }

    // This type
    std::string this_arg;
    std::size_t length_this = 0;
    if (!func.this_type.empty()) {
        WisFunctionParameter this_param;
        this_param.type     = func.this_type;
        this_param.name     = "self";
        this_param.modifier = Modifier(Modifier::Pointer | func.modifier & Modifier::Const);

        auto full_this_type = GetMemberTypeString(this_param, re_impl);
        this_arg            = wis::format("{} {}", full_this_type, this_param.name);
        if (func.parameters.size() > 0) {
            this_arg += ",\n";
        }
        length_this = full_this_type.size();
    }

    size_t length         = full_return_type.size() + 1 + pre_decl.size() + 1 + function_full_name.size();
    size_t max_arg_length = std::max(post_return_length, length_this);

    for (size_t i = 0; i < func.parameters.size(); ++i) {
        const auto& p        = func.parameters[i];
        std::string type_str = GetMemberTypeString(p, re_impl);
        max_arg_length       = std::max(max_arg_length, type_str.length());
    }

    ReplaceAll(post_return, "{}", std::string(max_arg_length - post_return_length, ' '));

    std::string params;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        const auto& p = func.parameters[i];
        std::string prefix_spaces;
        if (i > 0 || !func.this_type.empty()) {
            prefix_spaces = std::string(length, ' ');
        }

        std::string type_str = GetMemberTypeString(p, re_impl);
        std::string padding;
        size_t      pad_length = max_arg_length > type_str.length() ? max_arg_length - type_str.length() : 0;
        padding                = std::string(pad_length, ' ');

        params += wis::format("{}{}{} {}", prefix_spaces, type_str, padding, p.name);
        if (i < func.parameters.size() - 1) {
            params += ",\n";
        }
        max_arg_length = std::max(max_arg_length, type_str.length());
    }

    return wis::format("{}{} {}({}{}{});\n",
                       pre_decl,
                       full_return_type,
                       function_full_name,
                       this_arg,
                       params,
                       post_return);
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl, DocKind kind, ProtoType type)
{
    // Inverted situation for C++
    // The return type is always direct, and the out parameter is used for result
    // Expected will be implemented later

    ImplementedFor impl_code   = ImplCode(impl);
    auto           re_impl     = GetImplString(impl_code);
    auto           type_prefix = type != ProtoType::Universal ? re_impl : "";
    auto           func_prefix = type != ProtoType::Prefixed ? "" : re_impl;
    std::string    xclass_code;
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
        full_return_type = GetMemberTypeString<Lang::CPP>(func.return_type, type_prefix);
        break;
    case ResultOnly:
        full_return_type = "wis::Result";
        break;
    case ResultAndValue:
        full_return_type = GetMemberTypeString<Lang::CPP>(func.return_type, type_prefix);
        // Add out parameter for result
        {
            std::string prefix = "";
            size_t      length = full_return_type.size() + 1 + pre_decl.size() + 1 + func.name.size() + func_prefix.size() + xclass_code.size();
            if (func.parameters.size() > 0) {
                prefix = ",\n" + std::string(length, ' ');
            }
            std::string type_str = "wis::Result&";
            std::string arg_name = "out_result";
            post_return          = wis::format("{}{} {{}}{}",
                                      prefix,
                                      type_str,
                                      arg_name);
            post_return_length   = type_str.size();
        }
        break;
    default:
        break;
    }

    size_t length         = full_return_type.size() + 1 + pre_decl.size() + 1 + func.name.size() + func_prefix.size() + xclass_code.size();
    size_t max_arg_length = post_return_length;

    // account for spans
    bool last_was_span = false;
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        if (last_was_span) {
            last_was_span = false;
            continue;
        }

        const auto& p = func.parameters[i];

        std::string type_str = GetMemberTypeString<Lang::CPP>(p, type_prefix);
        max_arg_length       = std::max(max_arg_length, type_str.length());
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

        std::string type_str = GetMemberTypeString<Lang::CPP>(p, type_prefix);
        std::string padding;
        size_t      pad_length = max_arg_length > type_str.length() ? max_arg_length - type_str.length() : 0;
        padding                = std::string(pad_length, ' ');

        params += wis::format("{}{}{} {}", prefix_spaces, type_str, padding, p.name);

        // edge case for spans - if last argument was a span, skip the next one (the size)
        // That means we need to check if i<func.parameters.size()-2 for the comma
        bool span_last = (p.modifier & Modifier::Span) != 0 && (i == func.parameters.size() - 2);

        if (i < func.parameters.size() - 1 && !span_last) {
            params += ",\n";
        }
        max_arg_length = std::max(max_arg_length, type_str.length());
    }

    

    return wis::format("{}{} {}{}{}({}{}){} noexcept;\n",
                       pre_decl,
                       full_return_type,
                       func_prefix,
                       xclass_code,
                       func.name,
                       params,
                       post_return,
                       func.modifier & Modifier::Const ? " const" : "");
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCFunctionDecl(const WisFunction& func, std::string_view impl, std::string_view pre_decl, DocKind kind)
{
    std::string func_decl = MakeCFunctionProto(func, impl, pre_decl, kind);
    if (!func.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation(func, kind);
        func_decl        = wis::format("{}\n{}", xdoc, func_decl);
    }
    return func_decl;
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCPPFunctionImpl(const WisFunction& func, std::string_view impl, std::string_view pre_decl, DocKind kind, ProtoType type)
{
    std::string add_decl;
    if (func.return_type.IsRV() || func.return_type.IsDirect()) {
        add_decl = wis::format("{} {}", "WIS_NODISCARD", pre_decl);
    }

    std::string func_decl = MakeCPPFunctionProto(func, impl, add_decl.empty() ? pre_decl : add_decl, kind, type);
    if (!func.doc.empty()) {
        std::string xdoc = MakeTypeDocumentation<Lang::CPP>(func, kind);
        func_decl        = wis::format("{}\n{}", xdoc, func_decl);
    }
    if (kind != DocKind::Full) {
        return func_decl;
    }

    ImplementedFor impl_code = ImplCode(impl);
    auto           re_impl   = GetImplString(impl_code);

    // Convert args and call C function
    std::string body = "{\n";

    switch (func.return_type.GetKind()) {
    case ReturnTypeKind::ResultAndValue: {
        auto ret_value_name = func.return_type.opt_name.empty()
                ? wis::format("out_{}", MakeSnakeCase(func.return_type.type))
                : std::string(func.return_type.opt_name);

        // Prepare out parameter
        body += wis::format("    {} {};\n", GetMemberTypeString<Lang::CPP>(func.return_type, re_impl), ret_value_name);

        body += wis::format("    out_result = convert_result(::{}({}",
                            GetCFullTypename(func.name, re_impl),
                            func.this_type.empty()
                                    ? ""
                                    : "&_impl_storage");

        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }

        for (size_t i = 0; i < func.parameters.size(); ++i) {
            auto& p = func.parameters[i];

            if (p.modifier & Modifier::Span) {
                body += wis::format("reinterpret_cast<{}>({}.data()), {}.size()",
                                    GetMemberTypeString<Lang::C>(p, re_impl),
                                    p.name,
                                    p.name);
                i++; // skip next parameter (the size)
                if (i < func.parameters.size() - 1) {
                    body += arg_prefix;
                }
                continue;
            }

            switch (GetType(p.type)) {
            case TypeKind::Enum:
            case TypeKind::Bitmask:
                body += wis::format("static_cast<{}>({})",
                                    GetMemberTypeString<Lang::C>(p, re_impl),
                                    p.name);
                break;
            case TypeKind::None:
            case TypeKind::Base:
                body += p.name;
                break;
            default:
                body += wis::format("reinterpret_cast<{}>({})", GetMemberTypeString<Lang::C>(p, re_impl), p.name);
                break;
            }

            if (i < func.parameters.size() - 1) {
                body += arg_prefix;
            }
        }

        auto ret_type = GetType(func.return_type.type);

        if (ret_type == TypeKind::Handle) {
            body += wis::format(", {}.GetStorage()));\n", ret_value_name);
        } else {
            body += wis::format(", reinterpret_cast<{}*>(&{})));\n",
                                GetMemberTypeString<Lang::C>(func.return_type, re_impl),
                                ret_value_name);
        }
        body += wis::format("    return {};\n", ret_value_name);
    } break;
    case ReturnTypeKind::ResultOnly: {
        body += wis::format("    return reinterpret_cast<wis::Result&&>(::{}({}",
                            GetCFullTypename(func.name, re_impl),
                            func.this_type.empty() ? "" : "&_impl_storage");
        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }
        for (size_t i = 0; i < func.parameters.size(); ++i) {
            auto& p = func.parameters[i];
            if (p.modifier & Modifier::Span) {
                body += wis::format("reinterpret_cast<{}>({}.data()), {}.size()",
                                    GetMemberTypeString<Lang::C>(p, re_impl),
                                    p.name,
                                    p.name);
                i++; // skip next parameter (the size)
                if (i < func.parameters.size() - 1) {
                    body += arg_prefix;
                }
                continue;
            }
            body += wis::format("reinterpret_cast<{}>({})", GetMemberTypeString<Lang::C>(p, re_impl), p.name);
            if (i < func.parameters.size() - 1) {
                body += arg_prefix;
            }
        }
        body += "));\n";
    } break;
    case ReturnTypeKind::Direct: {
        auto        ret_type = GetType(func.return_type.type);
        std::string return_cast;
        switch (ret_type) {
        case TypeKind::Base:
            break;
        case TypeKind::Enum:
        case TypeKind::Bitmask:
            return_cast = wis::format("static_cast<{}>",
                                      GetMemberTypeString<Lang::CPP>(func.return_type, re_impl));
            break;
        case TypeKind::Handle:
            throw std::runtime_error(wis::format("Function {} return type cannot be a handle in direct return.", func.name));
            break;
        default:
            return_cast = wis::format("reinterpret_cast<{}>",
                                      GetMemberTypeString<Lang::CPP>(func.return_type, re_impl));
            break;
        }

        body += wis::format("    return {}(::{}({}",
                            return_cast,
                            GetCFullTypename(func.name, re_impl),
                            func.this_type.empty() ? "" : "&_impl_storage");
        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }
        for (size_t i = 0; i < func.parameters.size(); ++i) {
            auto& p = func.parameters[i];
            if (p.modifier & Modifier::Span) {
                body += wis::format("reinterpret_cast<{}>({}.data()), {}.size()",
                                    GetMemberTypeString<Lang::C>(p, re_impl),
                                    p.name,
                                    p.name);
                i++; // skip next parameter (the size)
                if (i < func.parameters.size() - 1) {
                    body += arg_prefix;
                }
                continue;
            }
            body += wis::format("reinterpret_cast<{}>({})", GetMemberTypeString<Lang::C>(p, re_impl), p.name);
            if (i < func.parameters.size() - 1) {
                body += arg_prefix;
            }
        }
        body += "));\n";
    } break;
    case ReturnTypeKind::Void: {
        body += wis::format("    ::wis{}{}{}({}",
                            re_impl,
                            func.this_type,
                            func.name,
                            func.this_type.empty() ? "" : "&_impl_storage");
        constexpr static std::string_view arg_prefix = ",\n    ";
        if (func.parameters.size() > 0 && !func.this_type.empty()) {
            body += arg_prefix;
        }
        for (size_t i = 0; i < func.parameters.size(); ++i) {
            auto& p = func.parameters[i];
            if (p.modifier & Modifier::Span) {
                body += wis::format("reinterpret_cast<{}>({}.data()), {}.size()",
                                    GetMemberTypeString<Lang::C>(p, re_impl),
                                    p.name,
                                    p.name);
                i++; // skip next parameter (the size)
                if (i < func.parameters.size() - 1) {
                    body += arg_prefix;
                }
                continue;
            }
            body += wis::format("reinterpret_cast<{}>({})", GetMemberTypeString<Lang::C>(p, re_impl), p.name);
            if (i < func.parameters.size() - 1) {
                body += arg_prefix;
            }
        }
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

//-----------------------------------------------------------------------------
std::string Generator::MakeFunctionDescription(const WisFunction& s)
{
    std::string description = " * ";
    if (!s.this_type.empty()) {
        description += wis::format("- **this** `self` self is a pointer to the valid {{{}::}} instance.\n",
                                   s.this_type);
    }

    for (auto& p : s.parameters) {
        description += wis::format("- `{}` {}\n", p.name, p.doc.empty() ? "No description." : p.doc);
    }

    switch (s.return_type.GetKind()) {
    case ReturnTypeKind::Direct:
        description += wis::format("\n- **return** {}\n", s.return_type.doc.empty() ? "No description." : s.return_type.doc);
        break;
    case ReturnTypeKind::ResultOnly:
        description += wis::format("\n- **return** denoting the outcome of operation.\n");
        break;
    case ReturnTypeKind::ResultAndValue: {
        std::string arg_name = s.return_type.opt_name.empty()
                ? wis::format("out_{}", MakeSnakeCase(s.return_type.type))
                : std::string(s.return_type.opt_name);
        description += wis::format("- `{}` {}\n",
                                   s.return_type.opt_name.empty() ? "value" : s.return_type.opt_name,
                                   s.return_type.doc.empty() ? "No description." : s.return_type.doc);
        description += wis::format("\n- **return** denoting the outcome of operation.\n");
        break;
    }
    default:
        break;
    }

    return description;
}

//-----------------------------------------------------------------------------
void Generator::WriteFunctionDocumentation(std::filesystem::path func_output_path)
{
    for (auto& func_name : functions_in_order) {
        auto  full_func_name = GetCFullTypename(func_name, "");
        auto  func_doc_path  = func_output_path / wis::format("{}_function.h", MakeSnakeCase(full_func_name.substr(3)));
        auto& func_def       = function_map[func_name];

        std::string vk_code      = MakeCFunctionDecl(func_def, "vk", "", DocKind::VersionOnly);
        std::string dx_code      = MakeCFunctionDecl(func_def, "dx", "", DocKind::VersionOnly);
        std::string regular_code = MakeCFunctionDecl(func_def, "", "", DocKind::VersionOnly);

        std::string vk_code_cpp      = func_def.modifier & Modifier::Destroy ? "" : MakeCPPFunctionImpl(func_def, "vk", "", DocKind::VersionOnly);
        std::string dx_code_cpp      = func_def.modifier & Modifier::Destroy ? "" : MakeCPPFunctionImpl(func_def, "dx", "", DocKind::VersionOnly);
        std::string regular_code_cpp = func_def.modifier & Modifier::Destroy ? "" : MakeCPPFunctionImpl(func_def, "", "", DocKind::VersionOnly);

        std::string func_template_content = GetSpecificationCode(regular_code, vk_code + '\n' + dx_code, regular_code_cpp, vk_code_cpp + '\n' + dx_code_cpp);

        std::string func_description = MakeFunctionDescription(func_def);
        std::string func_refs        = GetRefs(func_def.name);
        std::string vuids            = MakeValidationForType(func_def.name);

        ReplaceAll(func_description, "\n", "\n * ");
        ReplaceAll(func_refs, "\n", "\n * ");
        func_description = FinalizeCDocumentation(func_description, func_name);

        WriteDocumentation(func_doc_path,
                           function_doc_template,
                           GetCFullTypename(func_name, ""),
                           func_template_content,
                           vuids,
                           func_description,
                           func_refs);
    }
}

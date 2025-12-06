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

std::string Generator::MakeCFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl, DocKind kind)
{
    ImplementedFor impl_code = ImplCode(impl);
    auto           re_impl   = GetImplString(impl_code);

    std::string full_return_type;
    std::string post_return;
    std::string function_full_name = GetCFullTypename(func.name, GetImplString(impl_code));
    size_t      post_return_length = 0;

    if (func.return_type.IsVoid()) {
        full_return_type = "void";
    } else if (func.return_type.has_result) {
        full_return_type     = GetCFullTypename("Result", "");
        std::string arg_name = func.return_type.opt_name.empty()
                ? wis::format("out_{}", MakeSnakeCase(func.return_type.type))
                : std::string(func.return_type.opt_name);

        std::string prefix = "";
        size_t      length = full_return_type.size() + 1 + pre_decl.size() + 1 + function_full_name.size() + 1;

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

    size_t length         = full_return_type.size() + 1 + pre_decl.size() + 1 + function_full_name.size() + 1;
    size_t max_arg_length = post_return_length;

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
        if (i > 0) {
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

    return wis::format("{} {} {}({}{});\n",
                       pre_decl,
                       full_return_type,
                       function_full_name,
                       params,
                       post_return);
}

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
std::string Generator::MakeFunctionDescription(const WisFunction& s)
{
    std::string description = " * ";
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
        auto  func_doc_path = func_output_path / wis::format("{}_function.h", MakeSnakeCase(func_name));
        auto& func_def      = function_map[func_name];

        std::string vk_code      = MakeCFunctionDecl(func_def, "vk", "", DocKind::VersionOnly);
        std::string dx_code      = MakeCFunctionDecl(func_def, "dx", "", DocKind::VersionOnly);
        std::string regular_code = MakeCFunctionDecl(func_def, "", "", DocKind::VersionOnly);

        std::string func_template_content = wis::format(" * General Version:\n```c\n{}```\nVulkan Version:\n```c\n{}```\nDX12 Version:\n```c\n{}```\n", regular_code, vk_code, dx_code);

        std::string func_description = MakeFunctionDescription(func_def);
        std::string func_refs        = GetRefs(func_def.name);
        std::string vuids            = MakeValidationForType(func_def.name);

        ReplaceAll(func_template_content, "\n", "\n * ");
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
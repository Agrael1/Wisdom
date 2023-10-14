#include "generator.h"
#include "../wisdom/include/wisdom/bridge/format.h"
#include <fstream>
#include <ranges>
#include <array>
#include <stdexcept>

using namespace tinyxml2;

// maybe add an array
std::string GetArgString(const ResolvedType& resolved, std::string_view impl, std::string_view name, std::string_view mod)
{
    std::string pre_mod;
    std::string post_mod;
    if (mod == "ptr") {
        post_mod += '*';
    }
    if (mod == "cptr") {
        pre_mod += "const";
        post_mod += '*';
    }
    if (mod == "const") {
        pre_mod += "const";
    }

    return resolved.first == TypeInfo::Handle
            ? wis::format("{} {}{} {}{}", pre_mod, impl, resolved.second, post_mod, name)
            : wis::format("{} {} {}{}", pre_mod, resolved.second, post_mod, name);
}

//-----------------------------------------------------------------------------

Generator::Generator(std::filesystem::path file)
{
    structs.clear();
    enums.clear();
    bitmasks.clear();

    if (doc.LoadFile(file.string().c_str()) != XML_SUCCESS)
        throw std::runtime_error("Failed to load file");

    auto* root = doc.FirstChildElement("registry");
    if (!root)
        throw std::runtime_error("Failed to load root");

    auto* types = root->FirstChildElement("types");
    ParseTypes(types);
    auto* handles = root->FirstChildElement("handles");
    ParseHandles(handles);
    auto* funcs = root->FirstChildElement("functions");
    ParseFunctions(funcs);
}

int Generator::GenerateCAPI()
{
    std::string output = "#pragma once\n#include <stdint.h>\n#include <stdbool.h>\n\n";
    output += GenerateCTypes();

    output += "//=================================DELEGATES=================================\n\n";
    for (auto& f : this->delegates) {
        output += MakeDelegate(*f);
    }
    output += "//==================================HANDLES==================================\n\n";
    for (auto& h : this->handles) {
        output += MakeHandle(*h);
    }
    output += "//=================================FUNCTIONS=================================\n\n";
    for (auto& f : this->functions) {
        output += MakeFunctionDecl(f);
    }

    // Function implementations
    std::string output_cpp = "#include \"wisdom.h\"\n#include <wisdom/wisdom.h>\n\n";
    for (auto& f : function_impl) {
        output_cpp += f;
    }

    std::filesystem::path output_path = output_dir;
    std::filesystem::create_directories(output_path);

    std::ofstream out_cpp(std::filesystem::absolute(output_path / "wisdom.cpp"));
    if (!out_cpp.is_open())
        return 1;
    out_cpp << output_cpp;

    std::ofstream out(std::filesystem::absolute(output_path / "wisdom.h"));
    if (!out.is_open())
        return 1;
    out << output;
    return 0;
}
int Generator::GenerateCPPAPI()
{
    std::string output_api = "#pragma once\n#include <array>\n\n";
    output_api += "namespace wis {\n";
    output_api += GenerateCPPTypes();
    output_api += "//=================================DELEGATES=================================\n\n";
    for (auto& f : this->delegates) {
        output_api += MakeCPPDelegate(*f);
    }
    output_api += "//==============================TYPE TRAITS==============================\n\n";
    output_api += "template <typename T> struct is_flag_enum : public std::false_type {};\n";
    for (auto& t : cpp_type_traits) {
        output_api += t;
    }

    output_api += "}\n";

    std::filesystem::path cpp_output_path = cpp_output_dir;
    std::filesystem::create_directories(cpp_output_path);
    std::ofstream out_api(std::filesystem::absolute(cpp_output_path / "api/api.h"));
    if (!out_api.is_open())
        return 1;
    out_api << output_api;

    std::ofstream out_wisdom(std::filesystem::absolute(cpp_output_path / "wisdom.hpp"));
    if (!out_wisdom.is_open())
        return 1;
    out_wisdom << GenerateCPPExportHeader();

    return 0;
}

std::string Generator::GenerateCTypes()
{
    std::string c_types = GenerateCTypedefs();
    for (auto& s : enums) {
        c_types += MakeCEnum(*s);
    }
    for (auto& s : bitmasks) {
        c_types += MakeCBitmask(*s);
    }
    for (auto& s : structs) {
        c_types += MakeCStruct(*s);
    }
    return c_types;
}
std::string Generator::GenerateCPPTypes()
{
    std::string c_types = GenerateCPPTypedefs();
    for (auto& s : enums) {
        c_types += MakeCPPEnum(*s);
    }
    for (auto& s : bitmasks) {
        c_types += MakeCPPBitmask(*s);
    }
    for (auto& s : structs) {
        c_types += MakeCPPStruct(*s);
    }
    return c_types;
}

std::string Generator::GenerateCTypedefs()
{
    std::string c_types;
    for (auto& s : structs) {
        c_types += wis::format("typedef struct Wis{} Wis{};\n", s->name, s->name);
    }
    for (auto& s : enums) {
        c_types += wis::format("typedef enum Wis{} Wis{};\n", s->name, s->name);
    }
    for (auto& s : bitmasks) {
        c_types += wis::format("typedef enum Wis{} Wis{};\n", s->name, s->name);
    }
    return c_types + '\n';
}
std::string Generator::GenerateCPPTypedefs()
{
    std::string c_types;
    for (auto& s : structs) {
        c_types += wis::format("struct {};\n", s->name);
    }
    return c_types + '\n';
}

std::string Generator::GenerateCPPPlatformTypedefs(std::string_view impl)
{
    std::string output{ "namespace wis{\n\n" };
    for (auto& h : handles) {
        if (h->impl == ImplementedFor::Both)
            output += wis::format("using {} = {}{};\n", h->name, impl, h->name);
    }

    for (auto& f : functions) {
        if (f.this_type.empty()) {
            output += MakeCPPPlatformFunc(f, impl);
        }
    }

    return output + "}\n";
}

std::string Generator::MakeCPPPlatformFunc(WisFunction& func, std::string_view impl)
{
    // 1. return type
    ResolvedType ret_t = func.return_types.empty()
            ? ResolvedType{ TypeInfo::None, "void" }
            : ResolveCPPType(func.return_types[0].type);

    std::vector<ResolvedType> other_rets;
    for (size_t i = 1; i < func.return_types.size(); i++) {
        other_rets.push_back(ResolveCPPType(func.return_types[i].type));
    }

    if (ret_t.first == TypeInfo::String)
        return {}; // skip string returns

    // 2. this type
    ResolvedType this_t = func.this_type.empty()
            ? std::pair{ TypeInfo::None, "" }
            : ResolveCPPType(func.this_type);

    // 3. parameters
    std::vector<ResolvedType> params_t;
    for (auto& p : func.parameters) {
        params_t.push_back(ResolveCPPType(p.type));
    }

    bool impl_based = this_t.first ==
                    TypeInfo::Handle ||
            ret_t.first == TypeInfo::Handle ||
            std::ranges::find_if(params_t.begin(), params_t.end(), [](const ResolvedType& t) {
                return t.first == TypeInfo::Handle;
            }) != params_t.end() ||
            std::ranges::find_if(other_rets.begin(), other_rets.end(), [](const ResolvedType& t) {
                return t.first == TypeInfo::Handle;
            }) != other_rets.end();
    if (!impl_based)
        return {};

    return wis::format("constexpr auto {} = {}{};\n", func.name, impl, func.name);
}

std::string Generator::GenerateCPPExportHeader()
{
    std::string output_wisdom{
        R"(#pragma once
// Select default API
// Override with WISDOM_FORCE_VULKAN

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

#if defined(WISDOM_VULKAN_FOUND) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if WISDOMDX12 && !FORCEVK_SWITCH
#include "wisdom_dx12.h"

)"
    };

    output_wisdom += GenerateCPPPlatformTypedefs("DX12");
    output_wisdom += R"(
#elif WISDOM_VULKAN_FOUND
#include "wisdom_vk.h"

)";
    output_wisdom += GenerateCPPPlatformTypedefs("VK");
    output_wisdom += R"(
#else
#error "No API selected"
#endif
)";
    return output_wisdom;
}

//-----------------------------------------------------------------------------

void Generator::ParseTypes(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("type"); type; type = type->NextSiblingElement("type")) {
        auto category = type->FindAttribute("category")->Value();
        if (std::string_view(category) == "struct") {
            ParseStruct(type);
        } else if (std::string_view(category) == "enum") {
            ParseEnum(type);
        } else if (std::string_view(category) == "bitmask") {
            ParseBitmask(type);
        } else if (std::string_view(category) == "delegate") {
            ParseDelegate(type);
        }
    }
}

std::vector<WisReturnType> ParseFunctionReturn(tinyxml2::XMLElement* func)
{
    std::vector<WisReturnType> ret;
    if (auto returns = func->FindAttribute("returns"))
        return { { returns->Value() } };

    for (auto* param = func->FirstChildElement("ret"); param; param = param->NextSiblingElement("ret")) {
        auto& p = ret.emplace_back();

        auto* type = param->FindAttribute("type")->Value();
        p.type = type;

        if (auto* name = param->FindAttribute("name"))
            p.opt_name = name->Value();

        if (auto* mod = param->FindAttribute("mod"))
            p.modifier = mod->Value();
    }
    return ret;
}

std::vector<WisFunctionParameter> ParseFunctionArgs(tinyxml2::XMLElement* func)
{
    std::vector<WisFunctionParameter> ret;
    for (auto* param = func->FirstChildElement("arg"); param; param = param->NextSiblingElement("arg")) {
        auto& p = ret.emplace_back();

        auto* type = param->FindAttribute("type")->Value();
        auto* name = param->FindAttribute("name")->Value();

        p.type = type;
        p.name = name;

        if (auto* mod = param->FindAttribute("mod"))
            p.modifier = mod->Value();
    }
    return ret;
}

void Generator::ParseFunctions(tinyxml2::XMLElement* type)
{
    for (auto* func = type->FirstChildElement("func"); func; func = func->NextSiblingElement("func")) {
        auto& ref = functions.emplace_back();

        auto name = func->FindAttribute("name")->Value();
        ref.name = name;
        ref.return_types = ParseFunctionReturn(func);
        ref.parameters = ParseFunctionArgs(func);

        if (auto* ret = func->FindAttribute("for"))
            ref.this_type = ret->Value();
    }
}

void Generator::ParseHandles(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("handle"); type; type = type->NextSiblingElement("handle")) {
        auto name = type->FindAttribute("name")->Value();
        auto& ref = handle_map[name];
        handles.emplace_back(&ref);
        ref.name = name;

        if (auto* impl = type->FindAttribute("impl"))
            ref.impl = (impl->Value() == std::string_view("dx")) ? ImplementedFor::DX12 : ImplementedFor::Vulkan;
    }
}

void Generator::ParseStruct(tinyxml2::XMLElement* type)
{
    auto name = type->FindAttribute("name")->Value();
    auto& ref = struct_map[name];
    structs.emplace_back(&ref);
    ref.name = name;

    for (auto* member = type->FirstChildElement("member"); member; member = member->NextSiblingElement("member")) {
        auto& m = ref.members.emplace_back();

        auto* type = member->FindAttribute("type")->Value();
        auto* name = member->FindAttribute("name")->Value();

        auto* arr = member->FindAttribute("array");

        m.type = type;
        m.name = name;
        if (auto* arr = member->FindAttribute("array")) {
            m.array_size = arr->Value();
        }
        if (auto* def = member->FindAttribute("default")) {
            m.default_value = def->Value();
        }
        if (auto* mod = member->FindAttribute("mod")) {
            m.modifier = mod->Value();
        }
    }
}

std::string_view ImplCode(std::string_view impl)
{
    if (impl == "dx")
        return "DX";
    if (impl == "vk")
        return "VK";
    return "";
}

void Generator::ParseEnum(tinyxml2::XMLElement* type)
{
    auto name = type->FindAttribute("name")->Value();
    auto& ref = enum_map[name];
    enums.emplace_back(&ref);
    ref.name = name;

    if (auto* size = type->FindAttribute("type"))
        ref.type = size->Value();

    for (auto* member = type->FirstChildElement("value"); member; member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        auto* name = member->FindAttribute("name")->Value();
        auto* value = member->FindAttribute("value")->Value();
        auto* impl = member->FindAttribute("impl");

        m.name = impl ? wis::format("{}{}", ImplCode(impl->Value()), name) : name;
        m.value = std::stoul(value);
    }
}

void Generator::ParseBitmask(tinyxml2::XMLElement* type)
{
    auto name = type->FindAttribute("name")->Value();
    auto& ref = bitmask_map[name];
    bitmasks.emplace_back(&ref);
    ref.name = name;

    if (auto* size = type->FindAttribute("size"))
        ref.size = std::stoul(size->Value());
    else
        ref.size = 32;

    for (auto* member = type->FirstChildElement("value"); member; member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        auto* name = member->FindAttribute("name")->Value();
        auto* impl = member->FindAttribute("impl");

        m.name = impl ? wis::format("{}{}", ImplCode(impl->Value()), name) : name;

        auto* value = member->FindAttribute("value");
        auto* bit = member->FindAttribute("bit");

        if (value) {
            m.value = std::stoul(value->Value());
            m.type = WisBitmaskValue::Type::Value;
            continue;
        }

        m.bit = std::stoul(bit->Value());
        m.type = WisBitmaskValue::Type::Bit;
    }
}

void Generator::ParseDelegate(tinyxml2::XMLElement* type)
{
    auto name = type->FindAttribute("name")->Value();
    auto& ref = delegate_map[name];
    delegates.emplace_back(&ref);
    ref.name = name;
    ref.parameters = ParseFunctionArgs(type);
}

//-----------------------------------------------------------------------------

std::string MakeCArray(std::string_view type, std::string_view name, std::string_view arr_len)
{
    return wis::format("    {} {}[{}];\n", type, name, arr_len);
}
std::string MakeCPPArray(std::string_view type, std::string_view name, std::string_view arr_len)
{
    return wis::format("    std::array<{}, {}> {} {{}};\n", type, arr_len, name);
}

std::string Generator::MakeCStruct(const WisStruct& s)
{
    auto st_decl = wis::format(
            "struct Wis{}{{\n", s.name);

    for (auto& m : s.members) {

        std::string res_type;
        if (auto it = standard_types.find(m.type); it != standard_types.end())
            res_type = it->second;
        else
            res_type = "Wis" + m.type;

        if (m.modifier == "ptr")
            res_type += '*';

        if (m.array_size.empty()) {
            st_decl += wis::format("    {} {};\n", res_type, m.name);
        } else {
            st_decl += MakeCArray(res_type, m.name, m.array_size);
        }
    }

    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPStruct(const WisStruct& s)
{
    auto st_decl = wis::format(
            "struct {}{{\n", s.name);

    for (auto& m : s.members) {

        std::string res_type;
        if (auto it = standard_types.find(m.type); it != standard_types.end())
            res_type = it->second;
        else
            res_type = "wis::" + m.type;

        if (m.modifier == "ptr")
            res_type += '*';

        if (m.array_size.empty()) {
            std::string def = "";
            if (!m.default_value.empty()) {
                def = enum_map.contains(m.type) || bitmask_map.contains(m.type)
                        ? wis::format(" = {}::{}", res_type, m.default_value)
                        : wis::format(" = {}", m.default_value);
            }
            st_decl += wis::format("    {} {}{};\n", res_type, m.name, def);
        } else {
            st_decl += MakeCPPArray(res_type, m.name, m.array_size);
        }
    }

    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCEnum(const WisEnum& s)
{
    std::string st_decl = wis::format("enum Wis{} {{\n", s.name);

    for (auto& m : s.values) {
        st_decl += wis::format("    Wis{}{} = {},\n", s.name, m.name, m.value);
    }
    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPEnum(const WisEnum& s)
{
    std::string st_decl = !s.type.empty() ? wis::format("enum class {} : {} {{\n", s.name, standard_types.at(s.type)) : wis::format("enum class {} {{\n", s.name);

    for (auto& m : s.values) {
        st_decl += wis::format("    {} = {},\n", m.name, m.value);
    }
    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCBitmask(const WisBitmask& s)
{
    std::string st_decl = wis::format("enum Wis{} {{\n", s.name);

    for (auto& m : s.values) {
        if (m.type == WisBitmaskValue::Type::Value) {
            st_decl += wis::format("    Wis{}{} = 0x{:X},\n", s.name, m.name, m.value);
        } else {
            st_decl += wis::format("    Wis{}{} = 1 << {},\n", s.name, m.name, m.bit);
        }
    }
    st_decl += wis::format("    Wis{}Max = 0x{:X},\n", s.name, (1ull << s.size) - 1);

    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPBitmask(const WisBitmask& s)
{
    std::string st_decl = s.size ? wis::format("enum class {} : uint{}_t {{\n", s.name, s.size) : wis::format("enum class {} {{\n", s.name);

    for (auto& m : s.values) {
        if (m.type == WisBitmaskValue::Type::Value) {
            st_decl += wis::format("    {} = 0x{:X},\n", m.name, m.value);
        } else {
            st_decl += wis::format("    {} = 1 << {},\n", m.name, m.bit);
        }
    }
    st_decl += wis::format("    Max = 0x{:X},\n", (1ull << s.size) - 1);

    st_decl += "};\n\n";
    cpp_type_traits.emplace_back(wis::format("template <> struct is_flag_enum<wis::{}>:public std::true_type {{}};\n", s.name));
    return st_decl;
}

//-----------------------------------------------------------------------------

ResolvedType Generator::ResolveType(const std::string& type)
{
    if (type == "Result")
        return { TypeInfo::Result, "WisResult" };

    if (auto it = standard_types.find(type); it != standard_types.end()) {
        return { type.contains("string") ? TypeInfo::String : TypeInfo::Regular, std::string(it->second) };
    }

    if (auto it = struct_map.find(type); it != struct_map.end()) {
        return {
            TypeInfo::Struct, wis::format("Wis{}", type)
        };
    }
    if (auto it = handle_map.find(type); it != handle_map.end()) {
        return {
            TypeInfo::Handle, wis::format("{}", type)
        };
    }
    if (auto it = delegate_map.find(type); it != delegate_map.end()) {
        return {
            TypeInfo::Delegate, wis::format("Wis{}", type)
        };
    }

    return {
        TypeInfo::Regular, "Wis" + type
    };
}
ResolvedType Generator::ResolveCPPType(const std::string& type)
{
    if (type == "Result")
        return { TypeInfo::Result, "wis::Result" };

    if (auto it = standard_types.find(type); it != standard_types.end()) {
        return { TypeInfo::Regular, std::string(it->second) };
    }

    if (auto it = struct_map.find(type); it != struct_map.end()) {
        return {
            TypeInfo::Struct, wis::format("wis::{}", type)
        };
    }
    if (auto it = handle_map.find(type); it != handle_map.end()) {
        return {
            TypeInfo::Handle, wis::format("{}", type)
        };
    }

    return {
        TypeInfo::Regular, "wis::" + type
    };
}

struct FuncInfo {
    ResolvedType return_type;
    ResolvedType this_t;
    std::span<ResolvedType> other_rets;
    std::span<ResolvedType> params;
    std::string_view impl;
    std::string_view decl;
};

std::string Generator::MakeFunctionDecl(const WisFunction& func)
{
    std::string st_decls;

    // 1. return type
    ResolvedType ret_t = func.return_types.empty()
            ? ResolvedType{ TypeInfo::None, "void" }
            : ResolveType(func.return_types[0].type);

    std::vector<ResolvedType> other_rets;
    for (size_t i = 1; i < func.return_types.size(); i++) {
        other_rets.push_back(ResolveType(func.return_types[i].type));
    }

    if (ret_t.first == TypeInfo::String)
        return {}; // skip string returns

    // 2. this type
    ResolvedType this_t = func.this_type.empty()
            ? std::pair{ TypeInfo::None, "" }
            : ResolveType(func.this_type);

    // 3. parameters
    std::vector<ResolvedType> params_t;
    for (auto& p : func.parameters) {
        params_t.push_back(ResolveType(p.type));
    }

    bool impl_based = this_t.first ==
                    TypeInfo::Handle ||
            ret_t.first == TypeInfo::Handle ||
            std::ranges::find_if(params_t.begin(), params_t.end(), [](const ResolvedType& t) {
                return t.first == TypeInfo::Handle;
            }) != params_t.end() ||
            std::ranges::find_if(other_rets.begin(), other_rets.end(), [](const ResolvedType& t) {
                return t.first == TypeInfo::Handle;
            }) != other_rets.end();

    constexpr static std::array<std::string_view, 2> impls{ "DX12", "VK" };
    std::array<std::string, 2> decls{};

    for (size_t j = 0; j < impl_based + 1; j++) {
        auto& st_decl = decls[j];
        std::string return_t = GetArgString(ret_t, impls[j], "", "");

        st_decl = wis::format("{} {}{}{}(",
                              return_t,
                              impl_based ? impls[j] : "",
                              this_t.second,
                              func.name);

        if (this_t.first != TypeInfo::None)
            st_decl += wis::format("{}, ", GetArgString(this_t, impls[j], "self", this_t.first == TypeInfo::Handle ? "" : "ptr"));

        for (uint32_t i = 0; i < params_t.size(); i++) {
            auto& t = params_t[i];
            auto& p = func.parameters[i];

            st_decl += wis::format("{}, ", GetArgString(t, impls[j], p.name, p.modifier));
        }
        for (uint32_t i = 0; i < other_rets.size(); i++) {
            auto& t = other_rets[i];
            auto& p = func.return_types[i + 1];

            st_decl += wis::format("{}* {}, ", GetArgString(t, impls[j], "", p.modifier), "out_" + p.opt_name);
        }

        if (st_decl.back() == ' ') {
            st_decl.pop_back();
            st_decl.pop_back();
        }

        st_decl += ")";
        function_impl.emplace_back(MakeFunctionImpl(func, { ret_t, this_t, other_rets, params_t, impls[j], st_decl }));
        st_decl += ";\n";
        st_decls += st_decl;
    }

    return st_decls;
}

std::string Generator::MakeFunctionImpl(const WisFunction& func, const FuncInfo& fi)
{
    std::string st_decl{ fi.decl };
    st_decl += "\n{\n";
    // bool constructor = func.name == "Create";
    bool has_this = !func.this_type.empty();

    // static cast as refs
    if (has_this) {
        st_decl += fi.this_t.first == TypeInfo::Handle
                ? wis::format("    auto* xself = reinterpret_cast<wis::{}{}*>(self);\n", fi.impl, func.this_type)
                : st_decl += wis::format("    auto* xself = reinterpret_cast<wis::{}*>(self);\n", func.this_type);
    }

    if (func.name == "Destroy") {
        st_decl += "    delete xself;\n}\n";
        return st_decl;
    }

    std::string args_str;
    for (size_t i = 0; i < fi.params.size(); i++) {
        auto& a = fi.params[i];
        auto& p = func.parameters[i];

        if (a.first == TypeInfo::Handle) {
            args_str += wis::format("*reinterpret_cast<wis::{}{}*>({}), ", fi.impl, p.type, p.name);
        } else if (a.first == TypeInfo::Regular || a.first == TypeInfo::String) {
            args_str += wis::format("{}, ", p.name);
        } else if (a.first == TypeInfo::Enum || a.first == TypeInfo::Delegate) {
            args_str += wis::format("reinterpret_cast<wis::{}>({}), ", p.type, p.name);
        } else if (a.first == TypeInfo::Struct) {
            args_str += wis::format("reinterpret_cast<wis::{}*>({}), ", p.type, p.name);
        }
    }
    if (!args_str.empty() && args_str.back() == ' ') {
        args_str.pop_back();
        args_str.pop_back();
    }

    std::string call = has_this
            ? wis::format("    xself->{}({});\n", func.name, args_str)
            : wis::format("    wis::{}{}({});\n", fi.impl, func.name, args_str);

    if (func.return_types.empty()) {
        return st_decl + call + "}\n";
    }

    // return types
    st_decl += wis::format("    auto&& ret = {}", call);

    bool has_other_results = !fi.other_rets.empty();

    if (!has_other_results) {
        if (fi.return_type.first == TypeInfo::Handle) {
            std::string type = wis::format("wis::{}{}", fi.impl, func.return_types[0].type);
            return st_decl + wis::format("    return reinterpret_cast<{}{}>(new wis::{}(std::move(ret)) }};\n", fi.impl, fi.return_type.second, type);
        }
        return st_decl + wis::format("    return reinterpret_cast<{}&>(ret);\n}}\n", fi.return_type.second);
    }

    // get result index
    bool first_result = fi.return_type.first == TypeInfo::Result;
    int64_t result_idx = -1;
    if (fi.return_type.first == TypeInfo::Result)
        result_idx = 0;
    else {
        auto result = std::ranges::find_if(func.return_types, [](const WisReturnType& t) {
            return t.type == "Result";
        });
        result_idx = result == func.return_types.end() ? -1 : std::distance(func.return_types.begin(), result);
    }

    if (result_idx >= 0)
        st_decl += wis::format("    bool ok = std::get<{}>(ret).status == wis::Status::Success;\n", result_idx);

    for (size_t i = 1; i < func.return_types.size(); i++) {
        auto& a = fi.other_rets[i - 1];
        auto& p = func.return_types[i];

        std::string type = wis::format("wis::{}{}", fi.impl, p.type);
        std::string out_type = wis::format("{}{}", a.first == TypeInfo::Handle ? fi.impl : "", a.second);
        st_decl += result_idx < 0
                ? wis ::format("    *out_{} = reinterpret_cast<{}>(new {}(std::move(std::get<{}>(ret))));\n",
                               p.opt_name, out_type, type, i)
                : wis::format("    *out_{} = ok ? reinterpret_cast<{}>(new {}(std::move(std::get<{}>(ret)))) : reinterpret_cast<{}>(nullptr);\n",
                              p.opt_name, out_type, type, i, out_type);
    }
    return st_decl + wis::format("    return reinterpret_cast<{}&>(std::get<0>(ret));\n}}\n", fi.return_type.second);
}

std::string Generator::MakeCPPDelegate(const WisFunction& func)
{
    std::string st_decls;
    // 3. parameters
    std::vector<ResolvedType> params_t;
    for (auto& p : func.parameters) {
        params_t.push_back(ResolveCPPType(p.type));
    }

    bool impl_based = std::ranges::find_if(params_t.begin(), params_t.end(), [](const ResolvedType& t) {
                          return t.first == TypeInfo::Handle;
                      }) != params_t.end();

    constexpr static std::array<std::string_view, 2> impls{ "DX12", "VK" };

    for (size_t j = 0; j < impl_based + 1; j++) {
        std::string st_decl = wis::format("typedef void (*{}{})(",
                                          impl_based ? impls[j] : "",
                                          func.name);

        for (uint32_t i = 0; i < params_t.size(); i++) {
            auto& t = params_t[i];
            auto& p = func.parameters[i];

            std::string modified = p.modifier == "ptr" ? "*" : "";
            if (t.first == TypeInfo::Handle) {
                st_decl += wis::format("{}{} {}{}, ", impls[j], t.second, modified, p.name);
            } else {
                st_decl += wis::format("{} {}{}, ", t.second, modified, p.name);
            }
        }
        if (st_decl.back() == ' ') {
            st_decl.pop_back();
            st_decl.pop_back();
        }
        st_decl += ");\n";
        st_decls += st_decl;
    }

    return st_decls;
}

std::string GetFullArg(TypeInfo type, const WisFunctionParameter& arg, std::string_view impl)
{
    std::string post_decl;
    std::string pre_decl;
    if (arg.modifier == "ptr") {
        post_decl += '*';
    }
    if (arg.modifier == "const") {
        pre_decl += "const";
    }

    std::string xtype;
    if (type == TypeInfo::Handle) {
        xtype = wis::format("{}{}", impl, arg.type);
    } else if (type == TypeInfo::Struct || type == TypeInfo::Enum || type == TypeInfo::Delegate) {
        xtype = wis::format("Wis{}", arg.type);
    } else {
        xtype = arg.type;
    }

    return wis::format("{} {}{} {}", pre_decl, xtype, post_decl, arg.name);
}

std::string Generator::MakeDelegate(const WisFunction& func)
{
    std::string st_decls;
    // 3. parameters
    std::vector<ResolvedType> params_t;
    for (auto& p : func.parameters) {
        params_t.push_back(ResolveType(p.type));
    }

    bool impl_based = std::ranges::find_if(params_t.begin(), params_t.end(), [](const ResolvedType& t) {
                          return t.first == TypeInfo::Handle;
                      }) != params_t.end();

    constexpr static std::array<std::string_view, 2> impls{ "DX12", "VK" };

    for (size_t j = 0; j < impl_based + 1; j++) {
        std::string st_decl = wis::format("typedef void (*Wis{}{})(",
                                          impl_based ? impls[j] : "",
                                          func.name);

        for (uint32_t i = 0; i < params_t.size(); i++) {
            auto& t = params_t[i];
            auto& p = func.parameters[i];

            std::string modified = p.modifier == "ptr" ? "*" : "";
            if (t.first == TypeInfo::Handle) {
                st_decl += wis::format("{}{} {}{}, ", impls[j], t.second, modified, p.name);
            } else {
                st_decl += wis::format("{} {}{}, ", t.second, modified, p.name);
            }
        }
        if (st_decl.back() == ' ') {
            st_decl.pop_back();
            st_decl.pop_back();
        }
        st_decl += ");\n";
        st_decls += st_decl;
    }

    return st_decls;
}

std::string Generator::MakeHandle(const WisHandle& s)
{
    std::string st_decl;
    if (s.impl & ImplementedFor::DX12) {
        st_decl += wis::format("typedef struct DX12{}_t* DX12{};\n", s.name, s.name);
    }
    if (s.impl & ImplementedFor::Vulkan) {
        st_decl += wis::format("typedef struct VK{}_t* VK{};\n", s.name, s.name);
    }
    return st_decl + '\n';
}

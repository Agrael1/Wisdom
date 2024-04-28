#include "generator.h"
#include "../wisdom/include/wisdom/bridge/format.h"
#include <array>
#include <fstream>
#include <ranges>
#include <stdexcept>

using namespace tinyxml2;

//-----------------------------------------------------------------------------

Generator::Generator(XMLDocument& doc)
{
    auto* root = doc.FirstChildElement("registry");
    if (!root)
        throw std::runtime_error("Failed to load root");

    auto* handles = root->FirstChildElement("handles");
    ParseHandles(handles);
    auto* types = root->FirstChildElement("types");
    ParseTypes(types);
    auto* funcs = root->FirstChildElement("functions");
    ParseFunctions(funcs);
}

int Generator::GenerateCAPI()
{
    using namespace std::chrono;
    std::string output = "//GENERATED\n#pragma once\n#include <stdint.h>\n#include <stdbool.h>\n\n";
    output += GenerateCTypes();

    output += "//"
              "=================================DELEGATES========================"
              "=========\n\n";
    for (auto& [name, d] : delegate_map) {
        output += MakeDelegate(d);
    }
    output += "//"
              "==================================HANDLES========================="
              "=========\n\n";
    for (auto& [name, h] : handle_map) {
        output += MakeHandle(h);
    }
    output += "//"
              "=================================FUNCTIONS========================"
              "=========\n\n";
    for (auto& f : this->functions) {
        output += MakeFunctionDecl(f);
    }

    for (auto& f : function_decls) {
        output += f;
    }

    // Function implementations
    std::string output_cpp ="#include \"wisdom.h\"\n#include <wisdom/wisdom.hpp>\n\n";
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
    using namespace std::chrono;

    std::string output_api = "//GENERATED\n#pragma once\n#include <array>\n\n";
    output_api += "namespace wis {\n";
    output_api += GenerateCPPTypes();
    output_api += "//"
                  "=================================DELEGATES===================="
                  "=============\n\n";
    for (auto& [name, d] : delegate_map) {
        output_api += MakeCPPDelegate(d);
    }
    output_api += "//==============================TYPE "
                  "TRAITS==============================\n\n";
    output_api += "template <typename T> struct is_flag_enum : public "
                  "std::false_type {};\n";
    for (auto& t : cpp_type_traits) {
        output_api += t;
    }

    output_api += "//============================== CONSTS "
                  "==============================\n\n";

    output_api += R"(
static inline constexpr Result success{
    wis::Status::Ok, "Operation succeeded"
};

template<typename RetTy>
struct ResultValue{
    Result status;
    RetTy value;
    
    constexpr ResultValue(RetTy value)noexcept :status(success), value(std::move(value)){}
    constexpr ResultValue(Result status)noexcept :status(status){}
    constexpr ResultValue(Result status, RetTy value)noexcept :status(status), value(std::move(value)){}
};
)";
    output_api += "}\n";

    std::filesystem::path cpp_output_path = cpp_output_dir;
    std::filesystem::path cpp_output_path_api = cpp_output_path / "generated/api";
    std::filesystem::path cpp_output_path_dx12 = cpp_output_path / "generated/dx12";
    std::filesystem::path cpp_output_path_vulkan = cpp_output_path / "generated/vulkan";

    std::filesystem::create_directories(cpp_output_path_api);
    std::filesystem::create_directories(cpp_output_path_dx12);
    std::filesystem::create_directories(cpp_output_path_vulkan);

    std::ofstream out_api(std::filesystem::absolute(cpp_output_path_api / "api.h"));
    if (!out_api.is_open())
        return 1;
    out_api << output_api;

    std::ofstream out_wisdom(std::filesystem::absolute(cpp_output_path / "wisdom.hpp"));
    if (!out_wisdom.is_open())
        return 1;
    out_wisdom << GenerateCPPExportHeader();

    std::ofstream out_dxapi(std::filesystem::absolute(cpp_output_path_dx12 / "dx12_structs.hpp"));
    if (!out_dxapi.is_open())
        return 1;

    std::string dxapi = 
            "#pragma once\n#include <wisdom/dx12/dx12_views.h>\n#include "
            "<wisdom/generated/api/api.h>\n#include "
            "<wisdom/util/flags.h>\n\nnamespace wis{\n";
    for (auto i : variants) {
        if (i->this_type.empty()) {
            dxapi += MakeCPPVariant(*i, ImplementedFor::DX12);
        }
    }
    for (auto& c : cpp_conversion) {
        if (c.impl == ImplementedFor::DX12)
            dxapi += c.value;
    }
    out_dxapi << dxapi + "}\n";

    std::ofstream out_vkapi(std::filesystem::absolute(cpp_output_path_vulkan / "vk_structs.hpp"));
    if (!out_vkapi.is_open())
        return 1;

    std::string vkapi = 
            "#pragma once\n#include <wisdom/vulkan/vk_views.h>\n#include "
            "<wisdom/generated/api/api.h>\n#include "
            "<wisdom/util/flags.h>\n\nnamespace wis{\n";
    for (auto i : variants) {
        if (i->this_type.empty()) {
            vkapi += MakeCPPVariant(*i, ImplementedFor::Vulkan);
        }
    }
    for (auto& c : cpp_conversion) {
        if (c.impl == ImplementedFor::Vulkan)
            vkapi += c.value;
    }

    out_vkapi << vkapi + "}\n";
    return 0;
}

std::string Generator::GenerateCTypes()
{
    std::string c_types = GenerateCTypedefs();
    for (auto& [name, enum_t] : enum_map) {
        c_types += MakeCEnum(enum_t);
    }
    for (auto& [name, enum_t] : bitmask_map) {
        c_types += MakeCBitmask(enum_t);
    }
    for (auto& s : structs) {
        c_types += MakeCStruct(*s);
    }
    for (auto& [name, v] : variant_map) {
        c_types += MakeCVariant(v);
    }
    return c_types;
}
std::string Generator::GenerateCPPTypes()
{
    std::string c_types = GenerateCPPTypedefs();
    for (auto& [name, enum_t] : enum_map) {
        c_types += MakeCPPEnum(enum_t);
    }
    for (auto& [name, enum_t] : bitmask_map) {
        c_types += MakeCPPBitmask(enum_t);
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
    for (auto& [name, _] : enum_map) {
        auto full_name = GetCFullTypename(name, "");
        c_types += wis::format("typedef enum {} {};\n", full_name, full_name);
    }
    for (auto& [name, bm] : bitmask_map) {
        auto full_name = GetCFullTypename(name, "");
        c_types += wis::format("typedef enum {}Bits {}Bits;\n", full_name, full_name);
        c_types += wis::format("typedef {} {};\n",
                               bm.type.empty() ? "uint32_t" : standard_types.at(bm.type), full_name);
    }
    for (auto& [name, s] : variant_map) {
        for (auto& impl : s.impls) {
            if (impl.impl == ImplementedFor::Both) {
                c_types += wis::format("typedef struct DX12{} DX12{};\n", s.name, s.name);
                c_types += wis::format("typedef struct VK{} VK{};\n", s.name, s.name);
                break;
            }
            c_types += wis::format("typedef struct {}{} {}{};\n", impls[+impl.impl], s.name,
                                   impls[+impl.impl], s.name);
        }
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

    output += wis::format("inline constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::{};\n\n",
                          impl == "VK" ? "SPIRV" : "DXIL");
    for (auto& [name, h] : handle_map) {
        if (h.impl == ImplementedFor::Both)
            output += wis::format("using {} = {}{};\n", name, impl, name);
    }

    for (auto& f : functions) {
        if (f.this_type.empty()) {
            output += MakeCPPPlatformFunc(f, impl);
        }
    }
    for (auto& v : variants) {
        output += wis::format("using {} = {}{};\n", v->name, impl, v->name);
    }

    return output + "}\n";
}

std::string Generator::MakeCPPPlatformFunc(WisFunction& func, std::string_view impl)
{
    if (func.impl != ImplementedFor::Both)
        return "";
    return wis::format("constexpr auto {} = wis::{}{};\n", func.name, impl, func.name);
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

#if WISDOM_VULKAN && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if WISDOM_DX12 && !FORCEVK_SWITCH
#include "wisdom_dx12.h"

)"
    };

    output_wisdom += GenerateCPPPlatformTypedefs("DX12");
    output_wisdom += R"(
#elif WISDOM_VULKAN
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
    for (auto* type = types->FirstChildElement("type"); type;
         type = type->NextSiblingElement("type")) {
        auto category = type->FindAttribute("category")->Value();
        if (std::string_view(category) == "struct") {
            ParseStruct(*type);
        } else if (std::string_view(category) == "enum") {
            ParseEnum(*type);
        } else if (std::string_view(category) == "bitmask") {
            ParseBitmask(*type);
        } else if (std::string_view(category) == "delegate") {
            ParseDelegate(type);
        } else if (std::string_view(category) == "variant") {
            ParseVariant(*type);
        }
    }
}

std::vector<WisReturnType> Generator::ParseFunctionReturn(tinyxml2::XMLElement* func)
{
    std::vector<WisReturnType> ret;
    if (auto returns = func->FindAttribute("returns")) {
        ret.emplace_back() = WisReturnType{
            .type_info = GetTypeInfo(returns->Value()),
            .type = returns->Value(),
        };
        return ret;
    }

    for (auto* param = func->FirstChildElement("ret"); param;
         param = param->NextSiblingElement("ret")) {
        auto& p = ret.emplace_back();

        auto* type = param->FindAttribute("type")->Value();
        p.type = type;
        p.type_info = GetTypeInfo(type);

        if (auto* name = param->FindAttribute("name"))
            p.opt_name = name->Value();

        if (auto* mod = param->FindAttribute("mod"))
            p.modifier = mod->Value();
    }
    return ret;
}

std::vector<WisFunctionParameter> Generator::ParseFunctionArgs(tinyxml2::XMLElement* func)
{
    std::vector<WisFunctionParameter> ret;
    for (auto* param = func->FirstChildElement("arg"); param;
         param = param->NextSiblingElement("arg")) {
        auto& p = ret.emplace_back();

        auto* type = param->FindAttribute("type")->Value();
        auto* name = param->FindAttribute("name")->Value();

        p.type = type;
        p.name = name;
        p.type_info = GetTypeInfo(type);

        if (auto* mod = param->FindAttribute("mod"))
            p.modifier = mod->Value();
    }
    return ret;
}

void Generator::ParseFunctions(tinyxml2::XMLElement* type)
{
    for (auto* func = type->FirstChildElement("func"); func;
         func = func->NextSiblingElement("func")) {
        auto& ref = functions.emplace_back();

        auto name = func->FindAttribute("name")->Value();
        ref.name = name;
        ref.return_types = ParseFunctionReturn(func);
        ref.parameters = ParseFunctionArgs(func);

        if (auto* ret = func->FindAttribute("for")) {
            ref.this_type = ret->Value();
            ref.this_type_info = GetTypeInfo(ref.this_type);
        }

        for (auto& i : ref.return_types) {
            if (ref.impl == Unspecified)
                (uint32_t&)ref.impl = GetImplementedFor(i.type);
            else
                (uint32_t&)ref.impl &= GetImplementedFor(i.type);
        }
        for (auto& i : ref.parameters) {
            if (ref.impl == Unspecified)
                (uint32_t&)ref.impl = GetImplementedFor(i.type);
            else
                (uint32_t&)ref.impl &= GetImplementedFor(i.type);
        }
        if (ref.impl == Unspecified)
            (uint32_t&)ref.impl = GetImplementedFor(ref.this_type);
        else
            (uint32_t&)ref.impl &= GetImplementedFor(ref.this_type);
    }
}

static ImplementedFor ImplCode(std::string_view impl) noexcept
{
    if (impl == "dx")
        return ImplementedFor::DX12;
    if (impl == "vk")
        return ImplementedFor::Vulkan;
    return ImplementedFor::Both;
}

void Generator::ParseHandles(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("handle"); type;
         type = type->NextSiblingElement("handle")) {
        auto name = type->FindAttribute("name")->Value();
        auto& ref = handle_map[name];
        ref.name = name;

        if (auto* impl = type->FindAttribute("impl"))
            ref.impl = ImplCode(impl->Value());
    }
}

void Generator::ParseStruct(tinyxml2::XMLElement& type)
{
    auto name = type.FindAttribute("name")->Value();
    auto& ref = struct_map[name];
    structs.emplace_back(&ref);
    ref.name = name;

    for (auto* member = type.FirstChildElement("member"); member;
         member = member->NextSiblingElement("member")) {
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

void Generator::ParseEnum(tinyxml2::XMLElement& type)
{
    std::unordered_map<std::string_view, std::string> cvts;

    auto name = type.FindAttribute("name")->Value();
    auto& ref = enum_map[name];
    ref.name = name;

    if (auto* size = type.FindAttribute("type"))
        ref.type = size->Value();

    for (auto* impl_type = type.FirstChildElement("impl_type"); impl_type;
         impl_type = impl_type->NextSiblingElement("impl_type")) {
        auto impl_for = impl_type->FindAttribute("for")->Value();
        auto impl_for_code = ImplCode(impl_for);
        auto impl_name = impl_type->FindAttribute("name")->Value();

        std::string_view def_value = "{}";
        if (auto xdefault = impl_type->FindAttribute("default"))
            def_value = xdefault->Value();

        if (auto direct = impl_type->FindAttribute("direct")) {
            auto cvt = wis::format("inline constexpr {} convert_{}({} value) noexcept {{\n  "
                                   "  return static_cast<{}>(value);\n}}\n",
                                   impl_name, impl_for, name, impl_name);
            cpp_conversion.emplace_back(std::move(cvt), impl_for_code);
            continue;
        }
        cvts.emplace(impl_for, wis::format("inline constexpr {} convert_{}({} value) noexcept{{\n    "
                                           "switch(value){{\n    default: return {};\n",
                                           impl_name, impl_for, name, def_value));
    }

    for (auto* member = type.FirstChildElement("value"); member;
         member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        m.name = member->FindAttribute("name")->Value();
        m.value = std::stoll(member->FindAttribute("value")->Value());
        if (auto* impl = member->FindAttribute("impl"))
            m.impl = ImplCode(impl->Value());

        for (auto* impl = member->FirstChildElement("impl"); impl;
             impl = impl->NextSiblingElement("impl")) {
            auto impl_name = impl->FindAttribute("name")->Value();
            auto value = impl->FindAttribute("value")->Value();

            cvts[impl_name] += wis::format("    case {}::{}: return {};\n", name, m.name, value);
        }
    }
    for (auto&& [k, v] : cvts) {
        v += "    }\n}\n";
        cpp_conversion.emplace_back(std::move(v), ImplCode(k));
    }
}

void Generator::ParseBitmask(tinyxml2::XMLElement& type)
{
    std::unordered_map<std::string_view, std::string> cvts;
    auto name = type.FindAttribute("name")->Value();
    auto& ref = bitmask_map[name];
    ref.name = name;

    if (auto* size = type.FindAttribute("type"))
        ref.type = size->Value();

    for (auto* impl_type = type.FirstChildElement("impl_type"); impl_type;
         impl_type = impl_type->NextSiblingElement("impl_type")) {
        auto impl_for = impl_type->FindAttribute("for")->Value();
        auto impl_for_code = ImplCode(impl_for);
        auto impl_name = impl_type->FindAttribute("name")->Value();

        std::string_view def_value = "{}";
        if (auto xdefault = impl_type->FindAttribute("default"))
            def_value = xdefault->Value();

        if (auto direct = impl_type->FindAttribute("direct")) {
            auto cvt = wis::format("inline constexpr {} convert_{}({} value) noexcept {{\n  "
                                   "  return static_cast<{}>(value);\n}}\n",
                                   impl_name, impl_for, name, impl_name);
            cpp_conversion.emplace_back(std::move(cvt), impl_for_code);
            continue;
        }
        cvts.emplace(impl_for, wis::format("inline constexpr {} convert_{}({} value) noexcept{{\n    "
                                           "{} output = {};\n",
                                           impl_name, impl_for, name, impl_name, def_value));
    }

    for (auto* member = type.FirstChildElement("value"); member;
         member = member->NextSiblingElement("value")) {
        auto& m = ref.values.emplace_back();

        m.name = member->FindAttribute("name")->Value();
        if (auto* impl = member->FindAttribute("impl"))
            m.impl = ImplCode(impl->Value());

        auto* value = member->FindAttribute("value");
        auto* bit = member->FindAttribute("bit");

        if (value) {
            m.value = std::stoull(value->Value());
            m.type = WisBitmaskValue::Type::Value;
            continue;
        }

        m.bit = std::stoul(bit->Value());
        m.type = WisBitmaskValue::Type::Bit;

        for (auto* impl = member->FirstChildElement("impl"); impl;
             impl = impl->NextSiblingElement("impl")) {
            auto impl_name = impl->FindAttribute("name")->Value();
            auto value = impl->FindAttribute("value")->Value();

            cvts[impl_name] += wis::format("    if(value & {}::{}) output |= {};\n", name, m.name, value);
        }
    }
    for (auto&& [k, v] : cvts) {
        v += "    return output;\n}\n";
        cpp_conversion.emplace_back(std::move(v), ImplCode(k));
    }
}

void Generator::ParseDelegate(tinyxml2::XMLElement* type)
{
    auto name = type->FindAttribute("name")->Value();
    auto& ref = delegate_map[name];
    ref.name = name;
    ref.parameters = ParseFunctionArgs(type);
}

void Generator::ParseVariant(tinyxml2::XMLElement& type)
{
    auto name = type.FindAttribute("name")->Value();
    auto& ref = variant_map[name];
    ref.name = name;
    variants.emplace_back(&ref);

    if (auto* this_t = type.FindAttribute("for")) {
        ref.this_type = this_t->Value();
    }

    for (auto* impl = type.FirstChildElement("impl"); impl; impl = impl->NextSiblingElement("impl")) {
        auto impl_name = impl->FindAttribute("name")->Value();
        auto& impl_ref = ref.impls.emplace_back();
        impl_ref.impl = ImplCode(impl_name);

        for (auto* member = impl->FirstChildElement("member"); member;
             member = member->NextSiblingElement("member")) {
            auto& m = impl_ref.members.emplace_back();

            auto* type = member->FindAttribute("type")->Value();
            auto* name = member->FindAttribute("name")->Value();

            auto* arr = member->FindAttribute("array");

            m.type = type;
            m.name = name;

            if (arr) {
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
}

//-----------------------------------------------------------------------------
std::string Generator::MakeCVariant(const WisVariant& s)
{
    using namespace std::string_literals;
    std::string st_decls;

    auto make_impl = [this](const WisVariant& s, const WisVariantImpl& impl,
                            std::string_view impl_tag) {
        auto full_name = GetCFullTypename(s.name, impl_tag);

        if (!s.this_type.empty()) {
            auto this_name = GetCFullTypename(s.this_type, impl_tag);
            function_decls.emplace_back(
                    wis::format("{} As{}({} self);\n", full_name, full_name, this_name));
            function_impl.emplace_back(
                    wis::format("{} As{}({} self)\n{{\n    return "
                                "reinterpret_cast<{}&>(static_cast<wis::{}>(reinterpret_cast<wis::{}&"
                                ">(*self)));\n}}\n",
                                full_name, full_name, this_name, full_name, full_name, this_name));
        }

        auto st_decl = wis::format("struct {}{{\n", full_name);

        for (auto& m : impl.members) {
            auto mfull_name = GetCFullTypename(m.type, impl_tag);
            std::string res_type;

            if (m.modifier == "ptr")
                res_type = '*';

            st_decl += m.array_size.empty()
                    ? wis::format("    {} {};\n", mfull_name + res_type, m.name)
                    : wis::format("    {} {}[{}];\n", mfull_name, m.name, m.array_size);
        }

        st_decl += "};\n\n";
        return st_decl;
    };

    for (auto& i : s.impls) {
        if (i.impl == ImplementedFor::Both) {
            st_decls += make_impl(s, i, impls[1]);
            st_decls += make_impl(s, i, impls[2]);
            return st_decls;
        }
        st_decls += make_impl(s, i, impls[+i.impl]);
    }
    return st_decls;
}

std::string Generator::MakeCPPVariant(const WisVariant& s, ImplementedFor impl)
{
    using namespace std::string_literals;
    std::string st_decls;

    auto make_impl = [this](const WisVariant& s, const WisVariantImpl& impl,
                            std::string_view impl_tag) {
        auto full_name = s.name;
        auto st_decl = wis::format("struct {}{}{{\n", impl_tag, full_name);

        for (auto& m : impl.members) {
            auto mfull_name = GetCPPFullTypename(m.type, impl_tag);

            std::string mod;
            if (m.modifier == "ptr")
                mod = '*';

            if (m.array_size.empty()) {
                std::string def = "";
                if (!m.default_value.empty()) {
                    def = enum_map.contains(m.type) || bitmask_map.contains(m.type)
                            ? wis::format(" = {}::{}", mfull_name, m.default_value)
                            : wis::format(" = {}", m.default_value);
                }
                st_decl += wis::format("    {} {}{};\n", mfull_name + mod, m.name, def);
            } else {
                st_decl += wis::format("    std::array<{}, {}> {} {{}};\n", mfull_name, m.array_size, m.name);
            }
        }

        st_decl += "};\n\n";
        return st_decl;
    };

    for (auto& i : s.impls) {
        st_decls += make_impl(s, i, impls[+impl]);
    }
    return st_decls;
}

std::string Generator::MakeCStruct(const WisStruct& s)
{
    auto full_name = GetCFullTypename(s.name);
    auto st_decl = wis::format("struct {}{{\n", full_name);

    for (auto& m : s.members) {
        auto mfullname = GetCFullTypename(m.type);

        std::string res_type;

        if (m.modifier == "ptr")
            res_type = '*';

        st_decl += m.array_size.empty()
                ? wis::format("    {} {};\n", mfullname + res_type, m.name)
                : wis::format("    {} {}[{}];\n", mfullname, m.name, m.array_size);
    }

    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPStruct(const WisStruct& s)
{
    auto full_name = s.name;
    auto st_decl = wis::format("struct {}{{\n", full_name);

    for (auto& m : s.members) {
        auto mfullname = GetCPPFullTypename(m.type);

        std::string mod;
        if (m.modifier == "ptr")
            mod = '*';

        if (m.array_size.empty()) {
            std::string def = "";
            if (!m.default_value.empty()) {
                def = enum_map.contains(m.type) || bitmask_map.contains(m.type)
                        ? wis::format(" = {}::{}", mfullname, m.default_value)
                        : wis::format(" = {}", m.default_value);
            }
            st_decl += wis::format("    {} {}{};\n", mfullname + mod, m.name, def);
        } else {
            st_decl += wis::format("    std::array<{}, {}> {} {{}};\n", mfullname, m.array_size, m.name);
        }
    }

    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCEnum(const WisEnum& s)
{
    auto full_name = GetCFullTypename(s.name, "");
    std::string st_decl = wis::format("enum {} {{\n", full_name);

    for (auto& m : s.values) {
        st_decl += wis::format("    {}{}{} = {},\n", s.name, impls[+m.impl], m.name, m.value);
    }

    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPEnum(const WisEnum& s)
{
    std::string st_decl =
            !s.type.empty() ? wis::format("enum class {} : {} {{\n", s.name, standard_types.at(s.type))
                            : wis::format("enum class {} {{\n", s.name);

    for (auto& m : s.values) {
        st_decl += wis::format("    {}{} = {},\n", impls[+m.impl], m.name, m.value);
    }
    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCBitmask(const WisBitmask& s)
{
    auto full_name = GetCFullTypename(s.name, "");
    std::string st_decl = wis::format("enum {}Bits {{\n", full_name);

    for (auto& m : s.values) {
        st_decl += m.type == WisBitmaskValue::Type::Value
                ? wis::format("    {}{}{} = 0x{:X},\n", s.name, impls[+m.impl], m.name, m.value)
                : wis::format("    {}{}{} = 1 << {},\n", s.name, impls[+m.impl], m.name, m.bit);
    }
    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPBitmask(const WisBitmask& s)
{
    auto full_name = GetCFullTypename(s.name, "");
    std::string st_decl =
            !s.type.empty() ? wis::format("enum class {} : {} {{\n", s.name, standard_types.at(s.type))
                            : wis::format("enum class {} {{\n", s.name);

    for (auto& m : s.values) {
        st_decl += m.type == WisBitmaskValue::Type::Value
                ? wis::format("    {}{} = 0x{:X},\n", impls[+m.impl], m.name, m.value)
                : wis::format("    {}{} = 1 << {},\n", impls[+m.impl], m.name, m.bit);
    }

    st_decl += "};\n\n";
    cpp_type_traits.emplace_back(wis::format(
            "template <> struct is_flag_enum<wis::{}>:public std::true_type {{}};\n", s.name));
    return st_decl;
}

//-----------------------------------------------------------------------------

std::string Generator::MakeFunctionDecl(const WisFunction& func)
{
    std::string st_decls;
    size_t max = func.impl == Both ? 2 : 1;
    size_t min = func.impl == Both ? 1 : func.impl;
    auto ximpls = func.impl == Unspecified ? std::span{ impls }.subspan(0, 1)
                                           : std::span{ impls }.subspan(min, max);

    for (auto&& impl : ximpls) {
        std::string return_t =
                func.return_types.empty() ? "void" : GetCFullTypename(func.return_types[0].type, impl);

        std::string this_t = func.this_type.empty() ? "" : GetCFullTypename(func.this_type, impl);

        std::string params =
                this_t.empty()
                ? ""
                : wis::format("{} self, ",
                              func.this_type_info == TypeInfo::Handle ? this_t : this_t + '*');
        for (auto& p : func.parameters) {
            params += wis::format("{}, ", GetCFullArg(p, impl));
        }
        for (size_t i = 1; i < func.return_types.size(); i++) {
            params += wis::format("{}* out_{}, ", GetCFullTypename(func.return_types[i].type, impl),
                                  func.return_types[i].opt_name);
        }

        if (params.ends_with(", ")) {
            params.pop_back();
            params.pop_back();
        }

        std::string st_decl =
                wis::format("{} {}{}{}({})", return_t, impl, func.name == "Destroy" ? func.this_type : "",
                            func.name, params);

        function_impl.emplace_back(MakeFunctionImpl(func, st_decl, impl));

        st_decl += ";\n";
        st_decls += st_decl;
    }

    return st_decls;
}

std::string Generator::MakeFunctionImpl(const WisFunction& func, std::string_view decl,
                                        std::string_view impl)
{
    std::string st_decl{ decl };
    st_decl += "\n{\n";
    bool has_this = !func.this_type.empty();

    if (has_this) {
        st_decl += wis::format("    auto* xself = reinterpret_cast<{}*>(self);\n",
                               GetCPPFullTypename(func.this_type, impl));
    }

    if (func.name == "Destroy") {
        st_decl += "    delete xself;\n}\n";
        return st_decl;
    }

    std::string args_str;
    for (auto& a : func.parameters) {
        if (a.type_info == TypeInfo::Handle) {
            args_str += wis::format("*reinterpret_cast<{}*>({}), ", GetCPPFullTypename(a.type), a.name);
        } else if (a.type_info == TypeInfo::Regular || a.type_info == TypeInfo::String) {
            args_str += wis::format("{}, ", a.name);
        } else {
            std::string_view mod_post;
            if (a.modifier == "ptr") {
                mod_post = "*";
            }

            args_str +=
                    wis::format("reinterpret_cast<{}{}>({}), ", GetCPPFullTypename(a.type), mod_post, a.name);
        }
    }
    if (!args_str.empty() && args_str.back() == ' ') {
        args_str.pop_back();
        args_str.pop_back();
    }

    std::string call = has_this ? wis::format("xself->{}({});\n", func.name, args_str)
                                : wis::format("wis::{}{}({});\n",
                                              func.impl == ImplementedFor::Unspecified ? "" : impl,
                                              func.name, args_str);
    if (func.return_types.empty()) {
        return st_decl + call + "}\n";
    }

    // return types
    st_decl += wis::format("    auto&& ret = {}", call);

    if (func.return_types.size() <= 1) {
        if (func.return_types[0].type_info == TypeInfo::Handle) {
            return st_decl + wis::format("    return reinterpret_cast<{}>(new {}(std::move(ret)) }};\n", GetCFullTypename(func.return_types[0].type), GetCPPFullTypename(func.return_types[0].type));
        }
        return st_decl + wis::format("    return reinterpret_cast<{}&>(ret);\n}}\n", GetCFullTypename(func.return_types[0].type));
    }

    // get result index
    auto result = std::ranges::find_if(func.return_types,
                                       [](const WisReturnType& t) { return t.type == "Result"; });
    int64_t result_idx =
            result == func.return_types.end() ? -1 : std::distance(func.return_types.begin(), result);

    if (result_idx >= 0)
        st_decl += wis::format("    bool ok = std::get<{}>(ret).status == wis::Status::Success;\n",
                               result_idx);

    for (size_t i = 1; i < func.return_types.size(); i++) {
        auto& p = func.return_types[i];
        std::string type = GetCPPFullTypename(p.type, impl);
        std::string out_type = GetCFullTypename(p.type, impl);

        if (p.type_info == TypeInfo::Handle)
            st_decl += result_idx < 0 ? wis ::format("    *out_{} = reinterpret_cast<{}>(new "
                                                     "{}(std::move(std::get<{}>(ret))));\n",
                                                     p.opt_name, out_type, type, i)
                                      : wis::format("    *out_{} = ok ? reinterpret_cast<{}>(new "
                                                    "{}(std::move(std::get<{}>(ret)))) : "
                                                    "reinterpret_cast<{}>(nullptr);\n",
                                                    p.opt_name, out_type, type, i, out_type);
        else
            st_decl += wis::format("    if(ok) *out_{} = reinterpret_cast<{}&>(std::get<{}>(ret))",
                                   p.opt_name, out_type, i);
    }
    return st_decl + wis::format("    return reinterpret_cast<{}&>(std::get<0>(ret));\n}}\n", GetCFullTypename(func.return_types[0].type));
}

std::string Generator::GetCFullTypename(std::string_view type, std::string_view impl)
{
    if (auto it = standard_types.find(type); it != standard_types.end())
        return std::string(it->second);

    if (auto it = enum_map.find(type); it != enum_map.end())
        return "Wis" + std::string(type);

    if (auto it = bitmask_map.find(type); it != bitmask_map.end())
        return "Wis" + std::string(type);

    if (auto it = variant_map.find(type); it != variant_map.end())
        return std::string(impl) + std::string(type);

    if (auto it = struct_map.find(type); it != struct_map.end())
        return "Wis" + std::string(type);

    if (auto it = handle_map.find(type); it != handle_map.end())
        return std::string(impl) + std::string(type);

    if (auto it = delegate_map.find(type); it != delegate_map.end())
        return std::string(type);

    return "";
}

std::string Generator::GetCPPFullTypename(std::string_view type, std::string_view impl)
{
    if (auto it = standard_types.find(type); it != standard_types.end())
        return std::string(it->second);

    if (auto it = enum_map.find(type); it != enum_map.end())
        return "wis::" + std::string(type);

    if (auto it = bitmask_map.find(type); it != bitmask_map.end())
        return "wis::" + std::string(type);

    if (auto it = variant_map.find(type); it != variant_map.end())
        return "wis::" + std::string(impl) + std::string(type);

    if (auto it = struct_map.find(type); it != struct_map.end())
        return "wis::" + std::string(type);

    if (auto it = handle_map.find(type); it != handle_map.end())
        return "wis::" + std::string(impl) + std::string(type);

    if (auto it = delegate_map.find(type); it != delegate_map.end())
        return std::string(type);

    return "";
}

std::string Generator::GetCFullArg(const WisFunctionParameter& arg, std::string_view impl)
{
    std::string post_decl;
    std::string pre_decl;
    if (arg.modifier == "ptr") {
        post_decl += '*';
    }
    if (arg.modifier == "const") {
        pre_decl += "const";
    }
    return wis::format("{} {}{} {}", pre_decl, GetCFullTypename(arg.type, impl), post_decl, arg.name);
}

std::string Generator::GetCPPFullArg(const WisFunctionParameter& arg, std::string_view impl)
{
    std::string post_decl;
    std::string pre_decl;
    if (arg.modifier == "ptr") {
        post_decl += '*';
    }
    if (arg.modifier == "const") {
        pre_decl += "const";
    }
    return wis::format("{} {}{} {}", pre_decl, GetCPPFullTypename(arg.type, impl), post_decl,
                       arg.name);
}

TypeInfo Generator::GetTypeInfo(std::string_view type)
{
    if (standard_types.contains(type))
        return TypeInfo::Regular;

    if (enum_map.contains(type))
        return TypeInfo::Enum;

    if (bitmask_map.contains(type))
        return TypeInfo::Bitmask;

    if (variant_map.contains(type))
        return TypeInfo::Variant;

    if (struct_map.contains(type))
        return TypeInfo::Struct;

    if (handle_map.contains(type))
        return TypeInfo::Handle;

    if (delegate_map.contains(type))
        return TypeInfo::Delegate;

    return TypeInfo::None;
}

ImplementedFor Generator::GetImplementedFor(std::string_view type)
{
    if (auto it = variant_map.find(type); it != variant_map.end()) {
        ImplementedFor ret = ImplementedFor::None;
        for (auto& i : it->second.impls) {
            ret = ImplementedFor(+ret | +i.impl);
        }
        return ret;
    }

    if (auto it = handle_map.find(type); it != handle_map.end())
        return it->second.impl;
    return ImplementedFor::Unspecified;
}

std::string Generator::MakeCPPDelegate(const WisFunction& func)
{
    std::string st_decls;
    size_t max = func.impl == Both ? 2 : 1;
    size_t min = func.impl == Both ? 1 : func.impl;
    auto ximpls = func.impl == Unspecified ? std::span{ impls }.subspan(0, 1)
                                           : std::span{ impls }.subspan(min, max);

    for (auto&& impl : ximpls) {
        std::string return_t =
                func.return_types.empty() ? "void" : GetCPPFullTypename(func.return_types[0].type, impl);

        std::string this_t = func.this_type.empty() ? "" : GetCPPFullTypename(func.this_type, impl);

        std::string params =
                this_t.empty()
                ? ""
                : wis::format("{} self, ",
                              func.this_type_info == TypeInfo::Handle ? this_t : this_t + '*');
        for (auto& p : func.parameters) {
            params += wis::format("{}, ", GetCPPFullArg(p, impl));
        }
        for (size_t i = 1; i < func.return_types.size(); i++) {
            params += wis::format("{}* out_{}, ", GetCPPFullTypename(func.return_types[i].type, impl),
                                  func.return_types[i].opt_name);
        }

        if (params.ends_with(", ")) {
            params.pop_back();
            params.pop_back();
        }

        std::string st_decl = wis::format("typedef {} (*{}{})({})", return_t, impl, func.name, params);

        st_decl += ";\n";
        st_decls += st_decl;
    }

    return st_decls;
}

std::string Generator::MakeDelegate(const WisFunction& func)
{
    std::string st_decls;
    size_t max = func.impl == Both ? 2 : 1;
    size_t min = func.impl == Both ? 1 : func.impl;
    auto ximpls = func.impl == Unspecified ? std::span{ impls }.subspan(0, 1)
                                           : std::span{ impls }.subspan(min, max);

    for (auto&& impl : ximpls) {
        std::string return_t =
                func.return_types.empty() ? "void" : GetCFullTypename(func.return_types[0].type, impl);

        std::string this_t = func.this_type.empty() ? "" : GetCFullTypename(func.this_type, impl);

        std::string params =
                this_t.empty()
                ? ""
                : wis::format("{} self, ",
                              func.this_type_info == TypeInfo::Handle ? this_t : this_t + '*');
        for (auto& p : func.parameters) {
            params += wis::format("{}, ", GetCFullArg(p, impl));
        }
        for (size_t i = 1; i < func.return_types.size(); i++) {
            params += wis::format("{}* out_{}, ", GetCFullTypename(func.return_types[i].type, impl),
                                  func.return_types[i].opt_name);
        }

        if (params.ends_with(", ")) {
            params.pop_back();
            params.pop_back();
        }

        std::string st_decl = wis::format("typedef {} (*{}{})({})", return_t, impl, func.name, params);

        st_decl += ";\n";
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

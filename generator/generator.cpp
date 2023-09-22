#include "generator.h"
#include "../wisdom/include/wisdom/bridge/format.h"
#include <fstream>
#include <ranges>
#include <array>

using namespace tinyxml2;

int Generator::GenerateCAPI(std::filesystem::path file)
{
    structs.clear();
    enums.clear();
    bitmasks.clear();

    if (doc.LoadFile(file.string().c_str()) != XML_SUCCESS)
        return 1;

    auto* root = doc.FirstChildElement("registry");
    if (!root)
        return 1;

    auto* types = root->FirstChildElement("types");
    ParseTypes(types);
    auto* handles = root->FirstChildElement("handles");
    ParseHandles(handles);
    auto* funcs = root->FirstChildElement("functions");
    ParseFunctions(funcs);

    std::string output = wis::format("#pragma once\n#include <stdint.h>\n\n");
    output += GenerateCTypes();
    output += "//==================================HANDLES==================================\n\n";
    for (auto& h : this->handles) {
        output += MakeHandle(h);
    }
    output += "//=================================FUNCTIONS=================================\n\n";
    for (auto& f : this->functions) {
        output += MakeFunctionDecl(f);
    }

    // Function implementations
    std::string output_cpp = wis::format("#include \"wisdom.h\"\n#include <wisdom/wisdom.h>\n\n");
    for (auto& f : function_impl) {
        output_cpp += f;
    }

    std::string output_api = wis::format("#pragma once\n#include <array>\n\nnamespace wis\n{{\n");
    output_api += GenerateCPPTypes();

    output_api += "//==============================TYPE TRAITS==============================\n\n";
    output_api += "template <typename T> struct is_flag_enum : public std::false_type {};\n";
    for (auto& t : cpp_type_traits) {
        output_api += t;
    }

    output_api += "}\n";

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

    std::filesystem::path cpp_output_path = cpp_output_dir;
    std::filesystem::create_directories(cpp_output_path);
    std::ofstream out_api(std::filesystem::absolute(cpp_output_path / "api.h"));
    if (!out_api.is_open())
        return 1;
    out_api << output_api;

    return 0;
}

std::string Generator::GenerateCTypes()
{
    std::string c_types = GenerateCTypedefs();
    for (auto& s : enums) {
        c_types += MakeCEnum(s);
    }
    for (auto& s : bitmasks) {
        c_types += MakeCBitmask(s);
    }
    for (auto& s : structs) {
        c_types += MakeCStruct(s);
    }
    return c_types;
}
std::string Generator::GenerateCPPTypes()
{
    std::string c_types = GenerateCPPTypedefs();
    for (auto& s : enums) {
        c_types += MakeCPPEnum(s);
    }
    for (auto& s : bitmasks) {
        c_types += MakeCPPBitmask(s);
    }
    for (auto& s : structs) {
        c_types += MakeCPPStruct(s);
    }
    return c_types;
}

std::string Generator::GenerateCTypedefs()
{
    std::string c_types;
    for (auto& s : structs) {
        c_types += wis::format("typedef struct Wis{} Wis{};\n", s.name, s.name);
    }
    for (auto& s : enums) {
        c_types += wis::format("typedef enum Wis{} Wis{};\n", s.name, s.name);
    }
    for (auto& s : bitmasks) {
        c_types += wis::format("typedef enum Wis{} Wis{};\n", s.name, s.name);
    }
    return c_types + '\n';
}
std::string Generator::GenerateCPPTypedefs()
{
    std::string c_types;
    for (auto& s : structs) {
        c_types += wis::format("struct {};\n", s.name);
    }
    for (auto& s : enums) {
        c_types += wis::format("enum class {};\n", s.name);
    }
    for (auto& s : bitmasks) {
        c_types += wis::format("enum class {};\n", s.name);
    }
    return c_types + '\n';
}

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
        }
    }
    for (auto& s : structs) {
        struct_map[s.name] = &s;
    }
}
void Generator::ParseFunctions(tinyxml2::XMLElement* type)
{
    for (auto* func = type->FirstChildElement("func"); func; func = func->NextSiblingElement("func")) {
        auto& ref = functions.emplace_back();
        auto name = func->FindAttribute("name")->Value();
        ref.name = name;

        if (auto* ret = func->FindAttribute("returns"))
            ref.return_type = ret->Value();

        if (auto* ret = func->FindAttribute("for"))
            ref.this_type = ret->Value();

        for (auto* param = func->FirstChildElement("param"); param; param = param->NextSiblingElement("param")) {
            auto& p = ref.parameters.emplace_back();

            auto* type = param->FindAttribute("type")->Value();
            auto* name = param->FindAttribute("name")->Value();

            p.type = type;
            p.name = name;
        }
    }
}

void Generator::ParseHandles(tinyxml2::XMLElement* types)
{
    for (auto* type = types->FirstChildElement("handle"); type; type = type->NextSiblingElement("handle")) {
        auto& ref = handles.emplace_back();
        auto name = type->FindAttribute("name")->Value();
        ref.name = name;

        if (auto* impl = type->FindAttribute("impl"))
            ref.impl = (impl->Value() == std::string_view("dx")) ? ImplementedFor::DX12 : ImplementedFor::Vulkan;
    }
    for (auto& h : handles) {
        handle_map[h.name] = &h;
    }
}

void Generator::ParseStruct(tinyxml2::XMLElement* type)
{
    auto& ref = structs.emplace_back();
    auto name = type->FindAttribute("name")->Value();
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
    auto& ref = enums.emplace_back();
    auto name = type->FindAttribute("name")->Value();
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
    auto& ref = bitmasks.emplace_back();
    auto name = type->FindAttribute("name")->Value();
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

std::string MakeCArray(std::string_view type, std::string_view name, std::string_view arr_len)
{
    return wis::format("    {} {}[{}];\n", type, name, arr_len);
}
std::string MakeCPPArray(std::string_view type, std::string_view name, std::string_view arr_len)
{
    return wis::format("    std::array<{}, {}> {};\n", type, arr_len, name);
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
            res_type = m.type;

        if (m.array_size.empty()) {
            st_decl += wis::format("    {} {};\n", res_type, m.name);
        } else {
            st_decl += MakeCPPArray(res_type, m.name, m.array_size);
        }
    }

    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCEnum(const WisEnum& s)
{
    std::string st_decl = !s.type.empty() ? wis::format("enum Wis{} : {} {{\n", s.name, standard_types.at(s.type)) : wis::format("enum Wis{} {{\n", s.name);

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
    std::string st_decl = s.size ? wis::format("enum Wis{} : uint{}_t {{\n", s.name, s.size) : wis::format("enum Wis{} {{\n", s.name);

    for (auto& m : s.values) {
        if (m.type == WisBitmaskValue::Type::Value) {
            st_decl += wis::format("    Wis{}{} = 0x{:X},\n", s.name, m.name, m.value);
        } else {
            st_decl += wis::format("    Wis{}{} = 1 << {},\n", s.name, m.name, m.bit);
        }
    }
    st_decl += wis::format("    Wis{}Max = 0x{:X};\n", s.name, (1ull << s.size) - 1);

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
    st_decl += wis::format("    Max = 0x{:X};\n", (1ull << s.size) - 1);

    st_decl += "};\n\n";
    cpp_type_traits.emplace_back(wis::format("template <> struct is_flag_enum<wis::{}>:public std::true_type {{}};\n", s.name));
    return st_decl;
}

ResolvedType Generator::ResolveType(const std::string& type)
{
    if (auto it = standard_types.find(type); it != standard_types.end()) {
        return { TypeInfo::Regular, std::string(it->second) };
    }

    if (auto it = struct_map.find(type); it != struct_map.end()) {
        return {
            TypeInfo::Struct, wis::format("Wis{}*", type)
        };
    }
    if (auto it = handle_map.find(type); it != handle_map.end()) {
        return {
            TypeInfo::Handle, wis::format("{}", type)
        };
    }

    return {
        TypeInfo::Regular, "Wis" + type
    };
}

std::string Generator::MakeFunctionImpl(const WisFunction& func, std::string_view func_decl, std::string_view impl)
{
    std::string st_decl{ func_decl };
    st_decl += "\n{\n";
    bool constructor = func.name == "Create";
    bool has_this = !func.this_type.empty();

    // static cast as refs
    if (has_this && !constructor) {
        st_decl += handle_map.contains(func.this_type)
                ? wis::format("    auto* xself = reinterpret_cast<wis::{}{}*>(self);\n", impl, func.this_type)
                : st_decl += wis::format("    auto* xself = reinterpret_cast<wis::{}*>(self);\n", func.this_type);
    }

    if (constructor) {
        st_decl += wis::format("    return reinterpret_cast<{}{}>(new wis::{}{}(", impl, func.this_type, impl, func.this_type);
        // TODO: handle parameters
        st_decl += "));\n";
    } else if (func.name == "Destroy") {
        st_decl += wis::format("    delete xself;\n", impl, func.this_type);
    } else if (has_this) {
        st_decl += wis::format("    return xself->{}(", impl, func.this_type, func.name);
        st_decl += ");\n";
    } else {
        st_decl += wis::format("    return wis::{}(", impl, func.name);
        st_decl += ");\n";
    }

    st_decl += "}\n";
    return st_decl;
}

std::string Generator::MakeFunctionDecl(const WisFunction& func)
{
    std::string st_decls;

    // 1. return type
    ResolvedType ret_t = [this, &func]() -> ResolvedType {
        if (func.return_type.empty())
            return std::pair{ TypeInfo::None, "void" };

        if (std::string_view(func.return_type).substr(2) == "string")
            return std::pair{ TypeInfo::None, "" };

        return ResolveType(func.return_type);
    }();

    if (ret_t.first == TypeInfo::None && ret_t.second.empty())
        return {};

    // 2. this type
    ResolvedType this_t = [this, &func]() -> ResolvedType {
        return func.this_type.empty() ? std::pair{ TypeInfo::None, "" } : ResolveType(func.this_type);
    }();

    // 3. parameters
    std::vector<ResolvedType> params_t;
    for (auto& p : func.parameters) {
        params_t.push_back(ResolveType(p.type));
    }

    bool impl_based = this_t.first == TypeInfo::Handle || ret_t.first == TypeInfo::Handle || std::ranges::find_if(params_t.begin(), params_t.end(), [](const ResolvedType& t) {
                                                                                                 return t.first == TypeInfo::Handle;
                                                                                             }) != params_t.end();
    constexpr static std::array<std::string_view, 2> impls{ "DX12", "VK" };
    std::array<std::string, 2> decls{};

    for (size_t j = 0; j < impl_based + 1; j++) {
        auto& st_decl = decls[j];
        std::string return_t = ret_t.first == TypeInfo::Struct
                ? "void"
                : ret_t.first == TypeInfo::Handle
                ? wis::format("{}{}", impls[j], ret_t.second)
                : ret_t.second;

        st_decl += wis::format("{} {}{}{}(",
                               return_t,
                               impl_based ? impls[j] : "",
                               this_t.second,
                               func.name);
        if (ret_t.second != this_t.second) {
            if (this_t.first == TypeInfo::Handle) {
                st_decl += wis::format("{}{} self, ", impl_based ? impls[j] : "", this_t.second);
            } else {
                st_decl += wis::format("{} self, ", this_t.second);
            }
        }

        for (uint32_t i = 0; i < params_t.size(); i++) {
            auto& t = params_t[i];
            auto& p = func.parameters[i];

            if (t.first == TypeInfo::Handle) {
                st_decl += wis::format("{}{} {}, ", impls[j], t.second, p.name);
            } else {
                st_decl += wis::format("{} {}, ", t.second, p.name);
            }
        }
        if (ret_t.first == TypeInfo::Struct) {
            st_decl += wis::format("{} out_struct", ret_t.second);
        }
        if (st_decl.back() == ' ') {
            st_decl.pop_back();
            st_decl.pop_back();
        }
        st_decl += ")";
        function_impl.emplace_back(MakeFunctionImpl(func, st_decl, impls[j]));
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

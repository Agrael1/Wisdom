#include "generator.h"
#include "../wisdom/include/wisdom/bridge/format.h"
#include <array>
#include <fstream>
#include <ranges>
#include <stdexcept>
#include <iostream>
#include <regex>

using namespace tinyxml2;

static inline void ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}
struct InlineTypeInfo {
    std::string_view type;
    std::string_view value;
    std::size_t pos;
    std::size_t after;
};

static inline InlineTypeInfo FindInlineType(std::string_view str, size_t initial)
{
    // Find caption enclosed in { }
    auto pos = str.find('{', initial);
    if (pos == std::string_view::npos) {
        return {};
    }

    auto end = str.find('}', pos);
    auto end_n = str.find('\n', pos);
    if (end == std::string_view::npos || end_n < end) {
        return {};
    }

    std::string_view caption = str.substr(pos + 1, end - pos - 1);
    std::string_view type, value;

    // seek if there is :: inside the caption
    auto colon = caption.find("::");
    if (colon != std::string_view::npos) {
        value = caption.substr(colon + 2);
        type = caption.substr(0, colon);
    }

    return { type, value, pos, end };
}

static ImplementedFor ImplCode(std::string_view impl) noexcept
{
    if (impl == "dx") {
        return ImplementedFor::DX12;
    }
    if (impl == "vk") {
        return ImplementedFor::Vulkan;
    }
    return ImplementedFor::Both;
}

static constexpr inline std::string_view k_delimiter = "\n//-------------------------------------------------------------------------\n\n";

struct WisHeaderGraphNode {
    std::string path;
    std::vector<WisHeaderGraphNode*> includes;
};

struct WisHeaderGraph {
    std::unordered_map<std::string, WisHeaderGraphNode> nodes;
    std::vector<WisHeaderGraphNode*> roots;

    void ParseIncludes(std::filesystem::path path, std::string rpath, std::string_view rel_dir)
    {
        auto& node = nodes["wisdom/" + rpath];
        node.path = "wisdom/" + rpath;

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << "\n";
            return;
        }

        std::string line;
        std::regex includeRegex(R"(#include\s*["<](wisdom.*)" + std::string(rel_dir) + R"(.*)[">])");
        std::smatch match;

        while (std::getline(file, line)) {
            if (line.find("#endif") != line.npos) {
                break;
            }
            if (std::regex_search(line, match, includeRegex)) {
                node.includes.push_back(&nodes[match[1].str()]);
                nodes[match[1].str()].path = match[1].str();
            }
        }
    }
    auto Visit(std::vector<std::string>& order, std::unordered_set<std::string>& visited, WisHeaderGraphNode& node)
    {
        if (visited.contains(node.path)) {
            return;
        }
        visited.insert(node.path);
        for (auto* inc : node.includes) {
            Visit(order, visited, *inc);
        }
        order.push_back(node.path);
    };
    std::vector<std::string> GetOrder()
    {
        std::vector<std::string> order;
        std::unordered_set<std::string> visited;
        for (auto& [_, node] : nodes) {
            Visit(order, visited, node);
        }
        return order;
    }
};

//-----------------------------------------------------------------------------

Generator::Generator(XMLDocument& doc)
{
    ParseFile(doc);
}

int Generator::GenerateCAPI()
{
    using namespace std::chrono;
    std::string output = "//GENERATED\n#pragma once\n#include \"wisdom_exports.h\"\n#include <stdint.h>\n#include <stdbool.h>\n\n";

    output += wis::format(documentation_header, WISDOM_VERSION);
    output += "\n*/\n\n";

    output += "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n";

    std::string out_dx;
    std::string out_vk;
    std::string out_default_dx;
    std::string out_default_vk;

    std::string out_vk_methods;
    std::string out_dx_methods;
    std::string out_vk_methods_def;
    std::string out_dx_methods_def;

    std::string out_cpp_dx;
    std::string out_cpp_vk;
    std::string output_cpp = "#include \"wisdom.h\"\n\n";

    std::string output_cpp_ext = "#include \"wisdom.h\"\n\n";

    {
        auto [gen, dx, vk] = GenerateCTypes();
        output += gen;
        out_dx += dx;
        out_vk += vk;
    }

    out_dx += "\n//-------------------------------------------------------------------------\n\n";
    out_vk += "\n//-------------------------------------------------------------------------\n\n";
    output += "\n//-------------------------------------------------------------------------\n\n";

    output += "//"
              "=================================DELEGATES========================"
              "=========\n\n";
    for (auto& [name, d] : delegate_map) {
        output += MakeCDelegate(d);
    }

    for (auto& [name, h] : handle_map) {
        if (!h.ext.empty()) {
            continue;
        }

        out_dx += MakeCHandle(h, impls[+ImplementedFor::DX12]);
        out_vk += MakeCHandle(h, impls[+ImplementedFor::Vulkan]);

        out_default_dx += MakeCHandleGeneric(h, impls[+ImplementedFor::DX12]);
        out_default_vk += MakeCHandleGeneric(h, impls[+ImplementedFor::Vulkan]);

        if (h.functions.empty()) {
            continue;
        }

        out_vk_methods += MakeCHandleMethods(h, impls[+ImplementedFor::Vulkan]) + '\n';
        out_dx_methods += MakeCHandleMethods(h, impls[+ImplementedFor::DX12]) + '\n';

        out_vk_methods_def += MakeCHandleMethodsGeneric(h, impls[+ImplementedFor::Vulkan]) + '\n';
        out_dx_methods_def += MakeCHandleMethodsGeneric(h, impls[+ImplementedFor::DX12]) + '\n';

        out_cpp_dx += MakeCHandleMethodImpls(h, impls[+ImplementedFor::DX12]) + '\n';
        out_cpp_vk += MakeCHandleMethodImpls(h, impls[+ImplementedFor::Vulkan]) + '\n';

        out_vk_methods += '\n';
        out_dx_methods += '\n';

        out_vk_methods_def += '\n';
        out_dx_methods_def += '\n';

        out_cpp_dx += '\n';
        out_cpp_vk += '\n';
    }

    out_dx += k_delimiter;
    out_dx += out_dx_methods;

    out_vk += k_delimiter;
    out_vk += out_vk_methods;

    for (auto* v : variants) {
        if (v->implemented_for == Language::CPP) {
            continue;
        }

        out_default_dx += MakeCVariantGeneric(*v, impls[+ImplementedFor::DX12]) + '\n';
        out_default_vk += MakeCVariantGeneric(*v, impls[+ImplementedFor::Vulkan]) + '\n';
    }

    out_default_vk += k_delimiter;
    out_default_dx += k_delimiter;

    out_default_dx += out_dx_methods_def;
    out_default_vk += out_vk_methods_def;

    out_dx += "\n//-------------------------------------------------------------------------\n\n";
    out_vk += "\n//-------------------------------------------------------------------------\n\n";
    out_default_dx += "\n//-------------------------------------------------------------------------\n\n";
    out_default_vk += "\n//-------------------------------------------------------------------------\n\n";

    for (auto& f : c_funcs) {
        out_dx += MakeCFunctionDecl(*f, impls[+ImplementedFor::DX12]);
        out_vk += MakeCFunctionDecl(*f, impls[+ImplementedFor::Vulkan]);

        out_default_dx += MakeCFunctionGenericDecl(*f, impls[+ImplementedFor::DX12]) + '\n';
        out_default_vk += MakeCFunctionGenericDecl(*f, impls[+ImplementedFor::Vulkan]) + '\n';
    }

    out_dx += "\n//-------------------------------------------------------------------------\n\n";
    out_vk += "\n//-------------------------------------------------------------------------\n\n";
    output += "\n//-------------------------------------------------------------------------\n\n";

    for (auto& f : function_decls) {
        output += f;
    }

    for (auto& f : function_decls_dx) {
        out_dx += f;
    }

    for (auto& f : function_decls_vk) {
        out_vk += f;
    }

    output += "#ifdef WISDOM_VULKAN\n";
    output += out_vk;
    output += "#endif\n\n";

    output += "#ifdef WISDOM_DX12\n";
    output += out_dx;
    output += "#endif\n\n";

    // Select default API
    output += "#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)\n#define FORCEVK_SWITCH 1\n#else\n#define FORCEVK_SWITCH 0\n#endif\n\n";
    output += "#if defined(WISDOM_DX12) && !FORCEVK_SWITCH\n";
    output += out_default_dx;
    output += "#elif defined(WISDOM_VULKAN)\n\n";
    output += out_default_vk;
    output += "#endif\n\n";

    for (auto& e : extension_map) {
        output += MakeCExtensionHeader(e.second);
    }

    output += "#ifdef __cplusplus\n}\n#endif\n";

    // Function implementations

    for (auto& f : function_impl) {
        output_cpp += f;
    }

    for (auto& f : function_impl_dx) {
        out_cpp_dx += f;
    }

    for (auto& f : function_impl_vk) {
        out_cpp_vk += f;
    }

    out_dx += "\n//-------------------------------------------------------------------------\n\n";
    out_vk += "\n//-------------------------------------------------------------------------\n\n";
    output += "\n//-------------------------------------------------------------------------\n\n";

    output_cpp += "#if defined(WISDOM_DX12)\n#include <wisdom/wisdom_dx12.hpp>\n\n";
    output_cpp += out_cpp_dx;
    output_cpp += "#endif\n\n";

    output_cpp += "#if defined(WISDOM_VULKAN)\n#include <wisdom/wisdom_vk.hpp>\n\n";
    output_cpp += out_cpp_vk;
    output_cpp += "#endif\n\n";

    // Collect includes
    std::string output_vk_ext_impl;
    std::string output_dx_ext_impl;
    for (auto& [name, h] : extension_map) {
        if (auto x = h.GetFile(ImplementedFor::DX12); x == h.GetFile(ImplementedFor::Vulkan)) {
            output_cpp_ext += wis::format("#include \"{}\"\n", x);
        } else {
            output_vk_ext_impl += wis::format("#include \"{}\"\n", h.GetFile(ImplementedFor::Vulkan));
            output_dx_ext_impl += wis::format("#include \"{}\"\n", h.GetFile(ImplementedFor::DX12));
        }
        output_vk_ext_impl += MakeCExtensionImpl(h, impls[+ImplementedFor::Vulkan]);
        output_dx_ext_impl += MakeCExtensionImpl(h, impls[+ImplementedFor::DX12]);
    }
    output_cpp_ext += "\n\n";

    output_cpp_ext += "#if defined(WISDOM_DX12)\n#include <wisdom/wisdom_dx12.hpp>\n\n";
    output_cpp_ext += MakeCExtensionMap(impls[+ImplementedFor::DX12]);
    output_cpp_ext += output_dx_ext_impl;
    output_cpp_ext += "#endif\n\n";

    output_cpp_ext += "#if defined(WISDOM_VULKAN)\n#include <wisdom/wisdom_vk.hpp>\n\n";
    output_cpp_ext += MakeCExtensionMap(impls[+ImplementedFor::Vulkan]);
    output_cpp_ext += output_vk_ext_impl;
    output_cpp_ext += "#endif\n\n";

    std::filesystem::path output_path = output_dir;
    std::filesystem::create_directories(output_path);

    auto output_cpp_abs = std::filesystem::absolute(output_path / "wisdom.cpp");
    std::ofstream out_cpp(output_cpp_abs);
    if (!out_cpp.is_open()) {
        return 1;
    }
    out_cpp << output_cpp;
    files.emplace_back(output_cpp_abs);

    auto output_cpp_ext_abs = std::filesystem::absolute(output_path / "wisdom_ext.inl");
    std::ofstream out_cpp_ext(output_cpp_ext_abs);
    if (!out_cpp_ext.is_open()) {
        return 1;
    }
    out_cpp_ext << output_cpp_ext;
    files.emplace_back(output_cpp_ext_abs);

    auto output_abs = std::filesystem::absolute(output_path / "wisdom.h");
    std::ofstream out(output_abs);
    if (!out.is_open()) {
        return 1;
    }
    out << output;
    files.emplace_back(output_abs);
    return 0;
}
int Generator::GenerateCPPAPI()
{
    using namespace std::chrono;

    std::string output_api = "//GENERATED\n#pragma once\n#ifndef WISDOM_MODULE_DECL\n#include <array>\n#include <cstdint>\n#include <functional>\n"
                             "#define WISDOM_EXPORT \n#else\n#define WISDOM_EXPORT export\n#endif\n\n";

    output_api += wis::format(documentation_header, WISDOM_VERSION);
    output_api += "\n*/\n\n";

    output_api += "WISDOM_EXPORT\nnamespace wis {\n";
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
inline constexpr Result success{
    wis::Status::Ok, "Operation succeeded"
};

template<typename RetTy>
struct ResultValue{
    Result status;
    RetTy value;

    constexpr ResultValue() noexcept = default;
    constexpr ResultValue(wis::Result status) noexcept
        : status(status)
    {
    }

    template<typename Callable, typename Callee, typename... Args>
    requires std::is_member_function_pointer_v<Callable>
    constexpr ResultValue(Callable&& f, Callee* self, Args&&... args) noexcept
        : value(std::invoke(f, self, status, std::forward<Args>(args)...))
    {
    }
    template<typename Callable, typename... Args>
    constexpr ResultValue(Callable && f, Args&&... args) noexcept
        : value(f(status, std::forward<Args>(args)...))
    {
    }

    template<typename Callable, typename Callee, typename... Args>
    static constexpr ResultValue<RetTy> from_member_func(Callable&& f, Callee* self, Args&&... args) noexcept
    {
        ResultValue<RetTy> rv;
        rv.value = std::invoke(f, self, rv.status, std::forward<Args>(args)...);
        return rv;
    }
};

template<uint32_t s, typename RetTy>
constexpr decltype(auto) get(ResultValue<RetTy>& rv) noexcept
{
    if constexpr (s == 0)
        return std::forward<Result>(rv.status);
    else
        return std::forward<RetTy>(rv.value);
}

)";
    output_api += "}\n";

    std::filesystem::path cpp_output_path = cpp_output_dir;
    std::filesystem::path cpp_output_path_api = cpp_output_path / "generated/api";
    std::filesystem::path cpp_output_path_dx12 = cpp_output_path / "generated/dx12";
    std::filesystem::path cpp_output_path_vulkan = cpp_output_path / "generated/vulkan";

    std::filesystem::create_directories(cpp_output_path_api);
    std::filesystem::create_directories(cpp_output_path_dx12);
    std::filesystem::create_directories(cpp_output_path_vulkan);

    auto output_api_abs = std::filesystem::absolute(cpp_output_path_api / "api.hpp");
    std::ofstream out_api(output_api_abs);
    if (!out_api.is_open()) {
        return 1;
    }
    out_api << output_api;
    files.emplace_back(output_api_abs);

    auto output_wisdom_dx_abs = std::filesystem::absolute(cpp_output_path / "wisdom_dx12.hpp");
    auto output_wisdom_vk_abs = std::filesystem::absolute(cpp_output_path / "wisdom_vk.hpp");
    auto output_wisdom_abs = std::filesystem::absolute(cpp_output_path / "wisdom.hpp");
    std::ofstream out_wisdom(output_wisdom_abs);
    if (!out_wisdom.is_open()) {
        return 1;
    }
    out_wisdom << GenerateCPPExportHeader();
    files.emplace_back(output_wisdom_abs);

    std::ofstream out_wisdom_dx(output_wisdom_dx_abs);
    if (!out_wisdom_dx.is_open()) {
        return 1;
    }
    out_wisdom_dx << GenerateCPPPlatformExportHeader(impls[+ImplementedFor::DX12]);
    files.emplace_back(output_wisdom_dx_abs);

    std::ofstream out_wisdom_vk(output_wisdom_vk_abs);
    if (!out_wisdom_vk.is_open()) {
        return 1;
    }
    out_wisdom_vk << GenerateCPPPlatformExportHeader(impls[+ImplementedFor::Vulkan]);
    files.emplace_back(output_wisdom_vk_abs);

    auto output_dx_abs = std::filesystem::absolute(cpp_output_path_dx12 / "dx12_structs.hpp");
    std::ofstream out_dxapi(output_dx_abs);
    if (!out_dxapi.is_open()) {
        return 1;
    }

    std::string dxapi =
            "#pragma once\n"
            "#ifndef WISDOM_MODULE_DECL\n"
            "#include <wisdom/dx12/dx12_views.h>\n#include "
            "<wisdom/generated/api/api.hpp>\n#include "
            "<wisdom/util/flags.h>\n#include <D3D12MemAlloc.h>\n#endif\n\nWISDOM_EXPORT\nnamespace wis{\n";
    for (auto i : variants) {
        if (i->implemented_for == Language::Hidden) {
            continue;
        }
        if (i->this_type.empty()) {
            dxapi += MakeCPPVariant(*i, ImplementedFor::DX12);
        }
    }
    for (auto& c : cpp_conversion) {
        if (c.impl == ImplementedFor::DX12) {
            dxapi += c.value;
        }
    }
    out_dxapi << dxapi + "}\n";
    files.emplace_back(output_dx_abs);

    auto output_vk_abs = std::filesystem::absolute(cpp_output_path_vulkan / "vk_structs.hpp");
    std::ofstream out_vkapi(output_vk_abs);
    if (!out_vkapi.is_open()) {
        return 1;
    }

    std::string vkapi =
            "#pragma once\n"
            "#ifndef WISDOM_MODULE_DECL\n"
            "#include <wisdom/vulkan/vk_views.h>\n#include "
            "<wisdom/generated/api/api.hpp>\n#include "
            "<wisdom/util/flags.h>\n#endif\n\nWISDOM_EXPORT\nnamespace wis{\n";
    for (auto i : variants) {
        if (i->implemented_for == Language::Hidden) {
            continue;
        }
        if (i->this_type.empty()) {
            vkapi += MakeCPPVariant(*i, ImplementedFor::Vulkan);
        }
    }
    for (auto& c : cpp_conversion) {
        if (c.impl == ImplementedFor::Vulkan) {
            vkapi += c.value;
        }
    }

    out_vkapi << vkapi + "}\n";
    files.emplace_back(output_vk_abs);

    GenerateCPPInlineDoc();
    return 0;
}

int Generator::GenerateCPPInlineDoc()
{
    auto write_doc = [this](const WisHandle& h, std::string_view file, std::string_view impl, std::filesystem::path output_path = cpp_output_dir) {
        auto output_abs = std::filesystem::absolute(output_path / file);
        if (std::filesystem::exists(output_abs)) {
            std::string& file_data = file_contents[output_abs];
            if (file_data.empty()) {
                // Load file
                std::ifstream t{ output_abs, std::ios::binary };
                t.seekg(0, std::ios::end);
                size_t size = t.tellg();
                file_data.resize(size);
                t.seekg(0);
                t.read(file_data.data(), size);

                ReplaceAll(file_data, "\r\n", "\n");
            }

            // Find #pragma region HandleName
            auto pragma = wis::format("#pragma region {}{}", impl, h.name);
            auto pos = file_data.find(pragma);
            if (pos == std::string::npos) {
                return;
            }
            pos += pragma.size();

            // Find #pragma endregion HandleName
            auto end = wis::format("#pragma endregion {}{}", impl, h.name);
            auto end_pos = file_data.find(end, pos);
            if (end_pos == std::string::npos) {
                return;
            }

            std::string class_decl = '\n' + MakeCPPHandle(h, impl);

            file_data.replace(pos, end_pos - pos, class_decl);

            files.emplace_back(output_abs);
        }
    };

    for (auto& [k, h] : handle_map) {
        if (h.functions.empty()) {
            continue;
        }

        if (!h.ext.empty()) {
            auto ext_folder = extension_map[h.ext].ext_folder;
            if (auto file = h.GetFile(ImplementedFor::DX12); !file.empty()) {
                write_doc(h, file, impls[+ImplementedFor::DX12], std::filesystem::path{ ext_dir } / ext_folder);
            }

            if (auto file = h.GetFile(ImplementedFor::Vulkan); !file.empty()) {
                write_doc(h, file, impls[+ImplementedFor::Vulkan], std::filesystem::path{ ext_dir } / ext_folder);
            }
            continue;
        }

        if (auto file = h.GetFile(ImplementedFor::DX12); !file.empty()) {
            write_doc(h, file, impls[+ImplementedFor::DX12]);
        }

        if (auto file = h.GetFile(ImplementedFor::Vulkan); !file.empty()) {
            write_doc(h, file, impls[+ImplementedFor::Vulkan]);
        }
    }

    for (auto& [k, v] : extension_map) {
        if (v.functions.empty()) {
            continue;
        }

        if (auto file = v.GetFile(ImplementedFor::DX12); !file.empty()) {
            write_doc(v, file, impls[+ImplementedFor::DX12], std::filesystem::path{ ext_dir } / v.ext_folder);
        }
        if (auto file = v.GetFile(ImplementedFor::Vulkan); !file.empty()) {
            write_doc(v, file, impls[+ImplementedFor::Vulkan], std::filesystem::path{ ext_dir } / v.ext_folder);
        }
    }

    // mt?
    for (auto& [p, d] : file_contents) {
        std::ofstream out(p);
        if (!out.is_open()) {
            return 1;
        }
        out << d << std::flush;
    }

    return 0;
}

int Generator::GenerateCPPModules()
{
    // Generate wisdom.api.ixx
    std::filesystem::path cpp_output_path = cpp_output_dir;
    std::filesystem::path cpp_output_path_api = cpp_output_path / "generated/api";
    std::filesystem::path cpp_output_path_dx12 = cpp_output_path / "generated/dx12";
    std::filesystem::path cpp_output_path_vulkan = cpp_output_path / "generated/vulkan";
    std::filesystem::path cpp_output_path_wisdom = std::filesystem::absolute(cpp_output_path / "wisdom.ixx");
    std::filesystem::path cpp_output_path_wisdom_fvk = std::filesystem::absolute(cpp_output_path / "wisdom.fvk.ixx");
    std::ofstream out_wisdom(cpp_output_path_wisdom);
    if (!out_wisdom.is_open()) {
        return 1;
    }
    out_wisdom << GenerateCPPExportModule();
    files.emplace_back(cpp_output_path_wisdom);

    std::ofstream out_wisdom_fvk(cpp_output_path_wisdom_fvk);
    if (!out_wisdom_fvk.is_open()) {
        return 1;
    }
    out_wisdom_fvk << GenerateCPPExportModuleForceVulkan();
    files.emplace_back(cpp_output_path_wisdom_fvk);

    std::string output_api = wis::format(documentation_header, WISDOM_VERSION);
    output_api += "\n*/\n\n";
    output_api += R"(module;
#include <wisdom/generated/api/api.include.h>
#define WISDOM_MODULE_DECL
#define WISDOM_EXPORT export
export module wisdom.api;

#include <wisdom/generated/api/api.hpp>
)";
    std::filesystem::path cpp_api_includes[] = {
        cpp_output_path / "global",
        cpp_output_path / "util"
    };

    WisHeaderGraph graph_api;
    for (auto& path : cpp_api_includes) {
        for (auto& file : std::filesystem::recursive_directory_iterator(path)) {
            std::filesystem::path fpath = std::filesystem::relative(file.path(), cpp_output_path);
            auto ext = fpath.extension();
            if (file.is_regular_file() && (ext == ".h" || ext == ".cpp" || ext == ".hpp")) {
                graph_api.ParseIncludes(file, fpath.generic_string(), "");
            }
        }
    }
    for (auto& file : graph_api.GetOrder()) {
        output_api += wis::format("#include <{}>\n", file);
    }

    auto output_api_abs = std::filesystem::absolute(cpp_output_path_api / "wisdom.api.ixx");
    std::ofstream out_api(output_api_abs);
    if (!out_api.is_open()) {
        return 1;
    }
    out_api << output_api;
    files.emplace_back(output_api_abs);

    // Generate wisdom.dx12.ixx
    std::string output_dx12 = wis::format(documentation_header, WISDOM_VERSION);
    output_dx12 += "\n*/\n\n";
    output_dx12 += R"(module;
#include <wisdom/generated/dx12/dx12.include.h>
#define WISDOM_MODULE_DECL
#define WISDOM_BUILD_BINARIES
#define WISDOM_EXPORT export
export module wisdom.dx12;

export import wisdom.api;

)";

    WisHeaderGraph graph;
    for (auto& file : std::filesystem::recursive_directory_iterator(cpp_output_path / "dx12")) {
        std::filesystem::path fpath = std::filesystem::relative(file.path(), cpp_output_path);
        auto ext = fpath.extension();
        if (file.is_regular_file() && (ext == ".h" || ext == ".cpp" || ext == ".hpp")) {
            graph.ParseIncludes(file, fpath.generic_string(), "dx12");
        }
    }
    for (auto& file : graph.GetOrder()) {
        output_dx12 += wis::format("#include <{}>\n", file);
    }

    auto impl = impls[+ImplementedFor::DX12];

    output_dx12 += k_delimiter;
    output_dx12 += "\nexport namespace wis {\n\n";

    // Generate platform funcs
    for (auto& f : functions) {
        std::string_view prefix = f->return_type.has_result ? "[[nodiscard]] inline" : "inline";

        output_dx12 += wis::format("{}{{ return wis::Impl{}; }}\n", MakeCPPFunctionProto(*f, impl, prefix, true, true, true),
                                   MakeCPPFunctionCall(*f, impl, true));

        output_dx12 += wis::format("{} noexcept{{\n    {} {};\n}}\n", MakeCPPFunctionProto(*f, impl, prefix, true, true, false),
                                   !f->return_type.IsVoid() ? "return" : "", MakeCPPRVFunctionCall(*f, wis::format("wis::Impl{}{}", impl, ""), impl));
    }

    // Footer
    output_dx12 += "\n}//namespace wis\n";

    auto output_dx12_abs = std::filesystem::absolute(cpp_output_path_dx12 / "wisdom.dx12.ixx");
    std::ofstream out_dx12(output_dx12_abs);
    if (!out_dx12.is_open()) {
        return 1;
    }
    out_dx12 << output_dx12;
    files.emplace_back(output_dx12_abs);

    // Generate wisdom.vk.ixx
    std::string output_vk = wis::format(documentation_header, WISDOM_VERSION);
    output_vk += "\n*/\n\n";
    output_vk += R"(module;
#include <wisdom/generated/vulkan/vk.include.h>
#define WISDOM_MODULE_DECL
#define WISVK_MODULE_DECL
#define WISDOM_BUILD_BINARIES
#define WISDOM_EXPORT export
export module wisdom.vk;

export import wisdom.api;

)";

    WisHeaderGraph graph_vk;
    for (auto& file : std::filesystem::recursive_directory_iterator(cpp_output_path / "vulkan")) {
        std::filesystem::path fpath = std::filesystem::relative(file.path(), cpp_output_path);
        std::string ext = fpath.extension().string();
        if (file.is_regular_file() && (ext == ".h" || ext == ".cpp" || ext == ".hpp")) {
            graph_vk.ParseIncludes(file, fpath.generic_string(), "vulkan");
        }
    }
    for (auto& file : graph_vk.GetOrder()) {
        output_vk += wis::format("#include <{}>\n", file);
    }

    impl = impls[+ImplementedFor::Vulkan];

    output_vk += k_delimiter;
    output_vk += "\nexport namespace wis {\n\n";

    // Generate platform funcs
    for (auto& f : functions) {
        std::string_view prefix = f->return_type.has_result ? "[[nodiscard]] inline" : "inline";

        output_vk += wis::format("{}{{ return wis::Impl{}; }}\n", MakeCPPFunctionProto(*f, impl, prefix, true, true, true),
                                 MakeCPPFunctionCall(*f, impl, true));

        output_vk += wis::format("{} noexcept{{\n    {} {};\n}}\n", MakeCPPFunctionProto(*f, impl, prefix, true, true, false),
                                 !f->return_type.IsVoid() ? "return" : "", MakeCPPRVFunctionCall(*f, wis::format("wis::Impl{}{}", impl, ""), impl));
    }

    // Footer
    output_vk += "\n}//namespace wis\n";

    auto output_vk_abs = std::filesystem::absolute(cpp_output_path_vulkan / "wisdom.vk.ixx");
    std::ofstream out_vk(output_vk_abs);
    if (!out_vk.is_open()) {
        return 1;
    }
    out_vk << output_vk;
    files.emplace_back(output_vk_abs);

    return 0;
}

std::string Generator::GenerateCPPModule(std::string_view impl)
{
    std::string output;

    for (auto& [name, v] : variant_map) {
        if (!v.ext.empty()) {
            continue;
        }

        output += wis::format("export wis::{}{};\n", impl, v.name);
    }

    output += k_delimiter;

    for (auto& [name, h] : handle_map) {
        if (!h.ext.empty()) {
            continue;
        }

        output += wis::format("export wis::{}{};\n", impl, name);
    }

    output += k_delimiter;

    output += "export namespace wis {\n";
    for (auto& f : cpp_funcs) {
        output += wis::format("using wis::{}{};\n", impl, f->name);
    }
    output += "}\n";
    output += k_delimiter;

    return output;
}

std::tuple<std::string, std::string, std::string> Generator::GenerateCTypes()
{
    std::string gen, dx, vk;
    for (auto& [name, enum_t] : enum_map) {
        gen += MakeCEnum(enum_t);
    }
    gen += "\n//-------------------------------------------------------------------------\n\n";
    for (auto& [name, enum_t] : bitmask_map) {
        gen += MakeCBitmask(enum_t);
    }
    gen += "\n//-------------------------------------------------------------------------\n\n";

    auto&& [genx, dxx, vkx] = GenerateCTypedefs();
    gen += genx;
    dx += dxx;
    vk += vkx;

    for (auto& s : structs) {
        gen += MakeCStruct(*s);
    }

    gen += "\n//-------------------------------------------------------------------------\n\n";
    for (auto& v : variants) {
        auto [dxx, vkx] = MakeCVariant(*v);
        dx += dxx;
        vk += vkx;
    }

    return { gen, dx, vk };
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

// generic, dx, vk
std::tuple<std::string, std::string, std::string> Generator::GenerateCTypedefs()
{
    std::string gen, dx, vk;
    for (auto& s : structs) {
        gen += wis::format("typedef struct Wis{} Wis{};\n", s->name, s->name);
    }
    for (auto& [name, _] : enum_map) {
        auto full_name = GetCFullTypename(name, "");
        gen += wis::format("typedef enum {} {};\n", full_name, full_name);
    }
    for (auto& [name, bm] : bitmask_map) {
        auto full_name = GetCFullTypename(name, "");
        gen += wis::format("typedef enum {}Bits {}Bits;\n", full_name, full_name);
        gen += wis::format("typedef {} {};\n",
                           bm.type.empty() ? "uint32_t" : standard_types.at(bm.type), full_name);
    }
    for (auto& [name, s] : variant_map) {
        for (auto& impl : s.impls) {
            if (impl.impl == ImplementedFor::Both) {
                dx += wis::format("typedef struct DX12{} DX12{};\n", s.name, s.name);
                vk += wis::format("typedef struct VK{} VK{};\n", s.name, s.name);
                break;
            }
            if (impl.impl == ImplementedFor::DX12) {
                dx += wis::format("typedef struct DX12{} DX12{};\n", s.name, s.name);
            } else if (impl.impl == ImplementedFor::Vulkan) {
                vk += wis::format("typedef struct VK{} VK{};\n", s.name, s.name);
            }
        }
    }
    gen += "\n//-------------------------------------------------------------------------\n\n";
    dx += "\n//-------------------------------------------------------------------------\n\n";
    vk += "\n//-------------------------------------------------------------------------\n\n";
    return { gen, dx, vk };
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
        if (!h.ext.empty()) {
            continue;
        }

        output += wis::format("using {} = {}{};\n", name, impl, name);
    }

    output += k_delimiter;

    for (auto& f : cpp_funcs) {
        output += MakeCPPFunctionGenericDecl(*f, impl, true) + '\n';
        output += MakeCPPFunctionGenericDecl(*f, impl, false) + '\n';
    }

    output += k_delimiter;

    for (auto& v : variants) {
        if (v->ext.empty()) {
            output += wis::format("using {} = {}{};\n", v->name, impl, v->name);
        }
    }

    return output + "}\n";
}

std::string Generator::GenerateCPPExportHeader()
{
    std::string output_wisdom{
        R"(#pragma once
// Select default API
// Override with WISDOM_FORCE_VULKAN
#include <wisdom/config.h>

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
#include "wisdom_dx12.hpp"

)"
    };

    output_wisdom += GenerateCPPPlatformTypedefs(impls[+ImplementedFor::DX12]);
    output_wisdom += R"(
#elif defined(WISDOM_VULKAN)
#include "wisdom_vk.hpp"

)";
    output_wisdom += GenerateCPPPlatformTypedefs(impls[+ImplementedFor::Vulkan]);
    output_wisdom += R"(
#else
#error "No API selected"
#endif
)";
    return output_wisdom;
}

std::string Generator::GenerateCPPExportModule()
{
    std::string output_wisdom{
        R"(module;
// Select default API
// Override with WISDOM_FORCE_VULKAN in CMake
#include <wisdom/config.h>
#include <wisdom/module.h>

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

export module wisdom;

export import wisdom.api;

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
import wisdom.dx12;

export )"
    };

    output_wisdom += GenerateCPPPlatformTypedefs(impls[+ImplementedFor::DX12]);
    output_wisdom += R"(
#elif defined(WISDOM_VULKAN)
import wisdom.vk;

export )";
    output_wisdom += GenerateCPPPlatformTypedefs(impls[+ImplementedFor::Vulkan]);
    output_wisdom += R"(
#else
#error "No API selected"
#endif
)";
    return output_wisdom;
}

std::string Generator::GenerateCPPExportModuleForceVulkan()
{
    std::string output_wisdom{
        R"(module;
// This module is forced to use Vulkan, regardless of the default API
#include <wisdom/config.h>
#include <wisdom/module.h>

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

export module wisdom.fvk;

export import wisdom.api;

#if defined(WISDOM_VULKAN)
import wisdom.vk;

export )"
    };

    output_wisdom += GenerateCPPPlatformTypedefs(impls[+ImplementedFor::Vulkan]);
    output_wisdom += R"(
#else
#error "No API selected"
#endif
)";
    return output_wisdom;
}

std::string Generator::GenerateCPPPlatformExportHeader(std::string_view impl)
{
    // Header
    std::string output = impl == "DX12" ? R"(//GENERATED
#pragma once
#include "dx12/dx12_factory.h"
#include "dx12/dx12_swapchain.h"
#include "dx12/dx12_device.h"
)"
                                        : R"(//GENERATED
#pragma once
#include "vulkan/vk_factory.h"
#include "vulkan/vk_swapchain.h"
#include "vulkan/vk_device.h"
)";
    output += "\nnamespace wis {\n\n";

    // Generate platform funcs
    for (auto& f : functions) {
        std::string_view prefix = f->return_type.has_result ? "[[nodiscard]] inline" : "inline";

        output += wis::format("{}{{ return wis::Impl{}; }}\n", MakeCPPFunctionProto(*f, impl, prefix, true, true, true),
                              MakeCPPFunctionCall(*f, impl, true));

        output += wis::format("{} noexcept{{\n    {} {};\n}}\n", MakeCPPFunctionProto(*f, impl, prefix, true, true, false),
                              !f->return_type.IsVoid() ? "return" : "", MakeCPPRVFunctionCall(*f, wis::format("wis::Impl{}{}", impl, ""), impl));
    }

    output += k_delimiter;

    // Footer
    output += "\n}//namespace wis\n";

    return output;
}

//-----------------------------------------------------------------------------

void Generator::ParseFile(tinyxml2::XMLDocument& doc)
{
    auto* root = doc.FirstChildElement("registry");
    if (!root) {
        throw std::runtime_error("Failed to load root");
    }

    if (auto* include = root->FirstChildElement("includes")) {
        ParseIncludes(include);
    }

    if (auto* handles = root->FirstChildElement("handles")) {
        ParseHandles(handles);
    }

    if (auto* types = root->FirstChildElement("types")) {
        ParseTypes(types);
    }

    if (auto* funcs = root->FirstChildElement("functions")) {
        ParseFunctions(funcs);
    }

    if (auto* exts = root->FirstChildElement("extensions")) {
        ParseExtensions(exts);
    }
}

void Generator::ParseTypes(tinyxml2::XMLElement* types, std::string_view extension)
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
            ParseVariant(*type, extension);
        }
    }
}

void Generator::ParseIncludes(tinyxml2::XMLElement* includes)
{
    for (auto* include = includes->FirstChildElement("include"); include;
         include = include->NextSiblingElement("include")) {
        auto file = include->GetText();
        auto rpath = std::filesystem::path(INPUT_FILE).parent_path() / file;
        auto absolute = std::filesystem::absolute(rpath);

        if (std::filesystem::exists(absolute) && !this->includes.contains(absolute)) {
            auto& doc = this->includes[absolute];
            if (doc.LoadFile(absolute.string().c_str()) != tinyxml2::XMLError::XML_SUCCESS) {
                throw std::runtime_error("Failed to load include file");
            }
            ParseFile(doc);
        }
    }
}

WisReturnType Generator::ParseFunctionReturn(tinyxml2::XMLElement* func)
{
    WisReturnType ret;

    if (auto* param = func->FirstChildElement("ret")) {
        if (auto* res = param->FindAttribute("result")) {
            ret.has_result = true;
        }

        auto* type = param->FindAttribute("type");
        if (!type) {
            return ret; // No return type
        }

        ret.type = type->Value();
        ret.type_info = GetTypeInfo(type->Value());

        if (auto* name = param->FindAttribute("name")) {
            ret.opt_name = name->Value();
        }

        if (auto* mod = param->FindAttribute("mod")) {
            ret.modifier = mod->Value();
        }

        if (auto* doc = param->FindAttribute("doc")) {
            ret.doc = doc->Value();
        }
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

        if (auto* mod = param->FindAttribute("mod")) {
            p.modifier = mod->Value();
        }

        if (auto* def = param->FindAttribute("default")) {
            p.default_value = def->Value();
        }

        if (auto* doc = param->FindAttribute("doc")) {
            p.doc = doc->Value();
        }

        if (auto replace = param->FirstChildElement("replace")) {
            using namespace std::string_view_literals;
            auto& rval = p.replaced.emplace();
            auto* type = replace->FindAttribute("with")->Value();
            auto* rfor = replace->FindAttribute("for")->Value();

            rval.type = type;
            rval.type_info = GetTypeInfo(type);

            if (rfor == "c"sv) {
                rval.replace_for = Language::C;
            } else if (rfor == "cpp"sv) {
                rval.replace_for = Language::CPP;
            }

            if (auto* doc = replace->FindAttribute("doc")) {
                rval.doc = doc->Value();
            }

            if (auto* mod = replace->FindAttribute("mod")) {
                rval.modifier = mod->Value();
            }

            if (auto* def = replace->FindAttribute("default")) {
                rval.default_value = def->Value();
            }

            if (auto* name = replace->FindAttribute("name")) {
                rval.name = name->Value();
            }
        }
    }
    return ret;
}

void Generator::ParseFunctions(tinyxml2::XMLElement* type)
{
    for (auto* func = type->FirstChildElement("func"); func;
         func = func->NextSiblingElement("func")) {

        // Refactor: This is a mess
        auto name = func->FindAttribute("name")->Value();
        auto* ret = func->FindAttribute("for");
        std::string key = wis::format("{}{}", ret ? ret->Value() : "", name);
        auto&& [xref, a] = function_map.emplace(std::move(key), WisFunction{});

        std::string_view xkey = xref->first;
        auto& ref = xref->second;
        ref.name = name;

        if (ret) {
            ref.this_type = ret->Value();
            ref.this_type_info = GetTypeInfo(ref.this_type);
            if (ref.this_type_info == TypeInfo::Handle) {
                handle_map[ref.this_type].functions.emplace_back(xkey);
            } else if (ref.this_type_info == TypeInfo::ExtHandle) {
                extension_map[ref.this_type].functions.emplace_back(xkey);
            }
        }

        if (auto* doc = func->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        ref.return_type = ParseFunctionReturn(func);
        ref.parameters = ParseFunctionArgs(func);

        if (!ret) {
            functions.emplace_back(&ref);
            cpp_funcs.emplace_back(&ref);
            c_funcs.emplace_back(&ref);
        }

        auto* mod = func->FindAttribute("mod");
        if (!mod) {
            continue;
        }

        std::string_view smod = mod->Value();
        if (smod.find("custom-impl") != std::string_view::npos) {
            ref.custom_impl = true;
        }
        if (smod.find("cpp-only") != std::string_view::npos) {
            ref.implemented_for = Language::CPP;
        } else if (smod.find("c-only") != std::string_view::npos) {
            ref.implemented_for = Language::C;
        }
        if (smod.find("const") != std::string_view::npos) {
            ref.const_func = true;
        }
    }
}

void Generator::ParseExtensions(tinyxml2::XMLElement* extensions)
{
    for (auto* ext = extensions->FirstChildElement("ext"); ext;
         ext = ext->NextSiblingElement("ext")) {

        auto name = ext->FindAttribute("name")->Value();
        auto& ref = extension_map[name];

        ref.name = name;

        if (auto* doc = ext->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        if (auto* ext_folder = ext->FindAttribute("folder")) {
            ref.ext_folder = ext_folder->Value();
        }

        for (auto* impl = ext->FirstChildElement("file"); impl;
             impl = impl->NextSiblingElement("file")) {

            if (auto* impl_for = impl->FindAttribute("for")) {
                auto impl_for_code = ImplCode(impl_for->Value());
                auto impl_file = impl->FindAttribute("include")->Value();
                ref.AddFile(impl_file, impl_for_code);
            } else {
                auto impl_file = impl->FindAttribute("include")->Value();
                ref.AddFile(impl_file, ImplementedFor::Both);
            }
        }

        std::string_view type = ext->FindAttribute("type")->Value();
        auto id = ext->FindAttribute("id")->Value();
        if (type == "Factory") {
            ref.type = ExtensionType::Factory;
            WisEnumValue val{
                .name = name,
                .doc = ref.doc,
                .value = std::stoll(id),
            };

            enum_map.at("FactoryExtID").values.emplace_back(val);
        } else if (type == "Device") {
            ref.type = ExtensionType::Device;
            WisEnumValue val{
                .name = name,
                .doc = ref.doc,
                .value = std::stoll(id),
            };

            enum_map.at("DeviceExtID").values.emplace_back(val);
        }

        ParseHandles(ext, name);
        ParseTypes(ext, name);
        ParseFunctions(ext);
    }
}

void Generator::ParseHandles(tinyxml2::XMLElement* types, std::string_view extension)
{
    for (auto* type = types->FirstChildElement("handle"); type;
         type = type->NextSiblingElement("handle")) {
        auto name = type->FindAttribute("name")->Value();
        auto& ref = handle_map[name];
        ref.name = name;
        ref.ext = extension;

        if (!extension.empty()) {
            extension_map[extension].handles.emplace_back(name);
        }

        if (auto* doc = type->FindAttribute("doc")) {
            ref.doc = doc->Value();
        }

        for (auto* impl = type->FirstChildElement("file"); impl;
             impl = impl->NextSiblingElement("file")) {
            auto impl_for = impl->FindAttribute("for")->Value();
            auto impl_for_code = ImplCode(impl_for);
            auto impl_file = impl->FindAttribute("include")->Value();

            ref.AddFile(impl_file, impl_for_code);
        }
    }
}

void Generator::ParseStruct(tinyxml2::XMLElement& type)
{
    auto name = type.FindAttribute("name")->Value();
    auto& ref = struct_map[name];
    structs.emplace_back(&ref);
    ref.name = name;

    if (auto* size = type.FindAttribute("doc")) {
        ref.doc = size->Value();
    }

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
        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }
    }
}

void Generator::ParseEnum(tinyxml2::XMLElement& type)
{
    std::unordered_map<std::string_view, std::string> cvts;

    // Local documentation
    std::string documentation;

    std::string impl_doc;

    auto name = type.FindAttribute("name")->Value();
    auto& ref = enum_map[name];
    ref.name = name;

    if (auto* size = type.FindAttribute("type")) {
        ref.type = size->Value();
    }

    if (auto* size = type.FindAttribute("doc")) {
        ref.doc = size->Value();
    }

    for (auto* impl_type = type.FirstChildElement("impl_type"); impl_type;
         impl_type = impl_type->NextSiblingElement("impl_type")) {
        auto impl_for = impl_type->FindAttribute("for")->Value();
        auto impl_for_code = ImplCode(impl_for);
        auto impl_name = impl_type->FindAttribute("name")->Value();

        ref.doc_translates += wis::format("Translates to {} for {} implementation.\n", impl_name, impl_for);

        std::string_view def_value = "{}";
        if (auto xdefault = impl_type->FindAttribute("default")) {
            def_value = xdefault->Value();
        }

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
        if (auto* impl = member->FindAttribute("impl")) {
            m.impl = ImplCode(impl->Value());
        }

        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }

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

    if (auto* size = type.FindAttribute("type")) {
        ref.type = size->Value();
    }

    if (auto* size = type.FindAttribute("doc")) {
        ref.doc = size->Value();
    }

    for (auto* impl_type = type.FirstChildElement("impl_type"); impl_type;
         impl_type = impl_type->NextSiblingElement("impl_type")) {
        auto impl_for = impl_type->FindAttribute("for")->Value();
        auto impl_for_code = ImplCode(impl_for);
        auto impl_name = impl_type->FindAttribute("name")->Value();

        ref.doc_translates += wis::format("Translates to {} for {} implementation.\n", impl_name, impl_for);

        std::string_view def_value = "{}";
        if (auto xdefault = impl_type->FindAttribute("default")) {
            def_value = xdefault->Value();
        }

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
        if (auto* impl = member->FindAttribute("impl")) {
            m.impl = ImplCode(impl->Value());
        }

        auto* value = member->FindAttribute("value");
        auto* bit = member->FindAttribute("bit");

        if (auto* doc = member->FindAttribute("doc")) {
            m.doc = doc->Value();
        }

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

    if (auto* size = type->FindAttribute("doc")) {
        ref.doc = size->Value();
    }
}

void Generator::ParseVariant(tinyxml2::XMLElement& type, std::string_view extension)
{
    auto name = type.FindAttribute("name")->Value();
    auto& ref = variant_map[name];
    ref.name = name;
    ref.ext = extension;
    variants.emplace_back(&ref);

    if (auto* this_t = type.FindAttribute("for")) {
        ref.this_type = this_t->Value();
    }

    if (auto* size = type.FindAttribute("doc")) {
        ref.doc = size->Value();
    }

    if (auto* mod = type.FindAttribute("mod")) {
        if (std::string_view(mod->Value()) == "cpp-only") {
            ref.implemented_for = Language::CPP;
        } else if (std::string_view(mod->Value()) == "c-only") {
            ref.implemented_for = Language::C;
        } else if (std::string_view(mod->Value()) == "hidden") {
            ref.implemented_for = Language::Hidden;
        }
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
            if (auto* doc = member->FindAttribute("doc")) {
                m.doc = doc->Value();
            }
        }
    }
}

std::string Generator::FinalizeCDocumentation(std::string doc, std::string_view this_type, std::string_view impl)
{
    if (doc.empty()) {
        return doc;
    }

    size_t pos = 0;
    std::string_view this_type_view = this_type;

    while (true) {
        auto&& [type, value, first, last] = FindInlineType(doc, pos);
        if (type.empty() && value.empty()) {
            break;
        }

        // Replace with this_type
        if (!type.empty()) {
            this_type_view = type;
        }

        std::string replacement;

        if (auto x = enum_map.find(this_type_view); x != enum_map.end()) {
            auto evalue = x->second.HasValue(value);
            replacement = evalue ? wis::format("{}{}{}", x->second.name, impls[+evalue->impl], evalue->name)
                                 : GetCFullTypename(x->second.name, impl);

        } else if (auto y = bitmask_map.find(this_type_view); y != bitmask_map.end()) {
            auto evalue = y->second.HasValue(value);
            replacement = evalue ? wis::format("{}{}{}", y->second.name, impls[+evalue->impl], evalue->name)
                                 : GetCFullTypename(y->second.name, impl);
        } else if (auto z = struct_map.find(this_type_view); z != struct_map.end()) {
            auto member = z->second.HasValue(value);
            replacement = member ? wis::format("{}::{}", GetCFullTypename(z->second.name, impl), member->name)
                                 : GetCFullTypename(z->second.name, impl);
        } else if (auto d = delegate_map.find(this_type_view); d != delegate_map.end()) {
            auto member = d->second.HasValue(value);
            replacement = member ? wis::format("{}::{}", GetCFullTypename(d->second.name, impl), member->name)
                                 : GetCFullTypename(d->second.name, impl);
        } else if (auto h = handle_map.find(this_type_view); h != handle_map.end()) {
            replacement = GetCFullTypename(h->second.name, impl);
        } else if (auto f = function_map.find(std::string(this_type_view)); f != function_map.end()) {
            auto member = f->second.HasValue(value);
            replacement = member ? wis::format("{}({})", GetCFullTypename(f->second.name, impl), member->name)
                                 : GetCFullTypename(f->second.name, impl);
        }

        pos = last;
        doc.replace(first, last - first + 1, replacement);
    }
    return doc;
}
std::string Generator::FinalizeCPPDocumentation(std::string doc, std::string_view this_type, std::string_view impl)
{
    if (doc.empty()) {
        return doc;
    }

    size_t pos = 0;
    std::string_view this_type_view = this_type;

    while (true) {
        auto&& [type, value, first, last] = FindInlineType(doc, pos);
        if (type.empty() && value.empty()) {
            break;
        }

        // Replace with this_type
        if (!type.empty()) {
            this_type_view = type;
        }

        std::string replacement;

        if (auto x = enum_map.find(this_type_view); x != enum_map.end()) {
            auto evalue = x->second.HasValue(value);
            replacement = evalue ? wis::format("{}::{}{}", GetCPPFullTypename(x->second.name, impl), impls[+evalue->impl], evalue->name)
                                 : GetCPPFullTypename(x->second.name, impl);

        } else if (auto y = bitmask_map.find(this_type_view); y != bitmask_map.end()) {
            auto evalue = y->second.HasValue(value);
            replacement = evalue ? wis::format("{}::{}{}", GetCPPFullTypename(y->second.name, impl), impls[+evalue->impl], evalue->name)
                                 : GetCPPFullTypename(y->second.name, impl);
        } else if (auto z = struct_map.find(this_type_view); z != struct_map.end()) {
            auto member = z->second.HasValue(value);
            replacement = member ? wis::format("{}::{}", GetCPPFullTypename(z->second.name, impl), member->name)
                                 : GetCPPFullTypename(z->second.name, impl);
        } else if (auto d = delegate_map.find(this_type_view); d != delegate_map.end()) {
            auto member = d->second.HasValue(value);
            replacement = member ? wis::format("{}::{}", GetCPPFullTypename(d->second.name, impl), member->name)
                                 : GetCPPFullTypename(d->second.name, impl);
        } else if (auto h = handle_map.find(this_type_view); h != handle_map.end()) {
            replacement = GetCPPFullTypename(h->second.name, impl);
        } else if (auto f = function_map.find(std::string(this_type_view)); f != function_map.end()) {
            auto member = f->second.HasValue(value);
            replacement = member ? wis::format("{}({})", GetCPPFullTypename(f->second.name, impl), member->name)
                                 : GetCPPFullTypename(f->second.name, impl);
        }

        pos = last;
        doc.replace(first, last - first + 1, replacement);
    }
    return doc;
}

std::string Generator::MakeCValueDocumentation(std::string value, std::string_view doc, std::string_view type_name)
{
    std::string documentation;
    bool pre_doc = false;
    if (!doc.empty()) {
        if (doc.find('\n') != std::string_view::npos) {
            pre_doc = true;
            documentation = wis::format("/**\n@brief {}\n*/", doc);
            ReplaceAll(documentation, "\n", "\n * ");
        } else {
            documentation = wis::format(" ///< {}", doc);
        }
        documentation = FinalizeCDocumentation(documentation, type_name);
    }
    return pre_doc
            ? wis::format("    {}\n    {}\n", documentation, value)
            : wis::format("{}{}\n", value, documentation);
}

std::string Generator::MakeCPPValueDocumentation(std::string value, std::string_view doc, std::string_view type_name)
{
    std::string documentation;
    bool pre_doc = false;
    if (!doc.empty()) {
        if (doc.find('\n') != std::string_view::npos) {
            pre_doc = true;
            documentation = wis::format("/**\n@brief {}\n*/", doc);
            ReplaceAll(documentation, "\n", "\n * ");
        } else {
            documentation = wis::format(" ///< {}", doc);
        }
        documentation = FinalizeCPPDocumentation(documentation, type_name);
    }
    return pre_doc
            ? wis::format("    {}\n    {}\n", documentation, value)
            : wis::format("{}{}\n", value, documentation);
}

//-----------------------------------------------------------------------------
std::pair<std::string, std::string> Generator::MakeCVariant(const WisVariant& s)
{
    using namespace std::string_literals;

    if (s.implemented_for == Language::CPP || s.implemented_for == Language::Hidden) {
        return {};
    }

    std::string st_decls;

    auto make_impl = [this](const WisVariant& s, const WisVariantImpl& impl,
                            std::string_view impl_tag) {
        auto full_name = GetCFullTypename(s.name, impl_tag);

        if (!s.this_type.empty()) {
            auto this_name = GetCFullTypename(s.this_type, impl_tag);

            auto as_decl = wis::format("WISDOM_API {} As{}({} self);\n", full_name, full_name, this_name);
            auto as_impl = wis::format("extern \"C\" {} As{}({} self)\n{{\n wis::{} xself = reinterpret_cast<wis::{}&"
                                       ">(*self);\n  return "
                                       "reinterpret_cast<{}&>(xself);\n}}\n",
                                       full_name, full_name, this_name, full_name, this_name, full_name);

            if (impl_tag == impls[1]) {
                function_decls_dx.emplace_back(std::move(as_decl));
                function_impl_dx.emplace_back(std::move(as_impl));
            } else if (impl_tag == impls[2]) {
                function_decls_vk.emplace_back(std::move(as_decl));
                function_impl_vk.emplace_back(std::move(as_impl));
            }
        }

        auto st_decl = wis::format("struct {}{{\n", full_name);

        if (!s.doc.empty()) {
            std::string documentation = wis::format("/**\n@brief {}\n*/", s.doc);
            ReplaceAll(documentation, "\n", "\n * ");
            st_decl = wis::format("{}\n{}", FinalizeCDocumentation(documentation, s.name), st_decl);
        }

        for (auto& m : impl.members) {
            WisFunctionParameter wfp{
                .type_info = GetTypeInfo(m.type),
                .type = m.type,
                .name = m.name,
                .modifier = m.modifier,
                .default_value = m.default_value,
                .doc = m.doc,
            };
            auto val_str = m.array_size.empty()
                    ? wis::format("    {};", GetCFullArg(wfp, impl_tag))
                    : wis::format("    {} {}[{}];", GetCFullArg(wfp, impl_tag, true), m.name, m.array_size);
            st_decl += MakeCValueDocumentation(val_str, m.doc, s.name);
        }

        st_decl += "};\n\n";
        return st_decl;
    };

    std::string dx, vk;
    for (auto& i : s.impls) {
        if (i.impl == ImplementedFor::Both) {
            dx += make_impl(s, i, impls[1]);
            vk += make_impl(s, i, impls[2]);
            return { std::move(dx), std::move(vk) };
        }
        if (+i.impl == 1) {
            dx += make_impl(s, i, impls[+i.impl]);
        } else if (+i.impl == 2) {
            vk += make_impl(s, i, impls[+i.impl]);
        }
    }
    return { dx, vk };
}

std::string Generator::MakeCPPVariant(const WisVariant& s, ImplementedFor impl)
{
    using namespace std::string_literals;
    std::string st_decls;

    auto make_impl = [this](const WisVariant& s, const WisVariantImpl& impl,
                            std::string_view impl_tag) {
        auto full_name = s.name;
        auto st_decl = wis::format("struct {}{}{{\n", impl_tag, full_name);

        if (!s.doc.empty()) {
            std::string documentation = wis::format("/**\n@brief {}\n*/", s.doc);
            ReplaceAll(documentation, "\n", "\n * ");
            st_decl = wis::format("{}\n{}", FinalizeCPPDocumentation(documentation, s.name), st_decl);
        }

        for (auto& m : impl.members) {
            std::string val_str;
            WisFunctionParameter wfp{
                .type_info = GetTypeInfo(m.type),
                .type = m.type,
                .name = m.name,
                .modifier = m.modifier,
                .default_value = m.default_value,
                .doc = m.doc,
            };

            if (m.array_size.empty()) {
                val_str = wis::format("    {};", GetCPPFullArg(wfp, impl_tag));
            } else {
                val_str = wis::format("    std::array<{}, {}> {} {{}};", GetCPPFullArg(wfp, impl_tag, true), m.array_size, m.name);
            }
            st_decl += MakeCPPValueDocumentation(val_str, m.doc, s.name);
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

    if (!s.doc.empty()) {
        std::string documentation = wis::format("/**\n@brief {}\n*/", s.doc);
        ReplaceAll(documentation, "\n", "\n * ");
        st_decl = wis::format("{}\n{}", FinalizeCDocumentation(documentation, s.name), st_decl);
    }

    for (auto& m : s.members) {
        WisFunctionParameter wfp{
            .type_info = GetTypeInfo(m.type),
            .type = m.type,
            .name = m.name,
            .modifier = m.modifier,
            .default_value = m.default_value,
            .doc = m.doc,
        };
        auto val_str = m.array_size.empty()
                ? wis::format("    {};", GetCFullArg(wfp, ""))
                : wis::format("    {} {}[{}];", GetCFullArg(wfp, "", true), m.name, m.array_size);
        st_decl += MakeCValueDocumentation(val_str, m.doc, s.name);
    }

    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPStruct(const WisStruct& s)
{
    auto full_name = s.name;
    auto st_decl = wis::format("struct {}{{\n", full_name);

    if (!s.doc.empty()) {
        std::string documentation = wis::format("/**\n@brief {}\n*/", s.doc);
        ReplaceAll(documentation, "\n", "\n * ");
        st_decl = wis::format("{}\n{}", FinalizeCPPDocumentation(documentation, s.name), st_decl);
    }

    for (auto& m : s.members) {
        std::string val_str;
        WisFunctionParameter wfp{
            .type_info = GetTypeInfo(m.type),
            .type = m.type,
            .name = m.name,
            .modifier = m.modifier,
            .default_value = m.default_value,
            .doc = m.doc,
        };

        if (m.array_size.empty()) {
            val_str = wis::format("    {};", GetCPPFullArg(wfp, ""));
        } else {
            val_str = wis::format("    std::array<{}, {}> {} {{}};", GetCPPFullArg(wfp, "", true), m.array_size, m.name);
        }
        st_decl += MakeCPPValueDocumentation(val_str, m.doc, s.name);
    }

    st_decl += "};\n\n";
    return st_decl;
}

std::string Generator::MakeCEnum(const WisEnum& s)
{
    auto full_name = GetCFullTypename(s.name, "");
    std::string st_decl = wis::format("enum {} {{\n", full_name);

    if (!s.doc.empty()) {
        std::string documentation = wis::format("/**\n@brief {}\n\n{}*/", s.doc, s.doc_translates);
        ReplaceAll(documentation, "\n", "\n * ");
        st_decl = wis::format("{}\n{}", FinalizeCDocumentation(documentation, s.name), st_decl);
    }

    for (auto& m : s.values) {
        st_decl += MakeCValueDocumentation(wis::format("{}{}{} = {},", s.name, impls[+m.impl], m.name, m.value),
                                           m.doc, s.name);
    }

    st_decl += "};\n\n";
    return st_decl;
}
std::string Generator::MakeCPPEnum(const WisEnum& s)
{
    std::string st_decl =
            !s.type.empty() ? wis::format("enum class {} : {} {{\n", s.name, standard_types.at(s.type))
                            : wis::format("enum class {} {{\n", s.name);

    if (!s.doc.empty()) {
        std::string documentation = wis::format("/**\n@brief {}\n\n{}*/", s.doc, s.doc_translates);
        ReplaceAll(documentation, "\n", "\n * ");
        st_decl = wis::format("{}\n{}", FinalizeCPPDocumentation(documentation, s.name), st_decl);
    }

    for (auto& m : s.values) {
        st_decl += MakeCPPValueDocumentation(wis::format("{}{} = {},", impls[+m.impl], m.name, m.value),
                                             m.doc, s.name);
    }
    st_decl += "};\n\n";

    return st_decl;
}

std::string Generator::MakeCBitmask(const WisBitmask& s)
{
    auto full_name = GetCFullTypename(s.name, "");
    std::string st_decl = wis::format("enum {}Bits {{\n", full_name);

    if (!s.doc.empty()) {
        std::string documentation = wis::format("/**\n@brief {}\n\n{}*/", s.doc, s.doc_translates);
        ReplaceAll(documentation, "\n", "\n * ");
        st_decl = wis::format("{}\n{}", FinalizeCDocumentation(documentation, s.name), st_decl);
    }

    for (auto& m : s.values) {
        std::string val_str = m.type == WisBitmaskValue::Type::Value
                ? wis::format("    {}{}{} = 0x{:X},", s.name, impls[+m.impl], m.name, m.value)
                : wis::format("    {}{}{} = 1 << {},", s.name, impls[+m.impl], m.name, m.bit);

        st_decl += MakeCValueDocumentation(val_str, m.doc, s.name);
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

    if (!s.doc.empty()) {
        std::string documentation = wis::format("/**\n@brief {}\n\n{}*/", s.doc, s.doc_translates);
        ReplaceAll(documentation, "\n", "\n * ");
        st_decl = wis::format("{}\n{}", FinalizeCPPDocumentation(documentation, s.name), st_decl);
    }

    for (auto& m : s.values) {
        auto val_str = m.type == WisBitmaskValue::Type::Value
                ? wis::format("    {}{} = 0x{:X},", impls[+m.impl], m.name, m.value)
                : wis::format("    {}{} = 1 << {},", impls[+m.impl], m.name, m.bit);

        st_decl += MakeCPPValueDocumentation(val_str, m.doc, s.name);
    }

    st_decl += "};\n\n";
    cpp_type_traits.emplace_back(wis::format(
            "template <> struct is_flag_enum<wis::{}>:public std::true_type {{}};\n", s.name));
    return st_decl;
}

#pragma region C API
std::string Generator::MakeCFunctionGenericDecl(const WisFunction& func, std::string_view impl)
{
    return wis::format("{}{{ {} {}; }}\n", MakeCFunctionProto(func, "Wis", "inline"),
                       func.return_type.IsVoid() ? "" : "return", MakeCFunctionCall(func, "", impl));
}
std::string Generator::MakeCFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl, bool enable_doc)
{
    std::string args;
    std::string return_t;
    std::string doc;

    if (!func.doc.empty()) {
        doc = wis::format("/**\n@brief {}\n", func.doc);
    }

    if (!func.this_type.empty()) {
        WisFunctionParameter this_arg{
            .type_info = func.this_type_info,
            .type = func.this_type,
            .name = "self",
        };
        doc += wis::format("@param {} valid handle to the {}\n", this_arg.name, func.this_type, func.this_type);
        args += GetCFullArg(this_arg, impl) + ", ";
    }

    // Arguments
    for (auto& arg : func.parameters) {
        if (arg.replaced && arg.replaced->replace_for == Language::C) {
            WisFunctionParameter repl{
                .type_info = arg.replaced->type_info,
                .type = arg.replaced->type,
                .name = arg.name,
                .modifier = arg.replaced->modifier,
                .default_value = arg.replaced->default_value,
                .doc = arg.replaced->doc,
            };
            doc += wis::format("@param {} {}\n", repl.name, repl.doc);
            args += GetCFullArg(repl, impl) + ", ";
        } else {
            doc += wis::format("@param {} {}\n", arg.name, arg.doc);
            args += GetCFullArg(arg, impl) + ", ";
        }
    }

    // Return type
    if (func.return_type.type == "") {
        return_t = func.return_type.has_result ? GetCFullTypename("Result") : "void";
    } else if (func.return_type.has_result) {
        WisFunctionParameter res{
            .type_info = func.return_type.type_info,
            .type = func.return_type.type,
            .name = func.return_type.opt_name,
            .modifier = func.return_type.modifier == "ptr" ? "pp" : "ptr",
        };
        doc += wis::format("@param {} {}\n", res.name, func.return_type.doc);
        doc += wis::format("@return {}\n", ResultDoc);
        return_t = GetCFullTypename("Result");
        args += GetCFullArg(res, impl);
    } else {
        WisFunctionParameter res{
            .type_info = func.return_type.type_info,
            .type = func.return_type.type,
            .name = "",
            .modifier = func.return_type.modifier,
        };

        doc += wis::format("@return {}\n", func.return_type.doc);
        return_t = GetCFullArg(res, impl);
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    // Documentation
    if (func.doc.empty() || !enable_doc) {
        doc = "";
    } else {
        doc += "*/";
        doc = FinalizeCDocumentation(doc, func.this_type, impl);
        ReplaceAll(doc, "\n", "\n * ");
        doc += "\n";
    }

    return wis::format("{}{} {} {}{}({})", doc, pre_decl, return_t,
                       func.this_type.empty()
                               ? std::string(impl)
                               : GetCFullTypename(func.this_type, impl),
                       func.name, args);
}
std::string Generator::MakeCFunctionDecl(const WisFunction& func, std::string_view impl, std::string_view pre_decl)
{
    return MakeCFunctionProto(func, impl, pre_decl) + ";\n";
}
std::string Generator::MakeCFunctionCall(const WisFunction& func, std::string_view prefix, std::string_view impl)
{
    std::string args;

    if (!func.this_type.empty()) {
        args += "self, ";
    }

    // Arguments
    for (auto& arg : func.parameters) {
        args += std::string(arg.name) + ", ";
    }

    // Return type
    if (func.return_type.has_result) {
        args += func.return_type.opt_name;
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    return wis::format("{}{}{}({})", prefix,
                       func.this_type.empty() ? std::string(impl) : GetCFullTypename(func.this_type, impl),
                       func.name, args);
}
std::string Generator::MakeCFunctionImpl(const WisFunction& func, std::string_view prefix, std::string_view impl)
{
    if (func.custom_impl) {
        return "";
    }

    std::string output;

    // Function prototype
    output += MakeCFunctionProto(func, impl, "extern \"C\"", false);

    // Function body
    std::string body;
    std::string call;

    // Convert this type if present
    if (!func.this_type.empty()) {
        WisFunctionParameter this_arg{
            .type_info = func.this_type_info,
            .type = func.this_type,
            .name = "self",
        };

        body += wis::format("    auto* xself = {};\n", ConvertFromCType(this_arg, impl));
        if (func.name == "Destroy") {
            body += "    delete xself;\n";
            output += wis::format("{{\n{}\n}}", body);
            return output;
        }
        call = "xself->";
    }

    // Arguments
    std::string args;
    for (auto& arg : func.parameters) {
        auto converted = ConvertFromCType(arg, impl);

        if ((arg.type_info == TypeInfo::Handle || arg.type_info == TypeInfo::ExtHandle) && arg.modifier.empty()) {
            converted = wis::format("*{}", converted);
        }
        args += converted + ", ";
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    call += wis::format("{}({});\n", func.name, args);

    // Return type
    if (func.return_type.type.empty()) {
        // No return type, just call the function
        if (!func.return_type.has_result) {
            body += call;
        } else {
            WisFunctionParameter res{
                .type_info = TypeInfo::Result,
                .type = "Result",
                .name = "res",
            };

            body += wis::format("    auto res = {};\n", call);
            body += wis::format("    return {};\n", ConvertToCType(res, impl));
        }
        output += wis::format("{{\n{}\n}}", body);
        return output;
    }

    // Return type
    if (func.return_type.has_result) {
        WisFunctionParameter result{
            .type_info = TypeInfo::Result,
            .type = "Result",
            .name = "res",
        };
        std::string ret_status = wis::format("return {};\n", ConvertToCType(result, impl));

        body += wis::format("    auto&& [res, value] = {}\n", call);
        body += wis::format("    if(res.status != wis::Status::Ok) {}\n", ret_status);

        if (func.return_type.type_info == TypeInfo::Handle) {
            WisFunctionParameter ares{
                .type_info = func.return_type.type_info,
                .type = func.return_type.type,
            };
            auto xtype = GetCPPFullArg(ares, impl, true);
            auto allocation = wis::format("new(std::nothrow){}(std::move(value))", xtype);
            WisFunctionParameter bres{
                .type_info = func.return_type.type_info,
                .type = func.return_type.type,
                .name = allocation,
            };

            body += wis::format("    *{} = {};\n", func.return_type.opt_name, ConvertToCType(bres, impl));
            body += wis::format("    if(!*{}) return WisResult{{ StatusOutOfMemory, \"Failed to allocate memory for {}.\" }};\n", func.return_type.opt_name, xtype);
        } else {
            WisFunctionParameter res{
                .type_info = func.return_type.type_info,
                .type = func.return_type.type,
                .name = "value",
            };
            body += wis::format("    *{} = {};\n", func.return_type.opt_name, ConvertToCType(res, impl));
        }

        WisFunctionParameter res{
            .type_info = func.return_type.type_info,
            .type = func.return_type.type,
            .name = "res",
        };
        body += wis::format("    {}", ret_status);
        output += wis::format("{{\n{}\n}}", body);
        return output;
    }

    WisFunctionParameter res{
        .type_info = func.return_type.type_info,
        .type = func.return_type.type,
        .name = "res",
        .modifier = func.return_type.modifier,
    };

    body += wis::format("    auto res = {};\n", call);
    body += wis::format("    return {};\n", ConvertToCType(res, impl));

    output += wis::format("{{\n{}\n}}", body);
    return output;
}
std::string Generator::MakeCDelegate(const WisFunction& func)
{
    std::string_view impl = "";
    std::string args;
    std::string return_t;
    std::string doc;

    if (!func.doc.empty()) {
        doc = wis::format("/**\n@brief {}\n", func.doc);
    }

    // Arguments
    for (auto& arg : func.parameters) {
        doc += wis::format("@param {} {}\n", arg.name, arg.doc);
        args += GetCFullArg(arg, impl) + ", ";
    }

    // Return type
    if (func.return_type.type == "") {
        return_t = "void";
    } else if (func.return_type.has_result) {
        WisFunctionParameter res{
            .type_info = func.return_type.type_info,
            .type = func.return_type.type,
            .name = func.return_type.opt_name,
            .modifier = "ptr",
        };
        doc += wis::format("@param {} {}\n", res.name, func.return_type.doc);
        doc += wis::format("@return {}\n", ResultDoc);
        return_t = GetCFullTypename("Result");
        args += GetCFullArg(res, impl);
    } else {
        doc += wis::format("@return {}\n", func.return_type.doc);
        return_t = GetCFullTypename(func.return_type.type, impl);
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    // Documentation
    if (func.doc.empty()) {
        doc = "";
    } else {
        doc += "*/";
        doc = FinalizeCDocumentation(doc, func.this_type, impl);
        ReplaceAll(doc, "\n", "\n * ");
    }

    return wis::format("{}\ntypedef {} (*{}{})({});\n", doc, return_t, impl, func.name, args);
}

std::string Generator::MakeCHandle(const WisHandle& s, std::string_view impl)
{
    return wis::format("typedef struct {}{}_t* {}{};\n", impl, s.name, impl, s.name);
}
std::string Generator::MakeCHandleGeneric(const WisHandle& s, std::string_view impl)
{
    return wis::format("typedef {}{} Wis{};\n", impl, s.name, s.name);
}

std::string Generator::MakeCHandleMethods(const WisHandle& s, std::string_view impl)
{
    std::string chunk = wis::format("// {} methods --\n", GetCFullTypename(s.name, impl));

    for (auto& f : s.functions) {
        auto& func = function_map[std::string(f)];

        chunk += MakeCFunctionDecl(func, impl, "WISDOM_API") + '\n';
    }
    return chunk;
}
std::string Generator::MakeCHandleMethodsGeneric(const WisHandle& s, std::string_view impl)
{
    std::string chunk = wis::format("// {} methods --\n", GetCFullTypename(s.name, "Wis"));

    for (auto& f : s.functions) {
        auto& func = function_map[std::string(f)];

        chunk += MakeCFunctionGenericDecl(func, impl) + '\n';
    }
    return chunk;
}

std::string Generator::MakeCHandleMethodImpls(const WisHandle& s, std::string_view impl)
{
    std::string chunk = wis::format("// {} methods --\n", GetCFullTypename(s.name, impl));
    for (auto& f : s.functions) {
        auto& func = function_map[std::string(f)];
        chunk += MakeCFunctionImpl(func, "", impl);
    }
    return chunk;
}

std::string Generator::MakeCExtensionHeader(const WisExtension& s)
{
    std::string output = wis::format("// {}--\n", s.name);
    output += wis::format("#ifndef WIS_{}\n#define WIS_{} 1\n#endif\n\n", s.name, s.name);

    output += "#ifdef WISDOM_VULKAN\n";
    auto impl_vk = impls[+ImplementedFor::Vulkan];
    output += wis::format("typedef {}FactoryExtension {}{};\n", impl_vk, impl_vk, s.name);
    for (auto& h : s.handles) {
        output += MakeCHandle(handle_map[h], impl_vk) + '\n';
    }

    output += MakeCHandleMethods(s, impl_vk) + '\n';
    for (auto& h : s.handles) {
        output += MakeCHandleMethods(handle_map[h], impl_vk) + '\n';
    }
    output += "#endif\n\n";

    output += "#ifdef WISDOM_DX12\n";
    auto impl_dx = impls[+ImplementedFor::DX12];
    output += wis::format("typedef {}FactoryExtension {}{};\n", impl_dx, impl_dx, s.name);
    for (auto& h : s.handles) {
        output += MakeCHandle(handle_map[h], impl_dx) + '\n';
    }

    output += MakeCHandleMethods(s, impl_dx) + '\n';
    for (auto& h : s.handles) {
        output += MakeCHandleMethods(handle_map[h], impl_dx) + '\n';
    }
    output += "#endif\n\n";

    output += "#if defined(WISDOM_DX12) && !FORCEVK_SWITCH\n";
    output += wis::format("typedef {}{} Wis{};\n", impl_dx, s.name, s.name);
    for (auto& h : s.handles) {
        output += MakeCHandleGeneric(handle_map[h], impl_dx) + '\n';
    }

    output += MakeCHandleMethodsGeneric(s, impl_dx) + '\n';

    for (auto& h : s.handles) {
        output += MakeCHandleMethodsGeneric(handle_map[h], impl_dx) + '\n';
    }

    output += "#elif defined(WISDOM_VULKAN)\n\n";
    output += wis::format("typedef {}{} Wis{};\n", impl_vk, s.name, s.name);
    for (auto& h : s.handles) {
        output += MakeCHandleGeneric(handle_map[h], impl_vk) + '\n';
    }

    output += MakeCHandleMethodsGeneric(s, impl_vk) + '\n';

    for (auto& h : s.handles) {
        output += MakeCHandleMethodsGeneric(handle_map[h], impl_vk) + '\n';
    }
    output += "#endif\n\n";

    output += k_delimiter;
    return output;
}
std::string Generator::MakeCExtensionImpl(const WisExtension& s, std::string_view impl)
{
    return MakeCHandleMethodImpls(s, impl) + '\n';
}
std::string Generator::MakeCExtensionMap(std::string_view impl)
{
    // Map contains size and type
    constexpr static auto generic_template_f = R"(template<{}>
struct {}FactoryExtensionMap{} {{
        using Type = wis::{}{};
    }};)";

    constexpr static auto generic_template_d = R"(template<{}>
struct {}DeviceExtensionMap{} {{
        using Type = wis::{}{};
    }};)";

    auto make_template_f = [](std::string_view ext_name) {
        return wis::format("<wis::FactoryExtID::{}>", ext_name);
    };
    auto make_template_d = [](std::string_view ext_name) {
        return wis::format("<wis::DeviceExtID::{}>", ext_name);
    };

    std::string bridge_f = wis::format(
            R"(template<template<typename T> typename Executor, typename...Args>
constexpr static inline decltype(auto) {}FactoryExtensionBridge(wis::FactoryExtID id, Args&&...args) {{
    switch(id) {{
)",
            impl);

    std::string bridge_d = wis::format(
            R"(template<template<typename T> typename Executor, typename...Args>
constexpr static inline decltype(auto) {}DeviceExtensionBridge(wis::DeviceExtID id, Args&&...args) {{
    switch(id) {{
)",
            impl);

    std::string factory_map = wis::format(generic_template_f, "wis::FactoryExtID", impl, "", impl, "FactoryExtension");
    std::string device_map = wis::format(generic_template_d, "wis::DeviceExtID", impl, "", impl, "DeviceExtension");
    for (auto& [k, v] : extension_map) {
        switch (v.type) {
        case ExtensionType::Device:
            device_map += wis::format(generic_template_d, "", impl, make_template_d(v.name), impl, k);
            bridge_d += wis::format(
                    "       case wis::DeviceExtID::{}: return Executor<typename {}DeviceExtensionMap{}::Type>{{}}(std::forward<Args>(args)...);",
                    v.name, impl, make_template_d(v.name));
            break;
        case ExtensionType::Factory:
            factory_map += wis::format(generic_template_f, "", impl, make_template_f(v.name), impl, k);
            bridge_f += wis::format(
                    "       case wis::FactoryExtID::{}: return Executor<typename {}FactoryExtensionMap{}::Type>{{}}(std::forward<Args>(args)...);",
                    v.name, impl, make_template_f(v.name));
            break;
        default:
            break;
        }
    }

    // Add default case and close switch with function
    bridge_f += wis::format("default: return Executor<wis::{}FactoryExtension>{{}}(std::forward<Args>(args)...);\n}}\n}}\n", impl);
    bridge_d += wis::format("default: return Executor<wis::{}DeviceExtension>{{}}(std::forward<Args>(args)...);\n}}\n}}\n", impl);

    return factory_map + std::string(k_delimiter) + bridge_f + std::string(k_delimiter) + device_map + std::string(k_delimiter) + bridge_d;
}
std::string Generator::MakeCVariantGeneric(const WisVariant& s, std::string_view impl)
{
    return wis::format("typedef {}{} Wis{};", impl, s.name, s.name);
}
#pragma endregion

#pragma region C++ API
std::string Generator::MakeCPPFunctionGenericDecl(const WisFunction& func, std::string_view impl, bool explicit_result)
{
    return wis::format("{}{{ return {}; }}", MakeCPPFunctionProto(func, "", "inline", true, true, explicit_result),
                       MakeCPPFunctionCall(func, impl, explicit_result));
}
std::string Generator::MakeCPPFunctionProto(const WisFunction& func, std::string_view impl, std::string_view pre_decl, bool enable_doc, bool impl_on_fdecl, bool explicit_result)
{
    std::string args;
    std::string return_t;
    std::string doc;

    if (!func.doc.empty()) {
        doc = wis::format("/**\n@brief {}\n", func.doc);
    }

    if (explicit_result && func.return_type.has_result && !func.return_type.type.empty()) {
        args += "wis::Result& result, ";
    }

    // Arguments
    for (auto& arg : func.parameters) {
        if (arg.replaced && arg.replaced->replace_for == Language::CPP) {
            WisFunctionParameter repl{
                .type_info = arg.replaced->type_info,
                .type = arg.replaced->type,
                .name = arg.replaced->name.empty() ? arg.name : arg.replaced->name,
                .modifier = arg.replaced->modifier,
                .default_value = arg.replaced->default_value,
                .doc = arg.replaced->doc,
            };
            doc += wis::format("@param {} {}\n", repl.name, repl.doc);
            args += GetCPPFullArg(repl, impl) + ", ";
        } else {
            doc += wis::format("@param {} {}\n", arg.name, arg.doc);
            args += GetCPPFullArg(arg, impl) + ", ";
        }
    }

    // Return type
    if (func.return_type.type == "") {
        return_t = func.return_type.has_result ? GetCPPFullTypename("Result") : "void";
    } else if (func.return_type.has_result) {
        return_t = explicit_result
                ? wis::format("{}", GetCPPFullTypename(func.return_type.type, impl))
                : wis::format("wis::ResultValue<{}>", GetCPPFullTypename(func.return_type.type, impl));

        doc += wis::format("@return {}\n", func.return_type.doc);
    } else {
        WisFunctionParameter res{
            .type_info = func.return_type.type_info,
            .type = func.return_type.type,
            .name = "",
            .modifier = func.return_type.modifier,
        };

        doc += wis::format("@return {}\n", func.return_type.doc);
        return_t = GetCPPFullArg(res, impl);
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    // Documentation
    if (func.doc.empty() || !enable_doc) {
        doc = "";
    } else {
        doc += "*/";
        doc = FinalizeCPPDocumentation(doc, func.this_type, impl);
        ReplaceAll(doc, "\n", "\n * ");
        doc += "\n";
    }

    return wis::format("{}{} {} {}{}({})", doc, pre_decl, return_t, impl_on_fdecl ? impl : "", func.name, args);
}
std::string Generator::MakeCPPFunctionDecl(const WisFunction& func, std::string_view impl, std::string_view pre_decl)
{
    return MakeCPPFunctionProto(func, impl, pre_decl) + ";\n";
}
std::string Generator::MakeCPPFunctionCall(const WisFunction& func, std::string_view impl, bool explicit_result)
{
    std::string args;

    if (explicit_result && func.return_type.IsRV()) {
        args += "result, ";
    }

    // Arguments
    for (auto& arg : func.parameters) {

        if (arg.type_info == TypeInfo::Handle && arg.modifier.empty()) {
            args += wis::format("std::move({}), ", arg.name);
        } else {
            args += std::string(arg.name) + ", ";
        }
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    return wis::format("{}{}({})", impl, func.name, args);
}

std::string Generator::MakeCPPRVFunctionCall(const WisFunction& func, std::string_view impl, std::string_view class_impl)
{
    // return wis::ResultValue<wis::DX12Adapter>{
    //     &ImplDX12Factory::GetAdapter, this, index, preference
    // };

    std::string args;

    // Arguments
    for (auto& arg : func.parameters) {

        if (arg.type_info == TypeInfo::Handle && arg.modifier.empty()) {
            args += wis::format("std::move({}), ", arg.name);
        } else {
            args += std::string(arg.name) + ", ";
        }
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }
    return func.this_type.empty()
            ? wis::format("wis::ResultValue<{}>{{&{}{},{}}}", GetCPPFullTypename(func.return_type.type, class_impl), impl, func.name, args)
            : wis::format("wis::ResultValue<{}>{{&{}{},this,{}}}", GetCPPFullTypename(func.return_type.type, class_impl), impl, func.name, args);
}
std::string Generator::MakeCPPDelegate(const WisFunction& func)
{
    std::string_view impl = "";
    std::string args;
    std::string return_t;
    std::string doc;

    if (!func.doc.empty()) {
        doc = wis::format("/**\n@brief {}\n", func.doc);
    }

    // Arguments
    for (auto& arg : func.parameters) {
        doc += wis::format("@param {} {}\n", arg.name, arg.doc);
        args += GetCPPFullArg(arg, impl) + ", ";
    }

    // Return type
    if (func.return_type.type == "") {
        return_t = "void";
    } else if (func.return_type.has_result) {
        WisFunctionParameter res{
            .type_info = func.return_type.type_info,
            .type = func.return_type.type,
            .name = func.return_type.opt_name,
            .modifier = "ptr",
        };
        doc += wis::format("@param {} {}\n", res.name, func.return_type.doc);
        doc += wis::format("@return {}\n", ResultDoc);
        return_t = GetCPPFullTypename("Result");
        args += GetCPPFullArg(res, impl);
    } else {
        doc += wis::format("@return {}\n", func.return_type.doc);
        return_t = GetCPPFullTypename(func.return_type.type, impl);
    }

    // Remove trailing ", "
    if (args.ends_with(", ")) {
        args.pop_back();
        args.pop_back();
    }

    // Documentation
    if (func.doc.empty()) {
        doc = "";
    } else {
        doc += "*/";
        doc = FinalizeCPPDocumentation(doc, func.this_type, impl);
        ReplaceAll(doc, "\n", "\n * ");
    }

    return wis::format("{}\nusing {}{} = {} (*)({});\n", doc, impl, func.name, return_t, args);
}
std::string Generator::MakeCPPHandle(const WisHandle& s, std::string_view impl)
{
    std::string doc;
    if (!s.doc.empty()) {
        doc = wis::format("/**\n@brief {}\n", s.doc);
    }
    if (s.doc.empty()) {
        doc = "";
    } else {
        doc += "*/";
        doc = FinalizeCPPDocumentation(doc, s.name, impl);
        ReplaceAll(doc, "\n", "\n * ");
    }

    std::string head = wis::format("{}\n WISDOM_EXPORT\nclass {}{} : public wis::Impl{}{}", doc, impl, s.name, impl, s.name);
    std::string ctor = wis::format("public:\n using wis::Impl{}{}::Impl{}{};", impl, s.name, impl, s.name);

    std::string funcs = "public:\n";
    for (auto& f : s.functions) {
        auto& func = function_map[std::string(f)];
        if (func.name == "Destroy" || func.implemented_for == Language::C || func.custom_impl) {
            continue;
        }

        if (func.return_type.IsRV()) {
            funcs += wis::format("{}{} noexcept{{\n    {} {};\n}}\n", MakeCPPFunctionProto(func, impl, func.return_type.has_result ? "[[nodiscard]] inline" : "inline", true, false, true),
                                 func.const_func ? "const" : "", !func.return_type.IsVoid() ? "return" : "", MakeCPPFunctionCall(func, wis::format("wis::Impl{}{}::", impl, s.name), true));
            funcs += wis::format("{}{} noexcept{{\n    {} {};\n}}\n", MakeCPPFunctionProto(func, impl, func.return_type.has_result ? "[[nodiscard]] inline" : "inline", true, false, false),
                                 func.const_func ? "const" : "", !func.return_type.IsVoid() ? "return" : "", MakeCPPRVFunctionCall(func, wis::format("wis::Impl{}{}::", impl, s.name), impl));
        } else {
            funcs += wis::format("{}{} noexcept{{\n    {} {};\n}}\n", MakeCPPFunctionProto(func, impl, func.return_type.has_result ? "[[nodiscard]] inline" : "inline", true, false),
                                 func.const_func ? "const" : "", !func.return_type.IsVoid() ? "return" : "", MakeCPPFunctionCall(func, wis::format("wis::Impl{}{}::", impl, s.name)));
        }
    }

    return wis::format("{} {{\n{}\n{}\n}};\n", head, ctor, funcs);
}
#pragma endregion

std::string Generator::GetCFullTypename(std::string_view type, std::string_view impl)
{
    if (type.empty()) {
        return "";
    }

    if (auto it = standard_types.find(type); it != standard_types.end()) {
        return std::string(it->second);
    }

    if (auto it = enum_map.find(type); it != enum_map.end()) {
        return "Wis" + std::string(type);
    }

    if (auto it = bitmask_map.find(type); it != bitmask_map.end()) {
        return "Wis" + std::string(type);
    }

    if (auto it = variant_map.find(type); it != variant_map.end()) {
        return std::string(impl) + std::string(type);
    }

    if (auto it = struct_map.find(type); it != struct_map.end()) {
        return "Wis" + std::string(type);
    }

    if (auto it = handle_map.find(type); it != handle_map.end()) {
        return std::string(impl) + std::string(type);
    }
    if (auto it = extension_map.find(type); it != extension_map.end()) {
        return std::string(impl) + std::string(type);
    }

    if (auto it = delegate_map.find(type); it != delegate_map.end()) {
        return std::string(type);
    }

    if (auto it = function_map.find(std::string(type)); it != function_map.end()) {
        return std::string(impl) + std::string(type);
    }

    return "";
}
std::string Generator::GetCPPFullTypename(std::string_view type, std::string_view impl)
{
    if (type.empty()) {
        return "";
    }

    if (auto it = standard_types.find(type); it != standard_types.end()) {
        return std::string(it->second);
    }

    if (auto it = enum_map.find(type); it != enum_map.end()) {
        return "wis::" + std::string(type);
    }

    if (auto it = bitmask_map.find(type); it != bitmask_map.end()) {
        return "wis::" + std::string(type);
    }

    if (auto it = variant_map.find(type); it != variant_map.end()) {
        return "wis::" + std::string(impl) + std::string(type);
    }

    if (auto it = struct_map.find(type); it != struct_map.end()) {
        return "wis::" + std::string(type);
    }

    if (auto it = handle_map.find(type); it != handle_map.end()) {
        return "wis::" + std::string(impl) + std::string(type);
    }
    if (auto it = extension_map.find(type); it != extension_map.end()) {
        return "wis::" + std::string(impl) + std::string(type);
    }

    if (auto it = delegate_map.find(type); it != delegate_map.end()) {
        return "wis::" + std::string(type);
    }

    if (auto it = function_map.find(std::string(type)); it != function_map.end()) {
        return "wis::" + std::string(impl) + std::string(type);
    }

    return "";
}

std::string Generator::GetCFullArg(const WisFunctionParameter& arg, std::string_view impl, bool only_type)
{
    std::string post_decl;
    std::string pre_decl;
    if (arg.modifier.find("ptr") != std::string_view::npos || arg.modifier.find("ref") != std::string_view::npos) {
        post_decl = '*';
    }
    if (arg.modifier.find("pp") != std::string_view::npos) {
        post_decl = "**";
    }
    if (arg.modifier.find("const") != std::string_view::npos) {
        pre_decl += "const";
    }
    return only_type
            ? wis::format("{} {}{}", pre_decl, GetCFullTypename(arg.type, impl), post_decl)
            : wis::format("{} {}{} {}", pre_decl, GetCFullTypename(arg.type, impl), post_decl, arg.name);
}
std::string Generator::GetCPPFullArg(const WisFunctionParameter& arg, std::string_view impl, bool only_type)
{
    std::string post_decl;
    std::string post_name;
    std::string pre_decl;
    std::string mfullname = GetCPPFullTypename(arg.type, impl);

    if (arg.modifier.find("ptr") != std::string_view::npos) {
        post_decl = '*';
    }
    if (arg.modifier.find("pp") != std::string_view::npos) {
        post_decl = "**";
    }
    if (arg.modifier.find("const") != std::string_view::npos) {
        pre_decl += "const";
    }
    if (arg.modifier.find("ref") != std::string_view::npos) {
        post_decl = '&';
    }
    if (!arg.default_value.empty()) {
        post_name = enum_map.contains(arg.type) || bitmask_map.contains(arg.type)
                ? wis::format(" = {}::{}", mfullname, arg.default_value)
                : wis::format(" = {}", arg.default_value);
    }
    return only_type
            ? wis::format("{} {}{}", pre_decl, GetCPPFullTypename(arg.type, impl), post_decl)
            : wis::format("{} {}{} {}{}", pre_decl, GetCPPFullTypename(arg.type, impl), post_decl,
                          arg.name, post_name);
}

std::string Generator::ConvertFromCType(const WisFunctionParameter& arg, std::string_view impl)
{
    std::string trsf;
    if (arg.modifier.find("ref") != std::string_view::npos) {
        WisFunctionParameter repl{
            .type_info = arg.type_info,
            .type = arg.type,
            .name = arg.name,
            .modifier = arg.modifier.find("const") != std::string_view::npos ? "const ptr" : "ptr",
            .default_value = arg.default_value,
            .doc = arg.doc,
        };
        std::string type = GetCPPFullArg(repl, impl, true);
        return wis::format("*reinterpret_cast<{}>({})", type, arg.name);
    }

    std::string type = GetCPPFullArg(arg, impl, true);

    switch (arg.type_info) {
    case TypeInfo::Regular:
        return std::string(arg.name);
    case TypeInfo::Handle:
    case TypeInfo::ExtHandle:
        trsf = wis::format("reinterpret_cast<{}*>({})", type, arg.name);
        break;
    case TypeInfo::Enum:
    case TypeInfo::Bitmask:
        trsf = wis::format("static_cast<{}>({})", type, arg.name);
        break;
    case TypeInfo::Delegate:
        trsf = wis::format("reinterpret_cast<{}>({})", type, arg.name);
        break;
    default:
        trsf = wis::format("reinterpret_cast<{}&>({})", type, arg.name);
        break;
    }

    return trsf;
}

std::string Generator::ConvertToCType(const WisFunctionParameter& arg, std::string_view impl)
{
    std::string trsf;
    std::string type = GetCFullArg(arg, impl, true);

    switch (arg.type_info) {
    case TypeInfo::Regular:
        return std::string(arg.name);
    case TypeInfo::Handle:
    case TypeInfo::ExtHandle:
        trsf = wis::format("reinterpret_cast<{}>({})", type, arg.name);
        break;
    case TypeInfo::Enum:
    case TypeInfo::Bitmask:
    case TypeInfo::Delegate:
        trsf = wis::format("reinterpret_cast<{}>({})", type, arg.name);
        break;
    default:
        trsf = wis::format("reinterpret_cast<{}&>({})", type, arg.name);
        break;
    }

    return trsf;
}

TypeInfo Generator::GetTypeInfo(std::string_view type)
{
    if (type == "Result") {
        return TypeInfo::Result;
    }

    if (standard_types.contains(type)) {
        return TypeInfo::Regular;
    }

    if (enum_map.contains(type)) {
        return TypeInfo::Enum;
    }

    if (bitmask_map.contains(type)) {
        return TypeInfo::Bitmask;
    }

    if (variant_map.contains(type)) {
        return TypeInfo::Variant;
    }

    if (struct_map.contains(type)) {
        return TypeInfo::Struct;
    }

    if (handle_map.contains(type)) {
        return TypeInfo::Handle;
    }

    if (extension_map.contains(type)) {
        return TypeInfo::ExtHandle;
    }

    if (delegate_map.contains(type)) {
        return TypeInfo::Delegate;
    }

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
    return ImplementedFor::Unspecified;
}

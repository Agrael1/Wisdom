#include "generator.hpp"
#include <iostream>
#include "../src/include/wisdom/bridge/format.hpp"

constexpr inline std::string_view clang_format_exe = CLANG_FORMAT_EXECUTABLE;
inline constexpr std::string_view input_file       = INPUT_FILE;

void FormatFiles(std::span<const std::filesystem::path> files)
{
    constexpr uint32_t repeats = 5;
    if (clang_format_exe.empty()) {
        return;
    }
    std::string cmd;
    for (auto f : files) {
        cmd += f.string();
        cmd += ' ';
    }
    std::cout << "Wisdom Vk Utils: Formatting:\n"
              << cmd << '\n';
    std::string command = wis::format("\"{}\" -i --style=file {}", clang_format_exe, cmd);

    int ret = 0;
    for (uint32_t i = 0; (ret = std::system(command.c_str())) != 0 && i < repeats; ++i)
        ;
    if (ret != 0) {
        std::cout << "Wisdom Vk Utils: failed to format files with error <" << ret << ">\n";
    }
}

int main(int argc, char** argv)
{
    Generator g;

    // if no args, just generate core
    if (argc == 1) {
        g.ParseFile(input_file);
        g.WriteModuleAPI();
        g.WriteModuleAPIDoc();
        FormatFiles(g.GetFiles());
        return 0;
    }

    // get second arg. If it's -h or --help, print usage. Otherwise, treat it as a platform module name and generate it.
    std::string_view arg = argv[1];
    if (arg == "-h" || arg == "--help") {
        std::cout << "Usage: " << argv[0] << " [module_name,...]\n"
                  << "If module_name is provided, generates API for that platform module. Otherwise, generates core API.\n"
                  << "Modules are stored in xml folder. For example, if module_name is 'platform', the generator will look for 'xml/platform.xml' and generate API for it.\n";
        return 0;
    }

    // Load core module first to ensure all types are available for platform modules
    g.ParseFile(input_file);

    // Split platform module names by comma and generate for each
    for (size_t i = 0; i < arg.size();) {
        size_t           next_comma           = arg.find(',', i);
        std::string_view platform_module_name = arg.substr(i, next_comma - i);
        auto             module_path          = std::filesystem::path(input_file).parent_path() / (std::string(platform_module_name) + std::string(".xml"));

        g.ParseFile(module_path);
        g.WriteModuleAPI();
        g.WriteModuleAPIDoc();
        if (next_comma == std::string_view::npos) {
            break;
        }
        i = next_comma + 1;
    }

    FormatFiles(g.GetFiles());
    return 0;
}

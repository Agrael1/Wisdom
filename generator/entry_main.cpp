#include "generator.hpp"
#include <iostream>
#include "../src/include/wisdom/bridge/format.hpp"

inline constexpr std::string_view input_file = INPUT_FILE;
inline constexpr std::string_view platform_file = PLATFORM_FILE;

constexpr inline std::string_view clang_format_exe = CLANG_FORMAT_EXECUTABLE;
void                              FormatFiles(std::span<const std::filesystem::path> files)
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

int main()
{
    Generator g;
    g.ParseFile(input_file);
    g.WriteMainAPI();
    g.WriteModuleAPIDoc();


    g.ParsePlatformFile(platform_file);
    g.WritePlatformAPI();
    g.WriteModuleAPIDoc();

    FormatFiles(g.GetFiles());
    return 0;
}

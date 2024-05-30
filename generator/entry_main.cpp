#include "generator.h"
#include <iostream>
#include "../wisdom/include/wisdom/bridge/format.h"

inline constexpr std::string_view input_file = INPUT_FILE;

constexpr inline std::string_view clang_format_exe = CLANG_FORMAT_EXECUTABLE;
void FormatFiles(std::span<const std::filesystem::path> files)
{
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
    if (int ret = std::system(command.c_str()); ret != 0) {
        std::cout << "Wisdom Vk Utils: failed to format files with error <" << ret << ">\n";
    }
}

int main()
{

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(input_file.data()) != tinyxml2::XMLError::XML_SUCCESS)
        return 1;

    Generator g(doc);
    g.GenerateCAPI();
    g.GenerateCPPAPI();
    FormatFiles(g.GetFiles());

    return 0;
}

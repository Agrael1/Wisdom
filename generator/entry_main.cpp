#include <iostream>
#include "generator.hpp"

constexpr inline std::string_view clang_format_exe = CLANG_FORMAT_EXECUTABLE;

//--------------------------------------------------------------
struct Args {
    std::filesystem::path input_directory = Generator::default_input_dir;
    std::string formatter{clang_format_exe};
    std::filesystem::path clang_format_file;
    std::filesystem::path core_module{"wis.xml"};
    std::vector<std::string> modules;
};

//--------------------------------------------------------------
static void PrintUsage(std::string_view executable)
{
    std::cout << "Usage: " << executable
              << " [module_name[,module_name...]] [options]\n"
                 "Generate the core API when no module name is provided; otherwise generate each named module.\n"
                 "Options:\n"
                 "  -wd, --work-directory <path>        Directory containing wis.xml and module XML files\n"
                 "  -f,  --formatter <path>             clang-format executable to use\n"
                 "  -cf, --clang-format-file <path>     clang-format configuration file\n"
                 "  -c,  --core                         Set core file\n"
                 "\n  -h,  --help                         Show this help\n";
}

//--------------------------------------------------------------
template <typename T>
int static ReadValue(
    std::span<char*> argv,
    int i,
    std::string_view option,
    T& out_value,
    std::size_t eq_offset = std::string_view::npos
)
{
    std::string_view arg = argv[i];
    if (arg.size() > eq_offset) {
        if (arg[eq_offset] != '=') {
            std::cerr << "Missing value for " << option << ".\n";
            PrintUsage(argv[0]);
            return -1;
        }
        out_value = arg.substr(eq_offset + 1);
        return 0;
    }

    if (++i >= argv.size()) {
        std::cerr << "Missing value for " << option << ".\n";
        PrintUsage(argv[0]);
        return -1;
    }
    out_value = argv[i];
    return 1;
}

//--------------------------------------------------------------
static int ReadLongArg(std::span<char*> argv, int i, Args& out_args)
{
    std::string_view arg = argv[i];
    switch (arg[2]) {
    case 'w':
        if (arg.starts_with("--work-directory")) {
            return ReadValue(argv, i, arg, out_args.input_directory, 16);
        }
        break;
    case 'f':
        if (arg.starts_with("--formatter")) {
            return ReadValue(argv, i, arg, out_args.formatter, 11);
        }
        break;
    case 'c':
        if (arg.starts_with("--clang-format-file")) {
            return ReadValue(argv, i, arg, out_args.clang_format_file, 19);
        } else if (arg.starts_with("--core")) {
            return ReadValue(argv, i, arg, out_args.core_module, 6);
        }
        break;
    case 'h':
    default:
        std::cerr << "Invalid argument: " << arg << '\n';
        break;
    }

    // Either help or unknown option. Print usage and return error code.
    PrintUsage(argv[0]);
    return -1;
}

//--------------------------------------------------------------
static int ReadShortArg(std::span<char*> argv, int i, Args& out_args)
{
    // assume offset == 1
    std::string_view arg = argv[i];
    switch (arg[1]) {
    case 'w':
        if (arg.starts_with("-wd")) {
            return ReadValue(argv, i, arg, out_args.input_directory, 3);
        }
        break;
    case 'f':
        if (arg.starts_with("-f")) {
            return ReadValue(argv, i, arg, out_args.formatter, 2);
        }
        break;
    case 'c':
        if (arg.starts_with("-cf")) {
            return ReadValue(argv, i, arg, out_args.clang_format_file, 3);
        } else if (arg.starts_with("-c")) {
            return ReadValue(argv, i, arg, out_args.core_module, 2);
        }
        break;
    case '-':
        return ReadLongArg(argv, i, out_args);
    case 'h':
    default:
        std::cerr << "Invalid argument: " << arg << '\n';
        break;
    }

    // Either help or unknown option. Print usage and return error code.
    PrintUsage(argv[0]);
    return -1;
}

//--------------------------------------------------------------
static int ReadArg(std::span<char*> argv, int i, Args& out_args)
{
    std::string_view arg = argv[i];
    switch (arg[0]) {
    case '-':
        if (arg.size() == 1) {
            std::cerr << "Invalid argument: " << arg << '\n';
            PrintUsage(argv[0]);
            return -1;
        }
        return ReadShortArg(argv, i, out_args);
    default:
        out_args.modules.emplace_back(arg);
        break;
    }

    // The argument is not an option, return it as a module name
    return 0;
}

//--------------------------------------------------------------
static std::pair<int, Args> ParseArgs(std::span<char*> argv)
{
    std::pair<int, Args> result{0, {}};
    for (int i = 1; i < argv.size(); ++i) {
        std::string_view arg = argv[i];

        auto added_i = ReadArg(argv, i, result.second);
        if (added_i < 0) {
            result.first = -1;
            return result;
        }

        i += added_i;
    }
    return result;
}

//--------------------------------------------------------------
static void FormatFiles(
    std::span<const std::filesystem::path> files,
    std::string_view formatter,
    std::string_view clang_format_file
)
{
    constexpr uint32_t repeats = 5;
    if (formatter.empty()) {
        return;
    }

    // break into chunks of 16 files to avoid command line length limits on some platforms
    for (size_t i = 0; i < files.size(); i += 16) {
        auto chunk_end = std::min<size_t>(16, files.size() - i);

        std::string cmd;
        for (auto f : files.subspan(i, chunk_end)) {
            cmd += std::format("\"{}\"", f.string());
            cmd += ' ';
        }
        std::cout << "Wisdom Vk Utils: Formatting:\n" << cmd << '\n';
        std::string style = !clang_format_file.empty() ? std::format("--style=file:{}", clang_format_file)
                                                       : "--style=file";
        std::string command = std::format("\"{}\" -i \"{}\" {}", formatter, style, cmd);

        int ret = 0;
        for (uint32_t i = 0; (ret = std::system(command.c_str())) != 0 && i < repeats; ++i)
            ;
        if (ret != 0) {
            std::cout << "Wisdom Vk Utils: failed to format files with error <" << ret << ">\n";
        }
    }
}

//--------------------------------------------------------------
int main(int argc, char** argv)
{
    auto [rc, args] = ParseArgs({argv, std::size_t(argc)});
    if (rc < 0) {
        return 0;
    }

    Generator g(args.input_directory);
    auto core_file = args.core_module.is_absolute() ? args.core_module : args.input_directory / args.core_module;

    // Load core module first to ensure all types are available for platform modules
    g.ParseFile(core_file);

    if (args.modules.empty()) {
        g.WriteModuleAPI();
        g.WriteModuleAPIDoc();
    } else {
        for (const auto& module_name : args.modules) {
            auto module_path = args.input_directory / (module_name + ".xml");
            g.ParseFile(module_path);
            g.WriteModuleAPI();
            g.WriteModuleAPIDoc();
        }
    }

    FormatFiles(g.GetFiles(), args.formatter, args.clang_format_file.string());
    return 0;
}

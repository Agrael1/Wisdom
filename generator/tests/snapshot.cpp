#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

struct Fingerprint {
    std::uint64_t hash = 14695981039346656037ull;
    std::uint64_t size = 0;

    bool operator==(const Fingerprint&) const = default;
};

using Manifest = std::map<std::string, Fingerprint>;

static Fingerprint ReadFingerprint(const fs::path& path)
{
    std::ifstream file{path};
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read generated file: " + path.string());
    }

    // FNV-1a over native text streams keeps snapshots independent of text-mode file I/O on each platform.
    Fingerprint result;
    char ch;
    while (file.get(ch)) {
        result.hash ^= static_cast<unsigned char>(ch);
        result.hash *= 1099511628211ull;
        ++result.size;
    }
    if (!file.eof()) {
        throw std::runtime_error("Failed reading generated file: " + path.string());
    }
    return result;
}

static Manifest ReadOutput(const fs::path& root)
{
    if (!fs::is_directory(root)) {
        throw std::runtime_error("Missing output directory: " + root.string());
    }
    Manifest result;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_regular_file()) {
            result.emplace(entry.path().lexically_relative(root).generic_string(), ReadFingerprint(entry.path()));
        }
    }
    if (result.empty()) {
        throw std::runtime_error("No generated files in: " + root.string());
    }
    return result;
}

static Manifest ReadSnapshot(const fs::path& path)
{
    std::ifstream file{path};
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read snapshot: " + path.string());
    }
    Manifest result;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream fields{line};
        Fingerprint fingerprint;
        std::string name;
        if (!(fields >> std::hex >> fingerprint.hash >> std::dec >> fingerprint.size >> std::ws)
            || !std::getline(fields, name) || name.empty() || !result.emplace(name, fingerprint).second) {
            throw std::runtime_error("Invalid snapshot entry in " + path.string() + ": " + line);
        }
    }
    if (!file.eof() || result.empty()) {
        throw std::runtime_error("Empty or unreadable snapshot: " + path.string());
    }
    return result;
}

static void WriteSnapshot(const fs::path& path, const Manifest& manifest)
{
    fs::create_directories(path.parent_path());
    std::ofstream file{path, std::ios::trunc};
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write snapshot: " + path.string());
    }
    for (const auto& [name, fingerprint] : manifest) {
        file << std::hex << std::setfill('0') << std::setw(16) << fingerprint.hash << std::dec << ' '
             << fingerprint.size << ' ' << name << '\n';
    }
    file.close();
    if (!file) {
        throw std::runtime_error("Failed writing snapshot: " + path.string());
    }
    std::cout << "Updated " << path << " (" << manifest.size() << " files)\n";
}

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4 || (argc == 4 && std::string_view(argv[3]) != "--update")) {
        std::cerr << "Usage: generator-snapshot <output-directory> <snapshot> [--update]\n";
        return 1;
    }
    try {
        const auto actual = ReadOutput(argv[1]);
        if (argc == 4) {
            WriteSnapshot(argv[2], actual);
            return 0;
        }
        const auto expected = ReadSnapshot(argv[2]);
        bool matches = true;
        for (const auto& [name, fingerprint] : expected) {
            auto it = actual.find(name);
            if (it == actual.end()) {
                std::cerr << "Missing output: " << name << '\n';
                matches = false;
            } else if (it->second != fingerprint) {
                std::cerr << "Changed output: " << name << " (expected " << std::hex << fingerprint.hash
                          << ", actual " << it->second.hash << std::dec << ")\n";
                matches = false;
            }
        }
        for (const auto& [name, fingerprint] : actual) {
            if (!expected.contains(name)) {
                std::cerr << "Unexpected output: " << name << '\n';
                matches = false;
            }
        }
        return matches ? 0 : 1;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
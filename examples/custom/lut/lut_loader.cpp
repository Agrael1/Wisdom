#include "lut_loader.h"
#include <fstream>
#include <charconv>

std::string_view TrimWhitespace(std::string_view str)
{
    while (!str.empty() && std::isspace(str.front()))
        str.remove_prefix(1);
    while (!str.empty() && std::isspace(str.back()))
        str.remove_suffix(1);
    return str;
}

std::pair<LutData, size_t> ParseHeader(std::string_view text)
{
    bool terminated = false;
    LutData data;
    size_t offset = 0;
    while (offset < text.size() && !terminated) {
        auto newline = text.find('\n', offset);
        if (newline == std::string::npos)
            return { std::move(data), offset };

        std::string_view line{ text.data() + offset, newline - offset };
        line = TrimWhitespace(line);

        if (line.starts_with("LUT_1D_SIZE")) {
            line.remove_prefix(11);

            auto xline = TrimWhitespace(line);

            // use charconv

            uint32_t count;
            auto err = std::from_chars(xline.data(), xline.data() + xline.size(), count);

            if (err.ec != std::errc())
                return { std::move(data), offset };

            data.stride = count;
            data.type = LutType::Lut1D;
        } else if (line.starts_with("LUT_3D_SIZE")) {
            line.remove_prefix(11);

            auto xline = TrimWhitespace(line);

            // use charconv

            uint32_t count;
            auto err = std::from_chars(xline.data(), xline.data() + xline.size(), count);

            if (err.ec != std::errc())
                return { std::move(data), offset };

            data.stride = count;
            data.type = LutType::Lut3D;
        } else if (line.starts_with("LUT_3D_INPUT_RANGE") || line.starts_with("LUT_1D_INPUT_RANGE")) {
            terminated = true;
        }

        if (!line.empty() && isdigit(line[0]))
            return { std::move(data), offset };

        offset = newline + 1;
    }
    return { std::move(data), offset };
}

std::unique_ptr<float[]> ParseLutData(std::string_view text, size_t count)
{
    std::unique_ptr<float[]> data = std::make_unique_for_overwrite<float[]>(count);

    auto tr_text = TrimWhitespace(text);
    for (size_t i = 0; i < count; i++) {
        float value;
        auto err = std::from_chars(tr_text.data(), tr_text.data() + tr_text.size(), value);

        data[i] = value;

        tr_text = TrimWhitespace(tr_text.substr(tr_text.find_first_of(" \n") + 1));
    }

    return data;
}

LutData LutLoader::LoadLut(std::filesystem::path path)
{
    if (!std::filesystem::exists(path))
        return {};

    // regular text file
    std::ifstream file(path);
    if (!file.is_open())
        return {};

    // load entire file into memory
    std::string text;
    file.seekg(0, std::ios::end);
    text.resize(file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(text.data(), text.size());
    file.close();

    // parse the file using string_view by line
    auto [data, offset] = ParseHeader(text);

    data.data = ParseLutData(std::string_view(text.data() + offset, text.size() - offset),
                             data.type == LutType::Lut1D ? data.stride * 3 : data.stride * data.stride * data.stride * 3);

    return std::move(data);
}

#pragma once
#include <string_view>
#include <memory>
#include <filesystem>

enum class LutType {
    Undefined,
    Lut1D,
    Lut3D,
};

// assume equal dimensions for 3D LUT
struct LutData {
    std::unique_ptr<float[]> data;
    size_t stride = 0;
    LutType type = LutType::Undefined;
};

class LutLoader
{
public:
    /**
     * Parses the LUT file. Logging the file information.
     * @param fileName Path to LUT file, including file name.
     * @throws When input file does not exist or contained data are not valid.
     * @return LUT data.
     */
    static LutData LoadLut(std::filesystem::path path);
};

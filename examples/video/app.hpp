#pragma once
#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>
#include <sdl_backend_cpp.h>
#include "graphics.hpp"
#include <span>
#include <cstdint>

struct NalUnit {
    uint32_t nal_unit_type;
    uint32_t temporal_id;
    std::vector<uint8_t> data;
};

class App
{
public:
    App(std::vector<uint8_t> file_data, std::string_view path)
        : file_data(std::move(file_data))
        , file_path(path)
    {}

public:
    std::span<const std::byte> GetHvcC();
    int Start();

private:
    std::vector<uint8_t> file_data;
    std::string_view file_path;
    SDLPlatformCpp platform;
    
    // Parsed bitstream data
    std::vector<NalUnit> nal_units;
    uint8_t length_size_minus_one = 3;
    std::span<const std::byte> mdat_payload;
};

std::optional<App> CreateApp(std::string_view video_path);

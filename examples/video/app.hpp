#pragma once
#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>
#include <sdl_backend_cpp.h>
#include "graphics.hpp"
#include <span>

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
};

std::optional<App> CreateApp(std::string_view video_path);

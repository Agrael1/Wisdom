#include "app.hpp"
#include "h265_common.h"
#include "h265_configuration_box_parser.h"
#include "mbmff.hpp"

std::span<const std::byte> App::GetHvcC()
{
    auto data = std::as_bytes(std::span(file_data));
    std::span<const std::byte> hvcC_payload;
    mbmff::hvcC_data hvcC{};

    mbmff::recursive_box_iterator it(data);
    for (; it != it.end(); ++it) {
        auto box_result = *it;
        if (!box_result || box_result->type() != mbmff::box_type::hvcC) {
            continue;
        }
        hvcC_payload = box_result->payload;
        auto hvcC_box = mbmff::box_cast<mbmff::box_type::hvcC>(*box_result);
        hvcC = hvcC_box.value();
        break;
    }

    if (hvcC.configuration_version != 1) {
        std::printf("No hvcC box found in '%s'\n", file_path.data());
        return hvcC_payload;
    }

    std::printf("Found hvcC box:\n");
    std::printf("  Configuration version: %u\n", hvcC.configuration_version);
    std::printf("  Profile space: %u\n", hvcC.general_profile_space);
    std::printf("  Tier flag: %u\n", hvcC.general_tier_flag);
    std::printf("  Profile IDC: %u\n", hvcC.general_profile_idc);
    std::printf("  Level IDC: %u\n", hvcC.general_level_idc);
    std::printf("  Chroma format: %u\n", hvcC.chroma_format);
    std::printf("  Bit depth luma: %u\n", hvcC.bit_depth_luma_minus8 + 8);
    std::printf("  Bit depth chroma: %u\n", hvcC.bit_depth_chroma_minus8 + 8);
    std::printf("  NAL length size: %u\n", hvcC.length_size_minus_one + 1);
    std::printf("  Number of NAL arrays: %u\n\n", hvcC.num_arrays);
    return hvcC_payload;
}

int App::Start()
{
    auto hvcC_payload = GetHvcC();
    h265nal::H265BitstreamParserState parser_state;
    auto config = h265nal::H265ConfigurationBoxParser::ParseConfigurationBox(
        reinterpret_cast<const uint8_t*>(hvcC_payload.data()),
        hvcC_payload.size(),
        &parser_state,
        h265nal::ParsingOptions{}
    );

    if (!config) {
        std::printf("Failed to parse hvcC with h265nal\n");
        return -1;
    }

    uint32_t width = 0, height = 0;
    uint32_t bit_depth = 8;
    uint32_t chroma_idc = 1;
    uint32_t profile_idc = config->general_level_idc;

    for (uint32_t i = 0; i < config->numOfArrays; i++) {
        for (uint32_t j = 0; j < config->numNalus[i]; j++) {
            auto& nalu = config->nalUnit[i][j];
            if (!nalu) {
                continue;
            }
            auto& pl = nalu->nal_unit_payload;
            if (pl->sps) {
                auto& sps = pl->sps;
                int w = 0, h = 0;
                sps->getResolution(&w, &h);
                width = static_cast<uint32_t>(w);
                height = static_cast<uint32_t>(h);
                chroma_idc = sps->chroma_format_idc;
                bit_depth = sps->bit_depth_luma_minus8 + 8;
            }
        }
    }

    if (width == 0 || height == 0) {
        std::printf("Failed to extract resolution from SPS\n");
        return -1;
    }

    std::printf(
        "Video parameters: %ux%u, chroma=%u, bitdepth=%u, profile=%u\n\n",
        width,
        height,
        chroma_idc,
        bit_depth,
        profile_idc
    );

    wis::StdCodecProfile codec_profile = wis::StdCodecProfile::H265Main;
    switch (profile_idc) {
    case 1:
        codec_profile = bit_depth > 8 ? wis::StdCodecProfile::H265Main10 : wis::StdCodecProfile::H265Main;
        break;
    case 2:
        codec_profile = wis::StdCodecProfile::H265Main10;
        break;
    case 3:
        codec_profile = wis::StdCodecProfile::H265Main;
        break;
    case 4:
        codec_profile = wis::StdCodecProfile::H265FormatRangeExt;
        break;
    default:
        break;
    }

    wis::DataFormat output_format = wis::DataFormat::NV12;
    if (chroma_idc == 1) {
        output_format = bit_depth > 8 ? wis::DataFormat::P010 : wis::DataFormat::NV12;
    }

    auto graphics = Graphics::Create(codec_profile, output_format, width, height);
    if (!graphics) {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Wisdom Hello Triangle C++", 800, 600, 0);
    if (!window) {
        return -1;
    }
    
    graphics->Frame();


    SDL_DestroyWindow(window);

    return 0;
}

std::optional<App> CreateApp(std::string_view video_path)
{
    std::optional<App> out;
    std::ifstream file(video_path.data(), std::ios::binary);
    if (!file) {
        std::printf("Failed to open file: %s\n", video_path.data());
        return out;
    }

    std::vector<uint8_t> file_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::printf("Loaded %zu bytes from '%s'\n", file_data.size(), video_path.data());

    out.emplace(std::move(file_data), video_path);
    return out;
}

#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include "h265_common.h"
#include "h265_configuration_box_parser.h"
#include "mbmff.hpp"

int main(int argc, char* argv[])
{
    const char* path = argc > 1 ? argv[1] : "assets/hevc_sample.heic";

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::printf("Failed to open file: %s\n", path);
        return -1;
    }

    std::vector<uint8_t> file_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto data = std::as_bytes(std::span(file_data));
    std::printf("Loaded %zu bytes from '%s'\n", data.size(), path);

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
        std::printf("No hvcC box found in '%s'\n", path);
        return -1;
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

    std::printf("h265nal parsed %u arrays:\n", config->numOfArrays);
    for (uint32_t i = 0; i < config->numOfArrays; i++) {
        auto type_name = h265nal::NalUnitTypeToString(config->NAL_unit_type[i]);
        std::printf(
            "  Array[%u]: type=%s(%u), completeness=%u, count=%u\n",
            i,
            type_name.c_str(),
            config->NAL_unit_type[i],
            config->array_completeness[i],
            config->numNalus[i]
        );

        for (uint32_t j = 0; j < config->numNalus[j]; j++) {
            auto& nalu = config->nalUnit[i][j];
            if (!nalu) {
                continue;
            }
            auto& hdr = nalu->nal_unit_header;
            auto& pl = nalu->nal_unit_payload;

            std::printf(
                "    NAL[%u]: type=%s(%u), layer=%u, tid=%u\n",
                j,
                h265nal::NalUnitTypeToString(hdr->nal_unit_type).c_str(),
                hdr->nal_unit_type,
                hdr->nuh_layer_id,
                hdr->nuh_temporal_id_plus1 - 1
            );

            if (pl->vps) {
                auto& vps = pl->vps;
                std::printf(
                    "      VPS id=%u, layers=%u, sublayers=%u\n",
                    vps->vps_video_parameter_set_id,
                    vps->vps_max_layers_minus1 + 1,
                    vps->vps_max_sub_layers_minus1 + 1
                );
            }
            if (pl->sps) {
                auto& sps = pl->sps;
                int w = 0, h = 0;
                sps->getResolution(&w, &h);
                std::printf(
                    "      SPS id=%u, %dx%d, chroma=%u, bitdepth=%u\n",
                    sps->sps_seq_parameter_set_id,
                    w,
                    h,
                    sps->chroma_format_idc,
                    sps->bit_depth_luma_minus8 + 8
                );
            }
            if (pl->pps) {
                auto& pps = pl->pps;
                std::printf(
                    "      PPS id=%u, sps=%u, tiles=(%u,%u)\n",
                    pps->pps_pic_parameter_set_id,
                    pps->pps_seq_parameter_set_id,
                    pps->num_tile_columns_minus1 + 1,
                    pps->num_tile_rows_minus1 + 1
                );
            }
        }
    }

    std::printf("\nSPS map (%zu entries):\n", parser_state.sps.size());
    for (auto& [id, sps] : parser_state.sps) {
        int w = 0, h = 0;
        sps->getResolution(&w, &h);
        std::printf("  SPS[%u]: %dx%d\n", id, w, h);
    }

    return 0;
}

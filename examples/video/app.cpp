#include "app.hpp"
#include "h265_common.h"
#include "h265_configuration_box_parser.h"
#include "mbmff.hpp"

#include <video/generated/cpp_api.hpp>
#include <map>

// ---------------------------------------------------------------------------
// Convert parsed h265nal state to wisdom StdVideoH265 types
// ---------------------------------------------------------------------------
struct ConvertedH265Params {
    wis::VideoDecodeH265Desc desc{};

    std::vector<wis::StdVideoH265VideoParameterSet> vps;
    std::vector<wis::StdVideoH265SequenceParameterSet> sps;
    std::vector<wis::StdVideoH265PictureParameterSet> pps;
    std::vector<wis::StdVideoH265ProfileTierLevel> vps_ptl;
    std::vector<wis::StdVideoH265ProfileTierLevel> sps_ptl;

    std::map<uint32_t, uint32_t> sps_to_vps; // sps_id → vps_id
};

static void ConvertVps(wis::StdVideoH265VideoParameterSet& out,
                        wis::StdVideoH265ProfileTierLevel& ptl,
                        const h265nal::H265VpsParser::VpsState& in)
{
    out.flags.vps_temporal_id_nesting_flag = in.vps_temporal_id_nesting_flag;
    out.flags.vps_sub_layer_ordering_info_present_flag = in.vps_sub_layer_ordering_info_present_flag;
    out.flags.vps_timing_info_present_flag = in.vps_timing_info_present_flag;
    out.flags.vps_poc_proportional_to_timing_flag = in.vps_poc_proportional_to_timing_flag;

    out.vps_video_parameter_set_id = static_cast<uint8_t>(in.vps_video_parameter_set_id);
    out.vps_max_sub_layers_minus1 = static_cast<uint8_t>(in.vps_max_sub_layers_minus1);
    out.vps_num_units_in_tick = in.vps_num_units_in_tick;
    out.vps_time_scale = in.vps_time_scale;
    out.vps_num_ticks_poc_diff_one_minus1 = in.vps_num_ticks_poc_diff_one_minus1;

    if (in.profile_tier_level && in.profile_tier_level->general) {
        auto& g = *in.profile_tier_level->general;
        ptl.flags.general_tier_flag = g.tier_flag;
        ptl.flags.general_progressive_source_flag = g.progressive_source_flag;
        ptl.flags.general_interlaced_source_flag = g.interlaced_source_flag;
        ptl.flags.general_non_packed_constraint_flag = g.non_packed_constraint_flag;
        ptl.flags.general_frame_only_constraint_flag = g.frame_only_constraint_flag;
        ptl.general_profile_idc = static_cast<wis::StdVideoH265ProfileIdc>(g.profile_idc);
        ptl.general_level_idc = static_cast<wis::StdVideoH265LevelIdc>(in.profile_tier_level->general_level_idc);
    }
    out.pProfileTierLevel = &ptl;
}

static void ConvertSps(wis::StdVideoH265SequenceParameterSet& out,
                        wis::StdVideoH265ProfileTierLevel& ptl,
                        const h265nal::H265SpsParser::SpsState& in)
{
    out.flags.sps_temporal_id_nesting_flag = in.sps_temporal_id_nesting_flag;
    out.flags.separate_colour_plane_flag = in.separate_colour_plane_flag;
    out.flags.conformance_window_flag = in.conformance_window_flag;
    out.flags.sps_sub_layer_ordering_info_present_flag = in.sps_sub_layer_ordering_info_present_flag;
    out.flags.scaling_list_enabled_flag = in.scaling_list_enabled_flag;
    out.flags.sps_scaling_list_data_present_flag = in.sps_scaling_list_data_present_flag;
    out.flags.amp_enabled_flag = in.amp_enabled_flag;
    out.flags.sample_adaptive_offset_enabled_flag = in.sample_adaptive_offset_enabled_flag;
    out.flags.pcm_enabled_flag = in.pcm_enabled_flag;
    out.flags.pcm_loop_filter_disabled_flag = in.pcm_loop_filter_disabled_flag;
    out.flags.long_term_ref_pics_present_flag = in.long_term_ref_pics_present_flag;
    out.flags.sps_temporal_mvp_enabled_flag = in.sps_temporal_mvp_enabled_flag;
    out.flags.strong_intra_smoothing_enabled_flag = in.strong_intra_smoothing_enabled_flag;
    out.flags.vui_parameters_present_flag = in.vui_parameters_present_flag;
    out.flags.sps_extension_present_flag = in.sps_extension_present_flag;
    out.flags.sps_range_extension_flag = in.sps_range_extension_flag;

    if (in.sps_range_extension) {
        auto& re = *in.sps_range_extension;
        out.flags.transform_skip_rotation_enabled_flag = re.transform_skip_rotation_enabled_flag;
        out.flags.transform_skip_context_enabled_flag = re.transform_skip_context_enabled_flag;
        out.flags.implicit_rdpcm_enabled_flag = re.implicit_rdpcm_enabled_flag;
        out.flags.explicit_rdpcm_enabled_flag = re.explicit_rdpcm_enabled_flag;
        out.flags.extended_precision_processing_flag = re.extended_precision_processing_flag;
        out.flags.intra_smoothing_disabled_flag = re.intra_smoothing_disabled_flag;
        out.flags.high_precision_offsets_enabled_flag = re.high_precision_offsets_enabled_flag;
        out.flags.persistent_rice_adaptation_enabled_flag = re.persistent_rice_adaptation_enabled_flag;
        out.flags.cabac_bypass_alignment_enabled_flag = re.cabac_bypass_alignment_enabled_flag;
    }

    out.flags.sps_scc_extension_flag = in.sps_scc_extension_flag;
    if (in.sps_scc_extension) {
        auto& scc = *in.sps_scc_extension;
        out.flags.sps_curr_pic_ref_enabled_flag = scc.sps_curr_pic_ref_enabled_flag;
        out.flags.palette_mode_enabled_flag = scc.palette_mode_enabled_flag;
        out.flags.sps_palette_predictor_initializers_present_flag = scc.sps_palette_predictor_initializers_present_flag;
        out.flags.intra_boundary_filtering_disabled_flag = scc.intra_boundary_filtering_disabled_flag;
        out.palette_max_size = static_cast<uint8_t>(scc.palette_max_size);
        out.delta_palette_max_predictor_size = static_cast<uint8_t>(scc.delta_palette_max_predictor_size);
        out.motion_vector_resolution_control_idc = static_cast<uint8_t>(scc.motion_vector_resolution_control_idc);
        out.sps_num_palette_predictor_initializers_minus1 = static_cast<uint8_t>(scc.sps_num_palette_predictor_initializers_minus1);
    }

    out.chroma_format_idc = static_cast<wis::StdVideoH265ChromaFormatIdc>(in.chroma_format_idc);
    out.pic_width_in_luma_samples = in.pic_width_in_luma_samples;
    out.pic_height_in_luma_samples = in.pic_height_in_luma_samples;
    out.sps_video_parameter_set_id = static_cast<uint8_t>(in.sps_video_parameter_set_id);
    out.sps_max_sub_layers_minus1 = static_cast<uint8_t>(in.sps_max_sub_layers_minus1);
    out.sps_seq_parameter_set_id = static_cast<uint8_t>(in.sps_seq_parameter_set_id);
    out.bit_depth_luma_minus8 = static_cast<uint8_t>(in.bit_depth_luma_minus8);
    out.bit_depth_chroma_minus8 = static_cast<uint8_t>(in.bit_depth_chroma_minus8);
    out.log2_max_pic_order_cnt_lsb_minus4 = static_cast<uint8_t>(in.log2_max_pic_order_cnt_lsb_minus4);
    out.log2_min_luma_coding_block_size_minus3 = static_cast<uint8_t>(in.log2_min_luma_coding_block_size_minus3);
    out.log2_diff_max_min_luma_coding_block_size = static_cast<uint8_t>(in.log2_diff_max_min_luma_coding_block_size);
    out.log2_min_luma_transform_block_size_minus2 = static_cast<uint8_t>(in.log2_min_luma_transform_block_size_minus2);
    out.log2_diff_max_min_luma_transform_block_size = static_cast<uint8_t>(in.log2_diff_max_min_luma_transform_block_size);
    out.max_transform_hierarchy_depth_inter = static_cast<uint8_t>(in.max_transform_hierarchy_depth_inter);
    out.max_transform_hierarchy_depth_intra = static_cast<uint8_t>(in.max_transform_hierarchy_depth_intra);
    out.num_short_term_ref_pic_sets = static_cast<uint8_t>(in.num_short_term_ref_pic_sets);
    out.num_long_term_ref_pics_sps = static_cast<uint8_t>(in.num_long_term_ref_pics_sps);
    out.pcm_sample_bit_depth_luma_minus1 = static_cast<uint8_t>(in.pcm_sample_bit_depth_luma_minus1);
    out.pcm_sample_bit_depth_chroma_minus1 = static_cast<uint8_t>(in.pcm_sample_bit_depth_chroma_minus1);
    out.log2_min_pcm_luma_coding_block_size_minus3 = static_cast<uint8_t>(in.log2_min_pcm_luma_coding_block_size_minus3);
    out.log2_diff_max_min_pcm_luma_coding_block_size = static_cast<uint8_t>(in.log2_diff_max_min_pcm_luma_coding_block_size);
    out.conf_win_left_offset = in.conf_win_left_offset;
    out.conf_win_right_offset = in.conf_win_right_offset;
    out.conf_win_top_offset = in.conf_win_top_offset;
    out.conf_win_bottom_offset = in.conf_win_bottom_offset;

    if (in.profile_tier_level && in.profile_tier_level->general) {
        auto& g = *in.profile_tier_level->general;
        ptl.flags.general_tier_flag = g.tier_flag;
        ptl.flags.general_progressive_source_flag = g.progressive_source_flag;
        ptl.flags.general_interlaced_source_flag = g.interlaced_source_flag;
        ptl.flags.general_non_packed_constraint_flag = g.non_packed_constraint_flag;
        ptl.flags.general_frame_only_constraint_flag = g.frame_only_constraint_flag;
        ptl.general_profile_idc = static_cast<wis::StdVideoH265ProfileIdc>(g.profile_idc);
        ptl.general_level_idc = static_cast<wis::StdVideoH265LevelIdc>(in.profile_tier_level->general_level_idc);
    }
    out.pProfileTierLevel = &ptl;
}

static void ConvertPps(wis::StdVideoH265PictureParameterSet& out,
                        const h265nal::H265PpsParser::PpsState& in,
                        uint32_t sps_vps_id)
{
    out.flags.dependent_slice_segments_enabled_flag = in.dependent_slice_segments_enabled_flag;
    out.flags.output_flag_present_flag = in.output_flag_present_flag;
    out.flags.sign_data_hiding_enabled_flag = in.sign_data_hiding_enabled_flag;
    out.flags.cabac_init_present_flag = in.cabac_init_present_flag;
    out.flags.constrained_intra_pred_flag = in.constrained_intra_pred_flag;
    out.flags.transform_skip_enabled_flag = in.transform_skip_enabled_flag;
    out.flags.cu_qp_delta_enabled_flag = in.cu_qp_delta_enabled_flag;
    out.flags.pps_slice_chroma_qp_offsets_present_flag = in.pps_slice_chroma_qp_offsets_present_flag;
    out.flags.weighted_pred_flag = in.weighted_pred_flag;
    out.flags.weighted_bipred_flag = in.weighted_bipred_flag;
    out.flags.transquant_bypass_enabled_flag = in.transquant_bypass_enabled_flag;
    out.flags.tiles_enabled_flag = in.tiles_enabled_flag;
    out.flags.entropy_coding_sync_enabled_flag = in.entropy_coding_sync_enabled_flag;
    out.flags.uniform_spacing_flag = in.uniform_spacing_flag;
    out.flags.loop_filter_across_tiles_enabled_flag = in.loop_filter_across_tiles_enabled_flag;
    out.flags.pps_loop_filter_across_slices_enabled_flag = in.pps_loop_filter_across_slices_enabled_flag;
    out.flags.deblocking_filter_control_present_flag = in.deblocking_filter_control_present_flag;
    out.flags.deblocking_filter_override_enabled_flag = in.deblocking_filter_override_enabled_flag;
    out.flags.pps_deblocking_filter_disabled_flag = in.pps_deblocking_filter_disabled_flag;
    out.flags.pps_scaling_list_data_present_flag = in.pps_scaling_list_data_present_flag;
    out.flags.lists_modification_present_flag = in.lists_modification_present_flag;
    out.flags.slice_segment_header_extension_present_flag = in.slice_segment_header_extension_present_flag;
    out.flags.pps_extension_present_flag = in.pps_extension_present_flag;
    out.flags.pps_range_extension_flag = in.pps_range_extension_flag;

    if (in.pps_scc_extension) {
        auto& scc = *in.pps_scc_extension;
        out.flags.pps_curr_pic_ref_enabled_flag = scc.pps_curr_pic_ref_enabled_flag;
        out.flags.residual_adaptive_colour_transform_enabled_flag = scc.residual_adaptive_colour_transform_enabled_flag;
        out.flags.pps_slice_act_qp_offsets_present_flag = scc.pps_slice_act_qp_offsets_present_flag;
        out.flags.pps_palette_predictor_initializers_present_flag = scc.pps_palette_predictor_initializer_present_flag;
        out.flags.monochrome_palette_flag = scc.monochrome_palette_flag;
        out.pps_act_y_qp_offset_plus5 = static_cast<int8_t>(scc.pps_act_y_qp_offset_plus5);
        out.pps_act_cb_qp_offset_plus5 = static_cast<int8_t>(scc.pps_act_cb_qp_offset_plus5);
        out.pps_act_cr_qp_offset_plus3 = static_cast<int8_t>(scc.pps_act_cr_qp_offset_plus3);
        out.pps_num_palette_predictor_initializers = static_cast<uint8_t>(scc.pps_num_palette_predictor_initializer);
        out.luma_bit_depth_entry_minus8 = static_cast<uint8_t>(scc.luma_bit_depth_entry_minus8);
        out.chroma_bit_depth_entry_minus8 = static_cast<uint8_t>(scc.chroma_bit_depth_entry_minus8);
    }

    out.pps_pic_parameter_set_id = static_cast<uint8_t>(in.pps_pic_parameter_set_id);
    out.pps_seq_parameter_set_id = static_cast<uint8_t>(in.pps_seq_parameter_set_id);
    out.sps_video_parameter_set_id = static_cast<uint8_t>(sps_vps_id);
    out.num_extra_slice_header_bits = in.num_extra_slice_header_bits;
    out.num_ref_idx_l0_default_active_minus1 = static_cast<uint8_t>(in.num_ref_idx_l0_default_active_minus1);
    out.num_ref_idx_l1_default_active_minus1 = static_cast<uint8_t>(in.num_ref_idx_l1_default_active_minus1);
    out.init_qp_minus26 = static_cast<int8_t>(in.init_qp_minus26);
    out.diff_cu_qp_delta_depth = static_cast<uint8_t>(in.diff_cu_qp_delta_depth);
    out.pps_cb_qp_offset = static_cast<int8_t>(in.pps_cb_qp_offset);
    out.pps_cr_qp_offset = static_cast<int8_t>(in.pps_cr_qp_offset);
    out.pps_beta_offset_div2 = static_cast<int8_t>(in.pps_beta_offset_div2);
    out.pps_tc_offset_div2 = static_cast<int8_t>(in.pps_tc_offset_div2);
    out.log2_parallel_merge_level_minus2 = static_cast<uint8_t>(in.log2_parallel_merge_level_minus2);
    out.num_tile_columns_minus1 = static_cast<uint8_t>(in.num_tile_columns_minus1);
    out.num_tile_rows_minus1 = static_cast<uint8_t>(in.num_tile_rows_minus1);

    // copy column_width_minus1 – pad/truncate to 19
    for (size_t j = 0; j < 19; ++j) {
        out.column_width_minus1[j] = j < in.column_width_minus1.size()
                                         ? static_cast<uint16_t>(in.column_width_minus1[j])
                                         : 0;
    }
    // copy row_height_minus1 – pad/truncate to 21
    for (size_t j = 0; j < 21; ++j) {
        out.row_height_minus1[j] = j < in.row_height_minus1.size()
                                       ? static_cast<uint16_t>(in.row_height_minus1[j])
                                       : 0;
    }
}

static ConvertedH265Params ConvertH265Params(const h265nal::H265BitstreamParserState& state)
{
    ConvertedH265Params r;

    // build SPS→VPS id mapping
    for (auto& [id, sps] : state.sps) {
        r.sps_to_vps[sps->sps_seq_parameter_set_id] = sps->sps_video_parameter_set_id;
    }

    // VPS
    {
        size_t i = 0;
        r.vps.resize(state.vps.size());
        r.vps_ptl.resize(state.vps.size());
        for (auto& [id, vps] : state.vps) {
            ConvertVps(r.vps[i], r.vps_ptl[i], *vps);
            ++i;
        }
    }

    // SPS
    {
        size_t i = 0;
        r.sps.resize(state.sps.size());
        r.sps_ptl.resize(state.sps.size());
        for (auto& [id, sps] : state.sps) {
            ConvertSps(r.sps[i], r.sps_ptl[i], *sps);
            ++i;
        }
    }

    // PPS
    {
        size_t i = 0;
        r.pps.resize(state.pps.size());
        for (auto& [id, pps] : state.pps) {
            uint32_t vps_id = 0;
            auto it = r.sps_to_vps.find(pps->pps_seq_parameter_set_id);
            if (it != r.sps_to_vps.end()) {
                vps_id = it->second;
            }
            ConvertPps(r.pps[i], *pps, vps_id);
            ++i;
        }
    }

    // populate the desc
    r.desc.max_vps_count = static_cast<uint32_t>(r.vps.size());
    r.desc.max_sps_count = static_cast<uint32_t>(r.sps.size());
    r.desc.max_pps_count = static_cast<uint32_t>(r.pps.size());
    r.desc.vps = r.vps.data();
    r.desc.vps_count = static_cast<uint32_t>(r.vps.size());
    r.desc.sps = r.sps.data();
    r.desc.sps_count = static_cast<uint32_t>(r.sps.size());
    r.desc.pps = r.pps.data();
    r.desc.pps_count = static_cast<uint32_t>(r.pps.size());

    return r;
}
// ---------------------------------------------------------------------------

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

    auto converted = ConvertH265Params(parser_state);
    std::printf("Converted H265 params: %zu VPS, %zu SPS, %zu PPS\n",
                converted.vps.size(), converted.sps.size(), converted.pps.size());

    auto graphics = Graphics::Create(codec_profile, output_format, width, height, &converted.desc);
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

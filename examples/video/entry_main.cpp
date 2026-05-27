#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_video.hpp>
#include <format>
#include <fstream>
#include <vector>
#include <mbmff/mbmff.hpp>

static bool check_result(wis::Result result, const char* where)
{
    if (result.status == wis::Status::Ok) {
        return true;
    }

    std::printf(
        "%s failed: %d, platform_code: %d, error: %s\n",
        where,
        static_cast<int>(result.status),
        result.platform_code,
        result.error ? result.error : "None"
    );
    return false;
}

class Application
{
    static void log_callback(wis::Severity severity, const char* message, uint64_t, void* data)
    {
        static_cast<Application*>(data)->log_callback(severity, message, 0);
    }
    void log_callback(wis::Severity severity, const char* message, uint64_t)
    {
        if (!_device.IsValid()) {
            return;
        }

        const char* severity_str = "UNKNOWN";
        switch (severity) {
        case wis::Severity::Verbose:
            severity_str = "VERBOSE";
            break;
        case wis::Severity::Info:
            severity_str = "INFO";
            break;
        case wis::Severity::Warning:
            severity_str = "WARNING";
            break;
        case wis::Severity::Error:
            severity_str = "ERROR";
            break;
        case wis::Severity::Fatal:
            severity_str = "FATAL";
            break;
        default:
            break;
        }
        std::printf("[Member %s] %s\n", severity_str, message ? message : "");
    }

public:
    Application()
        : _device(CreateDevice())
        , _video_decoder(CreateDecoder())
    {
        wis::Result result{};

        _video_queue = _device.CreateCommandQueue(wis::CommandQueueType::VideoDecode, result);
        if (!check_result(result, "CreateCommandQueue")) {
            return;
        }

        _video_command_allocator = _device.CreateCommandAllocator(wis::CommandQueueType::VideoDecode, result);
        if (!check_result(result, "CreateCommandAllocator")) {
            return;
        }
        _vcmd = _video_extension.CreateCommandList(_video_command_allocator, result);
        if (!check_result(result, "CreateVideoDecodeCommandList")) {
            return;
        }
    }

public:
    mbmff::av1C_box GetAv1CBox(std::span<const std::byte> avif_data)
    {
        mbmff::box_iterator it(avif_data, mbmff::iterator_flags::recursive);
        for (; it != it.end(); ++it) {
            const auto& box = *it;
            // Look for 'av1C' box which contains AV1 codec configuration
            if (!box || box->box_header.type != mbmff::box_type::av1C) {
                continue;
            }
            std::printf("Found av1C box, size: %zu bytes\n", box->payload.size());
            return mbmff::box_cast<mbmff::box_type::av1C>(*box);
        }
        return {};
    }
    mbmff::obu_sequence_header_view GetSequenceHeader(const mbmff::av1C_header& header)
    {
        mbmff::obu_iterator it(header);
        for (; it != it.end(); ++it) {
            const auto& obu = *it;
            // Look for OBU of type sequence header
            if (!obu || obu->type != mbmff::obu_type::sequence_header) {
                continue;
            }
            std::printf("Found OBU sequence header, size: %zu bytes\n", obu->payload.size());
            return mbmff::obu_cast<mbmff::obu_type::sequence_header>(*obu);
        }
        return {};
    }
    wis::VideoDecoderParameters CreateVideoDecoderParameters(std::span<const std::byte> avif_data)
    {
        wis::Result result{};
        wis::VideoDecoderParameters decoder_parameters{};
        auto box = GetAv1CBox(avif_data);
        auto header = box.header();

        // Get sequence header from av1C box and create video decoder parameters
        mbmff::obu_sequence_header_view sequence_header_view = GetSequenceHeader(header);
        mbmff::obu_sequence_header seq_header = sequence_header_view.header();

        wis::StdVideoAV1TimingInfo timing_info{
            .flags =
                {
                    .equal_picture_interval = seq_header.timing_info_data.equal_picture_interval,
                },
            .num_units_in_display_tick = seq_header.timing_info_data.num_units_in_display_tick,
            .time_scale = seq_header.timing_info_data.time_scale,
            .num_ticks_per_picture_minus_1 = seq_header.timing_info_data.num_ticks_per_picture_minus_1,
        };
        wis::StdVideoAV1ColorConfig color_config{
            .flags =
                {
                    .mono_chrome = seq_header.color_config_data.monochrome,
                    .color_range = seq_header.color_config_data.color_range,
                    .separate_uv_delta_q = seq_header.color_config_data.separate_uv_delta_q,
                    .color_description_present_flag = seq_header.color_config_data.color_description_present_flag,
                },
            .BitDepth = static_cast<uint8_t>(seq_header.color_config_data.high_bitdepth
                          ? (seq_header.color_config_data.twelve_bit ? 12u : 10u)
                                                                   : 8u),
            .subsampling_x = seq_header.color_config_data.subsampling_x,
            .subsampling_y = seq_header.color_config_data.subsampling_y,
            .color_primaries = wis::StdVideoAV1ColorPrimaries(
                seq_header.color_config_data.color_description_data.color_primaries
            ),
            .matrix_coefficients = wis::StdVideoAV1MatrixCoefficients(
                seq_header.color_config_data.color_description_data.matrix_coefficients
            ),
            .chroma_sample_position = wis::StdVideoAV1ChromaSamplePosition(
                seq_header.color_config_data.chroma_sample_position
            ),
        };
        wis::StdVideoAV1SequenceHeader sequence_header{
            .flags =
                {
                    .still_picture = seq_header.still_picture,
                    .reduced_still_picture_header = seq_header.reduced_still_picture_header,
                    .use_128x128_superblock = seq_header.use_128x128_superblock,
                    .enable_filter_intra = seq_header.enable_filter_intra,
                    .enable_intra_edge_filter = seq_header.enable_intra_edge_filter,
                    .enable_interintra_compound = seq_header.enable_interintra_compound,
                    .enable_masked_compound = seq_header.enable_masked_compound,
                    .enable_warped_motion = seq_header.enable_warped_motion,
                    .enable_dual_filter = seq_header.enable_dual_filter,
                    .enable_order_hint = seq_header.enable_order_hint,
                    .enable_jnt_comp = seq_header.enable_jnt_comp,
                    .enable_ref_frame_mvs = seq_header.enable_ref_frame_mvs,
                    .frame_id_numbers_present_flag = seq_header.frame_id_numbers_present_flag,
                    .enable_superres = seq_header.enable_superres,
                    .enable_cdef = seq_header.enable_cdef,
                    .enable_restoration = seq_header.enable_restoration,
                    .film_grain_params_present = seq_header.film_grain_params_present,
                    .timing_info_present_flag = seq_header.timing_info_present_flag,
                    .initial_display_delay_present_flag = seq_header.initial_display_delay_present_flag,
                },
            .seq_profile = wis::StdVideoAV1Profile(seq_header.seq_profile),
            .frame_width_bits_minus_1 = seq_header.frame_width_bits_minus_1,
            .frame_height_bits_minus_1 = seq_header.frame_height_bits_minus_1,
            .max_frame_width_minus_1 = seq_header.max_frame_width_minus_1,
            .max_frame_height_minus_1 = seq_header.max_frame_height_minus_1,
            .delta_frame_id_length_minus_2 = seq_header.delta_frame_id_length_minus_2,
            .additional_frame_id_length_minus_1 = seq_header.additional_frame_id_length_minus_1,
            .order_hint_bits_minus_1 = seq_header.order_hint_bits_minus_1,
            .seq_force_integer_mv = seq_header.seq_force_integer_mv,
            .seq_force_screen_content_tools = seq_header.seq_force_screen_content_tools,
            .pColorConfig = &color_config,
            .pTimingInfo = &timing_info,
        };

        wis::StdCodecProfile codec_profile = wis::StdCodecProfile::AV1Main;
        using enum wis::StdVideoAV1Profile;
        switch (sequence_header.seq_profile) {
        default:
            std::printf("Unsupported AV1 profile: %d\n", static_cast<int>(sequence_header.seq_profile));
            return decoder_parameters;
        case Main:
            codec_profile = wis::StdCodecProfile::AV1Main;
            break;
        case High:
            codec_profile = wis::StdCodecProfile::AV1High;
            break;
        case Professional:
            codec_profile = wis::StdCodecProfile::AV1Professional;
            break;
        }

        wis::ChromaSubsampling chroma_subsampling = wis::ChromaSubsampling::C420;
        if (sequence_header.pColorConfig) {
            if (sequence_header.pColorConfig->subsampling_x == 0 && sequence_header.pColorConfig->subsampling_y == 0) {
                chroma_subsampling = wis::ChromaSubsampling::C444;
            } else if (sequence_header.pColorConfig->subsampling_x == 0
                       && sequence_header.pColorConfig->subsampling_y == 1) {
                chroma_subsampling = wis::ChromaSubsampling::C422;
            }
        }

        wis::DataFormat image_format = wis::DataFormat::NV12;
        switch (chroma_subsampling) {
        case wis::ChromaSubsampling::C420:
            switch (color_config.BitDepth) {
            case 8:
                image_format = wis::DataFormat::NV12;
                break;
            case 10:
                image_format = wis::DataFormat::P010;
                break;
            case 12:
                image_format = wis::DataFormat::P012;
                break;
            }
            break;
        default:
            std::printf("Unsupported chroma subsampling format: %d\n", static_cast<int>(chroma_subsampling));
            return decoder_parameters;
        }

        

        // Query if sequence header parameters are supported by the video decoder
        result = _video_extension.QueryCodecCaps({
            .codec_profile = codec_profile,
            .image_format = image_format,
            .width = sequence_header.max_frame_width_minus_1 + 1u,
            .height = sequence_header.max_frame_height_minus_1 + 1u,
        });
        if (!check_result(result, "QueryCodecCaps with sequence header parameters")) {
            return decoder_parameters;
        }

        // Create video decoder parameters from sequence header
        decoder_parameters = _video_decoder.CreateParameters(&sequence_header, result);
        check_result(result, "CreateParameters with sequence header");
        return decoder_parameters;
    }

    int Run()
    {
        wis::Result result{};
        std::ifstream avif_file("assets/avif_sample.avif", std::ios::binary);
        if (!avif_file) {
            std::printf("Failed to open AVIF file\n");
            return -1;
        }

        std::vector<uint8_t> avif_data((std::istreambuf_iterator<char>(avif_file)), std::istreambuf_iterator<char>());
        std::span avif_span = std::as_bytes(std::span(avif_data.data(), avif_data.size()));

        wis::VideoDecoderParameters parameters = CreateVideoDecoderParameters(avif_span);


        return 0;
    }

private:
    wis::Device CreateDevice()
    {
        wis::Result result{};
        wis::Device device{};

        wis::DebugDesc debug_desc = {
            .enable_debug_layer = true,
            .callback = log_callback,
            .user_data = this,
        };

        wis::Instance instance = wis::CreateInstance(&debug_desc, {}, result);
        if (!check_result(result, "CreateInstance")) {
            return device;
        }

        wis::AdapterQuery adapters = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
        if (!check_result(result, "QueryAdapters")) {
            return device;
        }

        wis::DeviceExtensionHeader* extensions[] = {&_video_extension};
        wis::CommandQueueDesc queue_descs[] = {
            {wis::CommandQueueType::VideoDecode, wis::CommandQueuePriority::Normal},
        };
        wis::DeviceRequirements requirements{
            .queue_descs = {queue_descs},
            .extensions = {extensions},
        };

        for (size_t i = 0; i < adapters.GetAdapterCount(); ++i) {
            device = adapters.CreateDevice(i, requirements, result);
            if (result.status == wis::Status::Ok) {
                wis::AdapterDesc adapter_desc = adapters.GetAdapterDesc(i, result);
                std::printf(
                    "Successfully created device for adapter: %s, vendor_id: %u, device_id: %u\n",
                    adapter_desc.description.data(),
                    adapter_desc.vendor_id,
                    adapter_desc.device_id
                );
                PrintCapability();
                return device;
            }
        }

        assert(_device.IsValid() && "Failed to create device with video decoding support");
        return device;
    }
    wis::VideoDecoder CreateDecoder()
    {
        wis::Result result{};
        wis::VideoDecoderDesc decoder_desc{
            .width = 1920,
            .height = 1080,
            .image_format = wis::DataFormat::NV12,
            .codec_profile = wis::StdCodecProfile::AV1Main,
            .decode_picture_buffer_count = 4,
        };
        wis::VideoDecoder video_decoder = _video_extension.CreateDecoder(decoder_desc, result);
        if (!check_result(result, "CreateDecoder")) {
            return video_decoder;
        }
        return video_decoder;
    }
    void PrintCapability()
    {
        wis::Result result{};

        result = _video_extension.QueryCodecCaps({
            .codec_profile = wis::StdCodecProfile::AV1Main,
            .image_format = wis::DataFormat::NV12,
            .width = 1920,
            .height = 1080,
        });
        auto str = std::format(
            "Codec: AV1 Main; Format: NV12; {}",
            result.status == wis::Status::Ok ? "Supported" : "Not Supported"
        );
        std::printf("%s\n", str.c_str());
        assert(result.status == wis::Status::Ok && "AV1 Main profile with NV12 format should be supported");
    }

private:
    wis::VideoDecodingExtension _video_extension{wis::VideoCodecFlags::AV1};
    wis::Device _device;
    wis::VideoDecoder _video_decoder;
    wis::CommandQueue _video_queue;
    wis::CommandAllocator _video_command_allocator;
    wis::VideoDecodeCommandList _vcmd;
};

int main()
{
    Application app;
    return app.Run();
}

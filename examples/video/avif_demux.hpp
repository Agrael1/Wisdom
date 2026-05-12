#pragma once
#include <avif/avif.h>
#include <vector>
#include <span>
#include <memory>

class AvifDemuxer
{
public:
    struct Deleter {
        void operator()(avifDecoder* dec) const { avifDecoderDestroy(dec); }
    };

    AvifDemuxer() : decoder(avifDecoderCreate()) {}

    bool Load(std::span<const uint8_t> data)
    {
        avifResult res = avifDecoderSetIOMemory(decoder.get(), data.data(), data.size());
        if (res != AVIF_RESULT_OK) return false;

        res = avifDecoderParse(decoder.get());
        
        if(res == AVIF_RESULT_OK) {
            raw_data = data;
        }

        return res == AVIF_RESULT_OK;
    }

    uint32_t GetWidth() const { return decoder->image ? decoder->image->width : 0; }
    uint32_t GetHeight() const { return decoder->image ? decoder->image->height : 0; }
    uint32_t GetImageCount() const { return decoder->imageCount; }

    std::span<const uint8_t> GetFrameData(uint32_t frame_index) const
    {
        if (frame_index >= (uint32_t)decoder->imageCount) return {};

        avifExtent extent;
        avifResult res = avifDecoderNthImageMaxExtent(decoder.get(), frame_index, &extent);
        if (res != AVIF_RESULT_OK) return {};
        if (extent.size == 0 || extent.offset + extent.size > raw_data.size()) return {};

        return raw_data.subspan(extent.offset, extent.size);
    }

private:
    std::unique_ptr<avifDecoder, Deleter> decoder;
    std::span<const uint8_t> raw_data;
};

/**
 * @struct WisDataFormat WisDataFormat
 * @ingroup Enumerations Core
 *
 * @section WisDataFormat_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisDataFormat {
 *     WisDataFormatUnknown = 0,
 *     WisDataFormatRGBA32Float = 2,
 *     WisDataFormatRGBA32Uint = 3,
 *     WisDataFormatRGBA32Sint = 4,
 *     WisDataFormatRGB32Float = 6,
 *     WisDataFormatRGB32Uint = 7,
 *     WisDataFormatRGB32Sint = 8,
 *     WisDataFormatRGBA16Float = 10,
 *     WisDataFormatRGBA16Unorm = 11,
 *     WisDataFormatRGBA16Uint = 12,
 *     WisDataFormatRGBA16Snorm = 13,
 *     WisDataFormatRGBA16Sint = 14,
 *     WisDataFormatRG32Float = 16,
 *     WisDataFormatRG32Uint = 17,
 *     WisDataFormatRG32Sint = 18,
 *     WisDataFormatD32FloatS8Uint = 20,
 *     WisDataFormatRGB10A2Unorm = 24,
 *     WisDataFormatRGB10A2Uint = 25,
 *     WisDataFormatRG11B10Float = 26,
 *     WisDataFormatRGBA8Unorm = 28,
 *     WisDataFormatRGBA8UnormSrgb = 29,
 *     WisDataFormatRGBA8Uint = 30,
 *     WisDataFormatRGBA8Snorm = 31,
 *     WisDataFormatRGBA8Sint = 32,
 *     WisDataFormatRG16Float = 34,
 *     WisDataFormatRG16Unorm = 35,
 *     WisDataFormatRG16Uint = 36,
 *     WisDataFormatRG16Snorm = 37,
 *     WisDataFormatRG16Sint = 38,
 *     WisDataFormatD32Float = 40,
 *     WisDataFormatR32Float = 41,
 *     WisDataFormatR32Uint = 42,
 *     WisDataFormatR32Sint = 43,
 *     WisDataFormatD24UnormS8Uint = 45,
 *     WisDataFormatRG8Unorm = 49,
 *     WisDataFormatRG8Uint = 50,
 *     WisDataFormatRG8Snorm = 51,
 *     WisDataFormatRG8Sint = 52,
 *     WisDataFormatR16Float = 54,
 *     WisDataFormatD16Unorm = 55,
 *     WisDataFormatR16Unorm = 56,
 *     WisDataFormatR16Uint = 57,
 *     WisDataFormatR16Snorm = 58,
 *     WisDataFormatR16Sint = 59,
 *     WisDataFormatR8Unorm = 61,
 *     WisDataFormatR8Uint = 62,
 *     WisDataFormatR8Snorm = 63,
 *     WisDataFormatR8Sint = 64,
 *     WisDataFormatRGB9E5UFloat = 67,
 *     WisDataFormatBC1RGBAUnorm = 71,
 *     WisDataFormatBC1RGBAUnormSrgb = 72,
 *     WisDataFormatBC2RGBAUnorm = 74,
 *     WisDataFormatBC2RGBAUnormSrgb = 75,
 *     WisDataFormatBC3RGBAUnorm = 77,
 *     WisDataFormatBC3RGBAUnormSrgb = 78,
 *     WisDataFormatBC4RUnorm = 80,
 *     WisDataFormatBC4RSnorm = 81,
 *     WisDataFormatBC5RGUnorm = 83,
 *     WisDataFormatBC5RGSnorm = 84,
 *     WisDataFormatB5G6R5Unorm = 85,
 *     WisDataFormatB5G5R5A1Unorm = 86,
 *     WisDataFormatBGRA8Unorm = 87,
 *     WisDataFormatBGRA8UnormSrgb = 91,
 *     WisDataFormatBC6HUfloat16 = 95,
 *     WisDataFormatBC6HSfloat16 = 96,
 *     WisDataFormatBC7RGBAUnorm = 98,
 *     WisDataFormatBC7RGBAUnormSrgb = 99,
 *     WisDataFormatBGRA4Unorm = 115,
 * // Provided by Wisdom 0.7.1.     WisDataFormatNV12 = 256,
 * // Provided by Wisdom 0.7.1.     WisDataFormatP010 = 257,
 * // Provided by Wisdom 0.7.1.     WisDataFormatP012 = 258,
 * // Provided by Wisdom 0.7.1.     WisDataFormatP016 = 259,
 * } WisDataFormat;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class DataFormat {
 *     Unknown = 0,
 *     RGBA32Float = 2,
 *     RGBA32Uint = 3,
 *     RGBA32Sint = 4,
 *     RGB32Float = 6,
 *     RGB32Uint = 7,
 *     RGB32Sint = 8,
 *     RGBA16Float = 10,
 *     RGBA16Unorm = 11,
 *     RGBA16Uint = 12,
 *     RGBA16Snorm = 13,
 *     RGBA16Sint = 14,
 *     RG32Float = 16,
 *     RG32Uint = 17,
 *     RG32Sint = 18,
 *     D32FloatS8Uint = 20,
 *     RGB10A2Unorm = 24,
 *     RGB10A2Uint = 25,
 *     RG11B10Float = 26,
 *     RGBA8Unorm = 28,
 *     RGBA8UnormSrgb = 29,
 *     RGBA8Uint = 30,
 *     RGBA8Snorm = 31,
 *     RGBA8Sint = 32,
 *     RG16Float = 34,
 *     RG16Unorm = 35,
 *     RG16Uint = 36,
 *     RG16Snorm = 37,
 *     RG16Sint = 38,
 *     D32Float = 40,
 *     R32Float = 41,
 *     R32Uint = 42,
 *     R32Sint = 43,
 *     D24UnormS8Uint = 45,
 *     RG8Unorm = 49,
 *     RG8Uint = 50,
 *     RG8Snorm = 51,
 *     RG8Sint = 52,
 *     R16Float = 54,
 *     D16Unorm = 55,
 *     R16Unorm = 56,
 *     R16Uint = 57,
 *     R16Snorm = 58,
 *     R16Sint = 59,
 *     R8Unorm = 61,
 *     R8Uint = 62,
 *     R8Snorm = 63,
 *     R8Sint = 64,
 *     RGB9E5UFloat = 67,
 *     BC1RGBAUnorm = 71,
 *     BC1RGBAUnormSrgb = 72,
 *     BC2RGBAUnorm = 74,
 *     BC2RGBAUnormSrgb = 75,
 *     BC3RGBAUnorm = 77,
 *     BC3RGBAUnormSrgb = 78,
 *     BC4RUnorm = 80,
 *     BC4RSnorm = 81,
 *     BC5RGUnorm = 83,
 *     BC5RGSnorm = 84,
 *     B5G6R5Unorm = 85,
 *     B5G5R5A1Unorm = 86,
 *     BGRA8Unorm = 87,
 *     BGRA8UnormSrgb = 91,
 *     BC6HUfloat16 = 95,
 *     BC6HSfloat16 = 96,
 *     BC7RGBAUnorm = 98,
 *     BC7RGBAUnormSrgb = 99,
 *     BGRA4Unorm = 115,
 * // Provided by Wisdom 0.7.1.     NV12 = 256,
 * // Provided by Wisdom 0.7.1.     P010 = 257,
 * // Provided by Wisdom 0.7.1.     P012 = 258,
 * // Provided by Wisdom 0.7.1.     P016 = 259,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDataFormat_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Data format for data.
 * Used as a template for data in several cases.
 * Can be used to describe:
 * - Vertex buffer data format
 * - Texture data format
 * - Render target data format
 * - Depth stencil data format
 *
 * \note Translates to  `DXGI_FORMAT` for DirectX 12 implementation, and `VkFormat` for Vulkan implementation.
 *
 * Values:
 * - `WisDataFormatUnknown = 0`: Unknown format.
 * - `WisDataFormatRGBA32Float = 2`: 32 bit per channel RGBA format.
 * A four-component, 128-bit signed floating-point format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7,
 * a 32-bit B component in bytes 8..11,
 * a 32-bit A component in bytes 12..15.
 * - `WisDataFormatRGBA32Uint = 3`: 32 bit per channel RGBA format.
 * A four-component, 128-bit unsigned integer format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7,
 * a 32-bit B component in bytes 8..11,
 * a 32-bit A component in bytes 12..15.
 * - `WisDataFormatRGBA32Sint = 4`: 32 bit per channel RGBA format.
 * A four-component, 128-bit signed integer format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7,
 * a 32-bit B component in bytes 8..11,
 * a 32-bit A component in bytes 12..15.
 * - `WisDataFormatRGB32Float = 6`: 32 bit per channel RGB format.
 * A three-component, 96-bit signed floating-point format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7,
 * a 32-bit B component in bytes 8..11.
 * - `WisDataFormatRGB32Uint = 7`: 32 bit per channel RGB format.
 * A three-component, 96-bit unsigned integer format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7,
 * a 32-bit B component in bytes 8..11.
 * - `WisDataFormatRGB32Sint = 8`: 32 bit per channel RGB format.
 * A three-component, 96-bit signed integer format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7,
 * a 32-bit B component in bytes 8..11.
 * - `WisDataFormatRGBA16Float = 10`: 16 bit per channel RGBA format.
 * A four-component, 64-bit signed floating-point format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3,
 * a 16-bit B component in bytes 4..5,
 * a 16-bit A component in bytes 6..7.
 * - `WisDataFormatRGBA16Unorm = 11`: 16 bit per channel RGBA format.
 * A four-component, 64-bit unsigned normalized format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3,
 * a 16-bit B component in bytes 4..5,
 * a 16-bit A component in bytes 6..7.
 * - `WisDataFormatRGBA16Uint = 12`: 16 bit per channel RGBA format.
 * A four-component, 64-bit unsigned integer format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3,
 * a 16-bit B component in bytes 4..5,
 * a 16-bit A component in bytes 6..7.
 * - `WisDataFormatRGBA16Snorm = 13`: 16 bit per channel RGBA format.
 * A four-component, 64-bit signed normalized format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3,
 * a 16-bit B component in bytes 4..5,
 * a 16-bit A component in bytes 6..7.
 * - `WisDataFormatRGBA16Sint = 14`: 16 bit per channel RGBA format.
 * A four-component, 64-bit signed integer format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3,
 * a 16-bit B component in bytes 4..5,
 * a 16-bit A component in bytes 6..7.
 * - `WisDataFormatRG32Float = 16`: 32 bit per channel RG format.
 * A two-component, 64-bit signed floating-point format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7.
 * - `WisDataFormatRG32Uint = 17`: 32 bit per channel RG format.
 * A two-component, 64-bit unsigned integer format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7.
 * - `WisDataFormatRG32Sint = 18`: 32 bit per channel RG format.
 * A two-component, 64-bit signed integer format that has
 * a 32-bit R component in bytes 0..3,
 * a 32-bit G component in bytes 4..7.
 * - `WisDataFormatD32FloatS8Uint = 20`: 32 bit depth, 8 bit stencil format.
 * A combined depth/stencil format with a 32-bit depth component in bytes 0..3
 * and an 8-bit stencil component in bytes 4..4.
 * - `WisDataFormatRGB10A2Unorm = 24`: 10 bit per channel RGB format with 2 bit alpha.
 * A four-component, 32-bit unsigned normalized format that has
 * a 10-bit R component in bits 0..9,
 * a 10-bit G component in bits 10..19,
 * a 10-bit B component in bits 20..29,
 * a 2-bit A component in bits 30..31.
 * - `WisDataFormatRGB10A2Uint = 25`: 10 bit per channel RGB format with 2 bit alpha.
 * A four-component, 32-bit unsigned integer format that has
 * a 10-bit R component in bits 0..9,
 * a 10-bit G component in bits 10..19,
 * a 10-bit B component in bits 20..29,
 * a 2-bit A component in bits 30..31.
 * - `WisDataFormatRG11B10Float = 26`: 11 bit per channel RG format with 10 bit blue.
 * A three-component, 32-bit signed floating-point format that has
 * a 11-bit R component in bits 0..10,
 * a 11-bit G component in bits 11..21,
 * a 10-bit B component in bits 22..31.
 * - `WisDataFormatRGBA8Unorm = 28`: 8 bit per channel RGBA format.
 * A four-component, 32-bit unsigned normalized format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit B component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatRGBA8UnormSrgb = 29`: 8 bit per channel RGBA format. SRGB non-linear color space.
 * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit B component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatRGBA8Uint = 30`: 8 bit per channel RGBA format.
 * A four-component, 32-bit unsigned integer format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit B component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatRGBA8Snorm = 31`: 8 bit per channel RGBA format.
 * A four-component, 32-bit signed normalized format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit B component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatRGBA8Sint = 32`: 8 bit per channel RGBA format.
 * A four-component, 32-bit signed integer format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit B component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatRG16Float = 34`: 16 bit per channel RG format.
 * A two-component, 32-bit signed floating-point format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3.
 * - `WisDataFormatRG16Unorm = 35`: 16 bit per channel RG format.
 * A two-component, 32-bit unsigned normalized format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3.
 * - `WisDataFormatRG16Uint = 36`: 16 bit per channel RG format.
 * A two-component, 32-bit unsigned integer format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3.
 * - `WisDataFormatRG16Snorm = 37`: 16 bit per channel RG format.
 * A two-component, 32-bit signed normalized format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3.
 * - `WisDataFormatRG16Sint = 38`: 16 bit per channel RG format.
 * A two-component, 32-bit signed integer format that has
 * a 16-bit R component in bytes 0..1,
 * a 16-bit G component in bytes 2..3.
 * - `WisDataFormatD32Float = 40`: 32 bit depth format.
 * A 32-bit depth format supporting 32-bit floating-point depth values.
 * - `WisDataFormatR32Float = 41`: 32 bit per channel R format.
 * A one-component, 32-bit signed floating-point format that has
 * a 32-bit R component in bytes 0..3.
 * - `WisDataFormatR32Uint = 42`: 32 bit per channel R format.
 * A one-component, 32-bit unsigned integer format that has
 * a 32-bit R component in bytes 0..3.
 * - `WisDataFormatR32Sint = 43`: 32 bit per channel R format.
 * A one-component, 32-bit signed integer format that has
 * a 32-bit R component in bytes 0..3.
 * - `WisDataFormatD24UnormS8Uint = 45`: 24 bit depth, 8 bit stencil format.
 * A combined depth/stencil format with a 24-bit depth component in bytes 0..2
 * and an 8-bit stencil component in bytes 3..3.
 * - `WisDataFormatRG8Unorm = 49`: 8 bit per channel RG format.
 * A two-component, 16-bit unsigned normalized format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1.
 * - `WisDataFormatRG8Uint = 50`: 8 bit per channel RG format.
 * A two-component, 16-bit unsigned integer format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1.
 * - `WisDataFormatRG8Snorm = 51`: 8 bit per channel RG format.
 * A two-component, 16-bit signed normalized format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1.
 * - `WisDataFormatRG8Sint = 52`: 8 bit per channel RG format.
 * A two-component, 16-bit signed integer format that has
 * an 8-bit R component in bytes 0..0,
 * an 8-bit G component in bytes 1..1.
 * - `WisDataFormatR16Float = 54`: 16 bit per channel R format.
 * A one-component, 16-bit signed floating-point format that has
 * a 16-bit R component in bytes 0..1.
 * - `WisDataFormatD16Unorm = 55`: 16 bit depth format.
 * A 16-bit depth format supporting 16-bit unsigned normalized depth values.
 * - `WisDataFormatR16Unorm = 56`: 16 bit per channel R format.
 * A one-component, 16-bit unsigned normalized format that has
 * a 16-bit R component in bytes 0..1.
 * - `WisDataFormatR16Uint = 57`: 16 bit per channel R format.
 * A one-component, 16-bit unsigned integer format that has
 * a 16-bit R component in bytes 0..1.
 * - `WisDataFormatR16Snorm = 58`: 16 bit per channel R format.
 * A one-component, 16-bit signed normalized format that has
 * a 16-bit R component in bytes 0..1.
 * - `WisDataFormatR16Sint = 59`: 16 bit per channel R format.
 * A one-component, 16-bit signed integer format that has
 * a 16-bit R component in bytes 0..1.
 * - `WisDataFormatR8Unorm = 61`: 8 bit per channel R format.
 * A one-component, 8-bit unsigned normalized format that has
 * an 8-bit R component in bytes 0..0.
 * - `WisDataFormatR8Uint = 62`: 8 bit per channel R format.
 * A one-component, 8-bit unsigned integer format that has
 * an 8-bit R component in bytes 0..0.
 * - `WisDataFormatR8Snorm = 63`: 8 bit per channel R format.
 * A one-component, 8-bit signed normalized format that has
 * an 8-bit R component in bytes 0..0.
 * - `WisDataFormatR8Sint = 64`: 8 bit per channel R format.
 * A one-component, 8-bit signed integer format that has
 * an 8-bit R component in bytes 0..0.
 * - `WisDataFormatRGB9E5UFloat = 67`: 9 bit per channel RGB format with shared exponent.
 * A three-component, 32-bit unsigned floating-point format that has
 * a 9-bit R component in bits 0..8,
 * a 9-bit G component in bits 9..17,
 * a 9-bit B component in bits 18..26,
 * a shared 5-bit exponent in bits 27..31.
 * - `WisDataFormatBC1RGBAUnorm = 71`: BC1/DXT1 compressed format.
 * A four-component, block-compressed format where each 64-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGB texel data, and provides 1 bit of alpha.
 * - `WisDataFormatBC1RGBAUnormSrgb = 72`: BC1/DXT1 compressed format.
 * A four-component, block-compressed format where each 64-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
 * - `WisDataFormatBC2RGBAUnorm = 74`: BC2/DXT3 compressed format.
 * A four-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGBA texel data
 * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
 * - `WisDataFormatBC2RGBAUnormSrgb = 75`: BC2/DXT3 compressed format.
 * A four-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
 * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
 * - `WisDataFormatBC3RGBAUnorm = 77`: BC3/DXT5 compressed format.
 * A four-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
 * followed by 64 bits encoding RGB values
 * - `WisDataFormatBC3RGBAUnormSrgb = 78`: BC3/DXT5 compressed format.
 * A four-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
 * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
 * - `WisDataFormatBC4RUnorm = 80`: BC4 compressed format.
 * A single-component, block-compressed format where each 64-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized red texel data
 * - `WisDataFormatBC4RSnorm = 81`: BC4 compressed format.
 * A single-component, block-compressed format where each 64-bit
 * compressed texel block encodes a 4×4 rectangle
 * of signed normalized red texel data
 * - `WisDataFormatBC5RGUnorm = 83`: BC5 compressed format.
 * A two-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized red and green texel data
 * - `WisDataFormatBC5RGSnorm = 84`: BC5 compressed format.
 * A two-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of signed normalized red and green texel data
 * - `WisDataFormatB5G6R5Unorm = 85`: 16 bit per channel RGB format.
 * A three-component, 16-bit unsigned normalized format that has
 * a 5-bit B component in bits 0..4,
 * a 6-bit G component in bits 5..10,
 * a 5-bit R component in bits 11..15.
 * - `WisDataFormatB5G5R5A1Unorm = 86`: 16 bit per channel RGBA format.
 * A four-component, 16-bit unsigned normalized format that has
 * a 5-bit B component in bits 0..4,
 * a 5-bit G component in bits 5..9,
 * a 5-bit R component in bits 10..14,
 * a 1-bit A component in bits 15..15.
 * - `WisDataFormatBGRA8Unorm = 87`: 8 bit per channel RGBA format.
 * A four-component, 32-bit unsigned normalized format that has
 * an 8-bit B component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit R component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatBGRA8UnormSrgb = 91`: 8 bit per channel RGBA format. SRGB non-linear color space.
 * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
 * an 8-bit B component in bytes 0..0,
 * an 8-bit G component in bytes 1..1,
 * an 8-bit R component in bytes 2..2,
 * an 8-bit A component in bytes 3..3.
 * - `WisDataFormatBC6HUfloat16 = 95`: BC6H compressed format.
 * A three-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned floating-point RGB texel data
 * - `WisDataFormatBC6HSfloat16 = 96`: BC6H compressed format.
 * A three-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of signed floating-point RGB texel data
 * - `WisDataFormatBC7RGBAUnorm = 98`: BC7 compressed format.
 * A four-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGBA texel data
 * - `WisDataFormatBC7RGBAUnormSrgb = 99`: BC7 compressed format.
 * A four-component, block-compressed format where each 128-bit
 * compressed texel block encodes a 4×4 rectangle
 * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
 * - `WisDataFormatBGRA4Unorm = 115`: 4 bit per channel RGB format.
 * A four-component, 16-bit unsigned normalized format that has
 * a 4-bit B component in bits 0..3,
 * a 4-bit G component in bits 4..7,
 * a 4-bit R component in bits 8..11,
 * a 4-bit A component in bits 12..15.
 * - `WisDataFormatNV12 = 256`: NV12 video format.
 * A two-plane format with a single 8-bit Y plane followed by an interleaved UV plane, where the U and V components are
 * subsampled by a factor of 2 in both dimensions. The Y plane contains the luma (brightness) information, while the UV
 * plane contains the chroma (color) information. This format is commonly used for video encoding and decoding
 * applications.
 * - `WisDataFormatP010 = 257`: P010 video format.
 * A two-plane format similar to NV12, but with 10 bits per channel instead of 8. The Y plane contains 10-bit luma
 * information, and the UV plane contains interleaved 10-bit chroma information. This format is used for high-quality
 * video encoding and decoding, providing improved color fidelity compared to NV12.
 * - `WisDataFormatP012 = 258`: P012 video format.
 * A two-plane format similar to P010, but with 12 bits per channel instead of 10. The Y plane contains 12-bit luma
 * information, and the UV plane contains interleaved 12-bit chroma information. This format is used for professional
 * video applications that require higher color fidelity and dynamic range than P010.
 * - `WisDataFormatP016 = 259`: P016 video format.
 * A two-plane format similar to P010, but with 16 bits per channel instead of 10. The Y plane contains 16-bit luma
 * information, and the UV plane contains interleaved 16-bit chroma information. This format is used for professional
 * video applications that require the highest color fidelity and dynamic range.
 * \endcond
 *
 *
 * @section WisDataFormat_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureDesc, WisTextureBinding, WisInputAttributeDesc, WisRenderAttachmentsDesc, WisRenderTargetDesc,
 * WisSwapchainDesc, WisSwapchainUpdateDesc Functions: wisDeviceGetFormatPresentationSupport,
 * wisDeviceGetFormatProperties
 * \endcond
 */

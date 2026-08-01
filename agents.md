Implicit actions:

- If the you need to create or update structure/enum/variant that is located in generated/ directories:
- 1. Update the structure in .xml file that is named the same as extension folder: example video/generated/c_api -> video.xml
- 2. Run the generator target with xml name as argument: example: video.xml -> generator video

If the change is from core API, (under include folder) use generator without argument: include/generated/c_api -> generator

Set of rules to the code:

- Don't use STL containers. Only usage of wis::span and wis::unique_ptr is allowed. Occasional use of std::string_view is allowed.
- Don't use C++ exceptions.
- Don't use C++ RTTI (typeid, dynamic_cast, etc.).
- No virtual functions.
- API is C compatible, so all API facing functions must have C linkage WIS_EXTERN_C.
- Internal functions are allowed to use C++20 features, but not in header files.
- If several allocations are required, use a single allocation and place all data in it. This is to minimize the number of allocations and deallocations, which can be expensive.
- If allocation is required, try assuming the reasonable size of the buffer and use stack allocation.
- If the buffer size is unknown, use a two-pass approach: first call the function with a null buffer to get the required size, then allocate the buffer and call the function again to fill it.
- If the allocation is absolutely required in command list api, use provided bump allocator, but try to avoid it as much as possible.
- If the allocation is required in class handle, use unique_ptr + release().
- If the class has allocation, fill in destroy function that will free the memory.

<!-- ============================================================ -->
<!-- Session: Wed, Jun 28 — H.265 parameter wiring                -->
<!-- ============================================================ -->

## Goal
- Wire real parsed H.265 VPS/SPS/PPS data from `h265nal::H265BitstreamParserState` through the example into `WisVideoDecodeH265Desc` so `CreateParameters` injects actual parameter sets

## Summary
- Updated `Graphics::Create` signature: added `const wis::VideoDecodeH265Desc* h265_params = nullptr` parameter; when non-null, uses provided max-counts and parameter-set arrays instead of hardcoded empty ones
- Added conversion code in `app.cpp`:
  - `ConvertedH265Params` struct holds converted wisdom arrays + aux storage for `ProfileTierLevel`
  - `ConvertVps()` — maps all VPS flat fields + `pProfileTierLevel` from `H265VpsParser::VpsState`
  - `ConvertSps()` — maps all SPS flat fields + range extension/SCC extension flags + `pProfileTierLevel`; leaves sub-structure pointers (`pDecPicBufMgr`, `pScalingLists`, etc.) as `nullptr`
  - `ConvertPps()` — maps all PPS flat fields + SCC extension flags + tile arrays (padded/truncated to wisdom fixed-size); derives `sps_video_parameter_set_id` via SPS→VPS lookup table
  - `ConvertH265Params()` — orchestrates iteration over maps, builds `sps_to_vps` id mapping, populates `VideoDecodeH265Desc` with pointers into the owned vectors
- `App::Start()` now calls `ConvertH265Params(parser_state)` and passes `&converted.desc` to `Graphics::Create`
- Fixed incorrect SCC extension flag mapping in PPS (`cross_component_prediction`, `chroma_qp_offset_list` — these are from 3D/range extensions, not SCC)
- Builds successfully under MSVC for both `video-vk-cpp` and `video-dx12-cpp` targets

## Next Steps
- Run the example with a real H.265 file (`video-vk-cpp <file.mp4>`) to validate end-to-end parameter creation
- If Vulkan validation errors surface (e.g., missing `pDecPicBufMgr`), populate the corresponding sub-structures from h265nal parsed data
- Add `pProfileTierLevel` population guard for cases where `general` pointer is null (currently left as default-constructed PTL)
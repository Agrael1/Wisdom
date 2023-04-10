#pragma once

namespace wis
{
	enum class CommandListType
	{
        direct = 0,
        bundle = 1,
        compute = 2,
        copy = 3,
        video_decode = 4,
        video_process = 5,
        video_encode = 6,
        none = -1
	};
    struct Viewport
    {
        float top_leftx = 0;
        float top_lefty = 0;
        float width;
        float height;
        float min_depth = 0.0f;
        float max_depth = 1.0f;
    };
    struct ScissorRect
    {
        long left = 0;
        long top = 0;
        long right;
        long bottom;
    };
    enum class PrimitiveTopology
    {
       undefined = 0,
       pointlist = 1,
       linelist = 2,
       linestrip = 3,
       trianglelist = 4,
       trianglestrip = 5,
       trianglefan = 6,
       linelist_adj = 10,
       linestrip_adj = 11,
       trianglelist_adj = 12,
       trianglestrip_adj = 13,
       control_point_patchlist_1 = 33,
       control_point_patchlist_2 = 34,
       control_point_patchlist_3 = 35,
       control_point_patchlist_4 = 36,
       control_point_patchlist_5 = 37,
       control_point_patchlist_6 = 38,
       control_point_patchlist_7 = 39,
       control_point_patchlist_8 = 40,
       control_point_patchlist_9 = 41,
       control_point_patchlist_10 = 42,
       control_point_patchlist_11 = 43,
       control_point_patchlist_12 = 44,
       control_point_patchlist_13 = 45,
       control_point_patchlist_14 = 46,
       control_point_patchlist_15 = 47,
       control_point_patchlist_16 = 48,
       control_point_patchlist_17 = 49,
       control_point_patchlist_18 = 50,
       control_point_patchlist_19 = 51,
       control_point_patchlist_20 = 52,
       control_point_patchlist_21 = 53,
       control_point_patchlist_22 = 54,
       control_point_patchlist_23 = 55,
       control_point_patchlist_24 = 56,
       control_point_patchlist_25 = 57,
       control_point_patchlist_26 = 58,
       control_point_patchlist_27 = 59,
       control_point_patchlist_28 = 60,
       control_point_patchlist_29 = 61,
       control_point_patchlist_30 = 62,
       control_point_patchlist_31 = 63,
       control_point_patchlist_32 = 64,
    };
}
// This file is generated. Do not edit directly.
#ifndef WISDOM_C_API_H
#define WISDOM_C_API_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================
// Enums
//==============================================================

typedef enum WisStatus WisStatus;
/**
 * @brief Provided by Wisdom 0.7.0. Defines common return status codes. Compare against WisStatusOk for success.
 *
 * */
enum WisStatus {
    WisStatusOk = 0, ///< Operation succeded.
    WisStatusTimeout = 1, ///< Operation timed out.
    WisStatusPartial = 2, ///< Operation partially succeeded.
    WisStatusInvalidArgument = -1, ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    WisStatusOutOfHostMemory = -2, ///< There is no more host memory available.
    WisStatusOutOfDeviceMemory = -3, ///< There is no more device memory available.
    WisStatusDeviceLost = -4, ///< Device driver was forcefully stopped.
    WisStatusOccluded = -5, ///< Swapchain presentation was not visible to the user. Rendering is too fast.
    WisStatusError = -10000, ///< Operation failed.
};

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_API_H

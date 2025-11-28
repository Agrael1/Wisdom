// This file is generated. Do not edit directly.
#ifndef WISDOM_C_VK_API_H
#define WISDOM_C_VK_API_H
#include <wisdom/generated/api.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 *
 * */
WIS_DEFINE_HANDLE(WisVKInstance, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing device extension header. Used in extension development.
 *
 *
 * */
typedef struct WisVKDeviceExtensionHeader {
    const void* opaque; ///< defines opaque member. @vku_must not be changed directly outside extension development.
} WisVKDeviceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing instance extension header. Used in extension development.
 *
 *
 * */
typedef struct WisVKInstanceExtensionHeader {
    const void* opaque; ///< defines opaque member. @vku_must not be changed directly outside extension development.
} WisVKInstanceExtensionHeader;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_VK_API_H

// This file is generated. Do not edit directly.
#ifndef WISDOM_C_DX12_API_H
#define WISDOM_C_DX12_API_H
#include <wisdom/generated/c_api.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Instance, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing device extension header. Used in extension development.
 *
 * */
typedef struct WisDX12DeviceExtensionHeader {
    const void* opaque; ///< defines opaque member. In @wis_mustnot be changed directly outside extension development.
} WisDX12DeviceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing instance extension header. Used in extension development.
 *
 * */
typedef struct WisDX12InstanceExtensionHeader {
    const void* opaque; ///< defines opaque member. It @wis_mustnot be changed directly outside extension development.
} WisDX12InstanceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Creates the WisInstance with extensions, specified in extension array.
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to WisInstanceExtensionHeader.
 * @param extension_count counts the number of extensions in the `extensions` array.
 * @param instance points to WisInstance, which will be initialized on success (`WisStatusOk`).
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CreateInstance(bool                             debug_layer,
                                           WisDX12InstanceExtensionHeader** extensions,
                                           size_t                           extension_count,
                                           WisDX12Instance*                 instance);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_DX12_API_H

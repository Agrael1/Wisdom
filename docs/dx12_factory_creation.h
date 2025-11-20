/**
 * @page dx12_factory_creation DX12 Factory Creation
 * @ingroup dx12_core
 * 
 * @brief Detailed documentation for DX12 factory creation
 * 
 * @section dx12_factory_overview Overview
 * 
 * The factory is the entry point for the DirectX 12 graphics API in Wisdom.
 * It is responsible for enumerating and creating graphics adapters (physical devices).
 * 
 * @description
 * wis::ImplDX12CreateFactory creates a DX12 factory object that can be used to enumerate
 * graphics adapters available in the system. The factory supports optional debug layer
 * activation and can be extended with custom extensions.
 * 
 * The factory object is lightweight and can be safely destroyed after adapter selection.
 * It uses COM-based reference counting internally and is thread-safe after creation.
 * 
 * @section dx12_factory_creation_params Parameters
 * 
 * @subsection dx12_factory_param_result result
 * Pointer to a wis::Result object that will receive the operation result.
 * 
 * @subsection dx12_factory_param_debug enable_debug
 * Boolean flag indicating whether to enable the DirectX 12 debug layer.
 * 
 * @description
 * When set to true, the debug layer provides extensive validation and error reporting.
 * This includes resource state tracking, parameter validation, and warning messages.
 * 
 * 
 * @note The debug layer requires the Graphics Tools optional feature to be installed on Windows 10/11.
 * 
 * @subsection dx12_factory_param_extensions extensions
 * Pointer to an array of extension objects to enable during factory creation.
 * 
 * @description
 * Extensions allow customization of factory behavior and feature enablement.
 * Each extension must be a valid DX12FactoryExtension pointer.
 * 
 * 
 * @subsection dx12_factory_param_count extension_count
 * Number of extensions in the extensions array.
 * 
 * 
 * @section dx12_factory_creation_usage Usage
 * 
 * 
 * @validusage_begin
 * @li `result` @vku_must be a valid pointer to a wis::Result object
 * @li result @vku_mustnot be nullptr
 * @li If extension_count is greater than 0, extensions @vku_must be a valid pointer to an array of extension_count pointers
 * @li If extension_count is 0, extensions @vku_may be nullptr
 * @li Each element in extensions @vku_must be a valid pointer to a DX12FactoryExtension object
 * @li `extensions` array @vku_mustnot contain duplicate extension types
 * @li `enable_debug` @vku_should be set to true during development and testing
 * @li `enable_debug` @vku_should be set to false in release builds for optimal performance
 * @li If extensions is nullptr, extension_count @vku_must be 0
 * @li extension_count @vku_must be less than or equal to the maximum supported extension count
 * @validusage_end
 * @subsection dx12_factory_basic Basic Usage
 * 
 * @code{.cpp}
 * wis::Result result;
 * wis::DX12Factory factory = wis::ImplDX12CreateFactory(result, false, nullptr, 0);
 * 
 * if (result.status != wis::Status::Ok) {
 *     // Handle error
 *     return;
 * }
 * 
 * // Use factory to enumerate adapters
 * wis::DX12Adapter adapter = factory.GetAdapter(result, 0, wis::AdapterPreference::Performance);
 * @endcode
 * 
 * @subsection dx12_factory_debug Debug Layer Usage
 * 
 * @code{.cpp}
 * wis::Result result;
 * wis::DX12Factory factory = wis::ImplDX12CreateFactory(result, true, nullptr, 0);
 * 
 * if (result.status != wis::Status::Ok) {
 *     if (result.status == wis::Status::ErrorDebugNotSupported) {
 *         // Debug layer not available, fallback to non-debug
 *         factory = wis::ImplDX12CreateFactory(result, false, nullptr, 0);
 *     }
 * }
 * @endcode
 * 
 * @subsection dx12_factory_extensions Extension Usage
 * 
 * @code{.cpp}
 * wis::DX12FactoryExtension* extensions[] = {
 *     &my_custom_extension
 * };
 * 
 * wis::Result result;
 * wis::DX12Factory factory = wis::ImplDX12CreateFactory(
 *     result, 
 *     false, 
 *     extensions, 
 *     1
 * );
 * @endcode
 * 
 * @hostsynch
 * - None. Factory creation is thread-safe.
 * 
 * @threadrestrict
 * - This function can be called from any thread
 * - Multiple threads <b>MAY</b> call this function simultaneously
 * 
 * @returncodes
 * @success
 * @returncode{wis::Status::Ok} Factory created successfully
 * 
 * @error
 * @returncode{wis::Status::ErrorInvalidArgument} result is nullptr or invalid extension configuration
 * @returncode{wis::Status::ErrorDebugNotSupported} Debug layer requested but not available
 * @returncode{wis::Status::ErrorOutOfMemory} Insufficient memory to create factory
 * @returncode{wis::Status::ErrorDeviceNotFound} No compatible DirectX 12 runtime found
 * @returncode{wis::Status::ErrorExtensionNotSupported} Requested extension is not supported
 * 
 * @section dx12_factory_notes Notes
 * 
 * - The factory is a lightweight object and multiple instances can coexist
 * - Factory creation initializes COM if not already initialized
 * - The factory requires Windows 10 version 1809 (October 2018 Update) or later
 * - Debug layer validation has performance overhead and should only be used during development
 * 
 * @section dx12_factory_related Related Functions
 * 
 * - wis::DX12Factory::GetAdapter - Enumerate graphics adapters
 * - wis::ImplDX12CreateDevice - Create a graphics device from adapter
 * 
 * @see wis::DX12Factory
 * @see wis::DX12Adapter
 * @see wis::DX12FactoryExtension
 */

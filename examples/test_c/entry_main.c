#include <wisdom/wisdom.h>
#include <stdio.h>

void log_callback(WisSeverity severity, const char* message, uint64_t device, void* user_data)
{
    const char* severity_str = "";
    switch (severity) {
    case WisSeverityVerbose:
        severity_str = "VERBOSE";
        break;
    case WisSeverityInfo:
        severity_str = "INFO";
        break;
    case WisSeverityWarning:
        severity_str = "WARNING";
        break;
    case WisSeverityError:
        severity_str = "ERROR";
        break;
    case WisSeverityFatal:
        severity_str = "FATAL";
        break;
    default:
        severity_str = "UNKNOWN";
        break;
    }
    printf("[%s] %s\n", severity_str, message);
}

// Entry point for testing
int main()
{
    WisDebugDesc debug_desc       = { 0 };
    debug_desc.enable_debug_layer = true;
    debug_desc.callback           = log_callback;
    debug_desc.user_data          = NULL;

    WisInstance instance = { 0 };
    WisResult   result   = wisCreateInstance(&debug_desc, NULL, 0, &instance);
    printf("CreateInstance result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisAdapterQuery adapter_query = { 0 };
    result                        = wisInstanceQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapter_query);
    printf("QueryAdapters result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    // Destroy instance as we no longer need it
    wisDestroyInstance(&instance);

    WisDevice device        = { 0 };
    size_t    adapter_count = wisAdapterQueryGetAdapterCount(&adapter_query);
    printf("Adapter count: %zu\n", adapter_count);

    for (size_t i = 0; i < adapter_count; ++i) {
        WisAdapterDesc desc = { 0 };
        result              = wisAdapterQueryGetAdapterDesc(&adapter_query, i, &desc);
        printf("GetAdapterDesc result for adapter %zu: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");
        if (result.status == WisStatusOk) {
            printf("Adapter %zu: Name: %s, VendorID: %u, DeviceID: %u, Flags: %u\n", i, desc.description, desc.vendor_id, desc.device_id, desc.flags);
        }

        result = wisAdapterQueryCreateDevice(&adapter_query, i, NULL, &device);
        printf("CreateDevice result for adapter %zu: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");
        if (result.status == WisStatusOk) {
            printf("Device created successfully for adapter %zu.\n", i);
            break; // Successfully created a device, exit loop
        }
    }
    wisDestroyAdapterQuery(&adapter_query);

    // Create CommandQueue
    WisCommandQueue command_queue = { 0 };

    result = wisDeviceCreateCommandQueue(&device, WisCommandQueueTypeGraphics, &command_queue);
    printf("CreateCommandQueue result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisCommandList command_list = { 0 };

    result = wisDeviceCreateCommandList(&device, WisCommandQueueTypeGraphics, &command_list);
    printf("CreateCommandList result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisFence fence = { 0 };
    result         = wisDeviceCreateFence(&device, 0, &fence);
    printf("CreateFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisResourceAllocator allocator = { 0 };
    result                         = wisDeviceCreateResourceAllocator(&device, &allocator);

    // Create pipeline resources
    WisPushConstant push_constants[] = {
        { WisShaderStagesVertex, 16, 0, 0 },
        {  WisShaderStagesPixel, 32, 0, 0 },
    };
    WisPushDescriptor push_descriptors[] = {
        { WisShaderStagesVertex, WisDescriptorTypeConstantBuffer, 1 },
        {  WisShaderStagesPixel,         WisDescriptorTypeBuffer, 2 },
    };
    WisStaticSamplerDesc static_samplers[] = {
        {
         {
                        .min_filter          = WisFilterLinear,
                        .mag_filter          = WisFilterLinear,
                        .mip_filter          = WisFilterLinear,
                        .is_anisotropic      = false,
                        .max_anisotropy      = 1,
                        .address_u           = WisAddressModeRepeat,
                        .address_v           = WisAddressModeRepeat,
                        .address_w           = WisAddressModeRepeat,
                        .min_lod             = 0.0f,
                        .max_lod             = 1000.0f,
                        .mip_lod_bias        = 0.0f,
                        .comparison_op       = WisCompareOperationNever,
                        .static_border_color = WisStaticBorderOpaqueBlack,
                        .border_color        = { 0.0f, 0.0f, 0.0f, 0.0f },
                        .flags               = WisSamplerFlagsNone,
                },
         WisShaderStagesPixel,
         3,
         },
    };

    WisDescriptorTableEntry descriptor_table_entries[] = {
        { WisDescriptorTypeBuffer, 0, 0 },
        { WisDescriptorTypeTexture, 0, UINT32_MAX },
        { WisDescriptorTypeSampler, 1, 2 },
    };
    WisDescriptorTable tables[] = {
        {
         WisDescriptorHeapTypeDescriptor,
         WisShaderStagesVertex,
         descriptor_table_entries,
         1,
         0,
         },
        {
         WisDescriptorHeapTypeSampler,
         WisShaderStagesPixel,
         descriptor_table_entries + 2,
         1,
         0,
         },
        {
         WisDescriptorHeapTypeDescriptor,
         WisShaderStagesPixel,
         descriptor_table_entries + 1,
         1,
         2,
         }
    };

    WisPipelineLayoutDesc pipeline_desc = { 0 };
    pipeline_desc.push_constants        = push_constants;
    pipeline_desc.push_constant_count   = sizeof(push_constants) / sizeof(push_constants[0]);
    pipeline_desc.push_descriptors      = push_descriptors;
    pipeline_desc.push_descriptor_count = sizeof(push_descriptors) / sizeof(push_descriptors[0]);
    pipeline_desc.static_samplers       = static_samplers;
    pipeline_desc.static_sampler_count  = sizeof(static_samplers) / sizeof(static_samplers[0]);
    pipeline_desc.descriptor_tables     = tables;
    pipeline_desc.descriptor_table_count = sizeof(tables) / sizeof(tables[0]);
    WisPipelineLayout pipeline_layout   = { 0 };
    result                              = wisDeviceCreatePipelineLayout(&device, &pipeline_desc, &pipeline_layout);
    printf("CreatePipelineLayout result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    // Create Sampler
    WisSamplerDesc sampler_desc = {
        .min_filter          = WisFilterLinear,
        .mag_filter          = WisFilterLinear,
        .mip_filter          = WisFilterLinear,
        .is_anisotropic      = false,
        .max_anisotropy      = 1,
        .address_u           = WisAddressModeRepeat,
        .address_v           = WisAddressModeRepeat,
        .address_w           = WisAddressModeRepeat,
        .min_lod             = 0.0f,
        .max_lod             = 1000.0f,
        .mip_lod_bias        = 0.0f,
        .comparison_op       = WisCompareOperationNever,
        .static_border_color = WisStaticBorderOpaqueBlack,
        .border_color        = { 0.0f, 0.0f, 0.0f, 0.0f },
        .flags               = WisSamplerFlagsNone,
    };

    // Out of order destruction must still work
    wisDestroyDevice(&device);
    wisDestroyCommandQueue(&command_queue);
    wisDestroyCommandList(&command_list);
    wisDestroyFence(&fence);
    wisDestroyResourceAllocator(&allocator);
    wisDestroyPipelineLayout(&pipeline_layout);
    return 0;
}

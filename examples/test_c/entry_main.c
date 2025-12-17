#include <wisdom/wisdom.h>
#include <stdio.h>

// Entry point for testing
int main()
{
    WisInstance instance = { 0 };
    WisResult   result   = wisCreateInstance(false, NULL, 0, &instance);
    printf("CreateInstance result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisAdapterQuery adapter_query = { 0 };
    result                        = wisInstanceQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapter_query);
    printf("QueryAdapters result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    uint32_t adapter_count = wisAdapterQueryGetAdapterCount(&adapter_query);
    printf("Adapter count: %u\n", adapter_count);

    for (uint32_t i = 0; i < adapter_count; ++i) {
        WisAdapterDesc desc = { 0 };
        result             = wisAdapterQueryGetAdapterDesc(&adapter_query, i, &desc);
        printf("GetAdapterDesc result for adapter %u: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");
        if (result.status == WisStatusOk) {
            printf("Adapter %u: Name: %s, VendorID: %u, DeviceID: %u, Flags: %u\n", i, desc.description, desc.vendor_id, desc.device_id, desc.flags);
        }
    }

    wisDestroyInstance(&instance);
    wisDestroyAdapterQuery(&adapter_query);

    return 0;
}

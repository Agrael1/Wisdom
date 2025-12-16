#include <wisdom/wisdom.h>
#include <stdio.h>

// Entry point for testing
int main()
{
    WisInstance instance = { 0 };
    WisResult   result   = wisCreateInstance(false, NULL, 0, &instance);
    printf("CreateInstance result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisAdapterQuery adapter_query = { 0 };
    result                        = wisQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapter_query);
    printf("QueryAdapters result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    wisDestroyInstance(&instance);
    wisDestroyAdapterQuery(&adapter_query);

    return 0;
}

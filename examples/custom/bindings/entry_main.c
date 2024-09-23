#include <wisdom.h>
#include <stdio.h>

void ExDebugCallback(WisSeverity sev, const char* message, void* user_data)
{
    printf("Debug: %s\n", message);
}

int main()
{
    WisFactory factory = NULL;
    WisDevice device = NULL;
    WisDebugMessenger debug = NULL;

    WisFactoryExtQuery exts[] = {
        { FactoryExtIDDebugExtension, NULL },
    };

    WisCreateFactory(true, exts, 1, &factory);
    WisDebugExtension debug_ext = exts[0].result;

    // Get debug messenger
    WisDebugExtensionCreateDebugMessenger(debug_ext, &ExDebugCallback, NULL, &debug);

    // Create a device
    for (size_t i = 0;; i++) {
        WisAdapter adapter = NULL;
        WisResult result = WisFactoryGetAdapter(factory, i, AdapterPreferencePerformance, &adapter);

        if (result.status == StatusOk) {
            WisAdapterDesc desc = { 0 };
            WisAdapterGetDesc(adapter, &desc);
            printf("Adapter: %s\n", desc.description);

            WisResult res = WisCreateDevice(adapter, NULL, 0, false, &device);
            if (res.status == StatusOk) {
                WisAdapterDestroy(adapter);
                break;
            }
            WisAdapterDestroy(adapter);
        } else {
            break;
        }
    }

    WisDeviceDestroy(device);
    WisFactoryDestroy(factory);
    return 0;
}

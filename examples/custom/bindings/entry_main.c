#include <wisdom.h>
#include <stdio.h>

int main()
{
    WisFactory factory = NULL;
    WisDevice device = NULL;

    WisFactoryExtQuery exts[] = {
        { FactoryExtIDDebugExtension, NULL },
    };

    WisCreateFactory(true, exts, 1, &factory);

    for (size_t i = 0;; i++) {
        WisAdapter adapter = NULL;
        WisResult result = WisFactoryGetAdapter(factory, i, AdapterPreferencePerformance, &adapter);

        if (result.status == StatusOk) {
            WisAdapterDesc desc = {0};
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

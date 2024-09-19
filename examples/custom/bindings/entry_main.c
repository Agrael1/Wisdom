#include <wisdom.h>
#include <stdio.h>

int main()
{
    WisFactory factory;
    WisDevice device;

    WisFactoryExtQuery exts[] = {
        { FactoryExtIDDebugExtension, NULL },
    };

    WisCreateFactory(true, exts, 1, &factory);

    for (size_t i = 0;; i++) {
        WisAdapter adapter;
        WisResult result = WisFactoryGetAdapter(factory, i, AdapterPreferencePerformance, &adapter);

        if (result.status == StatusOk) {
            WisAdapterDesc desc;
            WisAdapterGetDesc(adapter, &desc);
            printf("Adapter: %s\n", desc.description);

            // WisResult res = WisCreateDevice(adapter, NULL, 0, false, &device);
            // if (res.status == StatusOk) {
            //     WisAdapterDestroy(adapter);
            //     break;
            // }
            WisAdapterDestroy(adapter);
        } else {
            break;
        }
    }

    WisFactoryDestroy(factory);
    return 0;
}

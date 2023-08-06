// #include <example/app.h>

// #include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/bindings/vulkan.h>
#include <stdio.h>

int main()
{
    // Test::App a{ 1920, 1080 };
    // return a.Start();

    wisVKFactory factory = wisVKFactoryCreate(NULL, NULL);
    char storage[16];
    wisVKFactoryGetAdapterStorage(factory, AdapterPreference_Performance, storage);

    wisVKAdapter adapter;
    //wisVKDevice device;
    while (adapter = wisVKFactoryGetNextAdapter(storage)) {
        struct wisAdapterDesc desc;
        wisVKAdapterGetDesc(adapter, &desc);
        printf("%s\n", desc.description);
        //device = wisVKDeviceCreate(adapter, NULL);
    }
    //if (device)
        //wisVKDeviceDestroy(device, NULL);
    wisVKFactoryFreeAdapterStorage(storage);
    wisVKFactoryDestroy(factory, NULL);


    return 0;
}
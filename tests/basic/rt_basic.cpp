#include <wisdom/wisdom.h>
#include <wisdom/wisdom_raytracing.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("check_rt")
{
    WisRaytracingExtension ext{0};
    REQUIRE(!wisHandleValid(&ext));

    wisInitRaytracingExtension(&ext);
    REQUIRE(wisHandleValid(&ext));

    wisDestroyRaytracingExtension(&ext);
    REQUIRE(!wisHandleValid(&ext));
}

TEST_CASE("check_rt_support")
{
    WisRaytracingExtension ext{0};
    wisInitRaytracingExtension(&ext);

    WisInstance instance = {0};
    wisCreateInstance(NULL, NULL, 0, &instance);

    WisAdapterQuery adapter_query = {0};
    wisInstanceQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapter_query);

    WisDeviceExtensionHeader* extensions[] = {
        &ext.header,
    };
    WisDevice device = {0};
    WisCommandQueueDesc queue_descs[] = {
        {WisCommandQueueTypeGraphics, WisCommandQueuePriorityHigh},
    };
    WisDeviceRequirements requirements = {
        .queue_descs = queue_descs,
        .queue_desc_count = sizeof(queue_descs) / sizeof(queue_descs[0]),
        .extensions = extensions,
        .extension_count = sizeof(extensions) / sizeof(extensions[0]),
    };
    bool supported = false;
    for (size_t i = 0; i < wisAdapterQueryGetAdapterCount(&adapter_query); ++i) {
        WisResult res = wisAdapterQueryCreateDevice(&adapter_query, i, &requirements, &device);
        if (res.status == WisStatusOk) {
            supported = wisRaytracingExtensionSupported(&ext);
            wisDestroyDevice(&device);
            break;
        }
    }

    printf("Raytracing supported: %s\n", supported ? "Yes" : "No");
    wisDestroyRaytracingExtension(&ext);
    wisDestroyAdapterQuery(&adapter_query);
    wisDestroyInstance(&instance);
}

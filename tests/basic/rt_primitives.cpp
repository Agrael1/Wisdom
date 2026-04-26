#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_raytracing.hpp>
#include <catch2/catch_test_macros.hpp>

void log_callback_rt(wis::Severity severity, const char* message, uint64_t device, void* user_data)
{
    const char* severity_str = "";
    switch (severity) {
    case wis::Severity::Verbose:
        severity_str = "VERBOSE";
        break;
    case wis::Severity::Info:
        severity_str = "INFO";
        break;
    case wis::Severity::Warning:
        severity_str = "WARNING";
        break;
    case wis::Severity::Error:
        severity_str = "ERROR";
        printf("[%s] %s\n", severity_str, message);
        break;
    case wis::Severity::Fatal:
        severity_str = "FATAL";
        printf("[%s] %s\n", severity_str, message);
        FAIL();
        break;
    default:
        severity_str = "UNKNOWN";
        break;
    }
    printf("[%s] %s\n", severity_str, message);
}

TEST_CASE("check_rt_acceleration_structure")
{
    wis::Result result{};
    wis::DebugDesc debug{
        .enable_debug_layer = true,
        .callback = log_callback_rt,
        .user_data = nullptr,
    };

    wis::Instance instance = wis::CreateInstance(&debug, {}, result);
    REQUIRE(result.status == wis::Status::Ok);

    wis::AdapterQuery adapter_query = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
    REQUIRE(result.status == wis::Status::Ok);

    wis::RaytracingExtension rt_extension{};

    wis::CommandQueueDesc queue_descs[] = {
        {wis::CommandQueueType::Graphics, wis::CommandQueuePriority::Normal},
    };
    wis::DeviceExtensionHeader* extensions[] = {
        &rt_extension,
    };
    wis::DeviceRequirements device_requirements{
        .queue_descs = queue_descs,
        .extensions = extensions,
    };
    wis::Device device{};
    for (size_t i = 0; i < adapter_query.GetAdapterCount(); ++i) {
        device = adapter_query.CreateDevice(i, device_requirements, result);
        if (result.status == wis::Status::Ok) {
            break;
        }
    }

    // If we don't have RT support -> skip the test.
    if (result.status != wis::Status::Ok) {
        printf(
            "Failed to create device for raytracing test: %d, platform_code: %d, error: %s\n",
            result.status,
            result.platform_code,
            result.error ? result.error : "None"
        );
        return;
    }

    wis::ResourceAllocator allocator = device.GetResourceAllocator(result);
    REQUIRE(result.status == wis::Status::Ok);

    wis::Buffer rtas_buffer = allocator.CreateBuffer(
        {
            .size_bytes = 1024,
            .usage_flags = wis::BufferUsageFlags::AccelerationStructureBuffer,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);

    wis::Buffer scratch_buffer = allocator.CreateBuffer(
        {
            .size_bytes = 1024,
            .usage_flags = wis::BufferUsageFlags::StorageBuffer,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);

    // we won't update the as
    wis::AccelerationStructure blas = rt_extension.CreateAccelerationStructure(
        rtas_buffer,
        {
            .level = wis::AccelerationStructureLevel::BottomLevel,
            .offset = 0,
            .size = 1024,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);

    uint64_t gpu_address = blas.GetGPUAddress();
    REQUIRE(gpu_address != 0);
}


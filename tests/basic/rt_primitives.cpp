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

    wis::Buffer vertex_buffer = allocator.CreateBuffer(
        {
            .size_bytes = 3 * sizeof(float) * 3,
            .usage_flags = wis::BufferUsageFlags::VertexBuffer,
            .memory_type = wis::MemoryType::Upload,
            .memory_flags = wis::MemoryFlags::Mapped,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);

    float* vertex_data = static_cast<float*>(vertex_buffer.Map());
    REQUIRE(vertex_data != nullptr);

    // fill in standard triangle vertices
    vertex_data[0] = 0.0f;
    vertex_data[1] = 0.0f;
    vertex_data[2] = 0.0f;

    vertex_data[3] = 1.0f;
    vertex_data[4] = 0.0f;
    vertex_data[5] = 0.0f;

    vertex_data[6] = 0.0f;
    vertex_data[7] = 1.0f;
    vertex_data[8] = 0.0f;

    wis::AcceleratedGeometryDesc geometry_desc{
        .type = wis::GeometryType::Triangles,
        .flags = wis::GeometryFlags::Opaque,
        .vertex_or_aabb_buffer_address = vertex_buffer.GetGPUAddress(),
        .vertex_or_aabb_stride = 3 * sizeof(float),
        .vertex_count = 3,
        .triangle_or_aabb_count = 1,
        .vertex_format = wis::DataFormat::RGB32Float,
    };
    wis::BottomLevelStructureBuildDesc blas_build_desc{
        .flags = wis::AccelerationStructureFlags::None,
        .geometry_count = 1,
        .geometries = &geometry_desc,
    };
    wis::TopLevelStructureBuildDesc tlas_build_desc{
        .flags = wis::AccelerationStructureFlags::AllowUpdate,
        .instance_count = 1,
    };

    wis::StructureAllocationInfo alloc_info = rt_extension.GetBottomLevelStructureInfo(blas_build_desc, result);
    REQUIRE(result.status == wis::Status::Ok);
    REQUIRE(alloc_info.structure_size > 0);

    wis::StructureAllocationInfo tlas_alloc_info = rt_extension.GetTopLevelStructureInfo(tlas_build_desc, result);
    REQUIRE(result.status == wis::Status::Ok);
    REQUIRE(tlas_alloc_info.structure_size > 0);
    REQUIRE(tlas_alloc_info.update_size > 0);

    wis::Buffer rtas_buffer = allocator.CreateBuffer(
        {
            .size_bytes = alloc_info.structure_size + tlas_alloc_info.structure_size,
            .usage_flags = wis::BufferUsageFlags::AccelerationStructureBuffer,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);

    wis::Buffer scratch_buffer = allocator.CreateBuffer(
        {
            .size_bytes = alloc_info.scratch_size + tlas_alloc_info.scratch_size,
            .usage_flags = wis::BufferUsageFlags::StorageBuffer,
        },
        result
    );
    REQUIRE(result.status == wis::Status::Ok);

    // we won't update the as
    wis::AccelerationStructure rtas[2];
    wis::AccelerationStructureDesc descs[]{
        {
            .level = wis::AccelerationStructureLevel::BottomLevel,
            .offset = 0,
            .size = alloc_info.structure_size,
        },
        {
            .level = wis::AccelerationStructureLevel::TopLevel,
            .offset = alloc_info.structure_size,
            .size = tlas_alloc_info.structure_size,
        }
    };
    result = rt_extension.CreateAccelerationStructures(rtas_buffer, descs, rtas);
    REQUIRE(result.status == wis::Status::Ok);

    uint64_t blas_gpu_address = rtas[0].GetGPUAddress();
    REQUIRE(blas_gpu_address != 0);
    uint64_t tlas_gpu_address = rtas[1].GetGPUAddress();
    REQUIRE(tlas_gpu_address != 0);
}

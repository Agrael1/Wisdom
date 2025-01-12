#include <wis_helper.h>
#include <wis_swapchain.h>
#include <wisdom/wisdom_raytracing.hpp>
#include <window.h>
#include <iostream>

#include <glm/vec3.hpp>

// In order to render with multiview, we need to have texture with array layers.
// In case of stereo rendering, we need to have 2 array layers.
// Stereo example is not implemented here, but it is possible to implement it by
// creating a swapchain with stereo=true. Stereo rendering is not supported by all graphics cards.
// You can check if stereo is supported by calling swap.StereoSupported().

// In this example, we will render to the texture with 2 array layers, imitating stereo rendering.
// One layer will be rendered with red color, and the other with blue color.

class App
{
    // Standard setup
    ex::Window window;
    ex::ExampleSetup setup;
    ex::Swapchain swap;
    ex::FramedCommandList cmd_list;
    ex::FramedCommandList copy_cmd_list;

    // Resources
    wis::Raytracing raytracing_extension;

    wis::Texture uav_texture;
    wis::Shader raygen_shader;

    wis::Buffer vertex_buffer;
    wis::Buffer index_buffer;

    wis::Buffer rtas_buffer;
    wis::Buffer rtas_scratch_buffer;
    wis::Buffer rtas_update_buffer;
    wis::Buffer rtas_instance_buffer;
    wis::Buffer sbt_buffer;

    wis::AccelerationStructure top_rtas;
    wis::AccelerationStructure bottom_rtas;

    wis::UnorderedAccessTexture uav_output;

    wis::RootSignature rt_root_signature;
    wis::DescriptorStorage rt_descriptor_storage;
    wis::RaytracingPipeline rt_pipeline;

    wis::RaytracingDispatchDesc rt_dispatch_desc; // just for convenience

public:
    App()
        : window("Raytracing", 800, 600)
    {
        wis::Result result = wis::success;
        wis::DeviceExtension* device_exts[] = { &raytracing_extension };
        setup.InitDefault(window.GetPlatformExtension(), device_exts);
        auto [w, h] = window.PixelSize();
        std::construct_at(&swap, setup.device, window.CreateSwapchain(result, setup, ex::swapchain_format), w, h);
        cmd_list = setup.CreateLists();

        // Load shaders
        auto buf = ex::LoadShader("shaders/raytracing.lib");
        raygen_shader = setup.device.CreateShader(result, buf.data(), uint32_t(buf.size()));

        // Create resources
        CreatePipeline();
        CreateSizeDependentResources(w, h);

        // Create ...
        CreatePrimitives();
        CreateAccelerationStructures();
        MakeTransitions();
    }

public:
    void Run()
    {
        while (true) {
            if (!ProcessEvents()) {
                break;
            }

            Frame();
        }
    }
    // Process window events
    bool ProcessEvents()
    {
        // Window events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_EVENT_WINDOW_RESIZED: {
                auto [w, h] = window.PixelSize();
                swap.Resize(setup.device, w, h);
                CreateSizeDependentResources(w, h);
                break;
            }
            case SDL_EVENT_QUIT:
                swap.Throttle(); // wait for GPU to finish, then exit
                return false;
            case SDL_EVENT_KEY_DOWN:
                OnKeyPressed(e);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                OnMouseButtonDown(e);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                OnMouseMove(e);
                break;
            default:
                break;
            }
        }
        return true;
    }
    void OnKeyPressed(const SDL_Event& event)
    {
        // Keyboard events
    }
    void OnMouseButtonDown(const SDL_Event& event)
    {
        // Mouse events
    }
    void OnMouseMove(const SDL_Event& event)
    {
        // Mouse events
    }

    // Render a frame
    void Frame()
    {
        uint32_t frame_index = swap.CurrentFrame();
        auto& swap_texture = swap.GetTexture(frame_index);

        auto& cmd = cmd_list[frame_index];
        std::ignore = cmd.Reset();

        raytracing_extension.SetRootSignature(cmd, rt_root_signature);
        raytracing_extension.SetDescriptorStorage(cmd, rt_descriptor_storage);
        raytracing_extension.SetPipelineState(cmd, rt_pipeline);
        raytracing_extension.DispatchRays(cmd, rt_dispatch_desc);
        // barrier for UAV texture

        wis::TextureBarrier2 before[] = {
            { .barrier = { .sync_before = wis::BarrierSync::Compute,
                           .sync_after = wis::BarrierSync::Copy,
                           .access_before = wis::ResourceAccess::UnorderedAccess,
                           .access_after = wis::ResourceAccess::CopySource,
                           .state_before = wis::TextureState::UnorderedAccess,
                           .state_after = wis::TextureState::CopySource },
              .texture = uav_texture },
            // swapchain
            { .barrier = { .sync_before = wis::BarrierSync::None,
                           .sync_after = wis::BarrierSync::Copy,
                           .access_before = wis::ResourceAccess::NoAccess,
                           .access_after = wis::ResourceAccess::CopyDest,
                           .state_before = wis::TextureState::Present,
                           .state_after = wis::TextureState::CopyDest },
              .texture = swap_texture },
        };

        cmd.TextureBarriers(before, std::size(before));

        wis::CopyTextureRegion region{
            .src = {
                    .size = { rt_dispatch_desc.width, rt_dispatch_desc.height, 1 },
                    .format = ex::swapchain_format,
            },
            .dst = {
                    .size = { rt_dispatch_desc.width, rt_dispatch_desc.height, 1 },
                    .format = ex::swapchain_format,
            },
        };
        cmd.CopyTexture(uav_texture, swap_texture, &region, 1);

        wis::TextureBarrier2 after[] = {
            { .barrier = { .sync_before = wis::BarrierSync::Copy,
                           .sync_after = wis::BarrierSync::Compute,
                           .access_before = wis::ResourceAccess::CopySource,
                           .access_after = wis::ResourceAccess::UnorderedAccess,
                           .state_before = wis::TextureState::CopySource,
                           .state_after = wis::TextureState::UnorderedAccess },
              .texture = uav_texture },
            // swapchain
            { .barrier = { .sync_before = wis::BarrierSync::Copy,
                           .sync_after = wis::BarrierSync::None,
                           .access_before = wis::ResourceAccess::CopyDest,
                           .access_after = wis::ResourceAccess::NoAccess,
                           .state_before = wis::TextureState::CopyDest,
                           .state_after = wis::TextureState::Present },
              .texture = swap_texture },
        };
        cmd.TextureBarriers(after, std::size(after));

        std::ignore = cmd.Close();

        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        swap.Present(setup.queue);
    }

private:
    void CreateSizeDependentResources(uint32_t width, uint32_t height)
    {
        using namespace wis; // for flag operators
        wis::Result result = wis::success;

        // Create UAV texture
        wis::TextureDesc desc{
            .format = ex::swapchain_format,
            .size = { width, height, 1 },
            .usage = wis::TextureUsage::CopySrc | wis::TextureUsage::UnorderedAccess,
        };
        uav_texture = setup.allocator.CreateTexture(result, desc);

        // Create UAV output
        wis::UnorderedAccessDesc uav_desc{
            .format = ex::swapchain_format,
            .view_type = wis::TextureViewType::Texture2D,
            .subresource_range = { 0, 1, 0, 1 },
        };
        uav_output = setup.device.CreateUnorderedAccessTexture(result, uav_texture, uav_desc);

        // Write to descriptor storage
        rt_descriptor_storage.WriteRWTexture(0, 0, uav_output);

        // Update dispatch desc
        rt_dispatch_desc.width = width;
        rt_dispatch_desc.height = height;
        rt_dispatch_desc.depth = 1;
    }
    void CreatePrimitives()
    {
        // clang-format off
        constexpr static float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };
        // clang-format on
        constexpr static uint16_t indices[] = { 0, 1, 2 };

        wis::Result result = wis::success;
        vertex_buffer = setup.allocator.CreateBuffer(result, sizeof(vertices), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);
        index_buffer = setup.allocator.CreateBuffer(result, sizeof(indices), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);

        auto memory = vertex_buffer.Map<float>();
        std::copy_n(vertices, std::size(vertices), memory);
        vertex_buffer.Unmap();

        auto memory2 = index_buffer.Map<uint16_t>();
        std::copy_n(indices, std::size(indices), memory);
        index_buffer.Unmap();
    }

    void MakeTransitions()
    {
        auto& cmd = cmd_list[0];
        std::ignore = cmd.Reset();
        // Transition UAV texture to UAV state
        cmd.TextureBarrier({ .sync_before = wis::BarrierSync::None,
                             .sync_after = wis::BarrierSync::None,
                             .access_before = wis::ResourceAccess::NoAccess,
                             .access_after = wis::ResourceAccess::NoAccess,
                             .state_before = wis::TextureState::Undefined,
                             .state_after = wis::TextureState::UnorderedAccess },
                           uav_texture);
        cmd.Close();

        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        setup.WaitForGPU();
    }
    void CreateAccelerationStructures()
    {
        using namespace wis; // for flag operators
        wis::Result result = wis::success;

        rtas_instance_buffer = setup.allocator.CreateBuffer(result, sizeof(wis::AccelerationInstance), wis::BufferUsage::AccelerationStructureInput, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);

        // get tlas size
        wis::TopLevelASBuildDesc build_desc{
            .flags = wis::AccelerationStructureFlags::AllowUpdate,
            .instance_count = 1,
            .gpu_address = rtas_instance_buffer.GetGPUAddress(),
            .indirect = false,
            .update = false,
        };
        auto as_size = raytracing_extension.GetTopLevelASSize(build_desc);

        // get blas size
        wis::AcceleratedGeometryInput geometry_input{
            .geometry_type = wis::ASGeometryType::Triangles,
            .flags = wis::ASGeometryFlags::Opaque,
            .vertex_or_aabb_buffer_address = vertex_buffer.GetGPUAddress(),
            .vertex_or_aabb_buffer_stride = sizeof(float[3]),
            .index_buffer_address = index_buffer.GetGPUAddress(),
            .transform_matrix_address = 0,
            .vertex_count = 3,
            .triangle_or_aabb_count = 1,
            .vertex_format = wis::DataFormat::RGB32Float,
            .index_format = wis::IndexType::UInt16,
        };
        wis::AcceleratedGeometryDesc geometry_desc = wis::CreateGeometryDesc(geometry_input);
        wis::BottomLevelASBuildDesc blas_desc{
            .flags = wis::AccelerationStructureFlags::None,
            .geometry_count = 1,
            .geometry_array = &geometry_desc,
            .update = false,
        };
        auto blas_size = raytracing_extension.GetBottomLevelASSize(blas_desc);

        rtas_buffer = setup.allocator.CreateBuffer(result, as_size.result_size + blas_size.result_size, wis::BufferUsage::AccelerationStructureBuffer);
        rtas_scratch_buffer = setup.allocator.CreateBuffer(result, as_size.scratch_size + blas_size.scratch_size, wis::BufferUsage::StorageBuffer);
        rtas_update_buffer = setup.allocator.CreateBuffer(result, as_size.update_size, wis::BufferUsage::StorageBuffer);

        top_rtas = raytracing_extension.CreateAccelerationStructure(result, rtas_buffer, 0, as_size.result_size, wis::ASLevel::Top);
        bottom_rtas = raytracing_extension.CreateAccelerationStructure(result, rtas_buffer, as_size.result_size, blas_size.result_size, wis::ASLevel::Bottom);

        // Fill instance buffer
        rtas_instance_buffer.Map<wis::AccelerationInstance>()[0] = {
            .transform = {
                    { 1.0f, 0.0f, 0.0f, 0.0f },
                    { 0.0f, 1.0f, 0.0f, 0.0f },
                    { 0.0f, 0.0f, 1.0f, 0.0f },
            },
            .instance_id = 0,
            .mask = 0xFF,
            .instance_offset = 0,
            .flags = uint32_t(wis::ASInstanceFlags::TriangleCullDisable),
            .acceleration_structure_handle = raytracing_extension.GetAccelerationStructureDeviceAddress(bottom_rtas),
        };
        rtas_instance_buffer.Unmap();

        // Build acceleration structures
        auto& cmd = cmd_list[0];
        std::ignore = cmd.Reset();
        raytracing_extension.BuildBottomLevelAS(cmd, blas_desc, bottom_rtas, rtas_scratch_buffer.GetGPUAddress() + as_size.scratch_size);
        // Add a barrier to make sure the BLAS is built before the TLAS build
        cmd.BufferBarrier({ .sync_before = wis::BarrierSync::BuildRTAS,
                            .sync_after = wis::BarrierSync::BuildRTAS,
                            .access_before = wis::ResourceAccess::AccelerationStructureWrite,
                            .access_after = wis::ResourceAccess::AccelerationStructureRead | wis::ResourceAccess::AccelerationStructureWrite },
                          rtas_buffer);

        raytracing_extension.BuildTopLevelAS(cmd, build_desc, top_rtas, rtas_scratch_buffer.GetGPUAddress());
        cmd.Close();

        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        setup.WaitForGPU();

        // Write acceleration structure to descriptor storage
        raytracing_extension.WriteAccelerationStructure(rt_descriptor_storage, 1, 0, top_rtas);
    }

    void CreatePipeline()
    {
        wis::Result result = wis::success;
        wis::DescriptorBindingDesc bindings[] = {
            { .binding_type = wis::DescriptorType::RWTexture, .binding_space = 0, .binding_count = ex::flight_frames },
            { .binding_type = wis::DescriptorType::AccelerationStructure, .binding_space = 1, .binding_count = 1 },
        };
        rt_descriptor_storage = setup.device.CreateDescriptorStorage(result, bindings, std::size(bindings));
        rt_root_signature = setup.device.CreateRootSignature(result, nullptr, 0, nullptr, 0, bindings, std::size(bindings));

        // Create pipeline
        wis::ShaderView shaders[]{
            raygen_shader, raygen_shader
        };
        wis::ShaderExport exports[]{
            { .entry_point = "RayGeneration", .shader_type = wis::RaytracingShaderType::Raygen, .shader_array_index = 1 },
            { .entry_point = "Miss", .shader_type = wis::RaytracingShaderType::Miss, .shader_array_index = 0 },
            { .entry_point = "ClosestHit", .shader_type = wis::RaytracingShaderType::ClosestHit, .shader_array_index = 1 },
        };
        wis::HitGroupDesc hit_groups[]{
            { .type = wis::HitGroupType::Triangles, .closest_hit_export_index = 2 },
        };
        wis::RaytracingPipelineDesc rt_pipeline_desc{
            .root_signature = rt_root_signature,
            .shaders = shaders,
            .shader_count = std::size(shaders),
            .exports = exports,
            .export_count = std::size(exports),
            .hit_groups = hit_groups,
            .hit_group_count = std::size(hit_groups),
            .max_recursion_depth = 1,
            .max_payload_size = 24,
            .max_attribute_size = 8,
        };
        rt_pipeline = raytracing_extension.CreateRaytracingPipeline(result, rt_pipeline_desc);

        // Create shader binding table
        wis::ShaderBindingTableInfo sbt_info = raytracing_extension.GetShaderBindingTableInfo();

        const uint8_t* shader_ident = rt_pipeline.GetShaderIdentifiers();

        // 1 raygen, 1 miss, 1 hit group
        sbt_buffer = setup.allocator.CreateBuffer(result, 1024, wis::BufferUsage::ShaderBindingTable, wis::MemoryType::Upload, wis::MemoryFlags::Mapped);
        auto memory = sbt_buffer.Map<uint8_t>();

        // raygen
        uint32_t table_increment = wis::detail::aligned_size(sbt_info.entry_size, sbt_info.table_start_alignment); // not real, just for demonstration

        // copies should have size of entry_size, only the last one should have the size aligned to table_start_alignment
        std::memcpy(memory, shader_ident, sbt_info.entry_size);
        memory += table_increment;

        // miss
        std::memcpy(memory, shader_ident + sbt_info.entry_size, sbt_info.entry_size);
        memory += table_increment;

        // hit group
        std::memcpy(memory, shader_ident + sbt_info.entry_size * 2, sbt_info.entry_size);
        memory += table_increment;
        sbt_buffer.Unmap();

        auto gpu_address = sbt_buffer.GetGPUAddress();

        rt_dispatch_desc.ray_gen_shader_table_address = gpu_address;
        rt_dispatch_desc.miss_shader_table_address = gpu_address + table_increment;
        rt_dispatch_desc.hit_group_table_address = gpu_address + table_increment * 2;
        rt_dispatch_desc.callable_shader_table_address = 0;
        rt_dispatch_desc.ray_gen_shader_table_size = sbt_info.entry_size;
        rt_dispatch_desc.miss_shader_table_size = sbt_info.entry_size;
        rt_dispatch_desc.hit_group_table_size = sbt_info.entry_size;
        rt_dispatch_desc.callable_shader_table_size = 0;
        rt_dispatch_desc.miss_shader_table_stride = sbt_info.entry_size;
        rt_dispatch_desc.hit_group_table_stride = sbt_info.entry_size;
        rt_dispatch_desc.callable_shader_table_stride = 0;
    }

private:
    void CopyTextureToSwapchain()
    {
    }
};

int main(int argc, char** argv)
{
    try {
        App{}.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

 #include <wisdom/vulkan/vk_command_list.h>
#ifndef WISDOM_MODULES
#include <wisdom/util/small_allocator.h>
#include <wisdom/global/definitions.h>
#endif

void wis::VKCommandList::BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer) noexcept
{
    auto acc_before = convert_vk(barrier.access_before);
    auto acc_after = convert_vk(barrier.access_after);

    if (!buffer || acc_before == acc_after)
        return;

    vk::BufferMemoryBarrier2 desc{
        convert_vk(barrier.sync_before),
        acc_before,
        convert_vk(barrier.sync_after),
        acc_after,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        buffer,
        0,
        VK_WHOLE_SIZE
    };
    vk::DependencyInfo depinfo{
        vk::DependencyFlagBits::eByRegion,
        0, nullptr,
        1, &desc,
        0, nullptr,
    };
    command_list.pipelineBarrier2(&depinfo);
}

void wis::VKCommandList::TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture) noexcept
{
    vk::ImageLayout vk_state_before = convert_vk(barrier.state_before);
    vk::ImageLayout vk_state_after = convert_vk(barrier.state_after);

    auto acc_before = convert_vk(barrier.access_before);
    auto acc_after = convert_vk(barrier.access_after);

    if (!texture.image || (vk_state_before == vk_state_after && acc_before == acc_after))
        return;

    vk::ImageMemoryBarrier2 image_memory_barrier{
        convert_vk(barrier.sync_before),
        acc_before,
        convert_vk(barrier.sync_after),
        acc_after,
        vk_state_before,
        vk_state_after,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        texture.image,
        convert_vk(barrier.range, texture.format)
    };
    vk::DependencyInfo depinfo{
        vk::DependencyFlagBits::eByRegion,
        0, nullptr,
        0, nullptr,
        1, &image_memory_barrier
    };
    command_list.pipelineBarrier2(&depinfo);
}

void wis::VKCommandList::IASetVertexBuffers(std::span<const VKVertexBufferView> resources, uint32_t start_slot) noexcept
{
    static constexpr size_t max_expected_entries = 16;
    assert(resources.size() <= max_expected_entries);
    std::array<vk::Buffer, max_expected_entries> buffers{};
    std::array<vk::DeviceSize, max_expected_entries> strides{};
    std::array<vk::DeviceSize, max_expected_entries> sizes{};
    constexpr static std::array<vk::DeviceSize, 16> offsets{};

    for (size_t i = 0; i < resources.size(); i++) {
        const auto& ii = resources[i].GetInternal();

        buffers[i] = ii.buffer;
        sizes[i] = ii.size_bytes;
        strides[i] = ii.stride_bytes;
    }

    command_list.bindVertexBuffers2(start_slot, uint32_t(resources.size()), buffers.data(), offsets.data(), sizes.data(), strides.data());
}

void wis::VKCommandList::BeginRenderPass(wis::VKRenderPassView rp,
                                         std::span<const std::pair<VKRenderTarget, ColorClear>> render_targets,
                                         std::pair<VKDepthStencil, DepthClear> depth) noexcept
{
    wis::internals::uniform_allocator<vk::ImageView, max_render_targets> image_views;
    wis::internals::uniform_allocator<vk::ClearValue, max_render_targets> image_clear;
    for (const auto& i : render_targets) {
        image_views.allocate(i.first.GetInternal().view.get());
        image_clear.allocate().setColor(i.second);
    }
    if (auto iv = depth.first.GetInternal().view.get()) {
        image_views.allocate(iv);
        image_clear.allocate().setDepthStencil(depth.second);
    }

    vk::RenderPassAttachmentBeginInfo attachment_begin_info{
        uint32_t(image_views.size()),
        image_views.data()
    };
    vk::RenderPassBeginInfo render_pass_info{
        rp.pass, rp.frame, vk::Rect2D{ { 0, 0 }, { rp.frame_size.width, rp.frame_size.height } }, uint32_t(image_clear.size()), image_clear.data(), &attachment_begin_info
    };
    command_list.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
}

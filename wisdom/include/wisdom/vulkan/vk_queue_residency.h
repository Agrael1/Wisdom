#pragma once
#include <atomic>
#include <vulkan/vulkan.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/util/flags.h>
#include <wisdom/util/small_allocator.h>

namespace wis::detail {
enum class QueueTypes : uint8_t { Graphics,
                                  Compute,
                                  Copy,
                                  VideoDecode,
                                  Count };
struct QueueResidency {
    struct QueueInfo {
        uint32_t index;
        uint32_t count;
    };
    struct QueueFormat {
    public:
        QueueFormat() noexcept = default;
        QueueFormat(uint16_t flags, uint8_t count, uint8_t family_index) noexcept
            : queue_flags(flags), count(count), family_index(family_index) { }
        QueueFormat(const QueueFormat&) = delete;
        QueueFormat& operator=(const QueueFormat&) = delete;
        QueueFormat(QueueFormat&& o) noexcept
            : queue_flags(o.queue_flags), count(o.count), family_index(o.family_index) { }
        QueueFormat& operator=(QueueFormat&& o) noexcept
        {
            queue_flags = o.queue_flags;
            count = o.count;
            family_index = o.family_index;
            return *this;
        }

        uint8_t GetNextInLine() const noexcept { return last.exchange((last + 1) % count); }
        bool Empty() const noexcept { return count == 0u; }

        uint16_t queue_flags = 0;
        uint8_t count = 0;
        uint8_t family_index = 0;
        mutable std::atomic<uint8_t> last{ 0 };
    };
    static constexpr size_t QueueIndex(QueueType type)
    {
        switch (type) {
        case wis::QueueType::Compute:
            return size_t(QueueTypes::Compute);
        case wis::QueueType::Copy:
            return size_t(QueueTypes::Copy);
        case wis::QueueType::VideoDecode:
            return size_t(QueueTypes::VideoDecode);
        default:
            return size_t(QueueTypes::Graphics);
        }
    }
    static constexpr size_t QueueFlag(QueueTypes type)
    {
        switch (type) {
        case QueueTypes::Copy:
            return VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT;
        case QueueTypes::Compute:
            return VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT;
        case QueueTypes::Graphics:
            return VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT;
        case QueueTypes::VideoDecode:
            return VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR;
        default:
            return 0;
        }
    }

    QueueResidency() noexcept = default;
    QueueResidency(const QueueResidency&) = delete;
    QueueResidency& operator=(const QueueResidency&) = delete;
    QueueResidency(QueueResidency&& o) noexcept
        : available_queues(std::move(o.available_queues)) { }
    QueueResidency& operator=(QueueResidency&& o) noexcept
    {
        available_queues = std::move(o.available_queues);
        return *this;
    }

public:
    const QueueFormat* GetOfType(QueueType type) const noexcept
    {
        auto idx = QueueIndex(type);
        const auto* q = &available_queues[idx];

        if (q->count == 0u) {
            idx = FindResembling(QueueTypes(idx));
            if (idx == -1)
                return nullptr;
            q = &available_queues[idx];
        }
        return q;
    }
    int32_t FindResembling(QueueTypes type) const noexcept
    {
        for (size_t i = 0; i < size_t(QueueTypes::Count); i++) {
            const auto& r = available_queues[i];
            if ((r.queue_flags & QueueFlag(type)) != 0u)
                return static_cast<int>(i);
        }
        return -1;
    }

    std::array<QueueFormat, size_t(QueueTypes::Count)> available_queues{};
};
} // namespace wis::detail
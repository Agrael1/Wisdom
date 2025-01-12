#ifndef WIS_VK_RAYTRACING_PIPELINE_H
#define WIS_VK_RAYTRACING_PIPELINE_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_handles.h>

namespace wis {
class VKRaytracingPipeline;

template<>
struct Internal<VKRaytracingPipeline> {
    wis::managed_handle_ex<VkPipeline> state_object;
    std::unique_ptr<uint8_t[]> shader_identifiers;
};

class VKRaytracingPipeline : public QueryInternal<VKRaytracingPipeline>
{
public:
    VKRaytracingPipeline() noexcept = default;
    // view operator
    const uint8_t* GetShaderIdentifiers() const noexcept
    {
        return shader_identifiers.get();
    }
    operator bool() const noexcept
    {
        return bool(state_object);
    }
};
} // namespace wis
#endif // !WIS_VK_RAYTRACING_PIPELINE_H
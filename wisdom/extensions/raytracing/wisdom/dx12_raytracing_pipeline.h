#ifndef WIS_DX12_RAYTRACING_PIPELINE_H
#define WIS_DX12_RAYTRACING_PIPELINE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3d12.h>
#include <memory>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
class DX12RaytracingPipeline;

using DX12RaytracingPipelineView = std::tuple<ID3D12StateObject*>;

template<>
struct Internal<DX12RaytracingPipeline> {
    wis::com_ptr<ID3D12StateObject> state_object;
    std::unique_ptr<uint8_t[]> shader_identifiers;
};

class DX12RaytracingPipeline : public QueryInternal<DX12RaytracingPipeline>
{
public:
    DX12RaytracingPipeline() noexcept = default;
    operator DX12RaytracingPipelineView() noexcept
    {
        return { state_object.get() };
    }
    const uint8_t* GetShaderIdentifiers() const noexcept
    {
        return shader_identifiers.get();
    }
    operator bool() const noexcept
    {
        return state_object.get() != nullptr;
    }
};
} // namespace wis
#endif // !WIS_DX12_RAYTRACING_PIPELINE_H

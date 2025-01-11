#ifndef WIS_DX12_RAYTRACING_PIPELINE_H
#define WIS_DX12_RAYTRACING_PIPELINE_H
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3d12.h>

namespace wis {
class DX12RaytracingPipeline;

template<>
struct Internal<DX12RaytracingPipeline> {
    wis::com_ptr<ID3D12StateObject> state_object;
};

class DX12RaytracingPipeline : public QueryInternal<DX12RaytracingPipeline>
{
public:
    DX12RaytracingPipeline() noexcept = default;
    // view operator
    operator bool() const noexcept
    {
        return bool(state_object);
    }
};
} // namespace wis
#endif // !WIS_DX12_RAYTRACING_PIPELINE_H
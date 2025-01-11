#ifndef WIS_RAYTRACING_IMPL_H
#define WIS_RAYTRACING_IMPL_H
#include <cstdint>
namespace wis {
struct AccelerationInstance {
    float transform[3][4];
    uint32_t instance_id : 24;
    uint32_t mask : 8;
    uint32_t instance_offset : 24;
    uint32_t flags : 8;
    uint64_t acceleration_structure_handle;
};
} // namespace wis
#endif 
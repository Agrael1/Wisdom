#include "wisdom.h"
#include "wisdom_ext.cpp"

#ifdef _WIN32
#define ALIGNED_ALLOC(size, align) _aligned_malloc(size, align)
#define ALIGNED_FREE(ptr) _aligned_free(ptr)
#else
#define ALIGNED_ALLOC(size, align) aligned_alloc(align, size)
#define ALIGNED_FREE(ptr) free(ptr)
#endif // _WIN32


template<typename T>
struct GetExtSize {
    consteval size_t operator()() const noexcept
    {
        if constexpr (std::same_as<T, void>) {
            return 0;
        } else {
            return sizeof(T);
        }
    }
};

template<typename T>
struct GetExtAlign {
    consteval size_t operator()() const noexcept
    {
        if constexpr (std::same_as<T, void>) {
            return 0;
        } else {
            return alignof(T);
        }
    }
};

template<typename T>
struct CreateExt {
    constexpr void operator()(void* memory) const noexcept
    {
        std::construct_at<T>((T*)memory);
    }
};
template<typename T>
struct DestroyExt {
    constexpr void operator()(void* memory) const noexcept
    {
        std::destroy_at<T>((T*)memory);
    }
};

template<typename T>
struct GetExtSupported {
    bool operator()(void* memory) const noexcept {
        return static_cast<T*>(memory)->Supported();
    }
};

static inline uint8_t* wisdom_alloc(size_t size, size_t align)
{
    return static_cast<uint8_t*>(ALIGNED_ALLOC(size, align));
}

static inline void wisdom_free(uint8_t* ptr)
{
    ALIGNED_FREE(ptr);
}

template<typename T, typename EnumT>
struct ExtLayout {
    T* pre_type;
    std::span<EnumT> exts;
    uint8_t* ext_memory;

    operator bool() const noexcept
    {
        return pre_type != nullptr;
    }
    uint8_t* data() const noexcept
    {
        return reinterpret_cast<uint8_t*>(pre_type);
    }
};

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_dx12.hpp>

template<typename T, template<typename X> typename A, typename... Args>
static inline decltype(auto) DX12BridgeSelector(Args&&... args)
{
    if constexpr (std::same_as<T, wis::DX12Factory>) {
        return DX12FactoryExtensionBridge<A>(std::forward<Args>(args)...);
    } else if constexpr (std::same_as<T, wis::DX12Device>) {
        return DX12DeviceExtensionBridge<A>(std::forward<Args>(args)...);
    }
}

template<typename ExtT, typename PreType, typename EnumT>
static inline ExtLayout<PreType, EnumT> wisdom_alloc_ext_dx12(std::span<ExtT> exts) noexcept
{
    size_t size = 0;
    size_t all_exts = 0;
    size_t first_al = 1;
    for (auto&& ext : exts) {
        if (ext.extension_id == 0) {
            continue;
        }
        size_t align = DX12BridgeSelector<PreType, GetExtAlign>(static_cast<EnumT>(ext.extension_id));
        if (all_exts == 0) {
            first_al = align;
        }

        size = wis::detail::aligned_size(size, align) + DX12BridgeSelector<PreType, GetExtSize>(static_cast<EnumT>(ext.extension_id));
        all_exts++;
    }

    // Add the size of the extension id x all_exts
    size_t array_size = all_exts * sizeof(EnumT); // can be 0
    size_t cap_size = sizeof(uint32_t);

    size_t base_size = wis::detail::aligned_size(sizeof(PreType), alignof(uint32_t)) + cap_size + array_size;
    size_t overall_size = wis::detail::aligned_size(base_size, first_al) + size;

    uint8_t* memory = wisdom_alloc(overall_size, alignof(PreType));
    *reinterpret_cast<uint32_t*>(memory + wis::detail::aligned_size(sizeof(PreType), alignof(uint32_t))) = all_exts;

    return ExtLayout<PreType, EnumT>{
        .pre_type = reinterpret_cast<PreType*>(memory),
        .exts = { reinterpret_cast<EnumT*>(memory + base_size - array_size), all_exts },
        .ext_memory = all_exts ? memory + overall_size - size : nullptr
    };
}
template<typename PreType, typename EnumT>
static inline ExtLayout<PreType, EnumT> from_stream_dx12(uint8_t* stream) noexcept
{
    auto* pre_type = reinterpret_cast<PreType*>(stream);
    uint8_t* ext_count = reinterpret_cast<uint8_t*>(wis::detail::aligned_size(size_t(stream) + sizeof(PreType), alignof(uint32_t)));

    uint32_t count = *reinterpret_cast<uint32_t*>(ext_count);
    if (count == 0) {
        return { pre_type, {}, nullptr };
    }

    auto span = std::span{ reinterpret_cast<EnumT*>(ext_count + sizeof(uint32_t)), count };

    size_t align = DX12BridgeSelector<PreType, GetExtAlign>(span[0]);
    uint8_t* ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)span.data() + span.size_bytes(), align));

    return ExtLayout<PreType, EnumT>{
        .pre_type = pre_type,
        .exts = span,
        .ext_memory = ext_ptr
    };
}

extern "C" WisResult DX12CreateFactory(bool debug_layer, WisFactoryExtQuery* extensions, uint32_t extension_count, DX12Factory* factory)
{
    auto memory = wisdom_alloc_ext_dx12<WisFactoryExtQuery, wis::DX12Factory, wis::FactoryExtID>(std::span{ extensions, extension_count });
    if (!memory) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory." };
    }

    wis::detail::limited_allocator<wis::DX12FactoryExtension*> allocator{ extension_count };
    auto data = allocator.data();
    if (!data) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory extensions." };
    }

    if (extensions) {
        uint8_t* ext_ptr = memory.ext_memory;
        auto ext_ids = memory.exts.data();

        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                auto id = static_cast<wis::FactoryExtID>(extensions[i].extension_id);
                *ext_ids = id;

                size_t align = DX12FactoryExtensionBridge<GetExtAlign>(id);
                ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, align));

                DX12FactoryExtensionBridge<CreateExt>(id, ext_ptr);
                *data = reinterpret_cast<wis::DX12FactoryExtension*>(ext_ptr);
                ext_ptr += DX12FactoryExtensionBridge<GetExtSize>(id);

                ext_ids++;
            } else {
                data[i] = reinterpret_cast<wis::DX12FactoryExtension*>(extensions[i].result);
            }
        }
    }

    auto&& [result, hfactory] = wis::DX12CreateFactory(debug_layer, allocator.data(), extension_count);
    std::construct_at(memory.pre_type, std::move(hfactory));


    if (result.status != wis::Status::Ok) {
        wisdom_free(memory.data());
        return reinterpret_cast<WisResult&>(result);
    }
    // Check if ext is supported
    if (extensions) {
        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                bool supported = DX12FactoryExtensionBridge<GetExtSupported>(static_cast<wis::FactoryExtID>(extensions[i].extension_id), extensions[i].result);
                if (!supported)
                    extensions[i].result = nullptr;
            }
        }
    }

    *factory = reinterpret_cast<DX12Factory>(memory.data());
    return reinterpret_cast<WisResult&>(result);
}

extern "C" void DX12FactoryDestroy(DX12Factory self)
{
    if (!self) {
        return;
    }

    auto memory = from_stream_dx12<wis::DX12Factory, wis::FactoryExtID>(reinterpret_cast<uint8_t*>(self));

    uint8_t* ext_ptr = memory.ext_memory;
    for (auto eid : memory.exts) {
        ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, DX12FactoryExtensionBridge<GetExtAlign>(eid)));
        DX12FactoryExtensionBridge<DestroyExt>(eid, ext_ptr);
        ext_ptr += DX12FactoryExtensionBridge<GetExtSize>(eid);
    }

    std::destroy_at(memory.pre_type);
    wisdom_free(memory.data());
}

//-------------------------------------------------------------------------------------------------

extern "C" WisResult DX12CreateDevice(DX12Adapter adapter, WisDeviceExtQuery* extensions, uint32_t extension_count, bool force, DX12Device* factory)
{
    auto memory = wisdom_alloc_ext_dx12<WisDeviceExtQuery, wis::DX12Device, wis::DeviceExtID>(std::span{ extensions, extension_count });
    if (!memory) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory." };
    }

    wis::detail::limited_allocator<wis::DX12DeviceExtension*> allocator{ extension_count };
    auto data = allocator.data();
    if (!data) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory extensions." };
    }

    if (extensions) {
        uint8_t* ext_ptr = memory.ext_memory;
        auto ext_ids = memory.exts.data();

        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                auto id = static_cast<wis::DeviceExtID>(extensions[i].extension_id);
                *ext_ids = id;

                size_t align = DX12DeviceExtensionBridge<GetExtAlign>(id);
                ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, align));

                *data = reinterpret_cast<wis::DX12DeviceExtension*>(ext_ptr);
                DX12DeviceExtensionBridge<CreateExt>(id, *data);
                extensions[i].result = ext_ptr;
                ext_ptr += DX12DeviceExtensionBridge<GetExtSize>(id);

                ext_ids++;
            } else {
                data[i] = reinterpret_cast<wis::DX12DeviceExtension*>(extensions[i].result);
            }
        }
    }

    auto&& [result, hfactory] = wis::DX12CreateDevice(*reinterpret_cast<wis::DX12Adapter*>(adapter), allocator.data(), extension_count);
    std::construct_at(memory.pre_type, std::move(hfactory));

    if (result.status != wis::Status::Ok) {
        wisdom_free(memory.data());
        return reinterpret_cast<WisResult&>(result);
    }

    for (size_t i = 0; i < extension_count; i++) {
        if (extensions[i].extension_id != 0) {
            bool supported = DX12DeviceExtensionBridge<GetExtSupported>(static_cast<wis::DeviceExtID>(extensions[i].extension_id), extensions[i].result);
            if (!supported)
                extensions[i].result = nullptr;
        }
    }

    *factory = reinterpret_cast<DX12Device>(memory.data());
    return reinterpret_cast<WisResult&>(result);
}

extern "C" void DX12DeviceDestroy(DX12Device self)
{
    if (!self) {
        return;
    }

    auto memory = from_stream_dx12<wis::DX12Device, wis::DeviceExtID>(reinterpret_cast<uint8_t*>(self));

    uint8_t* ext_ptr = memory.ext_memory;
    for (auto eid : memory.exts) {
        ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, DX12DeviceExtensionBridge<GetExtAlign>(eid)));
        DX12DeviceExtensionBridge<DestroyExt>(eid, ext_ptr);
        ext_ptr += DX12DeviceExtensionBridge<GetExtSize>(eid);
    }

    std::destroy_at(memory.pre_type);
    wisdom_free(memory.data());
}
#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/wisdom_vk.hpp>

template<typename T, template<typename X> typename A, typename... Args>
static inline decltype(auto) VKBridgeSelector(Args&&... args)
{
    if constexpr (std::same_as<T, wis::VKFactory>) {
        return VKFactoryExtensionBridge<A>(std::forward<Args>(args)...);
    } else if constexpr (std::same_as<T, wis::VKDevice>) {
        return VKDeviceExtensionBridge<A>(std::forward<Args>(args)...);
    }
}

template<typename ExtT, typename PreType, typename EnumT>
static inline ExtLayout<PreType, EnumT> wisdom_alloc_ext_vk(std::span<ExtT> exts) noexcept
{
    size_t size = 0;
    size_t all_exts = 0;
    size_t first_al = 1;
    for (auto&& ext : exts) {
        if (ext.extension_id == 0) {
            continue;
        }
        size_t align = VKBridgeSelector<PreType, GetExtAlign>(static_cast<EnumT>(ext.extension_id));
        if (all_exts == 0) {
            first_al = align;
        }

        size = wis::detail::aligned_size(size, align) + VKBridgeSelector<PreType, GetExtSize>(static_cast<EnumT>(ext.extension_id));
        all_exts++;
    }

    // Add the size of the extension id x all_exts
    size_t array_size = all_exts * sizeof(EnumT); // can be 0
    size_t cap_size = sizeof(uint32_t);

    size_t base_size = wis::detail::aligned_size(sizeof(PreType), alignof(uint32_t)) + cap_size + array_size;
    size_t overall_size = wis::detail::aligned_size(base_size, first_al) + size;

    uint8_t* memory = wisdom_alloc(overall_size, alignof(PreType));
    *reinterpret_cast<uint32_t*>(memory + wis::detail::aligned_size(sizeof(PreType), alignof(uint32_t))) = all_exts;

    return ExtLayout<PreType, EnumT>{
        .pre_type = reinterpret_cast<PreType*>(memory),
        .exts = { reinterpret_cast<EnumT*>(memory + base_size - array_size), all_exts },
        .ext_memory = all_exts ? memory + overall_size - size : nullptr
    };
}
template<typename PreType, typename EnumT>
static inline ExtLayout<PreType, EnumT> from_stream_vk(uint8_t* stream) noexcept
{
    auto* pre_type = reinterpret_cast<PreType*>(stream);
    uint8_t* ext_count = reinterpret_cast<uint8_t*>(wis::detail::aligned_size(size_t(stream) + sizeof(PreType), alignof(uint32_t)));

    uint32_t count = *reinterpret_cast<uint32_t*>(ext_count);
    if (count == 0) {
        return { pre_type, {}, nullptr };
    }

    auto span = std::span{ reinterpret_cast<EnumT*>(ext_count + sizeof(uint32_t)), count };

    size_t align = VKBridgeSelector<PreType, GetExtAlign>(span[0]);
    uint8_t* ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)span.data() + span.size_bytes(), align));

    return ExtLayout<PreType, EnumT>{
        .pre_type = pre_type,
        .exts = span,
        .ext_memory = ext_ptr
    };
}

extern "C" WisResult VKCreateFactory(bool debug_layer, WisFactoryExtQuery* extensions, uint32_t extension_count, VKFactory* factory)
{
    auto memory = wisdom_alloc_ext_vk<WisFactoryExtQuery, wis::VKFactory, wis::FactoryExtID>(std::span{ extensions, extension_count });
    if (!memory) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory." };
    }

    wis::detail::limited_allocator<wis::VKFactoryExtension*> allocator{ extension_count };
    auto data = allocator.data();
    if (!data) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory extensions." };
    }

    if (extensions) {
        uint8_t* ext_ptr = memory.ext_memory;
        auto ext_ids = memory.exts.data();

        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                auto id = static_cast<wis::FactoryExtID>(extensions[i].extension_id);
                *ext_ids = id;

                size_t align = VKFactoryExtensionBridge<GetExtAlign>(id);
                ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, align));

                VKFactoryExtensionBridge<CreateExt>(id, ext_ptr);
                *data = reinterpret_cast<wis::VKFactoryExtension*>(ext_ptr);
                ext_ptr += VKFactoryExtensionBridge<GetExtSize>(id);

                ext_ids++;
            } else {
                data[i] = reinterpret_cast<wis::VKFactoryExtension*>(extensions[i].result);
            }
        }
    }

    auto&& [result, hfactory] = wis::VKCreateFactory(debug_layer, allocator.data(), extension_count);
    std::construct_at(memory.pre_type, std::move(hfactory));

    if (result.status != wis::Status::Ok) {
        wisdom_free(memory.data());
        return reinterpret_cast<WisResult&>(result);
    }
    // Check if ext is supported
    if (extensions) {
        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                bool supported = VKFactoryExtensionBridge<GetExtSupported>(static_cast<wis::FactoryExtID>(extensions[i].extension_id), extensions[i].result);
                if (!supported)
                    extensions[i].result = nullptr;
            }
        }
    }

    *factory = reinterpret_cast<VKFactory>(memory.data());
    return reinterpret_cast<WisResult&>(result);
}

extern "C" void VKFactoryDestroy(VKFactory self)
{
    if (!self) {
        return;
    }

    auto memory = from_stream_vk<wis::VKFactory, wis::FactoryExtID>(reinterpret_cast<uint8_t*>(self));

    uint8_t* ext_ptr = memory.ext_memory;
    for (auto eid : memory.exts) {
        ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, VKFactoryExtensionBridge<GetExtAlign>(eid)));
        VKFactoryExtensionBridge<DestroyExt>(eid, ext_ptr);
        ext_ptr += VKFactoryExtensionBridge<GetExtSize>(eid);
    }

    std::destroy_at(memory.pre_type);
    wisdom_free(memory.data());
}

extern "C" WisResult VKCreateDevice(VKAdapter adapter, WisDeviceExtQuery* extensions, uint32_t extension_count, bool force, VKDevice* factory)
{
    auto memory = wisdom_alloc_ext_vk<WisDeviceExtQuery, wis::VKDevice, wis::DeviceExtID>(std::span{ extensions, extension_count });
    if (!memory) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory." };
    }

    wis::detail::limited_allocator<wis::VKDeviceExtension*> allocator{ extension_count };
    auto data = allocator.data();
    if (!data) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory extensions." };
    }

    if (extensions) {
        uint8_t* ext_ptr = memory.ext_memory;
        auto ext_ids = memory.exts.data();

        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                auto id = static_cast<wis::DeviceExtID>(extensions[i].extension_id);
                *ext_ids = id;

                size_t align = VKDeviceExtensionBridge<GetExtAlign>(id);
                ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, align));

                *data = reinterpret_cast<wis::VKDeviceExtension*>(ext_ptr);
                VKDeviceExtensionBridge<CreateExt>(id, *data);
                extensions[i].result = ext_ptr;
                ext_ptr += VKDeviceExtensionBridge<GetExtSize>(id);

                ext_ids++;
            } else {
                data[i] = reinterpret_cast<wis::VKDeviceExtension*>(extensions[i].result);
            }
        }
    }

    auto&& [result, hfactory] = wis::VKCreateDevice(*reinterpret_cast<wis::VKAdapter*>(adapter), allocator.data(), extension_count);
    std::construct_at(memory.pre_type, std::move(hfactory));

    if (result.status != wis::Status::Ok) {
        wisdom_free(memory.data());
        return reinterpret_cast<WisResult&>(result);
    }

    for (size_t i = 0; i < extension_count; i++) {
        if (extensions[i].extension_id != 0) {
            bool supported = VKDeviceExtensionBridge<GetExtSupported>(static_cast<wis::DeviceExtID>(extensions[i].extension_id), extensions[i].result);
            if (!supported)
                extensions[i].result = nullptr;
        }
    }

    *factory = reinterpret_cast<VKDevice>(memory.data());
    return reinterpret_cast<WisResult&>(result);
}

extern "C" void VKDeviceDestroy(VKDevice self)
{
    if (!self) {
        return;
    }

    auto memory = from_stream_vk<wis::VKDevice, wis::DeviceExtID>(reinterpret_cast<uint8_t*>(self));

    uint8_t* ext_ptr = memory.ext_memory;
    for (auto eid : memory.exts) {
        ext_ptr = reinterpret_cast<uint8_t*>(wis::detail::aligned_size((size_t)ext_ptr, VKDeviceExtensionBridge<GetExtAlign>(eid)));
        VKDeviceExtensionBridge<DestroyExt>(eid, ext_ptr);
        ext_ptr += VKDeviceExtensionBridge<GetExtSize>(eid);
    }

    std::destroy_at(memory.pre_type);
    wisdom_free(memory.data());
}
#endif // WISDOM_VK

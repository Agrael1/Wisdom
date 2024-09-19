#include "wisdom.h"
#include "wisdom_ext.cpp"

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

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_dx12.hpp>

extern "C" WisResult DX12CreateFactory(bool debug_layer, WisFactoryExtQuery* extensions, uint32_t extension_count, DX12Factory* factory)
{
    size_t overall_size = sizeof(wis::DX12Factory);
    size_t alloc_extension_count = 0;
    if (extensions != nullptr) {
        // Query the size of the official extensions with provided mapping
        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) // If the extension id is 0 that means the extension is already provided by the ptr
            {
                alloc_extension_count++;
                overall_size += DX12FactoryExtensionBridge<GetExtSize>(static_cast<wis::FactoryExtID>(extensions[i].extension_id));
            }
        }
    } else {
        extension_count = 0; // If the extensions are not provided, then the extension count is 0
    }

    overall_size += (alloc_extension_count + 1) * sizeof(wis::FactoryExtID); // store the extension id

    auto memory = wis::detail::make_fixed_allocation<uint8_t>(overall_size);
    if (!memory) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory." };
    }

    wis::detail::limited_allocator<wis::DX12FactoryExtension*> allocator{ extension_count };
    auto data = allocator.data();
    if (!data) {
        return WisResult{ WisStatus::StatusOutOfMemory, "Failed to allocate space for factory extensions." };
    }

    uint8_t* ext_ptr = memory.get() + sizeof(wis::DX12Factory);
    *((uint32_t*)ext_ptr) = alloc_extension_count;
    ext_ptr += sizeof(uint32_t);

    if (extensions) {
        uint8_t* data_ptr = ext_ptr + alloc_extension_count * sizeof(wis::FactoryExtID);
        for (size_t i = 0; i < extension_count; i++) {
            if (extensions[i].extension_id != 0) {
                *((wis::FactoryExtID*)ext_ptr[i]) = static_cast<wis::FactoryExtID>(extensions[i].extension_id);
                data[i] = reinterpret_cast<wis::DX12FactoryExtension*>(data_ptr);
                DX12FactoryExtensionBridge<CreateExt>(static_cast<wis::FactoryExtID>(extensions[i].extension_id), data[i]);
                data_ptr += DX12FactoryExtensionBridge<GetExtSize>(static_cast<wis::FactoryExtID>(extensions[i].extension_id));
                ext_ptr += sizeof(wis::FactoryExtID);
                extensions[i].result = data[i];
            } else {
                data[i] = reinterpret_cast<wis::DX12FactoryExtension*>(extensions[i].result);
            }
        }
    }

    auto&& [result, hfactory] = wis::DX12CreateFactory(debug_layer, data, alloc_extension_count);
    std::construct_at(reinterpret_cast<wis::DX12Factory*>(memory.get()), std::move(hfactory));

    if (result.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(result);
    }

    *factory = reinterpret_cast<DX12Factory>(memory.data.release());
    return reinterpret_cast<WisResult&>(result);
}

extern "C" void DX12FactoryDestroy(DX12Factory self)
{
    if (!self) {
        return;
    }

    auto factory = reinterpret_cast<wis::DX12Factory*>(self);
    uint32_t extension_count = *((uint32_t*)((uint8_t*)factory + sizeof(wis::DX12Factory)));
    std::span<wis::FactoryExtID> ext_ids{ reinterpret_cast<wis::FactoryExtID*>((uint8_t*)factory + sizeof(wis::DX12Factory) + sizeof(uint32_t)), extension_count };

    uint8_t* ext_ptr = (uint8_t*)factory + sizeof(wis::DX12Factory) + (extension_count + 1) * sizeof(wis::FactoryExtID);
    for (auto eid : ext_ids) {
        DX12FactoryExtensionBridge<DestroyExt>(eid, ext_ptr);
        ext_ptr += DX12FactoryExtensionBridge<GetExtSize>(eid);
    }

    std::destroy_at(factory);
    operator delete[](reinterpret_cast<uint8_t*>(self), std::nothrow);
}
#endif // WISDOM_DX12
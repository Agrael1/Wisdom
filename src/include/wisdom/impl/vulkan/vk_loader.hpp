#ifndef WISDOM_IMPL_VULKAN_VK_LOADER_HPP
#define WISDOM_IMPL_VULKAN_VK_LOADER_HPP
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace wis {

namespace detail {
inline void* InitializeVulkanLibrary() noexcept
{
#if defined(_WIN32)
    return LoadLibraryW(L"vulkan-1.dll");
#elif defined(__APPLE__)
    void* library = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!library) {
        library = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
    }
    if (!library) {
        library = dlopen("libMoltenVK.dylib", RTLD_NOW | RTLD_LOCAL);
    }
    return library;
#else
    void* library = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!library) {
        library = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    }
    return library;
#endif
    return nullptr;
}

inline void UninitializeVulkanLibrary(void* library) noexcept
{
#if defined(_WIN32)
    FreeLibrary(static_cast<HMODULE>(library));
#else
    dlclose(library);
#endif
}

template<typename PFN>
PFN GetProcAddress(void* library, const char* fname) noexcept
{
#if defined(__unix__) || defined(__APPLE__) || defined(__QNXNTO__) || defined(__Fuchsia__)
    return reinterpret_cast<PFN>(dlsym(library, fname));
#elif defined(_WIN32)
    return reinterpret_cast<PFN>(::GetProcAddress(static_cast<HMODULE>(library), fname));
#else
#error unsupported platform
#endif
}

struct LibraryDeleter {
    void operator()(void* library) const noexcept
    {
        UninitializeVulkanLibrary(library);
    }
};
using unique_library = std::unique_ptr<void, LibraryDeleter>;
} // namespace detail
} // namespace wis

#define GET_PROC_ADDRESS(library, name)             wis::detail::GetProcAddress<decltype(name)>(library, #name)
#define ASSIGN_PROC_ADDRESS_OPTIONAL(library, name) name = GET_PROC_ADDRESS(library, name)
#define ASSIGN_PROC_ADDRESS_CHECK(library, name) \
    do {                                         \
        name = GET_PROC_ADDRESS(library, name);  \
        if (name == nullptr) {                   \
            return false;                        \
        }                                        \
    } while (0)
#define ASSIGN_INSTANCE_PROC_ADDR_OPTIONAL(instance, name) \
    name = reinterpret_cast<decltype(name)>(vkGetInstanceProcAddr(instance, #name))
#define ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, name)                                  \
    do {                                                                                 \
        name = reinterpret_cast<decltype(name)>(vkGetInstanceProcAddr(instance, #name)); \
        if (name == nullptr) {                                                           \
            return false;                                                                \
        }                                                                                \
    } while (0)
#define ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, name, ...)                                         \
    do {                                                                                                 \
        constexpr static const char* name##_strings[]{ #name, __VA_ARGS__ };                             \
        for (auto name##_it : name##_strings) {                                                          \
            if ((name = reinterpret_cast<decltype(name)>(vkGetInstanceProcAddr(instance, name##_it)))) { \
                break;                                                                                   \
            }                                                                                            \
        }                                                                                                \
        if (name == nullptr) {                                                                           \
            return false;                                                                                \
        }                                                                                                \
    } while (0)
#define ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, name) \
    name = reinterpret_cast<decltype(name)>(vkGetDeviceProcAddr(device, #name))
#define ASSIGN_DEVICE_PROC_ADDR_CHECK(device, name)                                  \
    do {                                                                        \
        name = reinterpret_cast<decltype(name)>(vkGetDeviceProcAddr(device, #name)); \
        if (name == nullptr) {                                                  \
            return false;                                                     \
        }                                                                     \
    } while (0)
#define ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, name, ...)                                        \
    do {                                                                                             \
        constexpr static const char* name##_strings[]{ #name, __VA_ARGS__ };                         \
        for (auto name##_it : name##_strings) {                                                      \
            if ((name = reinterpret_cast<decltype(name)>(vkGetDeviceProcAddr(device, name##_it)))) { \
                break;                                                                               \
            }                                                                                        \
        }                                                                                            \
        if (name == nullptr) {                                                                       \
            return false;                                                                            \
        }                                                                                            \
    } while (0)

#endif // WISDOM_IMPL_VULKAN_VK_LOADER_HPP

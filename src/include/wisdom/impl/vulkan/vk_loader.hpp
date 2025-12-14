#ifndef WISDOM_IMPL_VULKAN_VK_LOADER_HPP
#define WISDOM_IMPL_VULKAN_VK_LOADER_HPP
#include <memory>

#ifdef _WIN32
typedef const char*         LPCSTR;
typedef struct HINSTANCE__* HINSTANCE;
typedef HINSTANCE           HMODULE;
#if defined(_MINWINDEF_)
/* minwindef.h defines FARPROC, and attempting to redefine it may conflict with -Wstrict-prototypes */
#elif defined(_WIN64)
typedef __int64(__stdcall* FARPROC)(void);
#else
typedef int(__stdcall* FARPROC)(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllimport) HMODULE __stdcall LoadLibraryA(LPCSTR);
__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
__declspec(dllimport) int __stdcall FreeLibrary(HMODULE);
#ifdef __cplusplus
}
#endif

#else
#include <dlfcn.h>
#endif

namespace wis {

namespace detail {
inline void* InitializeVulkanLibrary() noexcept
{
#if defined(_WIN32)
    return LoadLibraryA("vulkan-1.dll");
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
    FreeLibrary((HMODULE)library);
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
    return reinterpret_cast<PFN>(::GetProcAddress((HMODULE)library, fname));
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

#endif // WISDOM_IMPL_VULKAN_VK_LOADER_HPP

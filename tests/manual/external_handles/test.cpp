#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_platform.h>
#include <wisdom/wisdom_debug.h>
#include <iostream>
#include <chrono>

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

struct Test {
    wis::Device Init()
    {
        wis::Device device;
        wis::DebugExtension global_debug;

        wis::FactoryExtension* exts_i[] = {
            &global_debug,
        };

        auto [result, factory] = wis::CreateFactoryWithExtensions(true, exts_i, std::size(exts_i));
        auto [r2, m] = global_debug.CreateDebugMessenger(DebugCallback, &std::cout);
        global_messenger = std::move(m);

        for (size_t i = 0;; i++) {
            auto [res, adapter] = factory.GetAdapter(i);
            if (res.status == wis::Status::Ok) {
                wis::AdapterDesc desc;
                res = adapter.GetDesc(&desc);
                std::cout << "Adapter: " << desc.description.data() << "\n";

                wis::DeviceExtension* exts[] {
                    &global_interop
                };

                auto [res, hdevice] = wis::CreateDeviceWithExtensions(std::move(adapter), exts, std::size(exts));
                if (res.status == wis::Status::Ok) {
                    device = std::move(hdevice);
                    break;
                };

            } else {
                break;
            }
        }
        return device;
    }

public:
    wis::DebugMessenger global_messenger;
    wis::platform::InteropDeviceExtension global_interop;
};

wis::ResultValue<wis::VKFence>
CreateFence(const wis::Device& xdevice, bool ext)
{
    constexpr VkExportSemaphoreCreateInfo exportCreateInfo{
        .sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO,
        .handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT
    };

    auto& device = xdevice.GetInternal().device;
    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext = ext ? &exportCreateInfo : nullptr,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue = 0,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
        .flags = 0,
    };
    VkSemaphore sem;
    VkResult result = device.table().vkCreateSemaphore(device.get(), &desc, nullptr, &sem);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "vkCreateSemaphore failed to create a timeline semaphore.">(result);

    return wis::VKFence{ { sem, device, device.table().vkDestroySemaphore } };
}

wis::ResultValue<wis::VKResourceAllocator>
CreateAllocator(const wis::Device& xdevice, bool ext)
{
    auto& device = xdevice.GetInternal().device;

    uint32_t version = 0;
    auto& itable = xdevice.GetInternal().GetInstanceTable();
    auto& dtable = device.table();
    auto& gtable = device.gtable();
    auto& adapter_i = xdevice.GetInternal().adapter.GetInternal();
    gtable.vkEnumerateInstanceVersion(&version);

    auto allocator_functions = std::shared_ptr<VmaVulkanFunctions> {
        new (std::nothrow) VmaVulkanFunctions{
            .vkGetInstanceProcAddr = gtable.vkGetInstanceProcAddr,
            .vkGetDeviceProcAddr = gtable.vkGetDeviceProcAddr,
            .vkGetPhysicalDeviceProperties = itable.vkGetPhysicalDeviceProperties,
            .vkGetPhysicalDeviceMemoryProperties = itable.vkGetPhysicalDeviceMemoryProperties,
            .vkAllocateMemory = dtable.vkAllocateMemory,
            .vkFreeMemory = dtable.vkFreeMemory,
            .vkMapMemory = dtable.vkMapMemory,
            .vkUnmapMemory = dtable.vkUnmapMemory,
            .vkFlushMappedMemoryRanges = dtable.vkFlushMappedMemoryRanges,
            .vkInvalidateMappedMemoryRanges = dtable.vkInvalidateMappedMemoryRanges,
            .vkBindBufferMemory = dtable.vkBindBufferMemory,
            .vkBindImageMemory = dtable.vkBindImageMemory,
            .vkGetBufferMemoryRequirements = dtable.vkGetBufferMemoryRequirements,
            .vkGetImageMemoryRequirements = dtable.vkGetImageMemoryRequirements,
            .vkCreateBuffer = dtable.vkCreateBuffer,
            .vkDestroyBuffer = dtable.vkDestroyBuffer,
            .vkCreateImage = dtable.vkCreateImage,
            .vkDestroyImage = dtable.vkDestroyImage,
            .vkCmdCopyBuffer = dtable.vkCmdCopyBuffer,
            .vkGetBufferMemoryRequirements2KHR = dtable.vkGetBufferMemoryRequirements2,
            .vkGetImageMemoryRequirements2KHR = dtable.vkGetImageMemoryRequirements2,
            .vkBindBufferMemory2KHR = dtable.vkBindBufferMemory2,
            .vkBindImageMemory2KHR = dtable.vkBindImageMemory2,
            .vkGetPhysicalDeviceMemoryProperties2KHR = itable.vkGetPhysicalDeviceMemoryProperties2,
            .vkGetDeviceBufferMemoryRequirements = dtable.vkGetDeviceBufferMemoryRequirements,
            .vkGetDeviceImageMemoryRequirements = dtable.vkGetDeviceImageMemoryRequirements,
        }
    };

    if (!allocator_functions)
        return wis::make_result<FUNC, "Failed to allocate allocator functions.">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

    constexpr std::array<uint32_t, 32> handle_types = []() {
        std::array<uint32_t, 32> handle_types{};
        for (size_t i = 0; i < handle_types.size(); i++) {
            handle_types[i] = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;
        }
        return handle_types;
    }
    ();

    VmaAllocatorCreateInfo allocatorInfo{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice = adapter_i.adapter,
        .device = device.get(),
        .pVulkanFunctions = allocator_functions.get(),
        .instance = adapter_i.instance.get(),
        .vulkanApiVersion = version,
        .pTypeExternalMemoryHandleTypes = ext ? handle_types.data() : nullptr
    };

    VmaAllocator al;
    VkResult vr = vmaCreateAllocator(&allocatorInfo, &al);

    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to create an Allocator">(vr);

    return wis::VKResourceAllocator{ wis::shared_handle<VmaAllocator>{
            device, al
        }, ext };
}

wis::ResultValue<wis::Texture>
CreateTexture(const wis::Device& xdevice, const wis::VKResourceAllocator a)
{
    wis::TextureDesc desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = {
            .width = 1024,
            .height = 1024,
            .depth_or_layers = 1
        },
        .mip_levels = 1,
        .usage = wis::TextureUsage::ShaderResource,
    };
    auto [res, texture] = a.CreateTexture(desc);
    if (res.status != wis::Status::Ok) {
        return res;
    }
    return std::move(texture);
}

int64_t TestSemaphores(const wis::Device& xdevice)
{
    wis::VKFence fences_default[20];
    wis::VKFence fences_whandles[20];

    // std::cout << "Testing semaphore creation\n";
    // std::cout << "Default semaphores\n";

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 20; i++) {
        auto [res, fence] = CreateFence(xdevice, false);
        if (res.status != wis::Status::Ok) {
            std::cerr << "Failed to create fence\n";
            return std::numeric_limits<uint64_t>::max();
        }
        fences_default[i] = std::move(fence);
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto first = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Time: " << first.count() << "ms\n";

    // std::cout << "Semaphores with handles\n";

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 20; i++) {
        auto [res, fence] = CreateFence(xdevice, true);
        if (res.status != wis::Status::Ok) {
            std::cerr << "Failed to create fence\n";
            return std::numeric_limits<uint64_t>::max();
        }
        fences_whandles[i] = std::move(fence);
    }
    end = std::chrono::high_resolution_clock::now();

    auto second = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << "Time: " << second.count() << "ms\n";

    return first.count() - second.count();
}
int64_t TestAllocators(const wis::Device& xdevice)
{
    wis::VKResourceAllocator allocators_default[20];
    wis::VKResourceAllocator allocators_whandles[20];

    // std::cout << "Testing allocator creation\n";
    // std::cout << "Default allocators\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 20; i++) {
        auto [res, allocator] = CreateAllocator(xdevice, false);
        if (res.status != wis::Status::Ok) {
            std::cerr << "Failed to create allocator\n";
            return std::numeric_limits<uint64_t>::max();
        }
        allocators_default[i] = std::move(allocator);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto first = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Time: " << first.count() << "ms\n";

    // std::cout << "Allocators with handles\n";

    start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 20; i++) {
        auto [res, allocator] = CreateAllocator(xdevice, true);
        if (res.status != wis::Status::Ok) {
            std::cerr << "Failed to create allocator\n";
            return std::numeric_limits<uint64_t>::max();
        }
        allocators_whandles[i] = std::move(allocator);
    }
    end = std::chrono::high_resolution_clock::now();

    auto second = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << "Time: " << second.count() << "ms\n";

    return first.count() - second.count();
}
int64_t TestTextures(const wis::Device& xdevice)
{
    auto [r, a1] = CreateAllocator(xdevice, false);
    auto [r2, a2] = CreateAllocator(xdevice, true);

    wis::Texture textures_default[20];
    wis::Texture textures_whandles[20];

    // std::cout << "Testing texture creation\n";
    // std::cout << "Default textures\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 20; i++) {
        auto [res, texture] = CreateTexture(xdevice, a1);
        if (res.status != wis::Status::Ok) {
            std::cerr << "Failed to create texture\n";
            return std::numeric_limits<uint64_t>::max();
        }
        textures_default[i] = std::move(texture);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto first = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Time: " << first.count() << "ms\n";

    // std::cout << "Textures with handles\n";

    start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 20; i++) {
        auto [res, texture] = CreateTexture(xdevice, a2);
        if (res.status != wis::Status::Ok) {
            std::cerr << "Failed to create texture\n";
            return std::numeric_limits<uint64_t>::max();
        }
        textures_whandles[i] = std::move(texture);
    }
    end = std::chrono::high_resolution_clock::now();

    auto second = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << "Time: " << second.count() << "ms\n";

    return first.count() - second.count();
}

void TestHandles(const wis::Device& xdevice, wis::platform::InteropDeviceExtension& global_interop)
{

}

void TestAllocations(const wis::Device& xdevice, wis::platform::InteropDeviceExtension& global_interop)
{
    auto [r, a] = CreateAllocator(xdevice, true);

    a.AllocateImageMemory(1024, wis::TextureUsage::ShaderResource, wis::MemoryType::Default);
}

int main()
{
    Test test;
    auto device = test.Init();
    if (!device) {
        std::cerr << "Failed to create device\n";
        return 1;
    }
    TestAllocations(device, test.global_interop);


    std::array<int64_t, 10> results;
    for (size_t i = 0; i < 10; i++) {
        results[i] = TestSemaphores(device);
    }

    for (size_t i = 0; i < 10; i++) {
        results[i] = TestAllocators(device);
    }

    for (size_t i = 0; i < 10; i++) {
        results[i] = TestTextures(device);
    }

    TestHandles(device, test.global_interop);
}
#ifndef WIS_VK_PLATFORM_XCB_CPP
#define WIS_VK_PLATFORM_XCB_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XCB_PRESENT)

#ifndef WIS_USE_PLATFORM_XCB
#define WIS_USE_PLATFORM_XCB 1
#endif
#include <wisdom/wisdom.h>
#include <wisdom_platform/generated/c_platform_api.h>

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XCB_PRESENT)
#endif // WIS_VK_PLATFORM_XCB_CPP

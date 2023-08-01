// Has to be here, since Vulkan has a bug with static function
#if !defined(WISDOM_WINDOWS) || defined(WISDOM_VULKAN_FOUND) && defined(WISDOM_FORCE_VULKAN)
#include <vulkan/vulkan.hpp>
#endif

import app;

int main()
{
    App app(1920, 1080);
    return app.Start();
}
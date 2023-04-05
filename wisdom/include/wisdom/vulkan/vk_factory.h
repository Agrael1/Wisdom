#pragma once
#include <wisdom/api/api_factory.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/util/log_layer.h>
#include <vulkan/vulkan.hpp>
#include <unordered_set>
#include <wisdom/global/definitions.h>

namespace wis
{
	class VKFactory;

	template<>
	class Internal<VKFactory>
	{
		static constexpr inline bool valid = true;
	public:
		Internal()
		{
			ref_count++;
		}
		~Internal()
		{
			if (!--ref_count)
			{
				vkDestroyInstance(factory, nullptr);
			}
		}
		vk::Instance& GetInstance()
		{
			return factory;
		}
	protected:
		static inline std::atomic<uint32_t> ref_count{0};
		static inline vk::Instance factory{};
	};


	class VKFactory : public QueryInternal<VKFactory>
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	public:
		VKFactory(const ApplicationInfo& app_info)
		{
			if (factory)
				return;

			wis::lib_log(Severity::debug, "Initializing Instance");
			uint32_t version = 0;
			vkEnumerateInstanceVersion(&version);

			wis::lib_log(Severity::info, std::format("Vulkan ver: {}.{}.{}",
				VK_API_VERSION_MAJOR(version),
				VK_API_VERSION_MINOR(version),
				VK_API_VERSION_PATCH(version))
			);
			version &= ~(0xFFFU); //unsigned remove patch from instance for compatibility

			vk::ApplicationInfo info{
				app_info.application_name,
					VK_MAKE_API_VERSION(0, app_info.app_version.major, app_info.app_version.minor, app_info.app_version.patch),
					app_info.engine_name,
					VK_MAKE_API_VERSION(0, app_info.engine_version.major, app_info.engine_version.minor, app_info.engine_version.patch),
					version
			};

			auto found_extension = FoundExtensions();
			auto found_layers = FoundLayers();

			vk::InstanceCreateInfo create_info;
			create_info.pApplicationInfo = &info;
			create_info.enabledLayerCount = static_cast<uint32_t>(found_layers.size());
			create_info.ppEnabledLayerNames = found_layers.data();
			create_info.enabledExtensionCount = static_cast<uint32_t>(found_extension.size());
			create_info.ppEnabledExtensionNames = found_extension.data();

			if constexpr (debug_mode)
			{
				vk::DebugUtilsMessengerCreateInfoEXT create_instance_debug(
					vk::DebugUtilsMessengerCreateFlagsEXT(0), 
					vk::DebugUtilsMessageSeverityFlagsEXT(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT),
					vk::DebugUtilsMessageTypeFlagsEXT(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT),
					VKFactory::debugCallback
				);
				create_info.pNext = &create_instance_debug;
			}
			
			factory = vk::createInstance(create_info);
		}
	private:
		static std::vector<const char*> FoundExtensions()noexcept;
		static std::vector<const char*> FoundLayers()noexcept;
	};
}
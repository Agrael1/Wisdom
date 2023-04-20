#pragma once
#include <wisdom/api/api_factory.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/generator.h>
#include <wisdom/global/definitions.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <unordered_set>
#include <wisdom/util/profile.h>

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
			factory.add_ref();
		}
		~Internal()
		{
			if(factory.release() == 1)
				messenger.release();
		}
		static vk::Instance GetInstance()noexcept
		{
			return factory.get();
		}
		static uint32_t GetApiVer()noexcept
		{
			return api_version;
		}
		auto GetInstanceHandle()const noexcept
		{
			return factory;
		}
	protected:
		static inline wis::shared_handle<vk::Instance> factory{};
		static inline wis::shared_handle<vk::DebugUtilsMessengerEXT> messenger{};
		static inline uint32_t api_version{};
	};

	inline constexpr uint32_t order_performance(vk::PhysicalDeviceType t)
	{
		using enum vk::PhysicalDeviceType;
		switch (t)
		{
		default:
		case eOther:
		case eIntegratedGpu:
			return 3;
		case eDiscreteGpu:
			return 4;
		case eVirtualGpu:
			return 2;
		case eCpu:
			return 1;
		}
	}
	inline constexpr uint32_t order_power(vk::PhysicalDeviceType t)
	{
		using enum vk::PhysicalDeviceType;
		switch (t)
		{
		default:
		case eOther:
		case eIntegratedGpu:
			return 4;
		case eDiscreteGpu:
			return 3;
		case eVirtualGpu:
			return 2;
		case eCpu:
			return 1;
		}
	}


	class VKFactory : public QueryInternal<VKFactory>
	{
		friend class VKResourceAllocator;
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

			wis::lib_info("Initializing Instance");
			uint32_t version = 0;
			vkEnumerateInstanceVersion(&version);

			wis::lib_log(Severity::info, std::format("Vulkan ver: {}.{}.{}",
				VK_API_VERSION_MAJOR(version),
				VK_API_VERSION_MINOR(version),
				VK_API_VERSION_PATCH(version))
			);
			api_version = version &= ~(0xFFFU); //unsigned remove patch from instance for compatibility

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

			constexpr static vk::DebugUtilsMessengerCreateInfoEXT create_instance_debug(
				vk::DebugUtilsMessengerCreateFlagsEXT(0),
				vk::DebugUtilsMessageSeverityFlagsEXT(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT),
				vk::DebugUtilsMessageTypeFlagsEXT(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT),
				VKFactory::debugCallback
			);

			if constexpr (debug_mode)
			{
				create_info.pNext = &create_instance_debug;
			}

			factory = wis::shared_handle<vk::Instance>{ vk::createInstance(create_info) };
			DynamicLoader::loader = vk::DispatchLoaderDynamic{ factory.get(), vkGetInstanceProcAddr};
			DynamicLoader::init = true;

			if constexpr (debug_mode)
			{
				messenger = wis::shared_handle<vk::DebugUtilsMessengerEXT>{ factory->createDebugUtilsMessengerEXT(create_instance_debug, nullptr, DynamicLoader::loader), factory };
			}
		}
	public:
		[[nodiscard]]
		wis::generator<VKAdapter> EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance)const noexcept
		{
			auto adapters = factory->enumeratePhysicalDevices();

			if(adapters.size() > 1)
			switch (preference)
			{
			case wis::AdapterPreference::None:
				break;
			case wis::AdapterPreference::MinConsumption:
				std::ranges::sort(adapters, [](vk::PhysicalDevice a, vk::PhysicalDevice b)
					{
						auto x = a.getProperties();
						auto y = b.getProperties();

						if (order_power(x.deviceType) > order_power(y.deviceType))
							return true;
						return x.limits.maxMemoryAllocationCount > y.limits.maxMemoryAllocationCount;
					}
				);
				break;
			case wis::AdapterPreference::Performance:
				std::ranges::sort(adapters, [](vk::PhysicalDevice a, vk::PhysicalDevice b)
					{
						auto x = a.getProperties();
						auto y = b.getProperties();

						if (order_performance(x.deviceType) > order_performance(y.deviceType))
							return true;
						return x.limits.maxMemoryAllocationCount > y.limits.maxMemoryAllocationCount;
					}
				);
				break;
			default:
				break;
			}

			for (auto& a : adapters)
				co_yield VKAdapter{ a };
		}
	private:
		static std::vector<const char*> FoundExtensions()noexcept;
		static std::vector<const char*> FoundLayers()noexcept;
	};
}
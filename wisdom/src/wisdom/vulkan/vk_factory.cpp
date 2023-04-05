#include <wisdom/vulkan/vk_factory.h>

constexpr wis::Severity SeverityConvert(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		return wis::Severity::trace;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		return wis::Severity::info;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		return wis::Severity::warn;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		return wis::Severity::error;
	default:
		return wis::Severity::critical;
	}
}


VKAPI_ATTR VkBool32 VKAPI_CALL wis::VKFactory::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
{
	wis::lib_log(SeverityConvert(messageSeverity),
		std::format(
			"\n[Validation layer]: {}\n [Message]:{}"
			, pCallbackData->pMessageIdName, pCallbackData->pMessage

		));
	return false;
}

std::vector<const char*> wis::VKFactory::FoundExtensions() noexcept
{
	auto extensions = vk::enumerateInstanceExtensionProperties();

	if constexpr (debug_mode)
	{
		std::string debug_str1{"Available Extensions:\n"};
		for (auto& i : extensions)
		{
			std::format_to(std::back_inserter(debug_str1),
				"{},\n",
				i.extensionName.data());
		}
		wis::lib_info(std::move(debug_str1));
	}

	std::unordered_set<std::string_view> req_extension{
		VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_METAL_EXT)
			VK_EXT_METAL_SURFACE_EXTENSION_NAME,
#endif
	};
	if constexpr (debug_mode)
	{
		req_extension.emplace(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		req_extension.emplace(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	std::vector<const char*> found_extension;
	for (const auto& extension : extensions)
	{
		if (auto it = req_extension.find(extension.extensionName.data()); it!= req_extension.end())
			found_extension.push_back(it->data());
	}

	if constexpr (debug_mode)
	{
		std::string debug_str{"Used Extensions:\n"};
		for (auto* i : found_extension)
		{
			std::format_to(std::back_inserter(debug_str),
				"{},\n",
				i);
		}
		wis::lib_info(std::move(debug_str));
	}

	return found_extension;
}

std::vector<const char*> wis::VKFactory::FoundLayers() noexcept
{
	std::vector<const char*> out;
	if (debug_mode)
	{
		auto layers = vk::enumerateInstanceLayerProperties();
		std::string debug_str1{"Available Layers:\n"};
		for (auto& i : layers)
		{
			std::format_to(std::back_inserter(debug_str1),
				"{},\n",
				i.layerName.data());
		}
		wis::lib_info(std::move(debug_str1));

		for (const auto& layer : layers)
		{
			if (std::string_view(layer.layerName.data()) == "VK_LAYER_KHRONOS_validation")
			{
				out.push_back("VK_LAYER_KHRONOS_validation");
				break;
			}
		}

		std::string debug_str{"Used Layers:\n"};
		for (auto* i : out)
		{
			std::format_to(std::back_inserter(debug_str),
				"{},\n",
				i);
		}
		wis::lib_info(std::move(debug_str));
	}

	return out;
}

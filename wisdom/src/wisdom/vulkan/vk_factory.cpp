#include <wisdom/vulkan/vk_factory.h>
#include <ranges>
#include <unordered_map>
#include <wisdom/util/misc.h>

struct FactoryData
{
public:
	static const FactoryData& instance()noexcept
	{
		static FactoryData d;
		return d;
	}
	static auto& GetExtensions()noexcept
	{
		return instance().extensions;
	}
	static auto& GetLayers()noexcept
	{
		return instance().layers;
	}
	[[nodiscard]]
	static std::string ExtensionsString()noexcept
	{
		std::string debug_str1{"Available Extensions:\n"};
		for (auto& i : instance().extensions)
		{
			std::format_to(std::back_inserter(debug_str1),
				"{}\n",
				i.first);
		}
		return debug_str1;
	}
	[[nodiscard]]
	static std::string LayersString()noexcept
	{
		std::string debug_str1{"Available Layers:\n"};
		for (auto& i : instance().layers)
		{
			std::format_to(std::back_inserter(debug_str1),
				"\t{}\n",
				i.first);
		}
		return debug_str1;
	}
private:
	FactoryData(bool blayers = true, bool bextensions = true)
	{
		if (bextensions)
			LoadExtensions();
		if (blayers)
			LoadLayers();
	}
private:
	void LoadExtensions()noexcept
	{
		auto vextensions = vk::enumerateInstanceExtensionProperties();
		for (auto& i : vextensions)
			extensions.emplace(i.extensionName, i);
	}
	void LoadLayers()noexcept
	{
		auto vlayers = vk::enumerateInstanceLayerProperties();
		for (auto& i : vlayers)
			layers.emplace(i.layerName, i);
	}
private:
	std::unordered_map<std::string, vk::ExtensionProperties, wis::string_hash> extensions;
	std::unordered_map<std::string, vk::LayerProperties, wis::string_hash> layers;
};

constexpr inline std::array req_extensions{
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_METAL_EXT)
	VK_EXT_METAL_SURFACE_EXTENSION_NAME,
	VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#endif
#if DEBUG_MODE
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

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
			, pCallbackData->pMessageIdName? pCallbackData->pMessageIdName:"", pCallbackData->pMessage
		));
	return false;
}




std::vector<const char*> wis::VKFactory::FoundExtensions() noexcept
{
	const auto& extensions = FactoryData::GetExtensions();

	if constexpr (debug_mode)
		wis::lib_info(FactoryData::ExtensionsString());

	std::vector<const char*> found_extension;
	for (auto* extension : req_extensions)
	{
		if (extensions.contains(extension))
			found_extension.push_back(extension);
	}

	if constexpr (debug_mode)
	{
		std::string debug_str{"Used Extensions:\n"};
		for (auto* i : found_extension)
			std::format_to(std::back_inserter(debug_str),"{},\n",i);
		wis::lib_info(std::move(debug_str));
	}

	return found_extension;
}

std::vector<const char*> wis::VKFactory::FoundLayers() noexcept
{
	std::vector<const char*> out;
	if constexpr(debug_mode)
	{
		const auto& layers = FactoryData::GetLayers();
		if constexpr (debug_mode)
			wis::lib_info(FactoryData::LayersString());

		if(layers.contains("VK_LAYER_KHRONOS_validation"))
			out.push_back("VK_LAYER_KHRONOS_validation");

		std::string debug_str{"Used Layers:\n"};
		for (auto* i : out)
			std::format_to(std::back_inserter(debug_str),"{},\n",i);
		wis::lib_info(std::move(debug_str));
	}

	return out;
}

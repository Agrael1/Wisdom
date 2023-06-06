#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_factory.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/bridge/generator.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_adapter.h>
#endif


WIS_EXPORT namespace wis
{
	class VKFactory;

	template<>
	class Internal<VKFactory>
	{
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
		static auto GetInstanceHandle()noexcept
		{
			return factory;
		}
	protected:
		static inline wis::shared_handle<vk::Instance> factory{};
		static inline wis::shared_handle<vk::DebugUtilsMessengerEXT> messenger{};
		static inline uint32_t api_version{};
	};


	class VKFactory : public QueryInternal<VKFactory>
	{
		friend class VKResourceAllocator;
		friend class VKDevice;
		static WIS_INLINE VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	public:
		/// @brief Create a vulkan instance with the given application info
		/// @param app_info Application info
		/// @param unused 
		WIS_INLINE VKFactory(const ApplicationInfo& app_info, [[maybe_unused]] bool unused = true);		
	public:
		/// @brief Enumerates all adapters on the system
		/// @param preference Preference to use when enumerating adapters, changes the order of the adapters
		/// @return coroutine that yields VKAdapter
		[[nodiscard]] WIS_INLINE wis::generator<VKAdapter>
			EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance)const noexcept;
	private:
		static WIS_INLINE std::vector<const char*> FoundExtensions()noexcept;
		static WIS_INLINE std::vector<const char*> FoundLayers()noexcept;
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_factory.inl"
#endif
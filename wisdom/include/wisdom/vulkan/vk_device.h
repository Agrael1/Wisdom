#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <unordered_set>

namespace wis
{
	class VKDevice;
	template<>
	class Internal<VKDevice>
	{
	public:
		vk::Device GetDevice()const noexcept
		{
			return device.get();
		}
		wis::shared_handle<vk::Device> GetDeviceHandle()const noexcept
		{
			return device;
		}
	protected:
		wis::shared_handle<vk::Device> device;
	};

	class VKDevice : public QueryInternal<VKDevice>
	{
		static inline constexpr const auto max_queue_count = 4;
		static inline constexpr const auto max_ext_count = 16;
		struct QueueInfo
		{
			uint32_t index;
			uint32_t count;
		};
	public:
		VKDevice() = default;
		explicit VKDevice(VKAdapterView adapter)
		{
			Initialize(adapter);
		}
	public:
		bool Initialize(VKAdapterView adapter)
		{
			auto [families, count] = QueueFamilies(adapter);
			std::array<vk::DeviceQueueCreateInfo, max_queue_count> queue_infos{};
			size_t queue_info_size = 0;

			constexpr static float queue_priority = 1.0f;
			for (queue_info_size = 0; queue_info_size < count; queue_info_size++)
			{
				auto& q_info = queue_infos[queue_info_size];
				q_info.queueFamilyIndex = families[queue_info_size].index;
				q_info.queueCount = 1; //hard wired for now
				q_info.pQueuePriorities = &queue_priority;
			}

			auto [exts, e_cnt] = RequestExtensions(adapter);





			vk::DeviceCreateInfo desc
			{
				{}, count, queue_infos.data(), 0, nullptr, e_cnt, exts.data(),
			};


			//device = adapter.createDevice(desc);
			return true;
		}
	private:
		[[nodiscard]]
		static std::pair<std::array<QueueInfo, max_queue_count>, size_t> QueueFamilies(VKAdapterView adapter)noexcept
		{
			using namespace river::flags;
			auto family_props = adapter.getQueueFamilyProperties();
			wis::lib_info(std::format("The system supports {} queue families", family_props.size()));
			std::array<QueueInfo, max_queue_count> queue_types{};
			size_t size = 0;


			//load all queues
			for (uint32_t i = 0; i < family_props.size(); i++)
			{
				using enum vk::QueueFlagBits;
				auto& family = family_props[i];
				if ((family.queueFlags & eGraphics) == eGraphics && queue_types[0].count != 0)
				{
					queue_types[size++] = { .index = i, .count = family.queueCount };
					continue;
				}
				if ((family.queueFlags & eCompute) == eCompute && queue_types[1].count != 0)
				{
					queue_types[size++] = { .index = i, .count = family.queueCount };
					continue;
				}
				if ((family.queueFlags & eVideoDecodeKHR) == eVideoDecodeKHR && queue_types[3].count != 0)
				{
					queue_types[size++] = { .index = i, .count = family.queueCount };
					continue;
				}
				if ((family.queueFlags & eTransfer) == eTransfer && queue_types[2].count != 0)
				{
					queue_types[size++] = { .index = i, .count = family.queueCount };
					continue;
				}
			}
			return { queue_types,size };
		}

		[[nodiscard]]
		std::pair<std::array<const char*, max_ext_count>, size_t> RequestExtensions(VKAdapterView adapter)noexcept
		{
			constexpr static std::array req_extension{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
					VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
					VK_KHR_RAY_QUERY_EXTENSION_NAME,
					VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
					VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
					VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
					VK_KHR_MAINTENANCE3_EXTENSION_NAME,
					VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
					VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
					VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
					VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
					VK_KHR_MAINTENANCE1_EXTENSION_NAME,
					VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
					VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
					VK_NV_MESH_SHADER_EXTENSION_NAME,
					VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
			};


			auto extensions = adapter.enumerateDeviceExtensionProperties();
			std::unordered_set<const char*> ext_set;
			ext_set.reserve(extensions.size());

			for (const auto& e : extensions)
				ext_set.emplace(e);

			std::array<const char*, max_ext_count> avail_exts{};
			size_t size = 0;

			for (auto* i : req_extension)
			{
				if (!ext_set.contains(i))continue;
				avail_exts[size++] = i;

				if (i == VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME)
					vrs_supported = true;
				else if (i == VK_NV_MESH_SHADER_EXTENSION_NAME)
					mesh_shader_supported = true;
				else if (i == VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME)
					ray_tracing_supported = true;
				else if (i == VK_KHR_RAY_QUERY_EXTENSION_NAME)
					ray_query_supported = true;
				else if (i == VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME)
					draw_indirect_supported = true;
			}

			return { avail_exts, size };
		}
	private:
		bool vrs_supported : 1 = false;
		bool mesh_shader_supported : 1 = false;
		bool ray_tracing_supported : 1 = false;
		bool ray_query_supported : 1 = false;
		bool draw_indirect_supported : 1 = false;
	};
}
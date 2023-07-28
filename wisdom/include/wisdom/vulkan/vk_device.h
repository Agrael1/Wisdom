#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_render_pass.h>
#include <wisdom/api/api_input_layout.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_render_pass.h>
#include <wisdom/vulkan/vk_state_builder.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_descriptor_heap.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <wisdom/global/definitions.h>
#include <bitset>
#include <wisdom/util/small_allocator.h>
#endif


WIS_EXPORT namespace wis
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
		vk::PhysicalDevice GetAdapter()const noexcept
		{
			return adapter;
		}
		wis::shared_handle<vk::Device> GetDeviceHandle()const noexcept
		{
			return device;
		}
	protected:
		wis::shared_handle<vk::Device> device;
		vk::PhysicalDevice adapter;
	};


	class VKDevice : public QueryInternal<VKDevice>
	{
		constexpr static inline std::array required_extensions{
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
			VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME,
			VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
			VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
			VK_NV_MESH_SHADER_EXTENSION_NAME,
			VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
		};
		static inline constexpr const auto max_count = 4;
		struct QueueInfo
		{
			uint32_t index;
			uint32_t count;
		};
		struct QueueFormat
		{
		public:
			QueueFormat& operator=(QueueFormat&& o)noexcept
			{
				queue_flags = o.queue_flags;
				count = o.count;
				family_index = o.family_index;
				return *this;
			}
		public:
			uint8_t GetNextInLine()const noexcept
			{
				return last.exchange((last + 1) % count);
			}
			bool Empty()const noexcept
			{
				return !count;
			}
		public:
			uint16_t queue_flags = 0;
			uint8_t count = 0;
			uint8_t family_index = 0;
			mutable std::atomic<uint8_t> last{ 0 };
		};
		enum class QueueTypes : uint8_t
		{
			graphics,
			compute,
			copy,
			video_decode
		};
		struct QueueResidency
		{
			static constexpr size_t QueueIndex(QueueType type)
			{
				switch (type)
				{
				case wis::QueueType::compute:
					return +QueueTypes::compute;
				case wis::QueueType::copy:
					return +QueueTypes::copy;
				case wis::QueueType::video_decode:
					return +QueueTypes::video_decode;
				default:
					return +QueueTypes::graphics;
				}
			}
			static constexpr size_t QueueFlag(QueueTypes type)
			{
				using enum vk::QueueFlagBits;
				switch (type)
				{
				case QueueTypes::copy:
					return +eTransfer;
				case QueueTypes::compute:
					return +eCompute;
				case QueueTypes::graphics:
					return +eGraphics;
				case QueueTypes::video_decode:
					return +eVideoDecodeKHR;
				default:
					return 0;
				}
			}
		public:
			const QueueFormat* GetOfType(QueueType type)const
			{
				auto idx = QueueIndex(type);
				auto* q = &available_queues[idx];

				if (!q->count)
				{
					idx = FindResembling(QueueTypes(idx));
					if (idx == -1)return nullptr;
					q = &available_queues[idx];
				}
				return q;
			}
			int32_t FindResembling(QueueTypes type)const
			{
				for (size_t i = 0; i < max_count; i++)
				{
					auto& r = available_queues[i];
					if (r.queue_flags & QueueFlag(type))
						return i;
				}
				return -1;
			}
		public:
			std::array<QueueFormat, max_count> available_queues{};
		};


	public:
		VKDevice() = default;
		explicit VKDevice(VKAdapterView adapter)
		{
			Initialize(adapter);
		}
	public:
		operator VKDeviceView()const noexcept
		{
			return GetDeviceHandle();
		}
		WIS_INLINE bool Initialize(VKAdapterView adapter);
	public:
		[[nodiscard]] WIS_INLINE
			VKSwapChain CreateSwapchain(
				VKCommandQueueView render_queue, 
				wis::SwapchainOptions options, 
				wis::SurfaceParameters xsurface, 
				bool vsync = false)const;

		[[nodiscard]]
		VKCommandQueue CreateCommandQueue(QueueOptions options = QueueOptions{})
		{
			auto* queue = queues.GetOfType(options.type);
			if (!queue)return{};

			vk::DeviceQueueInfo2 info{
				{},
					queue->family_index,
					queue->GetNextInLine()
			};
			return VKCommandQueue{ device->getQueue2(info) };
		}

		[[nodiscard]]
		VKCommandList CreateCommandList(QueueType list_type)const
		{
			vk::CommandPoolCreateInfo cmd_pool_create_info{
				vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
					queues.GetOfType(list_type)->family_index
			};
			auto ca = device->createCommandPool(cmd_pool_create_info);//allocator

			vk::CommandBufferAllocateInfo cmd_buf_alloc_info{
				ca, vk::CommandBufferLevel::ePrimary, 1
			};

			return VKCommandList{ wis::shared_handle<vk::CommandPool>{ca, device},
				vk::CommandBuffer{device->allocateCommandBuffers(cmd_buf_alloc_info).at(0)} };
		}

		[[nodiscard]]
		VKFence CreateFence()const
		{
			vk::SemaphoreTypeCreateInfo timeline_desc
			{
				vk::SemaphoreType::eTimeline,
					0
			};
			vk::SemaphoreCreateInfo desc
			{
				{}, &timeline_desc
			};
			return VKFence{ wis::shared_handle<vk::Semaphore>{device->createSemaphore(desc), device} };
		}

		[[nodiscard]] WIS_INLINE
		VKRenderPass CreateRenderPass(Size2D frame_size, std::span<ColorAttachment> rtv_descs,
			DepthStencilAttachment dsv_desc = DepthStencilAttachment{},
			SampleCount samples = SampleCount::s1,
			DataFormat vrs_format = DataFormat::unknown)const;

		[[nodiscard]]
		vk::UniqueDescriptorSetLayout CreateDescriptorSetLayout(uint32_t binding, uint32_t count = 1u)const
		{
			vk::DescriptorSetLayoutBinding layout_binding{
				binding, vk::DescriptorType::eUniformBufferDynamic, count,
					vk::ShaderStageFlagBits::eVertex, nullptr
			};

			vk::DescriptorSetLayoutCreateInfo desc{
				{}, 1u, &layout_binding
			};
			return device->createDescriptorSetLayoutUnique(desc);
		}

		void WriteConstantBufferView(VKDescriptorSet set, VKBufferView buffer, uint32_t size)const
		{
			//vk::WriteDescriptorSet desc{
			//	set, 
			//};
			//VkWriteDescriptorSet writeDescriptorSet = {};
			//
			//// Write the descriptor of the uniform buffer.
			//// We need to pass the descriptor set where it is store and 
			//// the binding point associated with descriptor in the descriptor set.
			//writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			//writeDescriptorSet.dstSet = m_sampleParams.DescriptorSet.Handle;
			//writeDescriptorSet.descriptorCount = 1;
			//writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			//writeDescriptorSet.pBufferInfo = &m_sampleParams.HostVisibleBuffer.Descriptor;
			//writeDescriptorSet.dstBinding = 0;
			//
			//vkUpdateDescriptorSets(m_vulkanParams.Device, 1, &writeDescriptorSet, 0, nullptr);
		}

		[[nodiscard]]
		VKRootSignature CreateRootSignature()const
		{
			vk::PipelineLayoutCreateInfo pipeline_layout_info
			{
				//empty for now
			};
			return VKRootSignature{ wis::shared_handle<vk::PipelineLayout>{device->createPipelineLayout(pipeline_layout_info), device} };
		}

		[[nodiscard]] WIS_INLINE 
		VKPipelineState CreateGraphicsPipeline(wis::VKGraphicsPipelineDesc desc, std::span<const InputLayoutDesc> input_layout)const;

		[[nodiscard]]
		VKShader CreateShader(wis::shared_blob blob, ShaderType type)const
		{
			vk::ShaderModuleCreateInfo desc
			{
				vk::ShaderModuleCreateFlags{},
					blob.size(),
					blob.data<uint32_t>()
			};
			return VKShader{ wis::shared_handle<vk::ShaderModule>{device->createShaderModule(desc), device}, type };
		}

		/// @brief Create a Render target view object
		/// @param texture The texture to create the view for
		/// @param range Select the subresource range to create the view for
		/// @return View object
		[[nodiscard]] WIS_INLINE  VKRenderTargetView CreateRenderTargetView(VKTextureView texture, RenderSelector range = {})const;

		//TODO:Comment
		[[nodiscard]] VKDescriptorHeap CreateDescriptorHeap(uint32_t num_descs)const
		{
			//TODO: other types
			vk::DescriptorPoolSize size_desc{
				vk::DescriptorType::eUniformBufferDynamic, num_descs
			};
			vk::DescriptorPoolCreateInfo pool_desc{
				vk::DescriptorPoolCreateFlags{}, num_descs, 1u, & size_desc
			};
			wis::shared_handle pool{device->createDescriptorPool(pool_desc), device};
			return VKDescriptorHeap{ std::move(pool) };
		}

	private:
		WIS_INLINE void GetQueueFamilies(VKAdapterView adapter)noexcept;

		[[nodiscard]] WIS_INLINE 
		wis::internals::uniform_allocator<const char*, required_extensions.size()> 
			RequestExtensions(VKAdapterView adapter)noexcept;

		[[nodiscard]] WIS_INLINE 
			vk::PresentModeKHR GetPresentMode(vk::SurfaceKHR surface, bool vsync)const noexcept;

		WIS_INLINE void FillShaderStages(
			const VKGraphicsPipelineDesc& desc, 
			wis::internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages>& shader_stages
		)const noexcept;
	private:
		QueueResidency queues{};
		// bool vrs_supported : 1 = false;
		bool mesh_shader_supported : 1 = false;
		bool ray_tracing_supported : 1 = false;
		bool ray_query_supported : 1 = false;
		bool draw_indirect_supported : 1 = false;
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_device.inl"
#endif

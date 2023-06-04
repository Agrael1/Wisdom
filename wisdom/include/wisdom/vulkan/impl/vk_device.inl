#ifndef WISDOM_MODULES
#include <unordered_set>
#endif

bool wis::VKDevice::Initialize(VKAdapterView adapter)
{
	this->adapter = adapter;
	GetQueueFamilies(adapter);
	std::array<vk::DeviceQueueCreateInfo, max_count> queue_infos{};
	size_t queue_count = 0;

	constexpr static auto priorities = []() {
		std::array<float, 64> priorities{};
		priorities.fill(1.0f);
		return priorities;
		}();


		for (size_t queue_info_size = 0; queue_info_size < max_count; queue_info_size++)
		{
			auto& q_info = queue_infos[queue_info_size];
			auto& q = queues.available_queues[queue_info_size];
			if (!q.count)continue;

			q_info.queueFamilyIndex = q.family_index;
			q_info.queueCount = q.count; //hard wired for now
			q_info.pQueuePriorities = priorities.data();
			queue_count++;
		}

		auto exts = RequestExtensions(adapter);

		void* device_create_info_next = nullptr;
		auto add_extension = [&](auto& extension)
			{
				extension.pNext = device_create_info_next;
				device_create_info_next = &extension;
			};

		auto physical_device_features = adapter.getFeatures();
		vk::PhysicalDeviceFeatures2 device_features;
		{
			device_features.features.robustBufferAccess = physical_device_features.robustBufferAccess;
			device_features.features.fullDrawIndexUint32 = physical_device_features.fullDrawIndexUint32;
			device_features.features.imageCubeArray = physical_device_features.imageCubeArray;
			device_features.features.independentBlend = physical_device_features.independentBlend;
			device_features.features.geometryShader = physical_device_features.geometryShader;
			device_features.features.tessellationShader = physical_device_features.tessellationShader;
			device_features.features.sampleRateShading = physical_device_features.sampleRateShading;
			device_features.features.dualSrcBlend = physical_device_features.dualSrcBlend;
			device_features.features.logicOp = physical_device_features.logicOp;
			device_features.features.multiDrawIndirect = physical_device_features.multiDrawIndirect;
			device_features.features.drawIndirectFirstInstance = physical_device_features.drawIndirectFirstInstance;
			device_features.features.depthClamp = physical_device_features.depthClamp;
			device_features.features.depthBiasClamp = physical_device_features.depthBiasClamp;
			device_features.features.fillModeNonSolid = physical_device_features.fillModeNonSolid;
			device_features.features.depthBounds = physical_device_features.depthBounds;
			device_features.features.wideLines = physical_device_features.wideLines;
			device_features.features.largePoints = physical_device_features.largePoints;
			device_features.features.alphaToOne = physical_device_features.alphaToOne;
			device_features.features.multiViewport = physical_device_features.multiViewport;
			device_features.features.samplerAnisotropy = physical_device_features.samplerAnisotropy;
			device_features.features.textureCompressionETC2 = physical_device_features.textureCompressionETC2;
			device_features.features.textureCompressionASTC_LDR = physical_device_features.textureCompressionASTC_LDR;
			device_features.features.textureCompressionBC = physical_device_features.textureCompressionBC;
			device_features.features.occlusionQueryPrecise = physical_device_features.occlusionQueryPrecise;
			device_features.features.pipelineStatisticsQuery = physical_device_features.pipelineStatisticsQuery;
			device_features.features.vertexPipelineStoresAndAtomics = physical_device_features.vertexPipelineStoresAndAtomics;
			device_features.features.fragmentStoresAndAtomics = physical_device_features.fragmentStoresAndAtomics;
			device_features.features.shaderTessellationAndGeometryPointSize = physical_device_features.shaderTessellationAndGeometryPointSize;
			device_features.features.shaderImageGatherExtended = physical_device_features.shaderImageGatherExtended;
			device_features.features.shaderStorageImageExtendedFormats = physical_device_features.shaderStorageImageExtendedFormats;
			device_features.features.shaderStorageImageMultisample = physical_device_features.shaderStorageImageMultisample;
			device_features.features.shaderStorageImageReadWithoutFormat = physical_device_features.shaderStorageImageReadWithoutFormat;
			device_features.features.shaderStorageImageWriteWithoutFormat = physical_device_features.shaderStorageImageWriteWithoutFormat;
			device_features.features.shaderUniformBufferArrayDynamicIndexing = physical_device_features.shaderUniformBufferArrayDynamicIndexing;
			device_features.features.shaderSampledImageArrayDynamicIndexing = physical_device_features.shaderSampledImageArrayDynamicIndexing;
			device_features.features.shaderStorageBufferArrayDynamicIndexing = physical_device_features.shaderStorageBufferArrayDynamicIndexing;
			device_features.features.shaderStorageImageArrayDynamicIndexing = physical_device_features.shaderStorageImageArrayDynamicIndexing;
			device_features.features.shaderClipDistance = physical_device_features.shaderClipDistance;
			device_features.features.shaderCullDistance = physical_device_features.shaderCullDistance;
			device_features.features.shaderFloat64 = physical_device_features.shaderFloat64;
			device_features.features.shaderInt64 = physical_device_features.shaderInt64;
			device_features.features.shaderInt16 = physical_device_features.shaderInt16;
			device_features.features.shaderResourceResidency = physical_device_features.shaderResourceResidency;
			device_features.features.shaderResourceMinLod = physical_device_features.shaderResourceMinLod;
			device_features.features.sparseBinding = physical_device_features.sparseBinding;
			device_features.features.sparseResidencyBuffer = physical_device_features.sparseResidencyBuffer;
			device_features.features.sparseResidencyImage2D = physical_device_features.sparseResidencyImage2D;
			device_features.features.sparseResidencyImage3D = physical_device_features.sparseResidencyImage3D;
			device_features.features.sparseResidency2Samples = physical_device_features.sparseResidency2Samples;
			device_features.features.sparseResidency4Samples = physical_device_features.sparseResidency4Samples;
			device_features.features.sparseResidency8Samples = physical_device_features.sparseResidency8Samples;
			device_features.features.sparseResidency16Samples = physical_device_features.sparseResidency16Samples;
			device_features.features.sparseResidencyAliased = physical_device_features.sparseResidencyAliased;
			device_features.features.variableMultisampleRate = physical_device_features.variableMultisampleRate;
			device_features.features.inheritedQueries = physical_device_features.inheritedQueries;
		}
		add_extension(device_features);

		vk::PhysicalDeviceSynchronization2Features sync2;
		sync2.synchronization2 = true;
		add_extension(sync2);

		vk::PhysicalDeviceVulkan12Features device_vulkan12_features;
		device_vulkan12_features.drawIndirectCount = draw_indirect_supported;
		device_vulkan12_features.bufferDeviceAddress = true;
		device_vulkan12_features.timelineSemaphore = true;
		device_vulkan12_features.runtimeDescriptorArray = true;
		device_vulkan12_features.descriptorBindingVariableDescriptorCount = true;
		device_vulkan12_features.imagelessFramebuffer = true;
		add_extension(device_vulkan12_features);

		vk::PhysicalDeviceFragmentShadingRateFeaturesKHR fragment_shading_rate_features;
		fragment_shading_rate_features.attachmentFragmentShadingRate = vrs_supported;
		add_extension(fragment_shading_rate_features);

		//vk::PhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_pipeline_feature;
		//raytracing_pipeline_feature.rayTracingPipeline = ray_tracing_supported;
		//add_extension(raytracing_pipeline_feature);
		//
		//vk::PhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_feature;
		//acceleration_structure_feature.accelerationStructure = ray_tracing_supported;
		//add_extension(acceleration_structure_feature);
		//
		//vk::PhysicalDeviceRayQueryFeaturesKHR rayquery_pipeline_feature;
		//rayquery_pipeline_feature.rayQuery = ray_query_supported;
		//raytracing_pipeline_feature.rayTraversalPrimitiveCulling = ray_query_supported;
		//add_extension(rayquery_pipeline_feature);

		vk::DeviceCreateInfo desc{
			{}, uint32_t(queue_count), queue_infos.data(), 0, nullptr, uint32_t(exts.size()), exts.data(), nullptr, device_create_info_next
		};

		device = wis::shared_handle<vk::Device>{ adapter.createDevice(desc) };
		return device;
}

wis::VKSwapChain wis::VKDevice::CreateSwapchain(VKCommandQueueView render_queue, wis::SwapchainOptions options, wis::SurfaceParameters xsurface, bool vsync)const
{
	if (xsurface.IsWinRT())return{}; // Bail out, no support for UWP from Vulkan

	auto instance = VKFactory::Internal::GetInstanceHandle();

#if defined(WISDOM_WINDOWS)
	vk::Win32SurfaceCreateInfoKHR surface_desc{
		{}, GetModuleHandle(nullptr), xsurface.hwnd
	};
	wis::lib_info("Initializing Win32 Surface");
	vk::UniqueSurfaceKHR surface{ instance->createWin32SurfaceKHRUnique(surface_desc) };
#elif defined(WISDOM_MACOS)
	vk::MetalSurfaceCreateInfoEXT surface_desc = {};
	//surface_desc.pLayer = (__bridge CAMetalLayer*)window;
	vk::UniqueSurfaceKHR surface{ instance->createMetalSurfaceEXTUnique(surface_desc) };
	wis::lib_info("Initializing Metal Surface");
#elif defined(WISDOM_LINUX)
	vk::XcbSurfaceCreateInfoKHR surface_desc = {};
	surface_desc.setConnection(xsurface.x11.connection);
	surface_desc.setWindow((ptrdiff_t)xsurface.x11.window);
	wis::lib_info("Initializing XCB Surface");
	vk::UniqueSurfaceKHR surface{ instance->createXcbSurfaceKHRUnique(surface_desc) };
#endif
	int32_t present_queue = -1;
	for (uint16_t i = 0; i < max_count; i++)
	{
		auto& x = queues.available_queues[i];
		if (x.Empty())continue;

		if (adapter.getSurfaceSupportKHR(x.family_index, surface.get())) {
			present_queue = i; lib_info(wis::format("Present queue {} selected", i)); break;
		}
	}
	if (present_queue == -1)
	{
		lib_error("None of the queues support presenting to the surface");
		return{}; //Presentation is not supported
	}

	auto& queue = queues.available_queues[present_queue];
	vk::DeviceQueueInfo2 info{
		{},
			queue.family_index,
			queue.GetNextInLine()
	};
	vk::Queue qpresent_queue = device->getQueue2(info);


	auto surface_formats = adapter.getSurfaceFormatsKHR(surface.get());
	auto format = std::ranges::find_if(surface_formats,
		[=](const vk::SurfaceFormatKHR& fmt)
		{
			return fmt.format == vk_format(options.format);
		});
	if (format == surface_formats.end() || format->format == vk::Format::eUndefined)
	{
		lib_error(wis::format("Supplied format {} is not supported by surface", data_format_strings[+options.format]));
		return{}; //Format specified is not supported
	}

	auto cap = adapter.getSurfaceCapabilitiesKHR(surface.get());
	bool stereo = cap.maxImageArrayLayers > 1;
	if (options.stereo && stereo)
		lib_info(wis::format("Stereo mode is ativated"));

	uint32_t layers = options.stereo && stereo ? 2u : 1u;

	vk::SwapchainCreateInfoKHR desc
	{
		vk::SwapchainCreateFlagBitsKHR{}, surface.get(),
			options.frame_count, format->format, format->colorSpace,
			vk::Extent2D{ options.width, options.height },
			layers,
			vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
			vk::SharingMode::eExclusive, 0u, nullptr,
			vk::SurfaceTransformFlagBitsKHR::eIdentity,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			GetPresentMode(surface.get(), vsync), true, nullptr
	};

	return VKSwapChain{ wis::shared_handle<vk::SwapchainKHR>{device->createSwapchainKHR(desc), device}, std::move(surface), render_queue, VKCommandQueue{qpresent_queue}, CreateCommandList(QueueType::direct), format->format, stereo, layers };
}

wis::VKRenderPass wis::VKDevice::CreateRenderPass(Size2D frame_size, std::span<ColorAttachment> rtv_descs,
	DepthStencilAttachment dsv_desc,
	SampleCount samples,
	DataFormat vrs_format)const
{
	std::array<vk::AttachmentDescription2, max_render_targets + 2> attachment_descriptions{};
	std::array<vk::AttachmentReference2, max_render_targets + 2> attachment_references{};
	wis::internals::uniform_allocator<vk::FramebufferAttachmentImageInfo, max_render_targets + 2> image_md;

	size_t size = 0;

	for (auto& i : rtv_descs)
	{
		auto& desc = attachment_descriptions[size];
		auto& ref = attachment_references[size++];
		if (i.format == DataFormat::unknown)
		{
			ref.attachment = VK_ATTACHMENT_UNUSED;
			continue;
		}

		desc.format = vk_format(i.format);
		desc.samples = static_cast<vk::SampleCountFlagBits>(samples);
		desc.loadOp = convert(i.load);
		desc.storeOp = convert(i.store);
		desc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
		desc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

		ref.attachment = size - 1;
		ref.layout = vk::ImageLayout::eColorAttachmentOptimal;


		image_md.allocate(vk::ImageCreateFlags{}, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
			frame_size.width, frame_size.height, i.array_levels, 1u, &desc.format);

		if (size == 8)break;
	}

	vk::SubpassDescription2 sub_pass;
	sub_pass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	sub_pass.colorAttachmentCount = size;
	sub_pass.pColorAttachments = attachment_references.data();

	vk::AttachmentReference2 depth_reference;
	if (dsv_desc.format != DataFormat::unknown)
	{
		auto& desc = attachment_descriptions[size];
		auto& ref = attachment_references[size++];

		desc.format = vk_format(dsv_desc.format);
		desc.samples = static_cast<vk::SampleCountFlagBits>(samples);
		desc.loadOp = convert(dsv_desc.depth_load);
		desc.storeOp = convert(dsv_desc.depth_store);
		desc.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		desc.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		desc.stencilLoadOp = convert(dsv_desc.stencil_load);
		desc.stencilStoreOp = convert(dsv_desc.stencil_store);

		ref.attachment = size - 1;
		ref.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

		image_md.allocate(
			vk::ImageCreateFlags{}, vk::ImageUsageFlagBits::eDepthStencilAttachment,
			frame_size.width, frame_size.height, 1u, 1u, &desc.format
		);

		sub_pass.pDepthStencilAttachment = &attachment_references[size - 1];
	}

	vk::AttachmentReference2 shading_rate_image_attachment_reference;
	vk::FragmentShadingRateAttachmentInfoKHR fragment_shading_rate_attachment_info;
	fragment_shading_rate_attachment_info.pFragmentShadingRateAttachment = &shading_rate_image_attachment_reference;

	if (vrs_format != DataFormat::unknown && vrs_supported)
	{
		auto& desc = attachment_descriptions[size];
		auto& ref = attachment_references[size++];

		desc.format = vk_format(vrs_format);
		desc.samples = static_cast<vk::SampleCountFlagBits>(samples);
		desc.loadOp = vk::AttachmentLoadOp::eLoad;
		desc.storeOp = vk::AttachmentStoreOp::eStore;
		desc.initialLayout = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;
		desc.finalLayout = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;

		ref.attachment = size - 1;
		ref.layout = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;

		sub_pass.pDepthStencilAttachment = &attachment_references[size - 1];

		vk::PhysicalDeviceFragmentShadingRatePropertiesKHR shading_rate_image_properties;
		vk::PhysicalDeviceProperties2 device_props2;
		device_props2.pNext = &shading_rate_image_properties;
		adapter.getProperties2(&device_props2);
		auto vrs_size = shading_rate_image_properties.maxFragmentShadingRateAttachmentTexelSize.width;

		fragment_shading_rate_attachment_info.shadingRateAttachmentTexelSize.width = vrs_size;
		fragment_shading_rate_attachment_info.shadingRateAttachmentTexelSize.height = vrs_size;
		sub_pass.pNext = &fragment_shading_rate_attachment_info;
	}

	vk::RenderPassCreateInfo2 render_pass_info;
	render_pass_info.attachmentCount = size;
	render_pass_info.pAttachments = attachment_descriptions.data();
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &sub_pass;
	auto rp = wis::shared_handle{ device->createRenderPass2KHR(render_pass_info, nullptr, DynamicLoader::loader), device };


	vk::FramebufferAttachmentsCreateInfo attachments_create_info;
	attachments_create_info.attachmentImageInfoCount = image_md.size();
	attachments_create_info.pAttachmentImageInfos = image_md.data();

	vk::FramebufferCreateInfo desc{
		vk::FramebufferCreateFlagBits::eImageless,
			rp.get(), uint32_t(image_md.size()), nullptr, frame_size.width, frame_size.height, 1, &attachments_create_info
	};

	return VKRenderPass{
		std::move(rp),
		wis::shared_handle<vk::Framebuffer>{device->createFramebuffer(desc), device},
		frame_size
	};
}

wis::VKPipelineState wis::VKDevice::CreateGraphicsPipeline(wis::VKGraphicsPipelineDesc desc, std::span<const InputLayoutDesc> input_layout)const
{
	std::array<vk::VertexInputBindingDescription, max_vertex_bindings> bindings;
	wis::internals::uniform_allocator<vk::VertexInputAttributeDescription, max_vertex_bindings * 16> attributes;

	wis::internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages> shader_stages;
	FillShaderStages(desc, shader_stages);

	std::bitset<max_vertex_bindings> binding_map;
	for (auto& i : input_layout)
	{
		auto& b = bindings.at(i.input_slot);
		if (!binding_map[i.input_slot])
		{
			b.inputRate = vk::VertexInputRate(i.input_slot_class);
			b.binding = i.input_slot;
			b.stride = 0; // we don't care abot stride, since we bind dynamic vertex buffers
			binding_map.set(i.input_slot);
		}
		auto& at = attributes.allocate();
		at.binding = i.input_slot;
		at.format = convert_vk(i.format);
		at.location = i.location;
		at.offset = i.aligned_byte_offset;
	}

	// remove empty bindings and compact the array
	size_t rsize = 0;
	for (size_t i = rsize; i < max_vertex_bindings; i++)
		if (binding_map[i])bindings[rsize++] = bindings[i];


	vk::PipelineVertexInputStateCreateInfo ia{
		vk::PipelineVertexInputStateCreateFlagBits{},
			uint32_t(rsize),
			bindings.data(),
			uint32_t(attributes.size()),
			attributes.data()
	};


	vk::PipelineViewportStateCreateInfo viewport_state;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount = 1;

	vk::PipelineRasterizationStateCreateInfo rasterizer
	{
		vk::PipelineRasterizationStateCreateFlags{},
			false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise,
			false, 0.0f, 0.0f, 0.0f, 1.0f
	};

	vk::PipelineColorBlendAttachmentState color_blend_attachment[2]{ // 1 for now, TODO: proper blending
		vk::PipelineColorBlendAttachmentState{false, // disabled
			vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
			vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
			},
		vk::PipelineColorBlendAttachmentState{false, // disabled
			vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
			vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
			},

	};
	vk::PipelineColorBlendStateCreateInfo color_blending{
		vk::PipelineColorBlendStateCreateFlags{},
			false, vk::LogicOp::eCopy,
			2, color_blend_attachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f }
	};

	vk::PipelineMultisampleStateCreateInfo multisampling{};
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisampling.sampleShadingEnable = false;

	vk::PipelineInputAssemblyStateCreateInfo input_assembly{
		vk::PipelineInputAssemblyStateCreateFlags{},
			vk::PrimitiveTopology::eTriangleList, false
	};

	wis::internals::uniform_allocator<vk::DynamicState, 5> dynamic_state_enables;
	dynamic_state_enables.allocate(vk::DynamicState::eViewport);
	dynamic_state_enables.allocate(vk::DynamicState::eScissor);
	dynamic_state_enables.allocate(vk::DynamicState::ePrimitiveTopology);
	dynamic_state_enables.allocate(vk::DynamicState::eVertexInputBindingStride);
	if (vrs_supported)
		dynamic_state_enables.allocate(vk::DynamicState::eFragmentShadingRateKHR);

	vk::PipelineDynamicStateCreateInfo dss
	{
		{}, uint32_t(dynamic_state_enables.size()),
			dynamic_state_enables.data()
	};

	vk::GraphicsPipelineCreateInfo pipeline_desc
	{
		vk::PipelineCreateFlags{},
			uint32_t(shader_stages.size()),
			shader_stages.data(), // shader stages
			&ia, // vertex input
			&input_assembly, // input assembly
			nullptr, // tessellation
			&viewport_state, // viewport
			&rasterizer, // rasterizer
			&multisampling, // multisampling
			nullptr, // depth stencil
			&color_blending, // color blending
			&dss, // dynamic state
			desc.sig, // pipeline layout
			desc.pass.GetInternal().GetRenderPass(), // render pass					
	};


	return VKPipelineState{ wis::shared_handle<vk::Pipeline>{device->createGraphicsPipeline(nullptr, pipeline_desc).value, device} };
}


wis::VKRenderTargetView wis::VKDevice::CreateRenderTargetView(VKTextureView texture, RenderSelector range)const
{
	vk::ImageViewCreateInfo desc{
		vk::ImageViewCreateFlags{},
			texture.image, vk::ImageViewType::e2DArray,
			texture.format, {},
			vk::ImageSubresourceRange{
			aspect_flags(texture.format),
				range.mip, 1u,
				range.base_layer, range.extent_layers,
		}
	};
	return VKRenderTargetView{ wis::shared_handle{device->createImageView(desc), device} };
}


void wis::VKDevice::GetQueueFamilies(VKAdapterView adapter)noexcept
{
	using namespace river::flags;
	auto family_props = adapter.getQueueFamilyProperties();
	wis::lib_info(wis::format("The system supports {} queue families", family_props.size()));

	for (uint8_t i = 0; i < family_props.size(); i++)
	{
		using enum vk::QueueFlagBits;
		auto& family = family_props[i];
		if ((family.queueFlags & eGraphics) == eGraphics && queues.available_queues[+QueueTypes::graphics].Empty())
		{
			queues.available_queues[+QueueTypes::graphics] = {
				.queue_flags = uint16_t(uint32_t(family.queueFlags)),
				.count = uint8_t(family.queueCount),
				.family_index = i,
			};
			wis::lib_info(wis::format("\tDedicated Graphics queues: {}", family.queueCount));
			continue;
		}
		if ((family.queueFlags & eCompute) == eCompute && queues.available_queues[+QueueTypes::compute].Empty())
		{
			queues.available_queues[+QueueTypes::compute] = {
				.queue_flags = uint16_t(uint32_t(family.queueFlags)),
				.count = uint8_t(family.queueCount),
				.family_index = i,
			};
			wis::lib_info(wis::format("\tDedicated Compute queues: {}", family.queueCount));
			continue;
		}
		if ((family.queueFlags & eVideoDecodeKHR) == eVideoDecodeKHR && queues.available_queues[+QueueTypes::video_decode].Empty())
		{
			queues.available_queues[+QueueTypes::video_decode] = {
				.queue_flags = uint16_t(uint32_t(family.queueFlags)),
				.count = uint8_t(family.queueCount),
				.family_index = i,
			};
			wis::lib_info(wis::format("\tDedicated Video Decode queues: {}", family.queueCount));
			continue;
		}
		if ((family.queueFlags & eTransfer) == eTransfer && queues.available_queues[+QueueTypes::copy].Empty())
		{
			queues.available_queues[+QueueTypes::copy] = {
				.queue_flags = uint16_t(uint32_t(family.queueFlags)),
				.count = uint8_t(family.queueCount),
				.family_index = i,
			};
			wis::lib_info(wis::format("\tDedicated Data Transfer queues: {}", family.queueCount));
			continue;
		}
	}
}



wis::internals::uniform_allocator<const char*, wis::VKDevice::required_extensions.size()>
wis::VKDevice::RequestExtensions(VKAdapterView adapter)noexcept
{
	auto extensions = adapter.enumerateDeviceExtensionProperties();
	std::unordered_set<std::string_view, wis::string_hash> ext_set;
	ext_set.reserve(extensions.size());

	for (const auto& e : extensions)
		ext_set.emplace(e.extensionName.data());

	wis::internals::uniform_allocator<const char*, required_extensions.size()> avail_exts{};

	for (auto* i : required_extensions)
	{
		if (!ext_set.contains(i))continue;
		avail_exts.allocate(i);

		if (i == std::string_view(VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME))
			vrs_supported = true;
		else if (i == std::string_view(VK_NV_MESH_SHADER_EXTENSION_NAME))
			mesh_shader_supported = true;
		else if (i == std::string_view(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME))
			ray_tracing_supported = true;
		else if (i == std::string_view(VK_KHR_RAY_QUERY_EXTENSION_NAME))
			ray_query_supported = true;
		else if (i == std::string_view(VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME))
			draw_indirect_supported = true;
	}

	if constexpr (wis::debug_mode)
	{
		wis::lib_info("Active Device Extensions:");
		for (auto& i : avail_exts)
			wis::lib_info(wis::format("\t{}", i));
	}

	return avail_exts;
}

vk::PresentModeKHR wis::VKDevice::GetPresentMode(vk::SurfaceKHR surface, bool vsync)const noexcept
{
	using enum vk::PresentModeKHR;
	auto modes = adapter.getSurfacePresentModesKHR(surface);
	return vsync ?
		std::ranges::count(modes, eFifoRelaxed) ?
		eFifoRelaxed : eFifo
		: eImmediate;
}

void wis::VKDevice::FillShaderStages(const VKGraphicsPipelineDesc& desc, wis::internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages>& shader_stages)const noexcept
{
	if (desc.vs)
	{
		auto& vs = shader_stages.allocate();
		vs.stage = vk::ShaderStageFlagBits::eVertex;
		vs.module = desc.vs.GetInternal().GetShaderModule();
		vs.pName = "main";
	}
	if (desc.ps)
	{
		auto& vs = shader_stages.allocate();
		vs.stage = vk::ShaderStageFlagBits::eFragment;
		vs.module = desc.ps.GetInternal().GetShaderModule();
		vs.pName = "main";
	}
	if (desc.gs)
	{
		auto& vs = shader_stages.allocate();
		vs.stage = vk::ShaderStageFlagBits::eGeometry;
		vs.module = desc.gs.GetInternal().GetShaderModule();
		vs.pName = "main";
	}
	if (desc.hs)
	{
		auto& vs = shader_stages.allocate();
		vs.stage = vk::ShaderStageFlagBits::eTessellationControl;
		vs.module = desc.hs.GetInternal().GetShaderModule();
		vs.pName = "main";
	}
	if (desc.ds)
	{
		auto& vs = shader_stages.allocate();
		vs.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
		vs.module = desc.ds.GetInternal().GetShaderModule();
		vs.pName = "main";
	}
}
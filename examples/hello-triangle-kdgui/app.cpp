#include "app.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>


struct LogProvider : public wis::LogLayer
{
	virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current())override
	{
		std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
	};
};

constexpr wis::ApplicationInfo app_info{
	.application_name = "example",
		.engine_name = "none",
};

// not WinRT Compatible
template<class ShaderTy>
auto LoadShader(std::filesystem::path p)
{
	if constexpr (ShaderTy::language == wis::ShaderLang::dxil)
		p += ".cso";
	else if constexpr (ShaderTy::language == wis::ShaderLang::spirv)
		p += ".spv";

	std::ifstream t{ p, std::ios::binary };
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	wis::shared_blob ret{ size };
	t.seekg(0);
	t.read(ret.data<char>(), size);
	return ret;
}

template<class T>
std::span<std::byte> RawView(T& data)
{
	return { (std::byte*)&data, sizeof(T) };
}

Test::App::App(uint32_t width, uint32_t height)
	:wnd(app.createWindow(width, height))
{
	wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

	factory.emplace(app_info);

	for (auto&& a : factory->EnumerateAdapters(wis::AdapterPreference::Performance))
	{
		auto desc = a.GetDesc();

		if (desc.IsSoftware())
			wis::lib_warn("Loading WARP adapter");

		std::cout << desc.to_string();

		if (device.Initialize(a))
		{
			allocator = wis::ResourceAllocator{ device, a };
			break;
		}
	}

	queue = device.CreateCommandQueue();

	swap = device.CreateSwapchain(queue, wis::SwapchainOptions{
			uint32_t(width),
			uint32_t(height),
			wis::SwapchainOptions::default_frames,
			wis::SwapchainOptions::default_format,
			true
		}, wnd.GetSurfaceOptions());

	fence = device.CreateFence();
	context = device.CreateCommandList(wis::QueueType::direct);

	std::array cas2{
		wis::ColorAttachment {
			.format = wis::SwapchainOptions::default_format,
				.load = wis::PassLoadOperation::clear
		},
		wis::ColorAttachment {
			.format = wis::SwapchainOptions::default_format,
				.load = wis::PassLoadOperation::clear
		}
	};

	render_pass = device.CreateRenderPass({ width, height }, { cas2.data(), swap.StereoSupported() + 1u });

	vs = device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.vs"), wis::ShaderType::vertex);
	ps = device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.ps"), wis::ShaderType::pixel);

	root = device.CreateRootSignature();//empty

	static constexpr std::array<wis::InputLayoutDesc, 2> ia{
		wis::InputLayoutDesc{ 0, "POSITION", 0, wis::DataFormat::r32g32b32_float, 0, 0, wis::InputClassification::vertex, 0 },
		wis::InputLayoutDesc{ 1, "COLOR", 0, wis::DataFormat::r32g32b32a32_float, 0, 12, wis::InputClassification::vertex, 0 }
	};

	wis::GraphicsPipelineDesc desc{ root };
	desc.SetVS(vs);
	desc.SetPS(ps);
	desc.SetRenderPass(render_pass);
	pipeline = device.CreateGraphicsPipeline(std::move(desc), ia);

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec4 col;
	};
	auto aspect_ratio = float(width) / float(height);
	Vertex triangleVertices[] =
	{
		{ { 0.0f, 0.25f * aspect_ratio , 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.25f, -0.25f * aspect_ratio , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.25f, -0.25f * aspect_ratio , 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	vertex_buffer = allocator.CreatePersistentBuffer(sizeof(triangleVertices), wis::BufferFlags::VertexBuffer);

	auto upl_vbuf = allocator.CreateUploadBuffer(sizeof(triangleVertices));
	upl_vbuf.UpdateSubresource(RawView(triangleVertices));

	context.Reset();
	context.CopyBuffer(upl_vbuf, vertex_buffer, sizeof(triangleVertices));
	context.BufferBarrier({
		.access_before = wis::ResourceAccess::CopyDest,
		.access_after = wis::ResourceAccess::VertexBuffer
		}, vertex_buffer);
	context.Close();

	queue.ExecuteCommandList(context);
	WaitForGPU();

	vb = vertex_buffer.GetVertexBufferView(sizeof(Vertex));
	context.SetPipeline(pipeline);

	auto x = swap.GetRenderTargets();
	for (size_t i = 0; i < x.size(); i++)
	{
		rtvs[i] = device.CreateRenderTargetView(x[i]);
		if (swap.StereoSupported())
			rtvs2[i] = device.CreateRenderTargetView(x[i], { .base_layer = 1 });
	}

	uniforms = device.CreateDescriptorHeap(1u);
	device.CreateDescriptorSetLayout(0);

}
Test::App::~App()
{
	WaitForGPU();
}
int Test::App::Start()
{
	while (true)
	{
		app.ProcessEvents();
		if (!wnd.visible())
			return 0;
		//Process Events

		Frame();
	}
}
void Test::App::Frame()
{
	context.Reset();
	auto back = swap.GetBackBuffer();

	context.TextureBarrier({
		.state_before = wis::TextureState::Present,
		.state_after = wis::TextureState::RenderTarget,
		.access_before = wis::ResourceAccess::Common,
		.access_after = wis::ResourceAccess::RenderTarget
		}, back);

	constexpr wis::ColorClear color{ 0.0f, 0.2f, 0.4f, 1.0f };
	constexpr wis::ColorClear color2{ 1.0f, 0.2f, 0.4f, 1.0f };
	std::array rtvsx{
		std::pair{rtvs[swap.GetNextIndex()], color},
		std::pair{rtvs2[swap.GetNextIndex()], color2}
	};

	context.SetGraphicsRootSignature(root);
	context.RSSetViewport({ float(wnd.width()), float(wnd.height()) });
	context.RSSetScissorRect({ long(wnd.width()), long(wnd.height()) });
	context.IASetPrimitiveTopology(wis::PrimitiveTopology::trianglelist);
	context.IASetVertexBuffers({ &vb, 1 });

	context.BeginRenderPass(render_pass, { rtvsx.data(), swap.StereoSupported() + 1u });
	context.DrawInstanced(3);
	context.EndRenderPass();

	context.TextureBarrier({
		.state_before = wis::TextureState::RenderTarget,
		.state_after = wis::TextureState::Present,
		.access_before = wis::ResourceAccess::RenderTarget,
		.access_after = wis::ResourceAccess::Common,
		}, back);
	context.Close();
	queue.ExecuteCommandList(context);

	swap.Present();

	WaitForGPU();
}


void Test::App::WaitForGPU()
{
	const uint64_t vfence = fence_value;
	queue.Signal(fence, vfence);
	fence_value++;
	fence.Wait(vfence);
}

#include <example/app.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <DirectXMath.h>


struct LogProvider : public wis::LogLayer
{
	virtual void Log(wis::Severity sev, std::string message, std::source_location sl = std::source_location::current())override
	{
		std::cout << std::format("[{}]: {}\n", wis::severity_strings[+sev], message);
	};
};

constexpr wis::ApplicationInfo app_info{
	.application_name = "example",
		.engine_name = "none",
};

// not WinRT Compatible
//wis::Shader LoadShader(std::filesystem::path p, wis::ShaderType type)
//{
//	std::basic_ifstream<std::byte> t{p, std::ios::binary};
//	t.seekg(0, std::ios::end);
//	size_t size = t.tellg();
//	std::shared_ptr<std::byte[]> bytecode = std::make_shared<std::byte[]>(size);
//	t.seekg(0);
//	t.read(&bytecode[0], size);
//	return wis::Shader{ std::move(bytecode), size, type };
//}
template<class T>
std::span<std::byte> RawView(T& data)
{
	return { (std::byte*)&data, sizeof(T) };
}

Test::App::App(uint32_t width, uint32_t height)
	:wnd(width, height, "VTest")
{
	wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

	factory.emplace(app_info);

	for (auto&& a : factory->EnumerateAdapters(wis::AdapterPreference::Performance))
	{
		auto desc = a.GetDesc();
		if (desc.IsSoftware())
			wis::lib_warn("Loading WARP adapter");

		std::wcout << desc.to_string();

	//	if (device.Initialize(a)) 
	//	{ 
	//		allocator = { device, a };
	//		break;
	//	}
	}
	//
	//queue = device.CreateCommandQueue();
	//swap = factory->CreateSwapchain(queue, {
	//		.width = uint32_t(wnd.GetWidth()),
	//		.height = uint32_t(wnd.GetHeight())
	//	},
	//	wis::SurfaceParameters{
	//	wnd.GetHandle()
	//});
	//fence = device.CreateFence();
	//context = device.CreateCommandList(wis::CommandListType::direct);
	//
	//vs = LoadShader("shaders/example.vs.cso", wis::ShaderType::vertex);
	//ps = LoadShader("shaders/example.ps.cso", wis::ShaderType::pixel);
	//
	//root = device.CreateRootSignature();//empty
	//
	//static constexpr std::array<wis::InputLayoutDesc, 2> ia{
	//	wis::InputLayoutDesc{ "POSITION", 0, wis::DataFormat::r32g32b32_float, 0, 0, wis::InputClassification::vertex, 0 },
	//	wis::InputLayoutDesc{ "COLOR", 0, wis::DataFormat::r32g32b32a32_float, 0, 12, wis::InputClassification::vertex, 0 }
	//};
	//
	//wis::GraphicsPipelineDesc desc{root};
	//desc.SetVS(vs);
	//desc.SetPS(ps);
	//desc.SetRenderTarget(wis::DataFormat::b8g8r8a8_unorm, 0);
	//pipeline = device.CreateGraphicsPipeline(std::move(desc), ia);
	//
	//struct Vertex
	//{
	//	DirectX::XMFLOAT3 pos;
	//	DirectX::XMFLOAT4 col;
	//};
	//auto aspect_ratio = float(width) / float(height);
	//Vertex triangleVertices[] =
	//{
	//	{ { 0.0f, 0.25f * aspect_ratio , 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	//	{ { 0.25f, -0.25f * aspect_ratio , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	//	{ { -0.25f, -0.25f * aspect_ratio , 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	//};
	//
	//vertex_buffer = allocator.CreatePersistentBuffer(sizeof(triangleVertices));
	//auto upl_vbuf = allocator.CreateUploadBuffer(sizeof(triangleVertices));
	//upl_vbuf.UpdateSubresource(RawView(triangleVertices));
	//
	//context.Reset();
	//context.CopyBuffer(upl_vbuf, vertex_buffer, sizeof(triangleVertices));
	//context.ResourceBarrier(wis::TransitionBarrier{
	//	.resource = vertex_buffer,
	//	.before = wis::ResourceState::copy_dest,
	//	.after = wis::ResourceState::vertex_and_constant_buffer
	//});
	//context.Close();
	//
	//queue.ExecuteCommandList(context);
	//WaitForGPU();
	//
	//vb = vertex_buffer.GetVertexBufferView(sizeof(Vertex));
	//context.SetPipeline(pipeline);
}

int Test::App::Start()
{
	while (true)
	{
		if (const auto a = wnd.ProcessMessages())
			return (int)a.value();

		Frame();
	}
}

void Test::App::Frame()
{
	//context.Reset();
	//const auto& back = swap.GetBackBuffer();
	//auto rtv = swap.GetBackBufferRTV();
	//constexpr std::array<float, 4> color{0.0f, 0.2f, 0.4f, 1.0f};
	//
	//context.ResourceBarrier(wis::TransitionBarrier{
	//	.resource = back,
	//	.before = wis::ResourceState::present,
	//	.after = wis::ResourceState::render_target
	//});
	//
	//context.SetGraphicsRootSignature(root);
	//context.RSSetViewport({ .width = float(wnd.GetWidth()), .height = float(wnd.GetHeight()) });
	//context.RSSetScissorRect({ .right = wnd.GetWidth(), .bottom = wnd.GetHeight() });
	//context.IASetPrimitiveTopology(wis::PrimitiveTopology::trianglelist);
	//context.ClearRenderTarget(rtv, color);
	//context.IASetVertexBuffers({&vb, 1});
	//context.OMSetRenderTargets(std::array{rtv});
	//context.DrawInstanced(3);
	//
	//context.ResourceBarrier(wis::TransitionBarrier{
	//	.resource = back,
	//	.before = wis::ResourceState::render_target,
	//	.after = wis::ResourceState::present
	//});
	//context.Close();
	//
	//queue.ExecuteCommandList(context);
	//swap.Present();
	//wis::check_context();
	//WaitForGPU();
}

void Test::App::WaitForGPU()
{
	//const UINT64 vfence = fence_value;
	//queue.Signal(fence, vfence);
	//fence_value++;
	//
	//fence.Wait(vfence);
}

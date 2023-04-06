#include <example/app.h>
#include <iostream>
#include <filesystem>
#include <fstream>


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
wis::Shader LoadShader(std::filesystem::path p, wis::ShaderType type)
{
	std::vector<std::byte> bytecode;
	std::basic_ifstream<std::byte> t{p, std::ios::binary};
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	bytecode.resize(size);
	t.seekg(0);
	t.read(&bytecode[0], size);
	return wis::Shader{ std::move(bytecode), type };
}

Test::App::App(uint32_t width, uint32_t height)
	:wnd(width, height, "VTest")
{
	wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

	factory.emplace(app_info);

	for (auto&& a : factory->EnumerateAdapters(wis::AdapterPreference::Performance))
	{
		if (a.GetDesc().IsSoftware())
			wis::lib_warn("Loading WARP adapter");
		if (device.Initialize(a))break;
	}
	
	queue = device.CreateCommandQueue();
	swap = factory->CreateSwapchain(queue, {
			.width = uint32_t(wnd.GetWidth()),
			.height = uint32_t(wnd.GetHeight())
		},
		wis::SurfaceParameters{
		wnd.GetHandle()
	});
	context = device.CreateCommandList(wis::CommandListType::direct);
	fence = device.CreateFence();

	vs = LoadShader("shaders/example.vs.cso", wis::ShaderType::vertex);
	ps = LoadShader("shaders/example.ps.cso", wis::ShaderType::pixel);
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
	context.Reset();
	const auto& back = swap.GetBackBuffer();
	auto rtv = swap.GetBackBufferRTV();
	constexpr std::array<float, 4> color{0.0f, 0.2f, 0.4f, 1.0f};
	
	context.ResourceBarrier(wis::TransitionBarrier{
		.resource = back,
		.before = wis::ResourceState::present,
		.after = wis::ResourceState::render_target
	});
	
	context.ClearRenderTarget(rtv, color);
	
	context.ResourceBarrier(wis::TransitionBarrier{
		.resource = back,
		.before = wis::ResourceState::render_target,
		.after = wis::ResourceState::present
	});
	context.Close();
	
	
	queue.ExecuteCommandList(context);
	swap.Present();

	const UINT64 vfence = fence_value;
	queue.Signal(fence, vfence);
	fence_value++;
	
	fence.Wait(vfence);
}

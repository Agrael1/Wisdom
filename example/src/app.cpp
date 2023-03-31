#include <example/app.h>
#include <iostream>



struct LogProvider : public wis::LogLayer
{
	virtual void Log(wis::Severity sev, std::string message, std::source_location sl = std::source_location::current())override 
	{
		std::cout << std::format("[{}]: {}\n", wis::severity_strings[+sev], message);
	};
};

Test::App::App(uint32_t width, uint32_t height)
	:wnd(width, height, "VTest")
{
	wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

	for (auto&& a : factory.EnumerateAdapters(wis::AdapterPreference::Performance))
	{
		if (a.GetDesc().IsSoftware())
			wis::lib_warn("Loading WARP adapter");
		if (device.Initialize(a))break;
	}

	queue = device.CreateCommandQueue();
	swap = factory.CreateSwapchain(queue, {
			.width = uint32_t(wnd.GetWidth()),
			.height = uint32_t(wnd.GetHeight())
		},
		wis::SurfaceParameters{
		wnd.GetHandle()
	});
}
//ver::IAsyncAction Test::App::InitializeAsync()
//{
//	co_await gfx.InitializeAsync();
//	co_await gfx.InitializeSwapChainAsync(
//		{
//			.width = uint32_t(wnd.GetWidth()),
//			.height = uint32_t(wnd.GetHeight())
//		},
//		ver::SurfaceParameters{
//			wnd.GetHandle()
//		}
//	);
//
//}
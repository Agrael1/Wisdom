#pragma once
#include <example/window.h>
#include <wisdom/wisdom.h>

namespace Test
{
	class App
	{
	public:
		App(uint32_t width, uint32_t height);
	public:
		int Start();
		void Frame();
	private:
		Window wnd;

		wis::GraphicsInfo info;

		std::optional<wis::Factory> factory;
		wis::Device device;
		wis::CommandQueue queue;
		wis::SwapChain swap;
		wis::CommandList context;
		wis::Fence fence;

		uint64_t fence_value = 1;
	};
}
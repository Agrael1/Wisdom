#pragma once
#include <example/window.h>
//#define WFORCEVK
#include <wisdom/wisdom.h>

namespace Test
{
	class App
	{
	public:
		App(uint32_t width, uint32_t height);
		~App()
		{
			WaitForGPU();
		}
	public:
		int Start();
		void Frame();
		void WaitForGPU();
	private:
		Window wnd;

		std::optional<wis::Factory> factory;

		wis::Device device;
		wis::CommandQueue queue;
		wis::SwapChain swap;

		wis::CommandList context;
		wis::Fence fence;
		wis::ResourceAllocator allocator;
		
		wis::Shader vs;
		wis::Shader ps;
		
		wis::RootSignature root;
		wis::PipelineState pipeline;
		wis::VertexBufferView vb;
		wis::RenderTargetView rtvs[2];
		wis::RenderTargetView rtvs2[2];
		
		wis::Resource vertex_buffer;
		wis::RenderPass render_pass;
		uint64_t fence_value = 1;
	};
}
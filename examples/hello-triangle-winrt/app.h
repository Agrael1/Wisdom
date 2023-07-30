#pragma once
#include <wisdom/wisdom.h>

namespace Test {
	class App
	{
	public:
		App();
		~App();
		void Initialize(IUnknown* core_window, uint32_t width, uint32_t height);
	public:
		void Frame();
	private:
		void WaitForGPU();
	private:
		uint32_t width = 0;
		uint32_t height = 0;

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

		wis::Buffer vertex_buffer;
		wis::RenderPass render_pass;
		uint64_t fence_value = 1;
	};
}
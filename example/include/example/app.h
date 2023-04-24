#pragma once
#include <example/window.h>
#include <wisdom/wisdom.h>
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_allocator.h>

namespace Test
{
	class App
	{
	public:
		App(uint32_t width, uint32_t height);
	public:
		int Start();
		void Frame();
		void WaitForGPU();
		void WaitForGPU2();
	private:
		Window wnd;

		std::optional<wis::Factory> factory;
		std::optional<wis::DX12Factory> factory2;

		wis::Device device;
		wis::DX12Device device2;

		wis::CommandQueue queue;
		wis::DX12CommandQueue queue2;

		wis::SwapChain swap;
		wis::DX12SwapChain swap2;


		//wis::CommandList context;
		wis::Fence fence;
		wis::DX12Fence fence2;
		wis::ResourceAllocator allocator;
		wis::DX12ResourceAllocator allocator2;
		//
		//wis::Shader vs;
		//wis::Shader ps;
		//
		//wis::RootSignature root;
		//wis::PipelineState pipeline;
		//wis::VertexBufferView vb;
		//
		wis::Resource vertex_buffer;
		wis::DX12Resource vertex_buffer2;

		uint64_t fence_value = 1;
		uint64_t fence_value2 = 1;
	};
}
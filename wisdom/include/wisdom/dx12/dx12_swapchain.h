#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_rtv.h>
#include <wisdom/dx12/dx12_resource.h>

#include <dxgi1_6.h>
#include <d3dx12/d3dx12.h>
#include <span>

namespace wis
{
	class DX12SwapChain;

	template<>
	class Internal<DX12SwapChain>
	{
		friend class DX12SwapChain;
		static constexpr inline bool valid = true;
	public:
		Internal() = default;
		Internal(winrt::com_ptr<IDXGISwapChain4> chain)
			:chain(std::move(chain)){}
	public:
		template<class Self>
		[[nodiscard]] auto GetSwapChain(this Self&& s)noexcept {
			return s.chain;
		}
	protected:
		winrt::com_ptr<IDXGISwapChain4> chain{};
	};

	class DX12SwapChain : public QueryInternal<DX12SwapChain>
	{
		friend class DX12Factory;
	public:
		DX12SwapChain() = default;
		explicit DX12SwapChain(winrt::com_ptr<IDXGISwapChain4> xchain, uint32_t frame_count, bool stereo)
			:QueryInternal(std::move(xchain)), stereo(stereo)
		{
			winrt::com_ptr<ID3D12Device> device;
			chain->GetDevice(__uuidof(ID3D12Device), device.put_void());

			render_targets.reserve(frame_count);
			for (uint32_t n = 0; n < frame_count; n++)
			{
				winrt::com_ptr<ID3D12Resource> rc;
				wis::check_hresult(chain->GetBuffer(n, __uuidof(ID3D12Resource), rc.put_void()));
				render_targets.emplace_back(std::move(rc), nullptr);
			}
			
		}
	public:
		[[nodiscard]] 
		uint32_t GetNextIndex()const noexcept
		{
			return chain->GetCurrentBackBufferIndex();
		}
		template<class Self>
		[[nodiscard]] 
		std::span<wis::cv_type_t<Self, DX12Buffer>> GetRenderTargets(this Self&& s)noexcept
		{
			return s.render_targets;
		}
		template<class Self>
		[[nodiscard]] 
		auto& GetBackBuffer(this Self&& s)noexcept
		{
			return s.render_targets[s.chain->GetCurrentBackBufferIndex()];
		}

		bool Present()noexcept
		{
			return wis::succeded_weak(chain->Present(0, 0));
		}
		[[nodiscard]] 
		bool StereoSupported()const noexcept
		{
			return stereo;
		}
	private:
		std::vector<DX12Buffer> render_targets{};
		bool stereo = false;
	};
}

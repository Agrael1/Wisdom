#pragma once
#include <wisdom/util/log_layer.h>
#include <vector>
#include <winrt/base.h>
#include <dxgidebug.h>

namespace wis
{
	struct DXGIMessage
	{
		Severity severity;
		std::string description;
	};

	class DX12Info
	{
	public:
		DX12Info();
		~DX12Info();
		DX12Info(const DX12Info&) = delete;
		DX12Info& operator=(const DX12Info&) = delete;
	public:
		[[nodiscard]] static uint64_t GetNumMessages()noexcept;
		[[nodiscard]] static std::vector<DXGIMessage> GetMessages()noexcept;
	private:
		static winrt::com_ptr<IDXGIInfoQueue> info_queue;
	};
}
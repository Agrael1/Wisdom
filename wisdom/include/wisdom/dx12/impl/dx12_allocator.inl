#ifndef WISDOM_MODULES
#include <d3dx12/d3dx12.h>
#endif

wis::DX12ResourceAllocator::DX12ResourceAllocator(wis::DX12DeviceView device, wis::DX12AdapterView adapter)
{
	D3D12MA::ALLOCATOR_DESC desc{
		.Flags = D3D12MA::ALLOCATOR_FLAGS::ALLOCATOR_FLAG_NONE,
		.pDevice = device,
		.PreferredBlockSize = 0,
		.pAllocationCallbacks = nullptr,
		.pAdapter = adapter
	};
	wis::check_hresult(D3D12MA::CreateAllocator(&desc, allocator.put()));
}
wis::DX12Buffer wis::DX12ResourceAllocator::CreatePersistentBuffer(size_t size, [[maybe_unused]] BufferFlags flags)
{
	using namespace river::flags;
	winrt::com_ptr<ID3D12Resource> rc;
	winrt::com_ptr<D3D12MA::Allocation> al;
	auto desc = CD3DX12_RESOURCE_DESC1::Buffer(size);
	D3D12MA::ALLOCATION_DESC all_desc = {};
	all_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COPY_DEST;

	allocator->CreateResource2(&all_desc, &desc,
		state, nullptr,
		al.put(), __uuidof(*rc), rc.put_void());

	return DX12Buffer{ std::move(rc), std::move(al) };
}
wis::DX12Buffer wis::DX12ResourceAllocator::CreateUploadBuffer(size_t size)
{
	winrt::com_ptr<ID3D12Resource> rc;
	winrt::com_ptr<D3D12MA::Allocation> al;
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);
	D3D12MA::ALLOCATION_DESC all_desc = {};
	all_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

	allocator->CreateResource(&all_desc, &desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		al.put(), __uuidof(*rc), rc.put_void());

	return DX12Buffer{ std::move(rc), std::move(al) };
}
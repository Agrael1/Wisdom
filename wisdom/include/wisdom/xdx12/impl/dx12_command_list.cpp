#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_command_list.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xdx12/dx12_checks.h>

void wis::DX12CommandList::CopyBuffer(DX12BufferView source, DX12BufferView destination, wis::BufferRegion region) const noexcept
{
    list->CopyBufferRegion(std::get<0>(destination), region.dst_offset, std::get<0>(source), region.src_offset, region.size_bytes);
}

wis::Result wis::DX12CommandList::Reset(wis::DX12PipelineHandle pipeline) noexcept
{
    Close();
    auto hr = allocator->Reset();
    if (!wis::succeeded(hr)) {
        return wis::make_result<FUNC, "Reset failed (allocator)">(hr);
    }
    hr = list->Reset(allocator.get(), std::get<0>(pipeline));
    return wis::succeeded(hr) ? wis::success : wis::make_result<FUNC, "Reset failed (command list)">(hr);
}

bool wis::DX12CommandList::Close() noexcept
{
    if (closed) {
        return true;
    }
    return closed = succeeded(list->Close());
}
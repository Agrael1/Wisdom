/** \mainpage Wisdom API Documentation

<b>Version 0.6.6</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

module;
#include <wisdom/generated/api/api.hpp>
#include <wisdom/global/internal.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
export module wisdom.api;

export wis::ShaderStages;
export wis::Status;
export wis::QueueType;
export wis::DescriptorType;
export wis::MutiWaitFlags;
export wis::ASGeometryType;
export wis::AdapterPreference;
export wis::RaytracingShaderType;
export wis::Severity;
export wis::ASLevel;
export wis::HitGroupType;
export wis::InputClass;
export wis::CullMode;
export wis::DataFormat;
export wis::FillMode;
export wis::DescriptorMemory;
export wis::WindingOrder;
export wis::SampleRate;
export wis::SwapchainScaling;
export wis::Compare;
export wis::StencilOp;
export wis::BlendFactor;
export wis::BlendOp;
export wis::LogicOp;
export wis::MemoryType;
export wis::ShaderIntermediate;
export wis::TextureState;
export wis::LoadOperation;
export wis::TextureLayout;
export wis::DescriptorHeapType;
export wis::StoreOperation;
export wis::PrimitiveTopology;
export wis::TopologyType;
export wis::DeviceFeature;
export wis::Filter;
export wis::AddressMode;
export wis::TextureViewType;
export wis::ComponentSwizzle;
export wis::IndexType;
export wis::FactoryExtID;
export wis::DeviceExtID;
export wis::ASCopyMode;

//-------------------------------------------------------------------------

export wis::AdapterFlags;
export wis::DSSelect;
export wis::ColorComponents;
export wis::BufferUsage;
export wis::MemoryFlags;
export wis::RenderPassFlags;
export wis::BarrierSync;
export wis::ResourceAccess;
export wis::TextureUsage;
export wis::FenceFlags;
export wis::PipelineFlags;
export wis::ASGeometryFlags;
export wis::AccelerationStructureFlags;
export wis::ASInstanceFlags;

//-------------------------------------------------------------------------

export wis::Result;
export wis::Size2D;
export wis::Size3D;
export wis::BufferRegion;
export wis::AdapterDesc;
export wis::InputSlotDesc;
export wis::InputAttribute;
export wis::InputLayout;
export wis::RasterizerDesc;
export wis::SampleDesc;
export wis::StencilDesc;
export wis::DepthStencilDesc;
export wis::BlendAttachmentDesc;
export wis::BlendStateDesc;
export wis::RenderAttachmentsDesc;
export wis::PushConstant;
export wis::SwapchainDesc;
export wis::TextureDesc;
export wis::AllocationInfo;
export wis::TextureRegion;
export wis::BufferTextureCopyRegion;
export wis::PushDescriptor;
export wis::SubresourceRange;
export wis::RenderTargetDesc;
export wis::Viewport;
export wis::Scissor;
export wis::BufferBarrier;
export wis::TextureBarrier;
export wis::DescriptorTableEntry;
export wis::DescriptorTable;
export wis::SamplerDesc;
export wis::ComponentMapping;
export wis::ShaderResourceDesc;
export wis::UnorderedAccessDesc;
export wis::FactoryExtQuery;
export wis::DeviceExtQuery;
export wis::TopLevelASBuildDesc;
export wis::AcceleratedGeometryInput;
export wis::ASAllocationInfo;
export wis::DescriptorBindingDesc;
export wis::ShaderExport;
export wis::HitGroupDesc;
export wis::ShaderBindingTableInfo;
export wis::RaytracingDispatchDesc;
export wis::TextureCopyRegion;
export wis::DeviceConstants;
export wis::RaytracingConstants;

//-------------------------------------------------------------------------

export wis::DebugCallback;

//-------------------------------------------------------------------------

export wis::ResultValue;

export wis::Internal;
export wis::QueryInternal;
export wis::QueryInternalExtension;

// internal logging
export wis::LogLayer;
export wis::LibLogger;

// misc
export wis::string_hash;

export namespace wis {
using wis::success;
using wis::operator|;
using wis::operator&;
using wis::operator+;
using wis::operator~;
using wis::aligned_size;
} // namespace wis

#pragma once
#include <wisdom/api/api_common.h>

namespace wis
{
	enum class PassLoadOperation : uint8_t
	{
		load,
		clear,
		discard
	};
	enum class PassStoreOperation : uint8_t
	{
		store,
		discard
	};
	enum class SampleCount
	{
		s1 = 1,
		s2 = 2,
		s4 = 4,
		s8 = 8,
		s16= 16,
		s32= 32,
		s64= 64,
	};


	struct ColorAttachment
	{
		DataFormat format = DataFormat::unknown;
		PassLoadOperation load = PassLoadOperation::load;
		PassStoreOperation store = PassStoreOperation::store;
	};

	struct DepthStencilAttachment
	{
		DataFormat format = DataFormat::unknown;
		PassLoadOperation depth_load = PassLoadOperation::load;
		PassStoreOperation depth_store = PassStoreOperation::store;
		PassLoadOperation stencil_load = PassLoadOperation::load;
		PassStoreOperation stencil_store = PassStoreOperation::store;
	};

	
}
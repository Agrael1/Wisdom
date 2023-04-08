#pragma once
#include <wisdom/api/api_data_format.h>

namespace wis
{
	enum class InputClassification
	{
		vertex,
		instance
	};


	struct InputLayoutDesc
	{
		const char* semantic_name;
		uint32_t semantic_index = 0u;
		DataFormat format;
		uint32_t input_slot = 0u;
		uint32_t aligned_byte_offset;
		InputClassification input_slot_class = InputClassification::vertex;
		uint32_t instance_data_step_rate = 0u;
	};
}
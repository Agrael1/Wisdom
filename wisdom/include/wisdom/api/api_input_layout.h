#pragma once
#include <wisdom/api/api_common.h>

namespace wis
{
	/// @brief Input type for input assembler
	enum class InputClassification
	{
		vertex, //< Per-vertex input
		instance //< Per-instance input
	};

	/// @brief Data format for input assembler
	struct InputLayoutDesc
	{
		uint32_t location; //< Location of the input in the shader
		const char* semantic_name; //< Semantic name of the input in the shader (e.g. "POSITION")
		uint32_t semantic_index = 0u; //< Semantic index of the input in the shader (e.g. 0 for "POSITION0")
		DataFormat format; //< Data format of the input in the shader (e.g. r32g32b32a32_float for float4)
		uint32_t input_slot = 0u; //< Input slot in the input assembler
		uint32_t aligned_byte_offset; //< Offset in bytes from the start of the input slot
		InputClassification input_slot_class = InputClassification::vertex; //< Input type
		uint32_t instance_data_step_rate = 0u; //< Step rate for per-instance input (ignored for per-vertex input)
	};
}
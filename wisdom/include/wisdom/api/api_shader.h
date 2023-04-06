#pragma once

namespace wis
{
	enum class ShaderType
	{
		unknown,
		vertex,
		pixel,
		geometry,
		hull,
		domain,
		amplification,
		mesh,
		compute
	};
}
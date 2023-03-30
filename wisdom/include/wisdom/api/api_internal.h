#pragma once

namespace wis
{
	template<class Impl>
	class Internal
	{
		static_assert(requires{Internal<Impl>::valid; }, "Internal class may be used only by explicit api types");
	};
}
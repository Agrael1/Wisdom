#pragma once
#include <type_traits>

namespace wis
{
	template<class Impl>
	class Internal
	{
		static_assert(requires{Internal<Impl>::valid; }, "Internal class may be used only by explicit api types");
	};

	template<class T, class U>
	struct cv_type { using type = U; };

	template<class T, class U>
	struct cv_type<const T, U>{ using type = const U; };

	template<class T, class U>
	struct cv_type<const volatile T, U>{ using type = const volatile U; };

	template<class T, class U>
	struct cv_type<volatile T, U>{ using type = volatile U; };

	template<class In, class Out>
	using cv_type_t = cv_type<In, Out>::type;
	

	template<class Impl>
	class QueryInternal : protected Internal<Impl>
	{
	public:
		using Internal<Impl>::Internal;
	public:
		template<class Self>
		[[nodiscard]] auto& GetInternal(this Self&& s)
		{
			return static_cast<cv_type<std::remove_reference_t<Self>, Internal<Impl>>::type&>(s);
		}
	};

}
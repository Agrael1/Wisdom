#pragma once
#include <wisdom/api/api_internal.h>


namespace wis
{
	class VKCommandQueue;

	template<>
	class Internal<VKCommandQueue>
	{
		static constexpr inline bool valid = true;
	public:

	protected:

	};


	class VKCommandQueue : public QueryInternal<VKCommandQueue>
	{
		using intern = QueryInternal<VKCommandQueue>;
	public:
		VKCommandQueue() = default;
		explicit VKCommandQueue(bool)noexcept{}
	public:
		void ExecuteCommandList()
		{

		}
		bool Signal()
		{

		}
	};
}
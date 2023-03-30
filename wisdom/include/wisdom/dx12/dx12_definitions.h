#pragma once
#include <wisdom/global/definitions.h>
#include <d3dx12/d3dx12.h>
#include <winrt/base.h>
#include <dxgi1_6.h>
#include <array>

namespace wis
{
	template<class C>
	[[nodiscard]] inline auto** array_view(winrt::com_ptr<C>& self)
	{
		return reinterpret_cast<C**>(&self);
	}


	template<class T, size_t size> requires std::derived_from<T, IUnknown>
	class com_array : public std::array<winrt::com_ptr<T>, size>
	{
	public:
		[[nodiscard]] T** get_view()const
		{
			return reinterpret_cast<T**>(this->data());
		}
	};

	template<class T> requires std::derived_from<T, IUnknown>
	class com_vector : public std::vector<winrt::com_ptr<T>>
	{
	public:
		[[nodiscard]] T** get_view()const
		{
			return reinterpret_cast<T**>(this->data());
		}
	};
}
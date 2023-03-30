#include <wisdom/wisdom.h>
#include <iostream>

int main()
{
	wis::Factory factory;
	for (auto&& a : factory.EnumerateAdapters(wis::AdapterPreference::Performance))
	{
		std::wcout << a.GetDesc().to_string()<<'\n';
		//if (device.Initialize(a))break;
	}

	return 0;
}
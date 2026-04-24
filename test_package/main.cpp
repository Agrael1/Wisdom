#include <wisdom/wisdom.hpp>

int main()
{ 
	wis::Result result{};
	wis::DebugDesc debug_desc{true};
	wis::Instance instance = wis::CreateInstance(&debug_desc, {}, result);
	return 0; 
}
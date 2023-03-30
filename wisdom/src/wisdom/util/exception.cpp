#include <wisdom/util/exception.h>
#include <wisdom/util/log_layer.h>

wis::exception::exception(std::source_location sl, bool write) noexcept
	:sl(std::move(sl))
{
	if (write)log();
}

void wis::exception::log()
{
	wis::lib_critical(what());
}

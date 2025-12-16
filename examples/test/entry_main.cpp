#include <wisdom/wisdom.hpp>
#include <iostream>

// Entry point for testing
int main()
{
    wis::Result   result;
    wis::Instance instance = wis::CreateInstance(false, {}, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to create instance: " << result.error << "\n";
        return int(result.status);
    }
    std::cout << "Instance created successfully.\n";

    wis::AdapterQuery adapter_query = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
    if (result.status != wis::Status::Ok) {
        std::cerr << "Failed to query adapters: " << result.error << "\n";
        return int(result.status);
    }

    return 0;
}

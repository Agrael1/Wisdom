#include <exception>
#include <iostream>

import wisdom;


int main(int argc, char** argv)
{
    try {
        wis::Device device;
        wis::Result result = wis::success;

        wis::Factory factory = wis::CreateFactory(result);
        if (result.status != wis::Status::Ok) {
            throw std::runtime_error("Failed to create factory");
        }

        for (size_t i = 0;; i++) {
            wis::Adapter adapter = factory.GetAdapter(result, i);
            if (result.status != wis::Status::Ok) {
                break;
            }

            wis::AdapterDesc desc;
            result = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            device = wis::CreateDevice(result, std::move(adapter));
            if (result.status == wis::Status::Ok) {
                std::cout << "Device created\n";
                return 0;
            }
        }
        if (result.status != wis::Status::Ok) {
            throw std::runtime_error("Failed to create device");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

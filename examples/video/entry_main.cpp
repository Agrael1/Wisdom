#include "app.hpp"

WISDOM_EXPORT_AGILITY_SYMBOLS();

int main(int argc, char* argv[])
{
    const char* path = argc > 1 ? argv[1] : "assets/hevc_sample.heic";
    auto result = CreateApp(path);
    if (!result) {
        return -1;
    }

    return result->Start();
}

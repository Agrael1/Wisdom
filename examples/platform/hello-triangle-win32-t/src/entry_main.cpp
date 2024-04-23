#include <example/app.h>

// constexpr auto x = int(0.0f * 32.0f - 0.1f);
// constexpr auto x = int(1.0f * 32.0f - 0.1f);

int main()
{
    Test::App app(1920, 1080);
    app.Start();
}
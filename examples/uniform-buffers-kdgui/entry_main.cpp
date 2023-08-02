#include "app.h"

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
    {
        std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

int main()
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    Test::App app(1920, 1080);
    return app.Start();
}

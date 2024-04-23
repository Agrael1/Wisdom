// #include "app.h"
// #include <iostream>
// #include <filesystem>
// #include <fstream>
// #include <glm/vec4.hpp>
// #include <glm/vec3.hpp>
//
// struct LogProvider : public wis::LogLayer {
//     virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
//     {
//         std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
//     };
// };
//
//
// Test::App::App(uint32_t width, uint32_t height)
//     : wnd(app.createWindow(width, height))
//{
//     wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());
//     factory.Initialize(true);
// }
// Test::App::~App()
//{
//     WaitForGPU();
// }
// int Test::App::Start()
//{
//     while (true) {
//         app.ProcessEvents();
//         if (!wnd.visible())
//             return 0;
//         // Process Events
//         if (wnd.resized())
//             OnResize(wnd.width(), wnd.height());
//         Frame();
//     }
// }
// void Test::App::Frame()
//{
//     WaitForGPU();
// }
//
// void Test::App::WaitForGPU()
//{
//
// }
//
// void Test::App::OnResize(uint32_t width, uint32_t height)
//{
//
// }

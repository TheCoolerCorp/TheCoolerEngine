#include <iostream>
#include <thread>

#include "assimp/Logger.hpp"
#include "Core/Application.h"
#include "Core/Logger/Logger.h"
//#include "Core/Assertion/Assertion.h"

#define JE_BANDE_COMME_UN_DRAGON()

int main()
{
    JE_BANDE_COMME_UN_DRAGON();

    //LOG_INFO("Starting...");
    //Engine::Core::Debugging::Logger::Get().Log("bijour", Engine::Core::Debugging::LogLevel::INFO, Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::BOLD_YELLOW), __FILE__, __LINE__);

//#ifdef TCDEBUG
//    LOG_INFO("Debug mode enabled");
//#else
//    LOG_INFO("Release mode enabled");
//#endif
//    Engine::Core::Application app = Engine::Core::Application();
//    app.Create(1440, 1080);
//    app.Run();
//    app.Destroy();
    std::this_thread::sleep_for(std::chrono::duration<float>(10.f));
    return 0;
}

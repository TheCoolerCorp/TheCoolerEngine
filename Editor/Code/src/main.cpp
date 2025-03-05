#include <iostream>
#include "Math/vec3.h"
#include "Core/Application.h"
#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"

using namespace Engine::Math;
using namespace Engine::Core::Debugging;

int main()
{
    const vec3 t_vec = vec3(0.f, 1.f, 2.f);
    const vec3 t_vec2 = vec3(0.f, 3.f, 5.f);
    vec3 t_ve3 = vec3::CrossProduct(t_vec, t_vec2);
    t_ve3.Print();

    LOG(LogLevel::DEBUGLOG, "Message", ColorMap.at(LogColor::YELLOW));
    LOG_DEBUG("Debug message");
    LOG_INFO("Info message");
    LOG_WARNING("Warning message");
    LOG_ERROR("Error message");
    LOG_CRITICAL("Critical message");
    LOG_DEBUG(t_ve3.x);
    LOG_ERROR(true);
    LOG_CRITICAL(6);
    LOG_WARNING(56.264556);

    bool t_testBool = true;

    ASSERT(t_testBool, "Test if bool is true");

#ifdef MY_DEBUG
    std::cout << "Debug mode enabled\n";
#else
    std::cout << "Release mode enabled\n";
#endif
    Engine::Core::Application app = Engine::Core::Application();
    app.Create(1440, 1080);
    app.Run();
    app.Destroy();

    return 0;
}

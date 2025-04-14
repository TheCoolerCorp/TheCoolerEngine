#include "../Inlude/Application.h"
#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"

int main()
{
    LOG_INFO("Starting...");

#ifdef TCDEBUG
    LOG_INFO("Debug mode enabled");
#else
    LOG_INFO("Release mode enabled");
#endif
    Editor::Core::Application app = Editor::Core::Application();
    app.Create(1440, 1080);
    app.Run();
    app.Destroy();
    return 0;
}

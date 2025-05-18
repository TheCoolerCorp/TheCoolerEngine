#include "Application.h"
#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"
#include "Debugging/TCLogger.h"
#include "Core/Multithread/ThreadPool.h"
#include "Gameplay/ServiceLocator.h"

int main()
{
    LOG_INFO("Starting...");

    Engine::Core::Multithread::ThreadPool t_threadPool = Engine::Core::Multithread::ThreadPool(std::max(1u, std::thread::hardware_concurrency() - 2u));
    Engine::GamePlay::ServiceLocator::ProvideThreadPool(&t_threadPool);

    Editor::Core::Application app = Editor::Core::Application();
    app.Create(1440, 1080);
    app.Run();
    app.Destroy();

    t_threadPool.WaitUntilFinished();

    return 0;
}

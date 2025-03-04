#include <iostream>
#include "Math/vec3.h"
#include "Application.h"

using namespace TheCoolerMath;

int main()
{
    Core::Application app = Core::Application();
    app.Create(1440, 1080);
    app.Run();
    app.Destroy();

    return 0;
}

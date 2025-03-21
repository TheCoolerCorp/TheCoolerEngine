#include <iostream>
#include "Math/vec3.h"
#include "Application.h"
#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "imgui.h"
int main()
{
    LOG_INFO("Starting...");

#ifdef TCDEBUG
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

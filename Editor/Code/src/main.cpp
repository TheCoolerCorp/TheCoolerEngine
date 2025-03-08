#include <iostream>
#include "Math/vec3.h"
#include "Core/Application.h"
#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"

int main()
{
    //const Engine::Math::vec3 t_vec = Engine::Math::vec3(0.f, 1.f, 2.f);
    //const Engine::Math::vec3 t_vec2 = Engine::Math::vec3(0.f, 3.f, 5.f);
    //Engine::Math::vec3 t_ve3 = Engine::Math::vec3::CrossProduct(t_vec, t_vec2);
    //t_ve3.Print();

    //Engine::Core::Debugging::LOG(Engine::Core::Debugging::LogLevel::DEBUGLOG, "Message", Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::YELLOW));
    /*LOG_DEBUG("Debug message");
    LOG_INFO("Info message");
    LOG_WARNING("Warning message");
    LOG_ERROR("Error message");
    LOG_CRITICAL("Critical message");
    LOG_DEBUG(t_ve3.x);
    LOG_ERROR(true);
    LOG_CRITICAL(6);
    LOG_WARNING(56.264556);*/

    //bool t_testBool = true;

    //ASSERT(t_testBool, "Test if bool is true");

#ifdef TCDEBUG
    std::cout << "Debug mode enabled\n";
#else
    std::cout << "Release mode enabled\n";
#endif
    Engine::Core::Application app = Engine::Core::Application();
    app.Create(1440, 1080);
    Engine::Core::RHI::IInstance* instance = new Engine::Core::GraphicsAPI::VulkanInstance;
    Engine::Core::RHI::IValidationLayers* layers = new Engine::Core::GraphicsAPI::VulkanValidationLayers;
    Engine::Core::RHI::IPhysicalDevice* physicalDevice = new Engine::Core::GraphicsAPI::VulkanPhysicalDevice;
    Engine::Core::RHI::ILogicalDevice* logicalDevice = new Engine::Core::GraphicsAPI::VulkanLogicalDevice;
    Engine::Core::RHI::ISurface* surface = new Engine::Core::GraphicsAPI::VulkanSurface;
    Engine::Core::RHI::ISwapChain* swapChain = new Engine::Core::GraphicsAPI::VulkanSwapchain;
    instance->Create();
    layers->Create(instance);
    surface->Create(app.GetWindow(), instance);
    physicalDevice->Create(instance, surface);
    logicalDevice->Create(physicalDevice, surface);
    swapChain->Create(surface, app.GetWindow(), physicalDevice, logicalDevice);
    app.Run();
    swapChain->Destroy(logicalDevice);
    logicalDevice->Destroy();
    layers->Destroy(instance);
    surface->Destroy(instance);
    instance->Destroy();
    app.Destroy();
    delete swapChain;
    delete surface;
    delete logicalDevice;
    delete physicalDevice;
    delete layers;
    delete instance;

    return 0;
}

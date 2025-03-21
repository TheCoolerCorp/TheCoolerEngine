#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class ApiInterface;
			class IPhysicalDevice;
			class ILogicalDevice;
			class ICommandPool;
		}
	}

	namespace GamePlay
	{
		class Component
		{
		public:
			ENGINE_API virtual void Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool) = 0;
			ENGINE_API virtual void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice) = 0;

		protected:
			virtual void ComputeName(std::string a_path);

			std::string m_name;
		};
	}
}

#endif 
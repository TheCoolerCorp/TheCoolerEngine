#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>

namespace Engine
{
	namespace GamePlay
	{
		class Component
		{
		public:
			ENGINE_API virtual void Create(std::string a_path) = 0;
			ENGINE_API virtual void Destroy() = 0;

		protected:
			virtual void ComputeName(std::string a_path);

			std::string m_name;
		};
	}
}

#endif 
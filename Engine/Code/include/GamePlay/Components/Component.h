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

			template<typename T>
			static const std::type_info& GetCompId()
			{
				return typeid(T);
			}

			ENGINE_API virtual ~Component() = default;

			ENGINE_API virtual void Create(int& a_outId) = 0;
			ENGINE_API virtual void Destroy() = 0;

			ENGINE_API void SetGameObject(int a_gameObjectId) { m_gameObjectId = a_gameObjectId; }
			[[nodiscard]] ENGINE_API int GetGameObjectID() const { return m_gameObjectId; }
			[[nodiscard]] ENGINE_API int GetID() const { return m_uid; }
			ENGINE_API void SetId(int a_uid) { m_uid = a_uid; }


		protected:

			int m_gameObjectId = -1;
			int m_uid = -1;

		
		};

	}
}

#endif 
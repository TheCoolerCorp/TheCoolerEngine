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
		enum class ComponentType : uint8_t
		{
			TRANSFORM = 0,
			MESH = 1,
			MATERIAL = 2,
			RIGIDBODY = 3,
			COLLIDERMESH = 4,
			LIGHT = 5,
			CAMERA = 6,
			GAMECOMPONENT = 7,
			PLAYERCONTROLLER = 8,
		};

		class Component
		{
		private:
			static int SetCompId()
			{
				static int t_id = 0;
				return t_id++;
			}
			
		public:

			template<typename T>
			static int GetCompId()
			{
				static int t_id = SetCompId();
				return t_id;
			}

			ENGINE_API virtual ~Component() = default;

			ENGINE_API virtual ComponentType Create(int& a_outId) = 0;
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
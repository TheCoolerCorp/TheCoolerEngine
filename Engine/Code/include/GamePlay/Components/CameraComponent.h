#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include "GamePlay/ComponentsBase/Camera.h"

namespace Engine
{
	namespace GamePlay
	{
		class CameraComponent : public Component
		{
		public:
			ENGINE_API static void Register();
			ENGINE_API CameraComponent() = default;
			ENGINE_API ~CameraComponent() override = default;

			ENGINE_API void Create(int& a_outID) override;
			ENGINE_API void SetCamera(Camera& a_light);
			ENGINE_API void Destroy() override;

			ENGINE_API Camera& GetCamera() { return m_camera; }

			ENGINE_API static CameraComponent* GetComponent(int a_id);

			ENGINE_API static void RemoveComponent(int a_id);

			ENGINE_API void SetCameraData(const CameraSerializeData& a_cameraData);
			ENGINE_API CameraSerializeData GetCameraData();

		private:
			Camera m_camera;
		};
	}
}


#endif

#ifndef CAMERAGO_H
#define CAMERAGO_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"

#include "GameObject.h"
#include "GamePlay/Components/CameraComponent.h"
namespace Engine
{
	namespace GamePlay
	{
		class CameraGO : public GameObject
		{
		public:
			ENGINE_API CameraGO(std::string a_name = "Camera");
			ENGINE_API CameraGO(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name = "Camera");
			ENGINE_API void Set(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed, float a_sensitivity, bool a_freeCam = false);

			ENGINE_API ~CameraGO() override;

			ENGINE_API CameraComponent* GetCameraComponent() { return GetComponent<CameraComponent>(); }
		};


	}
}

#endif 
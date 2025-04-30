#include <utility>

#include "GamePlay/Others/LightGO.h"

#include <ranges>

#include "GamePlay/Components/Meshcomponent.h"
#include  "GamePlay/Components/MaterialComponent.h"
#include "Core/Utils.h"
namespace Engine
{
	namespace GamePlay
	{
		LightGO::LightGO(std::string a_name)
		{
			m_name = a_name;

			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();

			AddComponent<LightComponent>();
		}

		LightGO::LightGO(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name)
		{
			m_name = a_name;
			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();

			AddComponent<LightComponent>();
		}

		LightGO::~LightGO()
		{
			RemoveParent();
			ClearChildren();
		}
	}
}
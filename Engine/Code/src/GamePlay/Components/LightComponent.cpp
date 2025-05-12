#include "GamePlay/Components/LightComponent.h"

#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		void LightComponent::Register()
		{
			constexpr std::hash<std::string_view> t_hash{};

			meta::reflect<LightData>(t_hash("LightData"))
				.data<&LightData::m_position>(t_hash("position"))
				.data<&LightData::m_color>(t_hash("color"))
				.data<&LightData::m_intensity>(t_hash("intensity"));

			meta::reflect<LightComponent>(t_hash("LightComponent"))
				.data<&LightComponent::SetLightFromData, &LightComponent::GetData>(t_hash("Light"));
		}

		ComponentType LightComponent::Create(int& a_outID)
		{
			a_outID = ServiceLocator::GetRendererSystem()->AddComponent(this);
			SetId(a_outID);
			return ComponentType::LIGHT;
		}

		void LightComponent::SetLight(LightType a_type)
		{
			if (a_type == LightType::POINT)
			{
				m_light = new PointLight;
			}
			else
			{
				m_light = new DirectionalLight;
			}
		}

		void LightComponent::SetLightFromData(const LightData& a_lightData, LightType a_type)
		{
			if (a_type == LightType::POINT)
			{
				m_light = new PointLight(a_lightData.m_position, a_lightData.m_color, a_lightData.m_intensity);
			}
			else
			{
				m_light = new DirectionalLight(a_lightData.m_position, a_lightData.m_color, a_lightData.m_intensity, { 0.f, 0.f, 1.f });
			}
		}

		void LightComponent::Destroy()
		{
			delete m_light;
		}

		LightData* LightComponent::GetData()
		{
			return m_light->GetData();
		}

		LightComponent* LightComponent::GetComponent(int a_id)
		{
			return ServiceLocator::GetRendererSystem()->GetLightComponent(a_id);
		}

		void LightComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetRendererSystem()->RemoveLightComponent(a_id);
		}
	}
}
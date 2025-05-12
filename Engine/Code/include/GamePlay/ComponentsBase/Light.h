#ifndef LIGHT_H
#define LIGHT_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

#include "Math/vec3.h"

namespace Engine
{
	namespace GamePlay
	{
		struct LightData
		{
			alignas(16)Math::vec3 m_position = { 0.f, 0.f, 0.f };
			alignas(16)Math::vec3 m_color = { 1.f, 1.f, 1.f };
			float m_intensity = 15.f;
		};

		struct PointLightData : LightData
		{
			// Empty just to show which type of light data to pass
		};

		struct DirectionalLightData : LightData
		{
			// No need to align
			Math::vec3 m_direction = { 0.f, 0.f, 1.f };
		};

		enum class LightType
		{
			POINT = 0,
			DIRECTIONAL = 1
		};

		/*
		 *  Base Class
		 */
		class Light
		{
		public:
			ENGINE_API Light() = default;

			ENGINE_API virtual ~Light() = default;

			ENGINE_API virtual Math::vec3 GetPosition() = 0;
			ENGINE_API virtual void SetPosition(Math::vec3 a_position) = 0;

			ENGINE_API virtual Math::vec3 GetColor() = 0;
			ENGINE_API virtual void SetColor(Math::vec3 a_color) = 0;

			ENGINE_API virtual float GetIntensisty() = 0;
			ENGINE_API virtual void SetIntensity(float a_intensity) = 0;

			ENGINE_API virtual Math::vec3 GetDirection() = 0;
			ENGINE_API virtual void SetIntensity(Math::vec3 a_direction) = 0;

			ENGINE_API virtual LightData* GetData() = 0;

			ENGINE_API virtual LightType GetType() = 0;
		};


		class PointLight : public Light
		{
		public:
			ENGINE_API PointLight() = default;
			ENGINE_API PointLight(Math::vec3 a_position, Math::vec3 a_color, float a_intensity) { m_data.m_position = a_position; m_data.m_color = a_color; m_data.m_intensity = a_intensity; }

			ENGINE_API ~PointLight() override = default;

			ENGINE_API Math::vec3 GetPosition() override { return m_data.m_position; }
			ENGINE_API void SetPosition(Math::vec3 a_position) override { m_data.m_position = a_position; }

			ENGINE_API Math::vec3 GetColor() override { return m_data.m_color; }
			ENGINE_API void SetColor(Math::vec3 a_color) override { m_data.m_color = a_color; }

			ENGINE_API float GetIntensisty() override { return m_data.m_intensity; }
			ENGINE_API void SetIntensity(float a_intensity) override { m_data.m_intensity = a_intensity; }

			ENGINE_API Math::vec3 GetDirection() override { LOG_WARNING("Cannot Get value from base class"); return { 0.f, 0.f, 0.f }; }
			ENGINE_API void SetIntensity(Math::vec3 a_direction) override { LOG_WARNING("Cannot set value for base class"); }

			ENGINE_API PointLightData* GetData() override { return &m_data; }

			ENGINE_API virtual LightType GetType() { return m_type; }

		private:
			PointLightData m_data;
			LightType m_type = LightType::POINT;
		};

		class DirectionalLight : public Light
		{
		public:
			ENGINE_API DirectionalLight() = default;
			ENGINE_API DirectionalLight(Math::vec3 a_position, Math::vec3 a_color, float a_intensity, Math::vec3 a_direction) { m_data.m_position = a_position; m_data.m_color = a_color; m_data.m_intensity = a_intensity;  m_data.m_direction = a_direction; }

			ENGINE_API ~DirectionalLight() override = default;

			ENGINE_API Math::vec3 GetPosition() override { return m_data.m_position; }
			ENGINE_API void SetPosition(Math::vec3 a_position) override { m_data.m_position = a_position; }

			ENGINE_API Math::vec3 GetColor() override { return m_data.m_color; }
			ENGINE_API void SetColor(Math::vec3 a_color) override { m_data.m_color = a_color; }

			ENGINE_API float GetIntensisty() override { return m_data.m_intensity; }
			ENGINE_API void SetIntensity(float a_intensity) override { m_data.m_intensity = a_intensity; }

			ENGINE_API Math::vec3 GetDirection() override { return m_data.m_direction; }
			ENGINE_API void SetIntensity(Math::vec3 a_direction) override { m_data.m_direction = a_direction; }

			ENGINE_API DirectionalLightData* GetData() override { return &m_data; }

			ENGINE_API virtual LightType GetType() { return m_type; }

		private:
			DirectionalLightData m_data;
			LightType m_type = LightType::DIRECTIONAL;
		};
	}
}

#endif
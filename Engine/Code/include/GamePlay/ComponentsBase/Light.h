#ifndef LIGHT_H
#define LIGHT_H

#include "EngineExport.h"

#include "Math/vec3.h"

namespace Engine
{
	namespace GamePlay
	{
		struct LightData
		{
		public:
			alignas(16)Math::vec3 m_position = { 0.f, 0.f, 0.f };
			alignas(16)Math::vec3 m_color = { 1.f, 1.f, 1.f };
			float m_intensity = 100.f;
		};

		/*
		 *  Just point light for now, no directionnal
		 */
		class Light
		{
		public:
			ENGINE_API Light() = default;
			ENGINE_API Light(Math::vec3 a_position, Math::vec3 a_color, float a_intensity) { m_data.m_position = a_position; m_data.m_color = a_color; m_data.m_intensity = a_intensity; }

			ENGINE_API ~Light() = default;

			ENGINE_API Math::vec3 GetPosition() { return m_data.m_position; }
			ENGINE_API void SetPosition(Math::vec3 a_position) { m_data.m_position = a_position; }

			ENGINE_API Math::vec3 GetColor() { return m_data.m_color; }
			ENGINE_API void SetColor(Math::vec3 a_color) { m_data.m_color = a_color; }

			ENGINE_API float GetIntensisty() { return m_data.m_intensity; }
			ENGINE_API void SetIntensity(float a_intensity) { m_data.m_intensity = a_intensity; }

			ENGINE_API LightData& GetData() { return m_data; }

		private:
			LightData m_data;
		};
	}
}

#endif
#ifndef LIGHT_H
#define LIGHT_H

#include "EngineExport.h"

#include "Math/vec3.h"
#include "Math/quat.h"

namespace Engine
{
	namespace GamePlay
	{
		#define BASEDIR Math::vec3(0.f, 0.f, 1.f)

		struct LightData
		{
		public:
			alignas(16) Math::vec3 m_position = { 0.f, 0.f, 0.f };
			alignas(16) Math::vec3 m_color = { 1.f, 1.f, 1.f };
			alignas(16) Math::vec3 m_dir = { 0.f, 0.f, 1.f };
			float m_intensity = 15.f;
			uint32_t m_bDir = false;
		};

		enum class LightType
		{
			POINT = 0,
			DIR = 1
		};

		class Light
		{
		public:
			ENGINE_API Light() = default;
			ENGINE_API Light(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_color, float a_intensity, LightType a_type)
			{   m_data.m_position = a_position;
				m_data.m_color = a_color;
				m_data.m_dir = Math::quat(a_rotation) * BASEDIR;
				m_data.m_intensity = a_intensity;
				m_type = a_type;
				if (a_type == LightType::DIR) m_data.m_bDir = true;
			}

			ENGINE_API ~Light() = default;

			ENGINE_API Math::vec3 GetPosition() { return m_data.m_position; }
			ENGINE_API void SetPosition(Math::vec3 a_position) { m_data.m_position = a_position; }

			ENGINE_API Math::vec3 GetColor() { return m_data.m_color; }
			ENGINE_API void SetColor(Math::vec3 a_color) { m_data.m_color = a_color; }

			ENGINE_API float GetIntensisty() { return m_data.m_intensity; }
			ENGINE_API void SetIntensity(float a_intensity) { m_data.m_intensity = a_intensity; }

			ENGINE_API Math::vec3 GetDir() { return m_data.m_dir; }
			ENGINE_API void SetDir(Math::vec3 a_rotation) { m_data.m_dir = Math::quat(a_rotation) * BASEDIR; }

			ENGINE_API bool GetIsDir() { return m_data.m_bDir; }
			ENGINE_API void SetIsDir(bool a_bool) { m_data.m_bDir = a_bool; }

			ENGINE_API LightData GetData() const { return m_data; }

		private:
			LightData m_data;
			LightType m_type = LightType::POINT;
		};
	}
}

#endif
#include "Math/Transform.h"

namespace Engine
{
	namespace Math
	{
		Transform::Transform(vec3 a_pos, quat a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale)
		{
			m_model = mat4::Perspective(45.f, 1400.f / 1080.f, 0.1f, 100.f) * mat4::View(vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, -3.f), vec3(0.f, 0.f, 0.f)) * mat4::TRS(m_pos, m_rot, m_scale);
		}

		void Transform::UpdateMatrix()
		{
			if (!m_needUpdate)
			{
				return;
			}
			m_model = mat4::TRS(m_pos, m_rot, m_scale);
			m_needUpdate = false;
		}
	}
}
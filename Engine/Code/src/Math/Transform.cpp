#include "Math/Transform.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace Math
	{
		Transform::Transform() : m_pos(vec3()), m_rot(quat()), m_scale(vec3(1.f))
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);
		}

		Transform::Transform(vec3 a_pos, quat a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale)
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);
		}

		Transform::Transform(vec3 a_pos, vec3 a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale)
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);
		}
	}
}
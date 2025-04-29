#include "Math/Transform.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace Math
	{
		Transform::Transform() : m_pos(vec3()), m_rot(quat()), m_scale(vec3(1.f))
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();
		}

		Transform::Transform(vec3 a_pos, quat a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale)
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();
		}

		Transform::Transform(vec3 a_pos, vec3 a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale)
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();
		}

		void Transform::SetMatrix(const mat4& a_matrix)
		{
			m_transform = a_matrix;

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();
		}
	}
}
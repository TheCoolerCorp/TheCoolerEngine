#include "Math/Transform.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace Math
	{
		Transform::Transform(vec3 a_pos, quat a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale)
		{
			m_model = mat4::TRS(m_pos, m_rot, m_scale);
			m_model.Transpose();
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
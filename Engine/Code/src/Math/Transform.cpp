#include "Math/Transform.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace Math
	{
		Transform::Transform() : m_pos(vec3()), m_rot(quat()), m_scale(vec3(1.f)), m_globalPos(vec3()), m_globalRot(quat()), m_globalScale(vec3())
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();

			SetGlobalPositionFromMatrix();
			SetGlobalRotationFromMatrix();
			SetGlobalScaleFromMatrix();
		}

		Transform::Transform(vec3 a_pos, quat a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale), m_globalPos(vec3(a_pos)), m_globalRot(quat(a_rot)), m_globalScale(vec3(a_scale))
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();

			SetGlobalPositionFromMatrix();
			SetGlobalRotationFromMatrix();
			SetGlobalScaleFromMatrix();
		}

		Transform::Transform(vec3 a_pos, vec3 a_rot, vec3 a_scale) : m_pos(a_pos), m_rot(a_rot), m_scale(a_scale), m_globalPos(vec3(a_pos)), m_globalRot(quat(a_rot)), m_globalScale(vec3(a_scale))
		{
			m_transform = mat4::TRS(m_pos, m_rot, m_scale);

			m_normalMatrix = m_transform;
			m_normalMatrix.Inverse();
			m_normalMatrix.Transpose();

			SetGlobalPositionFromMatrix();
			SetGlobalRotationFromMatrix();
			SetGlobalScaleFromMatrix();
		}

		void Transform::TranslateLerp(const vec3 a_translate, float a_alpha)
		{
			m_pos = vec3::Lerp(m_pos, a_translate, a_alpha);
			m_needUpdate = true;
		}

		void Transform::RotateSlerp(const quat a_rotation, const float a_alpha)
		{
			m_rot = quat::Slerp(m_rot, a_rotation, a_alpha);
			m_needUpdate = true;
		}

		void Transform::SetGlobalPositionFromMatrix()
		{
			m_globalPos = vec3::GetPosition(m_transform);
		}

		void Transform::SetGlobalRotationFromMatrix()
		{
			m_globalRot = quat::GetRotation(m_transform);
		}

		void Transform::SetGlobalScaleFromMatrix()
		{
			m_globalScale = vec3::GetScale(m_transform);
		}

		vec3 Transform::GetForward()
		{
			Math::vec3 t_forward = vec3(0, 0, -1);
			Math::vec3 t_result = m_globalRot * t_forward;
			t_result.x = -t_result.x;
			return t_result;
		}

		vec3 Transform::GetRight()
		{
			Math::vec3 t_right = vec3(1, 0, 0);
			Math::vec3 t_result = m_globalRot * t_right;
			t_result.x = -t_result.x;
			return t_result;
		}

		vec3 Transform::GetUp()
		{
			Math::vec3 t_up = vec3(0, 1, 0);
			return m_globalRot * t_up;
		}

		UniformMatrixs& Transform::GetUniformsMatrixs()
		{
			m_uniforms = { m_transform, m_normalMatrix };
			m_uniforms.m_transform.Transpose();
			m_uniforms.m_normalMatrix.Transpose();
			return m_uniforms;
		}
	}
}
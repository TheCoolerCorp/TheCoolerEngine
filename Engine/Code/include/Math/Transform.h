#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "EngineExport.h"
#include "../../../../libs/imgui/imgui.h"

#include "Math/vec3.h"
#include "Math/quat.h"
#include "Math/mat4.h"

namespace Engine
{
	namespace Math
	{
		struct UniformMatrixs
		{
			mat4 m_transform;
			mat4 m_normalMatrix;
		};

		class Transform
		{
		public:
			ENGINE_API Transform();
			ENGINE_API Transform(vec3 a_pos, quat a_rot, vec3 a_scale);
			ENGINE_API Transform(vec3 a_pos, vec3 a_rot, vec3 a_scale);
			ENGINE_API ~Transform() = default;

			ENGINE_API void Translate(const vec3 a_translate) { m_pos += a_translate; m_needUpdate = true; }

			ENGINE_API void Rotate(const quat a_rotation) { m_rot *= a_rotation; m_needUpdate = true; }
			ENGINE_API void Rotate(const vec3 a_rotation) { m_rot *= quat(a_rotation); m_needUpdate = true; }

			ENGINE_API void Scale(const vec3 a_scale) { m_scale *= a_scale; m_needUpdate = true; }

			ENGINE_API vec3 GetPosition() const { return m_pos; }
			ENGINE_API vec3 GetGlobalPosition() const { return m_globalPos; }

			ENGINE_API quat GetRotation() const { return m_rot; }
			ENGINE_API quat GetGlobalRotation() const { return m_globalRot; }

			ENGINE_API vec3 GetScale() const { return m_scale; }
			ENGINE_API vec3 GetGlobalScale() const { return m_globalScale; }

			ENGINE_API mat4 GetTransformMatrix() const { return m_transform; }
			ENGINE_API mat4 GetNormalMatrix() const { return m_normalMatrix; }

			ENGINE_API UniformMatrixs& GetUniformsMatrixs();

			ENGINE_API bool GetNeedToUpdate() const { return m_needUpdate; }

			ENGINE_API void SetPosition(const vec3 a_pos) { m_pos = a_pos; m_needUpdate = true; }
			ENGINE_API void SetGlobalPositionFromMatrix();

			ENGINE_API void SetRotation(const quat a_rot) { m_rot = a_rot; m_needUpdate = true; }
			ENGINE_API void SetRotation(const vec3 a_rot) { m_rot = quat(a_rot); m_needUpdate = true; }
			ENGINE_API void SetGlobalRotationFromMatrix();

			ENGINE_API void SetScale(const vec3 a_scale) { m_scale = a_scale; m_needUpdate = true; }
			ENGINE_API void SetGlobalScaleFromMatrix();

			ENGINE_API void SetMatrix(const mat4& a_matrix) { m_transform = a_matrix; m_normalMatrix = m_transform; m_normalMatrix.Inverse();m_normalMatrix.Transpose();}

			ENGINE_API void SetNeedToUpdate(const bool a_needToUpdate) { m_needUpdate = a_needToUpdate; }

		private:
			bool m_needUpdate = true;
			vec3 m_pos;
			quat m_rot;
			vec3 m_scale;
			vec3 m_globalPos;
			quat m_globalRot;
			vec3 m_globalScale;
			mat4 m_transform;
			mat4 m_normalMatrix;

			UniformMatrixs m_uniforms;
		};
	}
}

#endif

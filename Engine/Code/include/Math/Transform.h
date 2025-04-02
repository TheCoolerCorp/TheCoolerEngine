#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "EngineExport.h"

#include "Math/vec3.h"
#include "Math/quat.h"
#include "Math/mat4.h"

namespace Engine
{
	namespace Math
	{
		class Transform
		{
		public:
			ENGINE_API Transform() = default;
			ENGINE_API Transform(vec3 a_pos, vec3 a_rot, vec3 a_scale);
			ENGINE_API ~Transform() = default;


			/*void Translate(vec3 a_translate) { m_pos += a_translate; m_needUpdate = true; }

			void Rotate(quat a_rotation) { m_rot *= a_rotation; m_needUpdate = true; }
			void Rotate(vec3 a_rotation) { m_rot *= quat(a_rotation); m_needUpdate = true; }

			void Scale(vec3 a_scale) { m_scale *= a_scale; m_needUpdate = true; }

			vec3 GetPosition() { return m_pos; }

			quat GetRotation() { return m_rot; }

			vec3 GetScale() { return m_scale; }


			void SetPosition(vec3 a_pos) { m_pos = a_pos; m_needUpdate = true; }

			void SetRotation(quat a_rot) { m_rot = a_rot; m_needUpdate = true; }
			void SetRotation(vec3 a_rot) { m_rot = quat(a_rot); m_needUpdate = true; }

			void SetScale(vec3 a_scale) { m_scale = a_scale; m_needUpdate = true; }*/

		private:
			vec3 m_pos;
			quat m_rot;
			vec3 m_scale;
		};
	}
}

#endif

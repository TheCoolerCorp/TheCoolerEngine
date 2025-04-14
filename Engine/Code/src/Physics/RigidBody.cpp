#include "Physics/RigidBody.h"

namespace Engine
{
	namespace Physics
	{
		JPH::BodyID RigidBody::GetBodyID() const
		{
			return m_body->GetID();
		}
	}
}

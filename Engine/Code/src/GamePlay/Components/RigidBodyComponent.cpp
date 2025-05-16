#include "GamePlay/Components/RigidBodyComponent.h"

#include <utility>
#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "Gameplay/ServiceLocator.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace GamePlay
	{
		void RigidBodyComponent::Register()
		{
			constexpr std::hash<std::string_view> t_hash{};

			meta::reflect<RigidBodyData>(t_hash("RigidBodyData"))
				.data<&RigidBodyData::mBodyType>(t_hash("body type"))
				.data<&RigidBodyData::mLayer>(t_hash("layer"))
				.data<&RigidBodyData::mColliderType>(t_hash("collider type"))
				.data<&RigidBodyData::mPos>(t_hash("position"))
				.data<&RigidBodyData::mScale>(t_hash("scale"))
				.data<&RigidBodyData::mRadius>(t_hash("radius"))
				.data<&RigidBodyData::mHalfHeight>(t_hash("half height"))
				.data<&RigidBodyData::mRot>(t_hash("rotation"))
				.data<&RigidBodyData::mMass>(t_hash("mass"))
				.data<&RigidBodyData::mEnable>(t_hash("enable"))
				.data<&RigidBodyData::mLockRotX>(t_hash("lock rotation X"))
				.data<&RigidBodyData::mLockRotY>(t_hash("lock rotation Y"))
				.data<&RigidBodyData::mLockRotZ>(t_hash("lock rotation Z"))
				.data<&RigidBodyData::mMeshId>(t_hash("mesh ID"))
				.data<&RigidBodyData::mFriction>(t_hash("friction")) // NEW
				.data<&RigidBodyData::mRestitution>(t_hash("restitution")) // NEW
				.data<&RigidBodyData::mMeshComponent>(t_hash("mesh component"));
			
			meta::reflect<RigidBodyComponent>(t_hash("RigidBodyComponent"))
				.data<&RigidBodyComponent::SetFromData, &RigidBodyComponent::GetRigidBodyData>(t_hash("RigidBody")); 
		}

		void RigidBodyComponent::Create(int& a_outId)
		{
			m_rigidBody = new Physics::RigidBody;
			a_outId = ServiceLocator::GetPhysicsSystem()->AddComponent(this);
			SetId(a_outId);
		}

		void RigidBodyComponent::CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale,
		                                            Math::quat a_rotation, const Math::Transform& a_transform, float a_mass, const bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetGlobalPosition();
			const Math::quat t_rot = a_transform.GetGlobalRotation() * m_localRot;
			m_bodyPos = t_pos;
			m_bodyRot = t_rot;

			m_rigidBody->CreateBoxBody(a_type, a_layer, t_pos, a_scale, t_rot, a_mass, a_enable);

			Core::Renderer* t_renderer = ServiceLocator::GetRenderer();

			m_meshComponent = new MeshComponent;
			m_meshComponent->Create(m_meshId);
			m_meshComponent->SetMesh("Assets/Meshes/WireframeCube.obj", t_renderer);
			Ref<Material> t_material = m_meshComponent->GetMaterial();
			t_material->Create(static_cast<MaterialType>(UNLIT));
			t_material->SetAlbedo("Assets/Textures/ColliderTexture.png", t_renderer);
		}

		void RigidBodyComponent::CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, float a_mass, bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetGlobalPosition();
			const Math::quat t_rot = a_transform.GetGlobalRotation() * m_localRot;
			m_bodyPos = t_pos;
			m_bodyRot = t_rot;

			m_rigidBody->CreateSphereBody(a_type, a_layer, t_pos, a_radius, t_rot, a_mass, a_enable);

			Core::Renderer* t_renderer = ServiceLocator::GetRenderer();

			m_meshComponent = new MeshComponent;
			m_meshComponent->Create(m_meshId);
			m_meshComponent->SetMesh("Assets/Meshes/WireframeSphere.obj", t_renderer);
			Ref<Material> t_material = m_meshComponent->GetMaterial();
			t_material->Create(static_cast<MaterialType>(UNLIT));
			t_material->SetAlbedo("Assets/Textures/ColliderTexture.png", t_renderer);
		}

		void RigidBodyComponent::CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, float a_mass, bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetGlobalPosition();
			const Math::quat t_rot = a_transform.GetGlobalRotation() * m_localRot;
			m_bodyPos = t_pos;
			m_bodyRot = t_rot;

			m_rigidBody->CreateCapsuleBody(a_type, a_layer, t_pos, a_halfHeight, a_radius, t_rot, a_mass, a_enable);

			Core::Renderer* t_renderer = ServiceLocator::GetRenderer();

			m_meshComponent = new MeshComponent;
			m_meshComponent->Create(m_meshId);
			m_meshComponent->SetMesh("Assets/Meshes/WireframeCapsule.obj", t_renderer);
			Ref<Material> t_material = m_meshComponent->GetMaterial();
			t_material->Create(static_cast<MaterialType>(UNLIT));
			t_material->SetAlbedo("Assets/Textures/ColliderTexture.png", t_renderer);
		}

		void RigidBodyComponent::UpdateFromTransform(const Math::Transform* a_transform, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
			SetActive(a_enable);
			const JPH::BodyID t_bodyID = m_rigidBody->GetBodyID();
			const Math::vec3 t_worldPos = a_transform->GetGlobalPosition() + (a_transform->GetGlobalRotation() * m_localPos);
			const Math::quat t_worldRot = Math::quat::Normalize(a_transform->GetGlobalRotation() * m_localRot);

			const JPH::Vec3 t_jphPos(t_worldPos.x, t_worldPos.y, t_worldPos.z);
			const JPH::Quat t_jphRot(t_worldRot.x, t_worldRot.y, t_worldRot.z, t_worldRot.w);

			t_bodyInterface->SetPosition(t_bodyID, t_jphPos, t_activation);
			m_bodyPos = t_worldPos;
			t_bodyInterface->SetRotation(t_bodyID, t_jphRot, t_activation);
			m_bodyRot = t_worldRot;
		}

		void RigidBodyComponent::UpdateObjectTransform(Math::Transform* a_transform)
		{
			const JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_bodyId = m_rigidBody->GetBodyID();
			const JPH::Vec3 t_bodyPos = t_bodyInterface->GetCenterOfMassPosition(t_bodyId);
			const JPH::Quat t_bodyRot = t_bodyInterface->GetRotation(t_bodyId);

			const Math::vec3 t_worldPos = { t_bodyPos.GetX(), t_bodyPos.GetY(), t_bodyPos.GetZ() };
			const Math::quat t_worldRot = Math::quat::Normalize({ t_bodyRot.GetX(), t_bodyRot.GetY(), t_bodyRot.GetZ(), t_bodyRot.GetW() });
			const Math::vec3 t_finalPos = t_worldPos - (t_worldRot * m_localPos);
			const Math::quat t_finalRot = t_worldRot * Math::quat::Conjugate(m_localRot);

			a_transform->Translate(t_finalPos - a_transform->GetGlobalPosition());
			m_bodyPos = t_worldPos;
			a_transform->Rotate(Math::quat::Normalize(t_finalRot) * Math::quat::Normalize(Math::quat::Conjugate(a_transform->GetGlobalRotation())));
			m_bodyRot = t_worldRot;
		}

		void RigidBodyComponent::NotifyCollision(const JPH::CollisionEvent a_collisionEvent, RigidBodyComponent* a_otherRigidBodyComponent) const
		{
			switch (a_collisionEvent)
			{
			case JPH::CollisionEvent::COLLISION_ENTER:
				OnCollisionEnter(a_otherRigidBodyComponent);
				return;
			case JPH::CollisionEvent::COLLISION_STAY:
				OnCollisionStay(a_otherRigidBodyComponent);
				return;
			case JPH::CollisionEvent::COLLISION_EXIT:
				OnCollisionExit(a_otherRigidBodyComponent);
				return;
			case JPH::CollisionEvent::TRIGGER_ENTER:
				OnTriggerEnter(a_otherRigidBodyComponent);
				return;
			case JPH::CollisionEvent::TRIGGER_STAY:
				OnTriggerStay(a_otherRigidBodyComponent);
				return;
			case JPH::CollisionEvent::TRIGGER_EXIT:
				OnTriggerExit(a_otherRigidBodyComponent);
			}
		}

		void RigidBodyComponent::OnCollisionEnter(RigidBodyComponent* a_otherRigidBodyComponent) const
		{

			m_onCollisionEnterEvent.Invoke(a_otherRigidBodyComponent);
		}

		void RigidBodyComponent::OnCollisionStay(RigidBodyComponent* a_otherRigidBodyComponent) const
		{
			m_onCollisionStayEvent.Invoke(a_otherRigidBodyComponent);
		}

		void RigidBodyComponent::OnCollisionExit(RigidBodyComponent* a_otherRigidBodyComponent) const
		{
			m_onCollisionExitEvent.Invoke(a_otherRigidBodyComponent);
		}

		void RigidBodyComponent::OnTriggerEnter(RigidBodyComponent* a_otherRigidBodyComponent) const
		{
			m_onTriggerEnterEvent.Invoke(a_otherRigidBodyComponent);
		}

		void RigidBodyComponent::OnTriggerStay(RigidBodyComponent* a_otherRigidBodyComponent) const
		{
			m_onTriggerStayEvent.Invoke(a_otherRigidBodyComponent);
		}

		void RigidBodyComponent::OnTriggerExit(RigidBodyComponent* a_otherRigidBodyComponent) const
		{
			m_onTriggerExitEvent.Invoke(a_otherRigidBodyComponent);
		}


		void RigidBodyComponent::SetLinearVelocity(const Math::vec3 a_velocity) const
		{
			ServiceLocator::GetPhysicsSystem()->EnqueueLinearVelocity(m_rigidBody->GetBodyID(), a_velocity);
		}

		void RigidBodyComponent::AddForce(Math::vec3 a_force) const
		{
			ServiceLocator::GetPhysicsSystem()->EnqueueAddForce(m_rigidBody->GetBodyID(), a_force);
		}

		void RigidBodyComponent::AddImpulse(Math::vec3 a_impulse) const
		{
			ServiceLocator::GetPhysicsSystem()->EnqueueAddImpulse(m_rigidBody->GetBodyID(), a_impulse);
		}

		void RigidBodyComponent::SetColliderType(Physics::ColliderType a_type)
		{
			m_rigidBody->SetColliderType(a_type);

			Core::Renderer* t_renderer = ServiceLocator::GetRenderer();

			switch (a_type)
			{
			case Physics::ColliderType::BOX:
				m_meshComponent->SetMesh("Assets/Meshes/WireframeCube.obj", t_renderer);
				break;
			case Physics::ColliderType::SPHERE:
				m_meshComponent->SetMesh("Assets/Meshes/WireframeSphere.obj", t_renderer);
				break;
			case Physics::ColliderType::CAPSULE:
				m_meshComponent->SetMesh("Assets/Meshes/WireframeCapsule.obj", t_renderer);
				break;
			}
		}

		void RigidBodyComponent::Destroy()
		{
			m_rigidBody->Remove();
			m_rigidBody->Destroy();
			delete m_rigidBody;
			m_rigidBody = nullptr;
		}

		void RigidBodyComponent::SetPosition(const Math::vec3& a_pos, const bool a_enable)
		{
			m_localPos = a_pos;
		}

		void RigidBodyComponent::Rotate(const Math::quat& a_rot, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			m_localRot *= a_rot;

			const JPH::BodyID t_bodyID = m_rigidBody->GetBodyID();
			const JPH::Quat t_currentRot = t_bodyInterface->GetRotation(t_bodyID);

			JPH::Quat t_newRot = t_currentRot * JPH::Quat(a_rot.x, a_rot.y, a_rot.z, a_rot.w);

			t_newRot = t_newRot.Normalized();

			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
			SetActive(a_enable);
			t_bodyInterface->SetRotation(t_bodyID, t_newRot, t_activation);

			m_bodyRot = Math::quat(t_newRot.GetX(), t_newRot.GetY(), t_newRot.GetZ(), t_newRot.GetW());
		}

		void RigidBodyComponent::SetRotation(const Math::quat& a_rot, bool a_enable)
		{
			m_localRot = a_rot;
		}

		void RigidBodyComponent::LockRotation(const char a_axis) const
		{
			m_rigidBody->LockRotation(a_axis);
		}
		void RigidBodyComponent::UnlockRotation(const char a_axis) const
		{
			m_rigidBody->UnlockRotation(a_axis);
		}

		RigidBodyData RigidBodyComponent::GetRigidBodyData() const
		{
			const RigidBodyData t_data = {
				.mBodyType = static_cast<int>(m_rigidBody->GetBodyType()),
				.mLayer = static_cast<int>(m_rigidBody->GetLayer()),
				.mColliderType = static_cast<int>(m_rigidBody->GetColliderType()),
				.mPos = m_localPos,
				.mScale = m_rigidBody->GetScale(),
				.mRadius = m_rigidBody->GetRadius(),
				.mHalfHeight = m_rigidBody->GetHalfHeight(),
				.mRot = m_localRot,
				.mMass = m_rigidBody->GetMass(),
				.mEnable = m_rigidBody->IsActive(),
				.mLockRotX = m_rigidBody->IsRotLockedX(),
				.mLockRotY = m_rigidBody->IsRotLockedY(),
				.mLockRotZ = m_rigidBody->IsRotLockedZ(),
				.mMeshId = GetMeshID(),
				.mFriction = m_rigidBody->GetFriction(),
				.mRestitution = m_rigidBody->GetRestitution(),
				.mMeshComponent = GetMeshComponent()
			};

			return t_data;
		}

		Math::vec3 RigidBodyComponent::GetVelocity()
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_bodyId = m_rigidBody->GetBodyID();
			const JPH::Vec3 t_velocity = t_bodyInterface->GetLinearVelocity(t_bodyId);
			return { t_velocity.GetX(), t_velocity.GetY(), t_velocity.GetZ() };
		}

		//shapecast towards the ground to check if anything is colliding within a specific distance threshold
		bool RigidBodyComponent::IsGrounded(Math::vec3 a_pos, Math::quat a_rot)
		{
			return m_rigidBody->IsGrounded(a_pos, a_rot);
		}

		RigidBodyComponent* RigidBodyComponent::GetComponent(const uint32_t a_id)
		{
			return ServiceLocator::GetPhysicsSystem()->GetComponent(a_id);
		}

		void RigidBodyComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetPhysicsSystem()->RemoveComponent(a_id);
		}

		void RigidBodyComponent::RemoveMesh()
		{
			MeshComponent::RemoveComponent(m_meshId);
		}
	}
}

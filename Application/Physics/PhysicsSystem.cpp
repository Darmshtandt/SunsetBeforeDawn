#include <Physics/PhysicsSystem.h>
#include <Objects/Components/PhysicComponents.h>
#include <Nt/Algorithms/GJK.h>
#include <Nt/Algorithms/EPA.h>

void PhysicsSystem::Update(const Float& deltaTime) const noexcept {
	for (const PhysicObject& object : m_Bodies) {
		RigidBody& rigidBody = object.pRigidBody->Body;
		if (rigidBody.GetMass() <= 0.f)
			continue;

		if (m_EnabledGravitation)
			rigidBody.AddForce(m_GravityDirection * deltaTime);

		if (rigidBody.GetForce().LengthSquare() == 0.f)
			continue;

		rigidBody.ApplyForces(deltaTime);

		object.pTransform->Translate(rigidBody.GetLinearVelocity() * deltaTime);

		object.pCollider->Collider.SetLocalToWorld(
			object.pTransform->LocalToWorld());

		CollisionResolve(object);
	}
}

void PhysicsSystem::RegisterObject(GameObject& object) {
	auto* pRigidBody = object.GetComponent<RigidBodyComponent>();
	if (pRigidBody == nullptr)
		return;

	auto* pTransform = RequireNotNull(object.GetComponent<Transform3D>());
	auto* pCollider = object.GetComponent<ColliderComponent>();

	m_Bodies.emplace_back(PhysicObject {
		.pTransform = pTransform,
		.pRigidBody = pRigidBody,
		.pCollider = pCollider,
		.pObject = &object
	});
}

void PhysicsSystem::UnregisterObject(const GameObject& object) {
	std::erase_if(m_Bodies, [&object] (const PhysicObject& other) {
		return other.pObject == &object;
	});
}

void PhysicsSystem::Clear() {
	m_Bodies.clear();
}

void PhysicsSystem::CollisionResolve(const PhysicObject& object) const {
	if (object.pCollider == nullptr)
		return;

	for (const PhysicObject& objectOther : m_Bodies) {
		if (&object == &objectOther || objectOther.pCollider == nullptr)
			continue;

		const Nt::ColliderPair colliderPair = {
			object.pCollider->Collider,
			objectOther.pCollider->Collider
		};

		const std::pair<Bool, Nt::Simplex> gjkResult =
			Nt::GJK::CheckCollision(colliderPair);

		if (!gjkResult.first)
			continue;

		const Nt::CollisionPoint point =
			Nt::EPA::CheckCollision(gjkResult.second, colliderPair);

		object.pTransform->Translate(-point.Normal * point.Depth);

		object.pCollider->Collider.SetLocalToWorld(
			object.pTransform->LocalToWorld());
	}
}

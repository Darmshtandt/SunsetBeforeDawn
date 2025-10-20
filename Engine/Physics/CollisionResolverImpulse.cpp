#include <iostream>
#include <Engine/Physics/CollisionResolverImpulse.h>
#include <World/Components/PhysicComponents.h>

void CollisionResolverImpulse::Resolve(const CollisionContact& contact) const {
	Nt::RigidBody& bodyA = contact.pA->pRigidBody->Body;
	Nt::RigidBody& bodyB = contact.pB->pRigidBody->Body;

	Nt::Float3D linearVelocityA = bodyA.GetLinearVelocity();
	Nt::Float3D linearVelocityB = bodyB.GetLinearVelocity();
	const Nt::Float3D relativeLinearVelocity = linearVelocityA - linearVelocityB;

	const Float velocityAlongNormal = relativeLinearVelocity.Dot(contact.Normal);
	if (velocityAlongNormal <= 0.f)
		return;

	const Float e = std::min(bodyA.GetRestitution(), bodyB.GetRestitution());
	Float j = -(1 + e) * velocityAlongNormal;
	j /= 1.f / (bodyA.GetMass() + bodyB.GetMass());

	const Nt::Float3D impulse = contact.Normal * -j;
	if (!bodyA.IsStatic()) {
		//bodyA.AddForce(impulse);
		contact.pA->pTransform->Translate(-contact.Normal * contact.Depth);
	}

	if (!bodyB.IsStatic()) {
		contact.pB->pTransform->Translate(contact.Normal * contact.Depth);
		//bodyB.AddForce(-impulse);
	}
}
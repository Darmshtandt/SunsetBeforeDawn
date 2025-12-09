#include <iostream>
#include <Engine/Physics/CollisionResolverImpulse.h>
#include <World/Components/PhysicComponents.h>

void CollisionResolverImpulse::Resolve(const ObjectContactPair& pair) const {
	const CollisionContact& contact = pair.Contact;
	if (pair.pA->pRigidBody == nullptr) {
		pair.pB->pTransform->Translate(contact.Normal * contact.Depth);
		return;
	}
	if (pair.pB->pRigidBody == nullptr) {
		pair.pA->pTransform->Translate(-contact.Normal * contact.Depth);
		return;
	}

	Nt::RigidBody& bodyA = pair.pA->pRigidBody->Body;
	Nt::RigidBody& bodyB = pair.pB->pRigidBody->Body;

	Nt::Float3D linearVelocityA = bodyA.GetLinearVelocity();
	Nt::Float3D linearVelocityB = bodyB.GetLinearVelocity();
	const Nt::Float3D relativeLinearVelocity = linearVelocityA - linearVelocityB;

	const Float velocityAlongNormal = relativeLinearVelocity.Dot(contact.Normal);
	if (velocityAlongNormal > 0.f)
		return;

	const Float e = std::min(bodyA.GetRestitution(), bodyB.GetRestitution());
	Float j = -(1 + e) * velocityAlongNormal;
	j /= 1.f / (bodyA.GetMass() + bodyB.GetMass());

	const Nt::Float3D impulse = contact.Normal * -j;
	if (!bodyA.IsStatic()) {
		//bodyA.AddForce(impulse);
		pair.pA->pTransform->Translate(-contact.Normal * contact.Depth);
	}

	if (!bodyB.IsStatic()) {
		pair.pB->pTransform->Translate(contact.Normal * contact.Depth);
		//bodyB.AddForce(-impulse);
	}
}
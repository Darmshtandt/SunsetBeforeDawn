#include <Engine/Physics/IntegratorEuler.h>
#include <World/Components/PhysicComponents.h>

void IntegratorEuler::Integrate(const PhysicObject& object, const Float& deltaTime) const {
	if (object.pRigidBody == nullptr)
		return;

	Nt::RigidBody& rigidBody = object.pRigidBody->Body;
	if (rigidBody.IsStatic())
		return;

	if (m_GravityVelocity.LengthSquare() > 0)
		rigidBody.AddForce(m_GravityVelocity * rigidBody.GetMass());

	//if (!rigidBody.HasForce())
	//	return;

	rigidBody.ApplyForces(1.f);
	object.pTransform->Translate(rigidBody.GetLinearVelocity() * deltaTime);
	rigidBody.SetLinearVelocity({ });
}

void IntegratorEuler::SetGravityVelocity(const Nt::Float3D& gravity) noexcept {
	m_GravityVelocity = gravity;
}
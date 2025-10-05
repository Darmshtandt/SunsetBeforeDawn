#include <Physics/RigidBody.h>

void RigidBody::ApplyForces(const Float& deltaTime) {
	if (m_Mass <= 0.f || m_Force.LengthSquare() == 0.f)
		return;

	const Nt::Float3D linearAcceleration = m_Force / m_Mass;
	m_LinearVelocity = linearAcceleration;

	m_Force = { };
}

void RigidBody::AddForce(const Nt::Float3D& force) noexcept {
	m_Force += force;
}

Nt::Float3D RigidBody::GetLinearVelocity() const noexcept {
	return m_LinearVelocity;
}

Nt::Float3D RigidBody::GetForce() const noexcept {
	return m_Force;
}

Float RigidBody::GetMass() const noexcept {
	return m_Mass;
}

Bool RigidBody::IsStatic() const noexcept {
	return m_Mass == 0.f;
}

void RigidBody::SetLinearVelocity(const Nt::Float3D& linearVelocity) noexcept {
	m_LinearVelocity = linearVelocity;
}
void RigidBody::SetForce(const Nt::Float3D& force) noexcept {
	m_Force = force;
}
void RigidBody::SetMass(const Float& mass) noexcept {
	m_Mass = mass;
}
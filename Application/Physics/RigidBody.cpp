#include <Physics/RigidBody.h>

RigidBody::RigidBody(NotNull<GameObject*> pOwner) :
	IBody(pOwner, Class<RigidBody>::ID())
{
}

void RigidBody::Update(const Float& deltaTime) {
	if (m_Parameters & (Awake | Active)) {
		RemoveParameter(Awake);
		AddForce(-m_LinearVelocity * (m_Friction + m_FrictionStatic));

		m_LinearAcceleration = m_Force / m_Mass;
		m_LinearVelocity += m_LinearAcceleration * deltaTime;

		m_pOwner->Translate(m_LinearVelocity * deltaTime);
		m_Force = { };
	}
}

void RigidBody::AddParameter(const Parameter& parameter) noexcept {
	m_Parameters |= parameter;
}

void RigidBody::RemoveParameter(const Parameter& parameter) noexcept {
	m_Parameters &= ~parameter;
}

void RigidBody::AddForce(const Nt::Float3D& force) noexcept {
	m_Force += force * GetWeight();
	if (m_Force != Nt::Float3D())
		AddParameter(Awake);
}

Nt::Float3D RigidBody::GetLinearVelocity() const noexcept {
	return m_LinearVelocity;
}
Nt::Float3D RigidBody::GetLinearAcceleration() const noexcept {
	return m_LinearAcceleration;
}

Float RigidBody::GetFriction() const noexcept {
	return m_Friction;
}
Float RigidBody::GetFrictionStatic() const noexcept {
	return m_FrictionStatic;
}
Float RigidBody::GetFrictionAir() const noexcept {
	return m_FrictionAir;
}
Float RigidBody::GetMass() const noexcept {
	return m_Mass;
}
Float RigidBody::GetWeight() const noexcept {
	return m_Mass * m_G;
}

Bool RigidBody::HasParameter(const Parameter& parameter) const noexcept {
	return m_Parameters & parameter;
}

void RigidBody::SetLinearVelocity(const Nt::Float3D& linearVelocity) noexcept {
	m_LinearVelocity = linearVelocity;
}
void RigidBody::SetLinearAcceleration(const Nt::Float3D& linearAcceleration) noexcept {
	m_LinearAcceleration = linearAcceleration;
}
void RigidBody::SetForce(const Nt::Float3D& force) noexcept {
	m_Force = force;
}
void RigidBody::SetFriction(const Float& friction) noexcept {
	m_Friction = friction;
}
void RigidBody::SetFrictionStatic(const Float& frictionStatic) noexcept {
	m_FrictionStatic = frictionStatic;
}
void RigidBody::SetMass(const Float& mass) noexcept {
	m_Mass = mass;
}

Bool RigidBody::HandleCollision(NotNull<const IBody*> pOther) {
	if (!HasParameter(Collision))
		return false;

	Nt::CollisionPoint point;

	const Bool result = m_Collider.CheckCollision(pOther->GetCollider(), &point);
	if (result)
		m_pOwner->Translate(-point.Normal * point.Depth);
	return result;
}

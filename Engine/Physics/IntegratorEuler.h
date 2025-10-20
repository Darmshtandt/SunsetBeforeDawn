#pragma once

#include <Engine/Physics/Base/PhysicsInterfaces.h>

class IntegratorEuler final : public IPhysicsIntegrator {
public:
	IntegratorEuler() noexcept = default;
	~IntegratorEuler() noexcept override = default;

	void Integrate(const PhysicObject& object, const Float& deltaTime) const override;
	void SetGravityVelocity(const Nt::Float3D& gravity) noexcept override;

private:
	Nt::Float3D m_GravityVelocity;
};
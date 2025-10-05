#pragma once

#include <Nt/Core/Math/Vector.h>

class RigidBody final {
public:
	RigidBody() noexcept = default;
	RigidBody(const RigidBody&) noexcept = default;
	RigidBody(RigidBody&&) noexcept = default;
	~RigidBody() noexcept = default;

	RigidBody& operator = (const RigidBody&) noexcept = default;
	RigidBody& operator = (RigidBody&&) noexcept = default;

	void ApplyForces(const Float& deltaTime);
	void AddForce(const Nt::Float3D& force) noexcept;

	[[nodiscard]] Nt::Float3D GetLinearVelocity() const noexcept;
	[[nodiscard]] Nt::Float3D GetForce() const noexcept;

	[[nodiscard]] Float GetMass() const noexcept;
	[[nodiscard]] Bool IsStatic() const noexcept;

	void SetLinearVelocity(const Nt::Float3D& linearVelocity) noexcept;
	void SetForce(const Nt::Float3D& force) noexcept;
	void SetMass(const Float& mass) noexcept;

private:
	Nt::Float3D m_Force;
	Nt::Float3D m_LinearVelocity;
	Float m_Mass = 1.f;
};
#pragma once

#include <Nt/IBody.h>

class RigidBody final : public IBody {
public:
	enum Parameter : Byte {
		Active = 1 << 0,
		Awake = 1 << 1,
		Collision = 1 << 2,
	};

public:
	explicit RigidBody(NotNull<GameObject*> pOwner);

	RigidBody() = delete;
	RigidBody(const RigidBody&) noexcept = default;
	RigidBody(RigidBody&&) noexcept = default;
	~RigidBody() noexcept override = default;

	RigidBody& operator = (const RigidBody&) noexcept = default;
	RigidBody& operator = (RigidBody&&) noexcept = default;

	void Update(const Float& deltaTime) override;

	void AddParameter(const Parameter& parameter) noexcept;
	void RemoveParameter(const Parameter& parameter) noexcept;

	void AddForce(const Nt::Float3D& force) noexcept;
	Bool HandleCollision(NotNull<const IBody*> pOther) override;

	[[nodiscard]] Nt::Float3D GetLinearVelocity() const noexcept;
	[[nodiscard]] Nt::Float3D GetLinearAcceleration() const noexcept;

	[[nodiscard]] Float GetFriction() const noexcept;
	[[nodiscard]] Float GetFrictionStatic() const noexcept;
	[[nodiscard]] Float GetFrictionAir() const noexcept;
	[[nodiscard]] Float GetMass() const noexcept;
	[[nodiscard]] Float GetWeight() const noexcept;

	[[nodiscard]] Bool HasParameter(const Parameter& parameter) const noexcept;

	void SetLinearVelocity(const Nt::Float3D& linearVelocity) noexcept;
	void SetLinearAcceleration(const Nt::Float3D& linearAcceleration) noexcept;
	void SetForce(const Nt::Float3D& force) noexcept;
	void SetFriction(const Float& friction) noexcept;
	void SetFrictionStatic(const Float& frictionStatic) noexcept;
	void SetMass(const Float& mass) noexcept;

private:
	Nt::Float3D m_Force;
	Nt::Float3D m_LinearAcceleration;
	Nt::Float3D m_LinearVelocity;

	Float m_Mass = 1.f;
	Float m_Friction = 0.01f;
	Float m_FrictionStatic = 0.f;
	Float m_FrictionAir = 0.01f;
	Float m_G = 9.81f;

	Byte m_Parameters = Active | Awake | Collision;
};
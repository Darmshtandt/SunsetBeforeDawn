#pragma once

#include <Physics/RigidBody.h>
#include <vector>

class PhysicsSystem final {
public:
	PhysicsSystem() noexcept = default;
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) noexcept = default;
	~PhysicsSystem() noexcept = default;

	PhysicsSystem& operator = (const PhysicsSystem&) = delete;
	PhysicsSystem& operator = (PhysicsSystem&&) noexcept = default;

	void Update(const Float& deltaTime) const noexcept;

	void AddObject(NotNull<GameObject*> pObject);
	void RemoveObject(NotNull<const GameObject*> pObject);
	void Clear();

public:
	Nt::Float3D m_GravityDirection = { 0.f, -1.f, 0.f };
	std::vector<IBody*> m_Bodies;
	Bool m_EnabledGravitation = false;
};
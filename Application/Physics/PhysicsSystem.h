#pragma once

#include <Objects/GameObject.h>
#include <vector>

class RigidBodyComponent;
class ColliderComponent;

class PhysicsSystem final {
public:
	struct PhysicObject {
		Transform3D* pTransform = nullptr;
		RigidBodyComponent* pRigidBody = nullptr;
		ColliderComponent* pCollider = nullptr;
		GameObject* pObject = nullptr;
	};

public:
	PhysicsSystem() noexcept = default;
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) noexcept = default;
	~PhysicsSystem() noexcept = default;

	PhysicsSystem& operator = (const PhysicsSystem&) = delete;
	PhysicsSystem& operator = (PhysicsSystem&&) noexcept = default;

	void Update(const Float& deltaTime) const noexcept;

	void RegisterObject(GameObject& object);
	void UnregisterObject(const GameObject& object);
	void Clear();

private:
	Nt::Float3D m_GravityDirection = { 0.f, -1.f, 0.f };
	std::vector<PhysicObject> m_Bodies;
	Float m_G = 9.81f;
	Bool m_EnabledGravitation = false;

private:
	void CollisionResolve(const PhysicObject& object) const;
};
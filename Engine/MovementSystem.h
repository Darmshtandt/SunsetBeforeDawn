#pragma once

#include <World/Objects/GameObject.h>

class RigidBody;

class MovementSystem final {
public:
	struct Movable final {
		Transform3D* pTransform = nullptr;
		Movement3D* pMovement = nullptr;
		RigidBody* pRigidBody = nullptr;
		GameObject* pObject = nullptr;
	};

public:
	MovementSystem() noexcept = default;
	MovementSystem(const MovementSystem&) noexcept = default;
	MovementSystem(MovementSystem&&) noexcept = default;
	~MovementSystem() noexcept = default;

	MovementSystem& operator = (const MovementSystem&) noexcept = default;
	MovementSystem& operator = (MovementSystem&&) noexcept = default;

	void RegisterObject(GameObject& object);
	void UnregisterObject(GameObject& object);

	void Update(const Float& deltaTime);

private:
	std::vector<Movable> m_Movables;
};
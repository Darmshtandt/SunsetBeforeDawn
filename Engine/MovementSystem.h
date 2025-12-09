#pragma once

#include <World/Objects/GameObject.h>
#include <Engine/ISystem.h>

class RigidBody;

class MovementSystem final : public ISystem {
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
	~MovementSystem() noexcept override = default;

	MovementSystem& operator = (const MovementSystem&) noexcept = default;
	MovementSystem& operator = (MovementSystem&&) noexcept = default;

	void RegisterObject(GameObject& object) override;
	void UnregisterObject(const GameObject& object) override;

	void Update(const Float& deltaTime) override;

private:
	std::vector<Movable> m_Movables;
};
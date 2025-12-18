#pragma once

#include <World/Components/BasicComponents.h>

class Armor;
class Health;

struct LivingPawn final {
	Transform3D* pTransform = nullptr;
	Health* pHealth = nullptr;
	Armor* pArmor = nullptr;
	const GameObject* pObject = nullptr;
};

struct PhysicObject;
struct IPhysicsOverlapper;

struct IMovementController {
	virtual ~IMovementController() noexcept = default;

	virtual void FollowRoute(Float deltaTime) = 0;

	virtual void MoveTo(const Nt::Float3D& point) = 0;
	virtual void MoveInDirection(const Nt::Float3D& direction) = 0;

	virtual void LookInDirection(const Nt::Float3D& direction, Float time) = 0;

	[[nodiscard]] virtual Bool IsAt(const Nt::Float3D& point) const = 0;
	[[nodiscard]] virtual Bool HasRoute() const = 0;
};

struct IAttackController {
	virtual ~IAttackController() noexcept = default;

	virtual void Attack() = 0;

	[[nodiscard]] virtual uLLong GetCooldown() const noexcept = 0;
	[[nodiscard]] virtual Bool HasTarget() const noexcept = 0;
};

struct ISenses {
	virtual ~ISenses() noexcept = default;

	virtual void Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) = 0;

	[[nodiscard]] virtual const std::vector<LivingPawn>& GetVisiblePawns() const noexcept = 0;
};


struct AIContext final {
	IMovementController* pMovementController = nullptr;
	IAttackController* pAttackController = nullptr;
	ISenses* pSenses = nullptr;
};

struct IControlled {
	virtual ~IControlled() noexcept = default;

	virtual IMovementController* GetMovementController() noexcept = 0;
	virtual IAttackController* GetAttackController() noexcept = 0;
	virtual ISenses* GetSenses() noexcept = 0;
};
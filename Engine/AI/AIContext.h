#pragma once

#include <Engine/AI/Blackboard.h>

struct PhysicObject;
struct IPhysicsOverlapper;

struct IMovementController {
	virtual ~IMovementController() noexcept = default;

	virtual void MoveTo(const Nt::Float3D& point) = 0;
	virtual void MoveInDirection(const Nt::Float3D& direction) = 0;

	virtual void LookInDirection(const Nt::Float3D& direction, Float time) = 0;

	[[nodiscard]] virtual Bool IsAt(const Nt::Float3D& point) const = 0;
	[[nodiscard]] virtual Route3D* GetRoute() = 0;
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
	Blackboard Memory;
};
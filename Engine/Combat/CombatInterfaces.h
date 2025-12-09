#pragma once

#include <World/Entities.h>

struct PhysicObject;
struct IPhysicsOverlapper;

struct IDamage {
	virtual ~IDamage() noexcept = default;

	[[nodiscard]]
	virtual std::vector<PhysicObject> FindTargets(const Transform3D& ownerTransform, NotNull<IPhysicsOverlapper*> pOverlapper) const noexcept = 0;
	virtual void Apply(const std::vector<GameObject*>& targets) const = 0;

	[[nodiscard]] virtual Float GetAmount() const noexcept = 0;
};

struct DamageCommand final {
	Transform3D* pOwnerTransform = nullptr;
	GameObject* pOwner = nullptr;
	const IDamage* pDamage = nullptr;
};

struct ICombatDispatcher {
	virtual ~ICombatDispatcher() noexcept = default;

	virtual void Dispatch(const DamageCommand& command) = 0;
};
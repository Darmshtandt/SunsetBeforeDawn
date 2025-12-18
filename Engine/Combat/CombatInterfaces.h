#pragma once

#include <World/Components/BasicComponents.h>

struct PhysicObject;
struct IPhysicsOverlapper;

struct IDamage {
	virtual ~IDamage() noexcept = default;

	[[nodiscard]]
	virtual std::vector<PhysicObject> HitDetect(const Nt::Float3D& position, const Nt::Float3D& rotation, NotNull<IPhysicsOverlapper*> pOverlapper) const noexcept = 0;
	virtual void Apply(const std::vector<GameObject*>& targets) const = 0;

	[[nodiscard]] virtual Float GetAmount() const noexcept = 0;
};

struct DamageCommand final {
	const IDamage* pDamage = nullptr;
	Nt::Float3D Position;
	Nt::Float3D Rotation;
	GameObject* pOwner = nullptr;
};

struct ICombatDispatcher {
	virtual ~ICombatDispatcher() noexcept = default;

	virtual void Dispatch(const DamageCommand& command) = 0;
};
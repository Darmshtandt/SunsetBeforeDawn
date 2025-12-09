#pragma once

#include <Engine/Combat/CombatInterfaces.h>

struct SphereDamage : public IDamage {
	~SphereDamage() noexcept override = default;

	[[nodiscard]] std::vector<PhysicObject> FindTargets(const Transform3D& ownerTransform, NotNull<IPhysicsOverlapper*> pOverlapper) const noexcept override;
	void Apply(const std::vector<GameObject*>& targets) const override;

	[[nodiscard]] Float GetAmount() const noexcept override;

	Float Radius = 0.f;
	Float Amount = 0.f;
};

struct RayDamage : public IDamage {
	~RayDamage() noexcept override = default;

	[[nodiscard]] std::vector<PhysicObject> FindTargets(const Transform3D& ownerTransform, NotNull<IPhysicsOverlapper*> pOverlapper) const noexcept override;
	void Apply(const std::vector<GameObject*>& targets) const override;

	[[nodiscard]] Float GetAmount() const noexcept override;

	Nt::Float3D Origin;
	Float Distance;
	Float Amount = 0.f;
};
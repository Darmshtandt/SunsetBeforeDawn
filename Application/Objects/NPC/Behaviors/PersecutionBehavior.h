#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Objects/NPC/Behaviors/IBehavior.h>

class PersecutionBehavior final : public IBehavior {
public:
	PersecutionBehavior() noexcept;
	PersecutionBehavior(const PersecutionBehavior&) noexcept = default;
	PersecutionBehavior(PersecutionBehavior&&) noexcept = default;
	~PersecutionBehavior() noexcept override = default;

	PersecutionBehavior& operator = (const PersecutionBehavior&) noexcept = default;
	PersecutionBehavior& operator = (PersecutionBehavior&&) noexcept = default;

	void Update(Creature& creature, const Float& deltaTime) override;

private:
	inline static BehaviorRegistrar<PersecutionBehavior> m_Registrar { "Persecution" };

private:
	Nt::Float3D _CalculateAngle(const Nt::Float3D& npcPosition, const Nt::Float3D& targetPosition);
};
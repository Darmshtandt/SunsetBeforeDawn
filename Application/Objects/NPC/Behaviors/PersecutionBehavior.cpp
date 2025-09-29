#include <Objects/NPC/Behaviors/PersecutionBehavior.h>
#include <Objects/NPC/Creature.h>

PersecutionBehavior::PersecutionBehavior() noexcept :
	IBehavior(Class<PersecutionBehavior>::ID())
{
}

void PersecutionBehavior::Update(Creature& creature, const Float& deltaTime) {
	RequireNotNull(creature.GetTarget().lock());

	const Nt::Float3D direction = _CalculateAngle(creature.GetPosition(), creature.GetTarget().lock()->GetPosition());
	creature.SetAngle({ 0.f, -std::atan2(direction.x, direction.z) / RADf, 0.f });

	if (!creature.CanInteract())
		creature.Move(direction * creature.GetSpeed() * deltaTime);
}

Nt::Float3D PersecutionBehavior::_CalculateAngle(const Nt::Float3D& npcPosition, const Nt::Float3D& targetPosition) {
	return (targetPosition - npcPosition).GetNormalize();
}
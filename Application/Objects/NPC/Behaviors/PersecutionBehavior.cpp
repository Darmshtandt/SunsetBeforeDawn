#include <Objects/NPC/Behaviors/PersecutionBehavior.h>
#include <Objects/NPC/Creature.h>

PersecutionBehavior::PersecutionBehavior() noexcept :
	IBehavior(Class<PersecutionBehavior>::ID())
{
}

void PersecutionBehavior::Update(MovableCreature& creature, const Float& deltaTime) {
	const Creature::Target& target = creature.pCreature->GetTarget();
	if (creature.pCreature->CanInteract())
		return;

	const Nt::Float3D direction =
		-creature.pTransform->CalculateAngle(*target.pTransform);

	creature.pMovement->Direction = direction;
	creature.pMovement->DesiredRotation.y = std::atan2(-direction.x, direction.z);
	creature.pMovement->DesiredRotation.y -= creature.pTransform->Rotation().y;
	creature.pMovement->DesiredRotation.y /= deltaTime * 4.f;

	if (!std::isfinite(creature.pMovement->DesiredRotation.y))
		creature.pMovement->DesiredRotation.y = 0.f;
}
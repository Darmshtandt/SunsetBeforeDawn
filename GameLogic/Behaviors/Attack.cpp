#include <GameLogic/Behaviors/Attack.h>
#include <World/Objects/Creature.h>
#include <World/Components/CombatComponents.h>

Attack::Attack() noexcept :
	IBehavior(Class<Attack>::ID())
{
}

void Attack::Update(Actor& actor, const Float& deltaTime) {
	Assert(actor.pObject != nullptr, "actor.pObject is null");

	auto* pCombat = RequireNotNull(actor.pObject->GetComponent<Combat>());
	pCombat->PerformAttack();
}
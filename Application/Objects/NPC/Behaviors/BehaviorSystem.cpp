#include <Objects/NPC/Behaviors/BehaviorSystem.h>
#include <Objects/NPC/Behaviors/WanderingBehavior.h>
#include <Objects/NPC/Behaviors/PersecutionBehavior.h>
#include <Objects/NPC/Creature.h>

BehaviorSystem::BehaviorSystem() noexcept :
	m_BehaviorMap(BehaviorFactory::Instance().CreateAll())
{
}

void BehaviorSystem::EvaluateBehavior(NotNull<Creature*> pCreature) {
	constexpr ClassID wanderingID = Class<WanderingBehavior>::ID();
	constexpr ClassID persecutionID = Class<PersecutionBehavior>::ID();

	pCreature->SetBehavior(m_BehaviorMap[wanderingID].get());

	pCreature->SetOnTargetFound([this] (Creature& creature) {
		creature.SetBehavior(m_BehaviorMap[persecutionID].get());
		});
	pCreature->SetOnTargetLost([this] (Creature& creature) {
		creature.SetBehavior(m_BehaviorMap[wanderingID].get());
		});
}
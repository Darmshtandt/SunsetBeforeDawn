#include <Objects/NPC/Behaviors/BehaviorSystem.h>
#include <Objects/NPC/Behaviors/Wandering.h>
#include <Objects/NPC/Behaviors/Persecution.h>

BehaviorSystem::BehaviorSystem() noexcept :
	m_BehaviorMap(BehaviorFactory::Instance().CreateAll())
{
}

void BehaviorSystem::EvaluateBehavior(NotNull<Creature*> pCreature) {
	constexpr ClassID wanderingID = Class<Wandering>::ID();
	constexpr ClassID persecutionID = Class<Persecution>::ID();

	pCreature->SetBehavior(m_BehaviorMap[wanderingID].get());

	pCreature->SetOnTargetFound([this] (Creature& creature) {
		creature.SetBehavior(m_BehaviorMap[persecutionID].get());
		});
	pCreature->SetOnTargetLost([this] (Creature& creature) {
		creature.SetBehavior(m_BehaviorMap[wanderingID].get());
		});
}
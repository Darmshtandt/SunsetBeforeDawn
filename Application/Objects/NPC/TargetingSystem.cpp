#include <Objects/NPC/TargetingSystem.h>
#include <World/Scene.h>

void TargetingSystem::Update(const Float& deltaTime) {
	for (Interval& interval : m_ScanIntervals)
		interval.Update();
}

void TargetingSystem::RegisterCreature(const Scene& scene, Creature& creature) {
	m_ScanIntervals.emplace_back(2000, 2000, [&] () {
		if (!creature.HasTarget())
			_ScanForTargets(scene, creature);
	});
}

void TargetingSystem::_ScanForTargets(const Scene& scene, Creature& creature) {
	const Creature::View& view = creature.GetView();

	const std::vector<PlayerPtr>& players = scene.GetAllPlayers();
	for (const PlayerPtr& pObject : players) {
		if (pObject->GetDistance(creature) <= view.Range) {
			creature.SetTarget(pObject);
			return;
		}
	}

	const std::vector<CreaturePtr>& nps = scene.GetAllCreatures();
	for (const CreaturePtr& pObject : nps) {
		if (pObject.get() == &creature)
			continue;

		if (pObject->GetDistance(creature) <= view.Range) {
			creature.SetTarget(pObject);
			return;
		}
	}
}
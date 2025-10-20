#include <Objects/NPC/TargetingSystem.h>

void TargetingSystem::RegisterObject(GameObject& object) {
	Target target;
	target.pTransform = RequireNotNull(object.GetComponent<Transform3D>());
	target.pObject = &object;

	m_Targets.emplace_back(target);

	if (object.GetType() != ObjectType::Creature)
		return;

	Targetable targetable;
	targetable.pTransform = target.pTransform;
	targetable.pCreature = static_cast<Creature*>(&object);
	m_Targetables.emplace_back(targetable);

	m_ScanIntervals.emplace_back(2000, 2000, [&] () {
		for (const Targetable& targetable : m_Targetables) {
			if (!targetable.pCreature->HasTarget())
				_ScanForTargets(targetable);
		}
		});
}

void TargetingSystem::UnregisterObject(GameObject& object) {
	std::erase_if(m_Targets, [&object] (const Target& target) {
		return target.pObject == &object;
		});

	if (object.GetType() != ObjectType::Creature)
		return;

	Creature* pCreature = static_cast<Creature*>(&object);
	std::erase_if(m_Targetables, [pCreature] (const Targetable& target) {
		return target.pCreature == pCreature;
		});
}

void TargetingSystem::Update(const Float& deltaTime) {
	for (Interval& interval : m_ScanIntervals)
		interval.Update();
}

void TargetingSystem::_ScanForTargets(const Targetable& targetable) {
	const Creature::View& view = targetable.pCreature->GetView();

	for (const Target& target : m_Targets) {
		if (targetable.pCreature == target.pObject)
			continue;

		const Float distanceSq =
			targetable.pTransform->DistanceSquare(*target.pTransform);
		if (distanceSq > view.RangeSq)
			continue;

		targetable.pCreature->SetTarget(target);
		return;
	}
}
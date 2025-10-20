#pragma once

#include <Interval.h>
#include <Objects/NPC/Creature.h>

class TargetingSystem final {
public:
	using Target = Creature::Target;

	struct Targetable final {
		Transform3D* pTransform = nullptr;
		Creature* pCreature = nullptr;
	};

public:
	TargetingSystem() noexcept = default;
	TargetingSystem(const TargetingSystem&) noexcept = default;
	TargetingSystem(TargetingSystem&&) noexcept = default;
	~TargetingSystem() noexcept = default;

	TargetingSystem& operator = (const TargetingSystem&) noexcept = default;
	TargetingSystem& operator = (TargetingSystem&&) noexcept = default;

	void RegisterObject(GameObject& object);
	void UnregisterObject(GameObject& object);

	void Update(const Float& deltaTime);

private:
	std::vector<Interval> m_ScanIntervals;
	std::vector<Targetable> m_Targetables;
	std::vector<Target> m_Targets;

private:
	void _ScanForTargets(const Targetable& targetable);
};
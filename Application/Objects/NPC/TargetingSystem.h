#pragma once

#include <Interval.h>

class Creature;
class Scene;

class TargetingSystem final {
public:
	TargetingSystem() noexcept = default;
	TargetingSystem(const TargetingSystem&) noexcept = default;
	TargetingSystem(TargetingSystem&&) noexcept = default;
	~TargetingSystem() noexcept = default;

	TargetingSystem& operator = (const TargetingSystem&) noexcept = default;
	TargetingSystem& operator = (TargetingSystem&&) noexcept = default;

	void Update(const Float& deltaTime);

	void RegisterCreature(const Scene& scene, Creature& creature);

private:
	std::vector<Interval> m_ScanIntervals;

private:
	void _ScanForTargets(const Scene& scene, Creature& creature);
};
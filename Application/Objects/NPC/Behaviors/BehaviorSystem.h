#pragma once

#include <memory>
#include <Objects/NPC/Behaviors/IBehavior.h>

class BehaviorSystem final {
public:
	BehaviorSystem() noexcept;
	BehaviorSystem(const BehaviorSystem&) noexcept = default;
	BehaviorSystem(BehaviorSystem&&) noexcept = default;
	~BehaviorSystem() noexcept = default;

	BehaviorSystem& operator = (const BehaviorSystem&) noexcept = default;
	BehaviorSystem& operator = (BehaviorSystem&&) noexcept = default;

	void EvaluateBehavior(NotNull<Creature*> pCreature);

private:
	std::unordered_map<ClassID, BehaviorPtr> m_BehaviorMap;
};
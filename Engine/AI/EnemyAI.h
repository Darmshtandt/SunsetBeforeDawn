#pragma once

#include <Engine/MovementSystem.h>
#include <Engine/AI/Behavior/BehaviorTree.h>
#include <World/Components/AIComponent.h>

class Creature;

class EnemyAI final : public IIntelligence {
public:
	explicit EnemyAI(NotNull<Creature*> pOwner);

	void Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) override;
	void Tick(Float deltaTime) override;

	[[nodiscard]] AIContext GetContext() const noexcept override;

private:
	BehaviorTree m_BehaviorTree;
	AIContext m_Context;
	Creature* m_pOwner;
};
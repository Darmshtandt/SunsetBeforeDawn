#include <Engine/AI/EnemyAI.h>
#include <Engine/AI/Behavior/BNComposites.h>
#include <Engine/AI/Behavior/BaseBehaviors.h>
#include <World/Objects/Creature.h>

EnemyAI::EnemyAI(NotNull<Creature*> pOwner) :
	m_pOwner(pOwner)
{
	m_Context.pMovementController = pOwner->GetMovementController();
	m_Context.pAttackController = pOwner->GetAttackController();
	m_Context.pSenses = pOwner->GetSenses();

	std::stack<IBehaviorNode*> bnStack;
	bnStack.push(m_BehaviorTree.SetRootNode<BehaviorSelectorNode>());

	bnStack.push(bnStack.top()->AddNode("Sequence"));
	bnStack.top()->AddNode("Pursue");
	bnStack.top()->AddNode("Attack");
	bnStack.pop();

	bnStack.top()->AddNode("Patrol");
}

void EnemyAI::Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) {
	m_Context.pSenses->Scan(pOverlapper, deltaTime);
}

void EnemyAI::Tick(Float deltaTime) {
	m_BehaviorTree.Tick(m_Context, deltaTime);
}

AIContext EnemyAI::GetContext() const noexcept {
	return m_Context;
}
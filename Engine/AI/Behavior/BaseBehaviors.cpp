#include <Engine/AI/Behavior/BaseBehaviors.h>

// ============================================================================
//	BehaviorNodePatrol
// ============================================================================
BehaviorNodePatrol::BehaviorNodePatrol() noexcept:
	m_ChangeStateInterval(1500, 1500, [this] () { _ChangeState(); })
{
}

BNStatus BehaviorNodePatrol::Tick(const AIContext& context, Float deltaTime) {
	m_ChangeStateInterval.Update();
	if (m_State == State::Stay)
		return BNStatus::Successful;

	context.pMovementController->MoveInDirection(m_Direction);
	context.pMovementController->LookInDirection(m_Direction, deltaTime * 4.f);

	return BNStatus::Successful;
}

IBehaviorNode* BehaviorNodePatrol::AddNode(const std::string& name) {
	Raise("The Condition does not contained nodes");
}

void BehaviorNodePatrol::_ChangeState() noexcept {
	if (std::rand() % 2 == 0) {
		m_State = State::Walking;
		m_Direction = Nt::Float3D(
			std::rand() + 1.f, 0.f, std::rand() + 1.f).GetNormalize();
	}
	else {
		m_State = State::Stay;
	}
}

// ============================================================================
//	BehaviorNodePursue
// ============================================================================
BNStatus BehaviorNodePursue::Tick(const AIContext& context, Float deltaTime) {
	if (!context.pAttackController->HasTarget() || !context.pMovementController->HasRoute())
		return BNStatus::Failed;

	context.pMovementController->FollowRoute(deltaTime);
	if (!context.pMovementController->HasRoute())
		return BNStatus::Successful;

	return BNStatus::Running;
}

IBehaviorNode* BehaviorNodePursue::AddNode(const std::string& name) {
	Raise("The Condition does not contained nodes");
}

// ============================================================================
//	BehaviorNodeAttack
// ============================================================================
BNStatus BehaviorNodeAttack::Tick(const AIContext& context, Float deltaTime) {
	if (m_Timer.GetElapsedTimeMs() < context.pAttackController->GetCooldown())
		return BNStatus::Running;

	if (context.pMovementController->HasRoute())
		return BNStatus::Failed;

	context.pAttackController->Attack();
	m_Timer.Restart();

	return BNStatus::Running;
}

IBehaviorNode* BehaviorNodeAttack::AddNode(const std::string& name) {
	Raise("The Condition does not contained nodes");
}
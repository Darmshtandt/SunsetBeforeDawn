#include <Objects/NPC/Behaviors/WanderingBehavior.h>
#include <Objects/NPC/Creature.h>

WanderingBehavior::WanderingBehavior() noexcept :
	IBehavior(Class<WanderingBehavior>::ID()),
	m_ChangeStateInterval(1500, 3000, [this] () { _ChangeState(); })
{
}

void WanderingBehavior::Update(MovableCreature& creature, const Float& deltaTime) {
	m_ChangeStateInterval.Update();

	if (m_State == State::Walking) {
		creature.pMovement->Direction = m_Direction;
		creature.pMovement->DesiredRotation.y = std::atan2(-m_Direction.x, m_Direction.z);
		creature.pMovement->DesiredRotation.y -= creature.pTransform->Rotation().y;
		creature.pMovement->DesiredRotation.y /= deltaTime * 4.f;

		if (!std::isfinite(creature.pMovement->DesiredRotation.y))
			creature.pMovement->DesiredRotation.y = 0.f;
	}
}

void WanderingBehavior::_ChangeState() noexcept {
	if (std::rand() % 2 == 0) {
		m_State = State::Walking;
		m_Direction = Nt::Float3D(
			std::rand() + 1.f, 0.f, std::rand() + 1.f).GetNormalize();
	}
	else {
		m_State = State::Idle;
	}
}
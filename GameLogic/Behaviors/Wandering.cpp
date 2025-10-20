#include <GameLogic/Behaviors/Wandering.h>
#include <World/Objects/Creature.h>

Wandering::Wandering() noexcept :
	IBehavior(Class<Wandering>::ID()),
	m_ChangeStateInterval(1500, 1500, [this] () { _ChangeState(); })
{
}

void Wandering::Update(Actor& actor, const Float& deltaTime) {
	m_ChangeStateInterval.Update();

	if (m_State == State::Walking) {
		actor.pMovement->Direction = m_Direction;
		actor.pMovement->DesiredRotation.y = std::atan2(m_Direction.x, m_Direction.z);
		actor.pMovement->DesiredRotation.y -= actor.pTransform->Rotation().y;
		actor.pMovement->DesiredRotation.y /= deltaTime * 4.f;

		if (!std::isfinite(actor.pMovement->DesiredRotation.y))
			actor.pMovement->DesiredRotation.y = 0.f;
	}
}

void Wandering::_ChangeState() noexcept {
	if (std::rand() % 2 == 0) {
		m_State = State::Walking;
		m_Direction = Nt::Float3D(
			std::rand() + 1.f, 0.f, std::rand() + 1.f).GetNormalize();
	}
	else {
		m_State = State::Idle;
	}
}
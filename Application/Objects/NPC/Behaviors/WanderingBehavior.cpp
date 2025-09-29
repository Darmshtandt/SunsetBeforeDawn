#include <Objects/NPC/Behaviors/WanderingBehavior.h>
#include <Objects/NPC/creature.h>

WanderingBehavior::WanderingBehavior() noexcept :
	IBehavior(Class<WanderingBehavior>::ID()),
	m_ChangeStateInterval(1500, 3000, [this] () { _ChangeState(); })
{
}

void WanderingBehavior::Update(Creature& creature, const Float& deltaTime) {
	m_ChangeStateInterval.Update();

	if (m_State == State::Walking) {
		creature.Move(m_NpcDirection * creature.GetSpeed() * deltaTime);
		creature.SetAngle({ 0.f, -std::atan2(m_NpcDirection.x, m_NpcDirection.z) / RADf, 0.f });
	}
}

void WanderingBehavior::_ChangeState() noexcept {
	if (std::rand() % 2 == 0) {
		m_State = State::Walking;
		m_NpcDirection = Nt::Float3D(std::rand(), 0.f, std::rand()).GetNormalize();
	}
	else {
		m_State = State::Idle;
	}
}
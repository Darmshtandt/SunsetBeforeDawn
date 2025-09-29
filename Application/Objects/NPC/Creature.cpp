#include <Objects/NPC/Creature.h>

Creature::Creature(const ClassID& id, std::string factionName) noexcept :
	Character(id, ObjectType::Creature, std::move(factionName))
{
}

void Creature::Update(const Float& deltaTime) {
	if (HasTarget() && GetDistance(*m_pTarget.lock()) >= m_PersecutionDistance) {
		m_pTarget.reset();
		m_OnTargetLost.Emmit(*this);
	}

	RequireNotNull(m_pBehavior)->Update(*this, deltaTime);
}

void Creature::SetTarget(std::weak_ptr<Character> pTarget) noexcept {
	const Bool hasChanged = m_pTarget.lock() != pTarget.lock();

	m_pTarget = std::move(pTarget);
	if (!hasChanged)
		return;

	if (m_pTarget.expired())
		m_OnTargetLost.Emmit(*this);
	else
		m_OnTargetFound.Emmit(*this);
}

void Creature::SetBehavior(NotNull<IBehavior*> pBehavior) noexcept {
	m_pBehavior = pBehavior;
}
void Creature::SetOnTargetFound(const Signal<Creature&>::Slot& slot) {
	m_OnTargetFound.Connect(slot);
}
void Creature::SetOnTargetLost(const Signal<Creature&>::Slot& slot) {
	m_OnTargetLost.Connect(slot);
}

Float Creature::GetPersecutionDistance() const noexcept {
	return m_PersecutionDistance;
}

const std::weak_ptr<Character>& Creature::GetTarget() const noexcept {
	return m_pTarget;
}

const Creature::View& Creature::GetView() const noexcept {
	return m_View;
}

Bool Creature::HasTarget() const noexcept {
	return !m_pTarget.expired();
}

Bool Creature::CanInteract() const noexcept {
	if (m_pTarget.expired())
		return false;
	return GetDistance(*m_pTarget.lock()) <= m_InteractionDistance;
}

#include <Objects/Actors/Actor.h>

Actor::Actor(const Health& health, const Armor& armor) noexcept :
	m_Health(health),
	m_Armor(armor)
{
}

void Actor::Drain(const Float& amount) noexcept {
	if (amount <= 0.0)
		return;

	const Float halfDamage = amount / 2.f;
	if (halfDamage <= m_Armor.GetAmount()) {
		m_Armor.TakeDamage(halfDamage);
		m_Health.Drain(halfDamage);
	}
	else {
		m_Health.Drain(amount - m_Armor.GetAmount());

		if (m_Armor.IsIntact())
			m_Armor.Break();
	}
}
void Actor::Heal(const Float& amount) noexcept {
	m_Health.Heal(amount);
}

void Actor::Kill() noexcept {
	m_Health.Kill();
}
void Actor::Revive() noexcept {
	m_Health.Revive();
}


Float Actor::GetHealth() const noexcept {
	return m_Health.GetAmount();
}
Float Actor::GetArmor() const noexcept {
	return m_Armor.GetAmount();
}

void Actor::SetHealth(const Float& amount) noexcept {
	m_Health.SetAmount(amount);
}
void Actor::SetArmor(const Float& amount) noexcept {
	m_Armor.SetAmount(amount);
}
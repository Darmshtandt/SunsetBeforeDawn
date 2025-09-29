#pragma once

#include <Objects/Actors/IAlive.h>
#include <Objects/Actors/IArmored.h>
#include <Objects/Actors/Health.h>
#include <Objects/Actors/Armor.h>

class Actor : public IAlive, public IArmored {
public:
	Actor(const Health& health, const Armor& armor) noexcept;
	Actor(const Actor&) noexcept = default;
	Actor(Actor&&) noexcept = default;
	virtual ~Actor() noexcept = default;

	void Drain(const Float& amount) noexcept override;
	void Heal(const Float& amount) noexcept override;

	void Kill() noexcept override;
	void Revive() noexcept override;

	[[nodiscard]] Float GetHealth() const noexcept override;
	[[nodiscard]] Float GetArmor() const noexcept override;

	void SetHealth(const Float& amount) noexcept override;
	void SetArmor(const Float& amount) noexcept override;

	Actor& operator = (const Actor&) noexcept = default;
	Actor& operator = (Actor&&) noexcept = default;

private:
	Health m_Health;
	Armor m_Armor;
};
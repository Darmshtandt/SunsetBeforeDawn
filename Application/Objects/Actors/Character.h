#pragma once

#include <Objects/GameObject.h>

class Health;
class Armor;

class Character : public GameObject {
protected:
	Character(const ClassID& id, const ObjectType& type, std::string factionName);

public:
	Character() = delete;
	Character(const Character&) noexcept = default;
	Character(Character&&) noexcept = default;
	~Character() noexcept override = default;

	Character& operator = (const Character&) noexcept = default;
	Character& operator = (Character&&) noexcept = default;

	[[nodiscard]] std::string GetFactionName() const noexcept;

protected:
	Health* m_pHealth;
	Armor* m_pArmor;

private:
	std::string m_FactionName;
};
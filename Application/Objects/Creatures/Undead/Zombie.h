#pragma once

#include <Objects/NPC/Creature.h>

class Zombie : public Creature {
public:
	Zombie() noexcept;
	Zombie(const Zombie&) noexcept = default;
	Zombie(Zombie&&) noexcept = default;
	~Zombie() noexcept override = default;

	Zombie& operator = (const Zombie&) noexcept = default;
	Zombie& operator = (Zombie&&) noexcept = default;

private:
	inline static ObjectRegistrar<Zombie> m_Registrar = { "Zombie" };
};
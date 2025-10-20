#pragma once

#include <World/Objects/Creature.h>

class Zombie : public Creature {
public:
	Zombie();
	Zombie(const Zombie&) noexcept = default;
	Zombie(Zombie&&) noexcept = default;
	~Zombie() noexcept override = default;

	Zombie& operator = (const Zombie&) noexcept = default;
	Zombie& operator = (Zombie&&) noexcept = default;

private:
	inline static ObjectRegistrar<Zombie> m_Registrar = { "Zombie" };
};
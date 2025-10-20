#pragma once

#include <World/Objects/Creature.h>

class Orc : public Creature {
public:
	Orc();
	Orc(const Orc&) noexcept = default;
	Orc(Orc&&) noexcept = default;
	~Orc() noexcept override = default;

	Orc& operator = (const Orc&) noexcept = default;
	Orc& operator = (Orc&&) noexcept = default;

private:
	inline static ObjectRegistrar<Orc> m_Registrar = { "Orc" };
};
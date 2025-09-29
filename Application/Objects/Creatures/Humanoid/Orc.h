#pragma once

#include <Objects/NPC/Creature.h>

class Orc : public Creature {
public:
	Orc() noexcept;
	Orc(const Orc&) noexcept = default;
	Orc(Orc&&) noexcept = default;
	~Orc() noexcept override = default;

	Orc& operator = (const Orc&) noexcept = default;
	Orc& operator = (Orc&&) noexcept = default;

private:
	inline static ObjectRegistrar<Orc> m_Registrar = { "Orc" };
};
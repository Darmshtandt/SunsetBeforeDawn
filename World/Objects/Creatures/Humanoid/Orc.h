#pragma once

#include <World/Objects/Creature.h>

class Orc : public Creature {
public:
	Orc();
	Orc(Orc&&) noexcept = default;
	~Orc() noexcept override = default;

	Orc& operator = (Orc&&) noexcept = default;

	[[nodiscard]] NotNull<IMovementController*> GetMovementController() const noexcept override;
	[[nodiscard]] NotNull<IAttackController*> GetAttackController() const noexcept override;
	[[nodiscard]] NotNull<ISenses*> GetSenses() const noexcept override;

private:
	std::unique_ptr<IMovementController> m_pMovementController;
	std::unique_ptr<IAttackController> m_pAttackController;
	std::unique_ptr<ISenses> m_pSenses;
	inline static ObjectRegistrar<Orc> m_Registrar = { "Orc" };
};
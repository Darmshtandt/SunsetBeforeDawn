#pragma once

#include <Engine/Combat/DamageLogic.h>
#include <World/Objects/Creature.h>
#include <World/Components/AIComponent.h>

class Zombie : public Creature {
public:
	Zombie();
	Zombie(const Zombie&) = delete;
	~Zombie() noexcept override = default;

	Zombie& operator = (const Zombie&) = delete;

	[[nodiscard]] NotNull<IMovementController*> GetMovementController() const noexcept override;
	[[nodiscard]] NotNull<IAttackController*> GetAttackController() const noexcept override;
	[[nodiscard]] NotNull<ISenses*> GetSenses() const noexcept override;

private:
	RayDamage m_RayDamage;
	std::unique_ptr<IMovementController> m_pMovementController;
	std::unique_ptr<IAttackController> m_pAttackController;
	std::unique_ptr<ISenses> m_pSenses;
	std::unique_ptr<IIntelligence> m_pAI;

	inline static ObjectRegistrar<Zombie> m_Registrar = { "Zombie" };
};
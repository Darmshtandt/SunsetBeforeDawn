#pragma once

#include <Engine/Combat/DamageLogic.h>
#include <Engine/AI/BehaviorTree.h>
#include <World/Objects/Creature.h>
#include <World/Components/AIComponent.h>

struct IMovementController;
struct IIntelligence;

class Zombie : public Creature {
public:
	Zombie();
	Zombie(const Zombie&) noexcept = default;
	Zombie(Zombie&&) noexcept = default;
	~Zombie() noexcept override = default;

	Zombie& operator = (const Zombie&) noexcept = default;
	Zombie& operator = (Zombie&&) noexcept = default;

	[[nodiscard]] IMovementController* GetMovementController() const noexcept;
	[[nodiscard]] IAttackController* GetAttackController() const noexcept;
	[[nodiscard]] ISenses* GetSenses() const noexcept;

private:
	RayDamage m_RayDamage;
	std::unique_ptr<IMovementController> m_pMovementController;
	std::unique_ptr<IAttackController> m_pAttackController;
	std::unique_ptr<ISenses> m_pSenses;
	std::unique_ptr<IIntelligence> m_pAI;

	inline static ObjectRegistrar<Zombie> m_Registrar = { "Zombie" };
};

class ZombieAI final : public IIntelligence {
public:
	explicit ZombieAI(NotNull<Zombie*> pOwner);

	void Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) override;
	void Tick(Float deltaTime) override;

private:
	BehaviorTree m_BehaviorTree;
	AIContext m_Context;
	Zombie* m_pOwner;
};
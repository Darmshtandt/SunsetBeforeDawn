#pragma once

#include <World/Objects/Character.h>
#include <World/Components/GameComponents.h>

class AIComponent;

class Creature : public Character {
protected:
	Creature(const ClassID& id, std::string factionName);

public:
	Creature() = delete;
	~Creature() noexcept override = default;

	[[nodiscard]] virtual NotNull<IMovementController*> GetMovementController() const noexcept = 0;
	[[nodiscard]] virtual NotNull<IAttackController*> GetAttackController() const noexcept = 0;
	[[nodiscard]] virtual NotNull<ISenses*> GetSenses() const noexcept = 0;

protected:
	Route3D* m_pRoute;
	Intent3D* m_pIntent;
	AIComponent* m_pAIComponent;
};

using CreaturePtr = std::shared_ptr<Creature>;
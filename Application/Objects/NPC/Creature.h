#pragma once

#include <Signal.h>
#include <Objects/Actors/Character.h>
#include <Objects/NPC/Behaviors/IBehavior.h>

class Creature : public Character {
public:
	struct View {
		Float Range = 0.f;
		Float Angle = 0.f;
	};

protected:
	Creature(const ClassID& id, std::string factionName) noexcept;

public:
	Creature() = delete;
	Creature(const Creature&) noexcept = default;
	Creature(Creature&&) noexcept = default;
	~Creature() noexcept override = default;

	Creature& operator = (const Creature&) noexcept = default;
	Creature& operator = (Creature&&) noexcept = default;

	void Update(const Float& deltaTime);

	void SetTarget(std::weak_ptr<Character> pTarget) noexcept;
	void SetBehavior(NotNull<IBehavior*> pBehavior) noexcept;
	void SetOnTargetFound(const Signal<Creature&>::Slot& slot);
	void SetOnTargetLost(const Signal<Creature&>::Slot& slot);

	[[nodiscard]] Float GetPersecutionDistance() const noexcept;
	[[nodiscard]] const std::weak_ptr<Character>& GetTarget() const noexcept;
	[[nodiscard]] const View& GetView() const noexcept;
	[[nodiscard]] Bool HasTarget() const noexcept;
	[[nodiscard]] Bool CanInteract() const noexcept;

protected:
	View m_View;
	Float m_InteractionDistance = 0.f;
	Float m_PersecutionDistance = 0.f;

private:
	Signal<Creature&> m_OnTargetFound;
	Signal<Creature&> m_OnTargetLost;
	std::weak_ptr<Character> m_pTarget;
	IBehavior* m_pBehavior = nullptr;
};

using CreaturePtr = std::shared_ptr<Creature>;
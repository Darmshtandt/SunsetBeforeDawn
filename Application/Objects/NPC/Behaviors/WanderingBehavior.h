#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Interval.h>
#include <Objects/NPC/Behaviors/IBehavior.h>

class WanderingBehavior final : public IBehavior {
public:
	enum class State : Byte {
		Idle,
		Walking
	};

public:
	WanderingBehavior() noexcept;
	WanderingBehavior(const WanderingBehavior&) noexcept = default;
	WanderingBehavior(WanderingBehavior&&) noexcept = default;
	~WanderingBehavior() noexcept override = default;

	WanderingBehavior& operator = (const WanderingBehavior&) noexcept = default;
	WanderingBehavior& operator = (WanderingBehavior&&) noexcept = default;

	void Update(MovableCreature& creature, const Float& deltaTime) override;

private:
	Nt::Float3D m_Direction;
	Interval m_ChangeStateInterval;
	State m_State = State::Idle;

	inline static BehaviorRegistrar<WanderingBehavior> m_Registrar { "Wandering" };

private:
	void _ChangeState() noexcept;
};
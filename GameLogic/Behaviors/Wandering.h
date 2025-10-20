#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Core/Interval.h>
#include <Engine/AI/IBehavior.h>

class Wandering final : public IBehavior {
public:
	enum class State : Byte {
		Idle,
		Walking
	};

public:
	Wandering() noexcept;
	Wandering(const Wandering&) noexcept = default;
	Wandering(Wandering&&) noexcept = default;
	~Wandering() noexcept override = default;

	Wandering& operator = (const Wandering&) noexcept = default;
	Wandering& operator = (Wandering&&) noexcept = default;

	void Update(Actor& actor, const Float& deltaTime) override;

private:
	Nt::Float3D m_Direction;
	Interval m_ChangeStateInterval;
	State m_State = State::Idle;

	inline static BehaviorRegistrar<Wandering> m_Registrar;

private:
	void _ChangeState() noexcept;
};

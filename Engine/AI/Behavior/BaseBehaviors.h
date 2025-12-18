#pragma once

#include <Engine/AI/Behavior/BehaviorNode.h>
#include <Core/Interval.h>
#include <Nt/Core/Math/Vector.h>

class BehaviorNodePatrol final : public IBehaviorNode {
private:
	enum class State : Byte {
		Stay,
		Walking
	};

public:
	BehaviorNodePatrol() noexcept;
	~BehaviorNodePatrol() noexcept override = default;

	[[nodiscard]] BNStatus Tick(const AIContext& context, Float deltaTime) override;
	IBehaviorNode* AddNode(const std::string& name) override;

private:
	Nt::Float3D m_Direction;
	Interval m_ChangeStateInterval;
	State m_State = State::Stay;

	inline static BNRegistrar<BehaviorNodePatrol> m_Registrar = { "Patrol" };

private:
	void _ChangeState() noexcept;
};

struct BehaviorNodePursue final : IBehaviorNode {
	~BehaviorNodePursue() noexcept override = default;

	[[nodiscard]] BNStatus Tick(const AIContext& context, Float deltaTime) override;
	IBehaviorNode* AddNode(const std::string& name) override;

private:
	inline static BNRegistrar<BehaviorNodePursue> m_Registrar = { "Pursue" };
};

struct BehaviorNodeAttack final : IBehaviorNode {
	~BehaviorNodeAttack() noexcept override = default;

	[[nodiscard]] BNStatus Tick(const AIContext& context, Float deltaTime) override;
	IBehaviorNode* AddNode(const std::string& name) override;

private:
	Nt::Timer m_Timer;
	inline static BNRegistrar<BehaviorNodeAttack> m_Registrar = { "Attack" };
};
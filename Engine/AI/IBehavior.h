#pragma once

#include <World/Components/GameComponents.h>

struct Actor final {
	const Transform3D* pTransform = nullptr;
	Movement3D* pMovement = nullptr;
	Route3D* pRoute = nullptr;
};

class IBehavior : public Identifier {
protected:
	explicit IBehavior(const ClassID& id) noexcept;

public:
	IBehavior() = delete;
	IBehavior(const IBehavior&) noexcept= default;
	IBehavior(IBehavior&&) noexcept = default;
	~IBehavior() noexcept override = default;

	IBehavior& operator = (const IBehavior&) noexcept = default;
	IBehavior& operator = (IBehavior&&) noexcept = default;

	virtual void Update(Actor& actor, const Float& deltaTime) = 0;
};

using BehaviorPtr = std::unique_ptr<IBehavior>;

#include <GameLogic/Behaviors/BehaviorRegistrar.h>
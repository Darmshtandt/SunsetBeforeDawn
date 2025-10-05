#pragma once

#include <RegistrarBase.h>

struct IAlive;
struct MovableCreature;

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

	virtual void Update(MovableCreature& creature, const Float& deltaTime) = 0;

	virtual void OnTargetAcquired(MovableCreature& movable, IAlive* target);
	virtual void OnTargetLost(MovableCreature& movable);
	virtual void OnDamaged(MovableCreature& movable, const Float& damage);
};

using BehaviorPtr = std::unique_ptr<IBehavior>;
using BehaviorFactory = FactoryBase<std::unique_ptr, IBehavior, BehaviorPtr(*)()>;

template <class _Ty>
using BehaviorRegistrar = RegistrarBase<std::unique_ptr, _Ty, BehaviorFactory>;
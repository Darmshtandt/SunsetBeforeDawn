#pragma once

#include <RegistrarBase.h>

struct IAlive;
class Creature;

class IBehavior : public Identifier {
public:
	explicit IBehavior(const ClassID& id) noexcept;
	IBehavior(const IBehavior&) noexcept= default;
	IBehavior(IBehavior&&) noexcept = default;
	~IBehavior() noexcept override = default;

	IBehavior& operator = (const IBehavior&) noexcept = default;
	IBehavior& operator = (IBehavior&&) noexcept = default;

	virtual void Update(Creature& npc, const Float& deltaTime) = 0;

	virtual void OnTargetAcquired(Creature& npc, IAlive* target);
	virtual void OnTargetLost(Creature& npc);
	virtual void OnDamaged(Creature& npc, const Float& damage);
};

using BehaviorPtr = std::unique_ptr<IBehavior>;
using BehaviorFactory = FactoryBase<std::unique_ptr, IBehavior, BehaviorPtr(*)()>;

template <class _Ty>
using BehaviorRegistrar = RegistrarBase<std::unique_ptr, _Ty, BehaviorFactory>;
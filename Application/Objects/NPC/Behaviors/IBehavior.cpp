#include <Objects/NPC/Behaviors/IBehavior.h>
#include <Objects/NPC/Creature.h>

IBehavior::IBehavior(const ClassID& id) noexcept :
	Identifier(id)
{
}

void IBehavior::OnTargetAcquired(MovableCreature& movable, IAlive* target)
{
}

void IBehavior::OnTargetLost(MovableCreature& movable)
{
}

void IBehavior::OnDamaged(MovableCreature& movable, const Float& damage)
{
}

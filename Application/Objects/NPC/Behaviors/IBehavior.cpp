#include <Objects/NPC/Behaviors/IBehavior.h>

IBehavior::IBehavior(const ClassID& id) noexcept :
	Identifier(id)
{
}

void IBehavior::OnTargetAcquired(Creature& npc, IAlive* target)
{
}

void IBehavior::OnTargetLost(Creature& npc)
{
}

void IBehavior::OnDamaged(Creature& npc, const Float& damage)
{
}

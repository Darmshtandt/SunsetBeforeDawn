#include <World/Objects/Creature.h>
#include <Engine/AI/AIContext.h>
#include <World/Components/AIComponent.h>

Creature::Creature(const ClassID& id, std::string factionName) :
	Character(id, ObjectType::Creature, std::move(factionName))
{
	m_pRoute = AddComponent<Route3D>();
	m_pIntent = AddComponent<Intent3D>();
	m_pAIComponent = AddComponent<AIComponent>();
	AddComponent<Blackboard>();
}
#include <World/Objects/Creatures/Faction.h>

Faction::Faction(std::string name) noexcept:
	m_Name(std::move(name))
{
}

Faction::Relation Faction::GetRelation(const Faction* other) const {
	RelationMap::const_iterator it = m_Relations.find(other);
	if (it != m_Relations.end())
		return it->second;
	return Relation::Neutral;
}

void Faction::SetRelation(const Faction* other, Relation relation) {
	m_Relations[other] = relation;
}
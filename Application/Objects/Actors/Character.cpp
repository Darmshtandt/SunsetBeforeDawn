#include <Objects/Actors/Character.h>
#include <Objects/Components/PhysicComponents.h>
#include <Objects/Components/GameComponents.h>

Character::Character(const ClassID& id, const ObjectType& type, std::string factionName) :
	GameObject(id, type),
	m_FactionName(std::move(factionName))
{
	AddComponent<Transform3D>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();
	m_pHealth = AddComponent<Health>();
	m_pArmor = AddComponent<Armor>();
}

std::string Character::GetFactionName() const noexcept {
	return m_FactionName;
}
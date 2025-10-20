#include <World/Objects/Character.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/GameComponents.h>

Character::Character(const ClassID& id, const ObjectType& type, std::string factionName) :
	GameObject(id, type),
	m_FactionName(std::move(factionName))
{
	AddComponent<Transform3D>();
	AddComponent<RigidBody>();
	AddComponent<Collider>();
	m_pHealth = AddComponent<Health>();
	m_pArmor = AddComponent<Armor>();
}

std::string Character::GetFactionName() const noexcept {
	return m_FactionName;
}
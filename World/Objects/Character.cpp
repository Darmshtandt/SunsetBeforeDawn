#include <World/Objects/Character.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/GameComponents.h>
#include <World/Components/RenderComponents.h>

Character::Character(const ClassID& id, const ObjectType& type, std::string factionName) :
	GameObject(id, type),
	m_FactionName(std::move(factionName))
{
	AddComponent<RigidBody>();
	m_pCollider = AddComponent<Collider>();

	m_pTransform = AddComponent<Transform3D>();
	m_pMovement = AddComponent<Movement3D>();

	m_pMesh = AddComponent<MeshRenderer>();
	m_pTexture = AddComponent<TextureRenderer>();

	m_pHealth = AddComponent<Health>();
	m_pArmor = AddComponent<Armor>();
}

std::string Character::GetFactionName() const noexcept {
	return m_FactionName;
}
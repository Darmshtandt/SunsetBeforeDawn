#include <World/Objects/Creature.h>
#include <World/Components/RenderComponents.h>

Creature::Creature(const ClassID& id, std::string factionName) :
	Character(id, ObjectType::Creature, std::move(factionName))
{
	m_pTransform = AddComponent<Transform3D>();
	m_pMesh = AddComponent<MeshRenderer>();
	m_pTexture = AddComponent<TextureRenderer>();
	m_pMovement = AddComponent<Movement3D>();
	m_pRoute = AddComponent<Route3D>();
	m_pIntent = AddComponent<Intent3D>();
}
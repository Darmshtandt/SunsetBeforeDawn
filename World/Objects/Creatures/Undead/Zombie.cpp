#include <World/Objects/Creatures/Undead/Zombie.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/RenderComponents.h>
#include <Core/StdH.h>

Zombie::Zombie() :
	Creature(Class<Zombie>::ID(), "Undead")
{
	m_pMesh->Set(MESH_CHARASTER);
	m_pTexture->Set(TEXTURE_TEST);
	m_pIntent->SetView({ 20.f, 360.f });
	m_pRoute->InteractionDistance = 4.f;
	m_pMovement->Speed = 2.5f;

	GetComponent<Collider>()->SetShape(m_pMesh->Get()->GetShape());
}
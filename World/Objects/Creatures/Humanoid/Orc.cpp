#include <World/Objects/Creatures/Humanoid/Orc.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/RenderComponents.h>
#include <Core/StdH.h>

Orc::Orc() :
	Creature(Class<Orc>::ID(), "Humanoid")
{
	m_pMesh->Set(MESH_CHARACTER);
	m_pTexture->Set(TEXTURE_TEST);
	m_pIntent->SetView({ 10.f, 360.f });
	m_pRoute->InteractionDistance = 4.f;
	m_pMovement->Speed = 20.0f;

	GetComponent<Collider>()->SetShape(m_pMesh->Get()->GetShape());
}
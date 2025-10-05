#include <Objects/Creatures/Humanoid/Orc.h>
#include <Objects/Components/PhysicComponents.h>
#include <Objects/Components/RenderComponents.h>
#include <StdH.h>

Orc::Orc() :
	Creature(Class<Orc>::ID(), "Humanoid")
{
	m_View.RangeSq = 100.f;
	m_View.Angle = 360.f;

	m_InteractionDistSq = 4.f;
	m_PersecutionDistSq = 100.f;

	m_pMesh->Handler.Set(MESH_CHARASTER);
	m_pTexture->Handler.Set(TEXTURE_TEST);
	m_pMovement->Speed = 4.0f;

	GetComponent<ColliderComponent>()->Collider
		.SetShape(m_pMesh->Handler.Get()->GetShape());
}
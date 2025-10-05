#include <Objects/Creatures/Undead/Zombie.h>
#include <Objects/Components/PhysicComponents.h>
#include <Objects/Components/RenderComponents.h>
#include <StdH.h>

Zombie::Zombie() :
	Creature(Class<Zombie>::ID(), "Undead")
{
	m_View.RangeSq = 400.f;
	m_View.Angle = 360.f;

	m_InteractionDistSq = 4.f;
	m_PersecutionDistSq = 400.f;

	m_pMesh->Handler.Set(MESH_CHARASTER);
	m_pTexture->Handler.Set(TEXTURE_TEST);
	m_pMovement->Speed = 2.5f;

	GetComponent<ColliderComponent>()->Collider
		.SetShape(m_pMesh->Handler.Get()->GetShape());
}
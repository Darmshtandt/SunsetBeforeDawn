#include <Objects/Creatures/Undead/Zombie.h>
#include <StdH.h>

Zombie::Zombie() noexcept :
	Creature(Class<Zombie>::ID(), "Undead")
{
	m_View.Range = 20.f;
	m_View.Angle = 360.f;

	m_InteractionDistance = 2.f;
	m_PersecutionDistance = 20.f;

	SetSpeed(2.5f);
	SetMesh(MESH_CHARASTER);
	SetTexture(TEXTURE_TEST);
	GetBodyPtr()->SetColliderShape(GetMesh().Get()->GetShape());
}
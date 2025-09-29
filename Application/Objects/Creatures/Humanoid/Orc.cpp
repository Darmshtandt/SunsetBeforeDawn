#include <Objects/Creatures/Humanoid/Orc.h>
#include <StdH.h>

Orc::Orc() noexcept :
	Creature(Class<Orc>::ID(), "Humanoid")
{
	m_View.Range = 10.f;
	m_View.Angle = 360.f;

	m_InteractionDistance = 2.f;
	m_PersecutionDistance = 10.f;

	SetSpeed(4.f);
	SetMesh(MESH_CHARASTER);
	SetTexture(TEXTURE_TEST);
	GetBodyPtr()->SetColliderShape(GetMesh().Get()->GetShape());
}
#include <World/Objects/Creatures/Humanoid/Orc.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/RenderComponents.h>
#include <Core/StdH.h>
#include <Engine/AI/AISenses.h>
#include <GameLogic/Controllers/AI/AIAttackController.h>
#include <GameLogic/Controllers/AI/AIMovementController.h>

Orc::Orc() :
	Creature(Class<Orc>::ID(), "Humanoid")
{
	m_pMovementController = std::make_unique<AIMovementController>(*this);
	m_pAttackController = std::make_unique<AIAttackController>(*this);
	m_pSenses = std::make_unique<AISenses>(*this, m_pIntent->GetView().Range, 2000);

	m_pMesh->Set(MESH_CHARACTER);
	m_pTexture->Set(TEXTURE_TEST);
	m_pIntent->SetView({ 10.f, 360.f });
	m_pRoute->InteractionDistance = 4.f;
	m_pMovement->Speed = 20.0f;

	GetComponent<Collider>()->SetShape(m_pMesh->Get()->GetShape());
}

NotNull<IMovementController*> Orc::GetMovementController() const noexcept {
	return m_pMovementController.get();
}
NotNull<IAttackController*> Orc::GetAttackController() const noexcept {
	return m_pAttackController.get();
}
NotNull<ISenses*> Orc::GetSenses() const noexcept {
	return m_pSenses.get();
}
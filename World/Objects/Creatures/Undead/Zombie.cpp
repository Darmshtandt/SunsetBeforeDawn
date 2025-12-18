#include <World/Objects/Creatures/Undead/Zombie.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/RenderComponents.h>
#include <World/Components/CombatComponents.h>
#include <GameLogic/Controllers/AI/AIAttackController.h>
#include <GameLogic/Controllers/AI/AIMovementController.h>
#include <Engine/AI/AISenses.h>
#include <Core/StdH.h>

#include <Engine/AI/EnemyAI.h>

Zombie::Zombie() :
	Creature(Class<Zombie>::ID(), "Undead")
{
	m_pMesh->Set(MESH_CHARACTER);
	m_pTexture->Set(TEXTURE_TEST);
	m_pIntent->SetView({ 20.f, 360.f });
	m_pRoute->InteractionDistance = 4.f;
	m_pMovement->Speed = 10.5f;

	const Nt::Shape colliderShape = Nt::ResourceManager::Instance().Get<Nt::Mesh>(MESH_CUBE)->GetShape();
	m_pCollider->SetShape(colliderShape);

	const Nt::BoundingBox meshBox = Nt::CalculateBoundingBox(m_pMesh->Get()->GetShape());
	const Nt::BoundingBox colliderBox = m_pCollider->GetBoundingBox();

	const Nt::Float3D meshSize = meshBox.Max - meshBox.Min;
	const Nt::Float3D colliderSize = colliderBox.Max - colliderBox.Min;
	m_pCollider->LocalTransform().Resize(meshSize - colliderSize);

	const Nt::Float3D origin(0.f, m_pCollider->LocalTransform().Size().y * 0.5f, 0.f);
	m_pCollider->LocalTransform().Translate(origin);

	m_RayDamage.Amount = 10.f;
	m_RayDamage.Origin.y = m_pTransform->Size().y * 0.75f;
	m_RayDamage.Distance = m_pRoute->InteractionDistance;
	AddComponent<Combat>()->SetDamageType(&m_RayDamage);

	m_pMovementController = std::make_unique<AIMovementController>(*this);
	m_pAttackController = std::make_unique<AIAttackController>(*this);
	m_pSenses = std::make_unique<AISenses>(*this, m_pIntent->GetView().Range, 2000);

	m_pAI = std::make_unique<EnemyAI>(this);
	m_pAIComponent->SetIntelligence(m_pAI.get());
}

NotNull<IMovementController*> Zombie::GetMovementController() const noexcept {
	return m_pMovementController.get();
}
NotNull<IAttackController*> Zombie::GetAttackController() const noexcept {
	return m_pAttackController.get();
}
NotNull<ISenses*> Zombie::GetSenses() const noexcept {
	return m_pSenses.get();
}
#include <World/Objects/Creatures/Undead/Zombie.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/RenderComponents.h>
#include <World/Components/CombatComponents.h>
#include <Core/StdH.h>
#include <Engine/AI/AISenses.h>
#include <GameLogic/Controllers/AI/AIMovementController.h>
#include <GameLogic/Controllers/AI/AIAttackController.h>

Zombie::Zombie() :
	Creature(Class<Zombie>::ID(), "Undead")
{
	m_pMesh->Set(MESH_CHARACTER);
	m_pTexture->Set(TEXTURE_TEST);
	m_pIntent->SetView({ 20.f, 360.f });
	m_pRoute->InteractionDistance = 4.f;
	m_pMovement->Speed = 10.5f;

	const Nt::Float3D halfSize = m_pTransform->Size() / 2.f;

	m_RayDamage.Amount = 10.f;
	m_RayDamage.Origin.y = m_pTransform->Size().y * 0.75f;
	m_RayDamage.Distance = m_pRoute->InteractionDistance;

	GetComponent<Collider>()->SetShape(m_pMesh->Get()->GetShape());
	AddComponent<Combat>()->SetDamageType(&m_RayDamage);

	m_pMovementController = std::make_unique<AIMovementController>(*this);
	m_pAttackController = std::make_unique<AIAttackController>(*this);
	m_pSenses = std::make_unique<AISenses>(*this, 20.f, 2000);

	m_pAI = std::make_unique<ZombieAI>(this);
	AddComponent<AIComponent>()->SetIntelligence(m_pAI.get());
}

IMovementController* Zombie::GetMovementController() const noexcept {
	return m_pMovementController.get();
}

IAttackController* Zombie::GetAttackController() const noexcept {
	return m_pAttackController.get();
}

ISenses* Zombie::GetSenses() const noexcept {
	return m_pSenses.get();
}


ZombieAI::ZombieAI(NotNull<Zombie*> pOwner) :
	m_pOwner(pOwner),
	m_pIntent(RequireNotNull(pOwner->GetComponent<Intent3D>())),
	m_pRoute(RequireNotNull(pOwner->GetComponent<Route3D>())),
	m_Movement(RequireNotNull(pOwner->GetComponent<Movement3D>()))
{
	m_Context.pMovementController = pOwner->GetMovementController();
	m_Context.pAttackController = pOwner->GetAttackController();
	m_Context.pSenses = pOwner->GetSenses();

	std::stack<IBehaviorNode*> bnStack;
	bnStack.push(m_BehaviorTree.SetRootNode<BehaviorSelectorNode>());

	bnStack.push(bnStack.top()->AddNode("Sequence"));
	bnStack.top()->AddNode("Pursue");
	bnStack.top()->AddNode("Attack");
	bnStack.pop();

	bnStack.top()->AddNode("Patrol");
}

void ZombieAI::Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) {
	RequireNotNull(m_Context.pSenses)->Scan(pOverlapper, deltaTime);
}

void ZombieAI::Tick(Float deltaTime) {
	if (!m_pIntent->HasTarget()) {
		const std::vector<LivingPawn>& visiblePawns = m_Context.pSenses->GetVisiblePawns();
		if (!visiblePawns.empty()) {
			const LivingPawn& pawn = visiblePawns.back();

			const Intent3D::Target target = {
				.pTransform = pawn.pTransform,
				.pHealth = pawn.pHealth,
				.pArmor = pawn.pArmor,
				.pObject = pawn.pObject
			};
			m_pIntent->SetTarget(target);
		}
	}
	else {
		m_pRoute->Clear();
		m_pRoute->PushMarker({
			m_pIntent->GetTarget().pTransform->Position(),
			m_Movement->Speed
			});
	}

	m_BehaviorTree.Tick(m_Context, deltaTime);
}
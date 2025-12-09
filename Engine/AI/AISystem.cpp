#include <Engine/AI/AISystem.h>
#include <GameLogic/Controllers/AI/AIMovementController.h>
#include <World/Components/AIComponent.h>
#include <GameLogic/AI/DefaultTargetScanner.h>
#include <GameLogic/AI/DefaultBehaviorSelector.h>

AISystem::AISystem() {
	m_ScanInterval.DelayMs = 2000;

	SetTargetScanner<DefaultTargetScanner>();
	SetBehaviorSelector<DefaultBehaviorSelector>();
}

void AISystem::RegisterObject(GameObject& object) {

	Pawn pawn = { };
	pawn.This.pTransform = object.GetComponent<Transform3D>();
	pawn.This.pMovement = object.GetComponent<Movement3D>();
	pawn.This.pRoute = object.GetComponent<Route3D>();
	pawn.This.pObject = &object;
	pawn.pIntent3D = object.GetComponent<Intent3D>();

	if (pawn.This.pMovement && pawn.This.pRoute && pawn.pIntent3D) {
		m_pBehaviorSelector->OnSpawned(pawn);
		m_Pawns.emplace_back(pawn);
	}

	Target target = { };
	target.pTransform = pawn.This.pTransform;
	target.pHealth = object.GetComponent<Health>();
	target.pArmor = object.GetComponent<Armor>();
	target.pObject = pawn.This.pObject;

	if (!target.pHealth && !target.pArmor)
		return;

	m_Targets.emplace_back(target);
}

void AISystem::UnregisterObject(const GameObject& object) {
	std::erase_if(m_Pawns, [&object] (const Pawn& pawn) noexcept {
		return pawn.This.pObject == &object;
		});
	std::erase_if(m_Targets, [&object] (const Target& target) noexcept {
		return target.pObject == &object;
		});
}

void AISystem::Update(const Float& deltaTime) {
	const Bool scanIsActive = m_ScanInterval.IsDone();
	for (Pawn& pawn : m_Pawns) {
		if (scanIsActive) {
			const Intent3D::View& view = pawn.pIntent3D->GetView();
			_UpdatePawnTarget(pawn, view);
		}

		if (pawn.pIntent3D->HasTarget() && !pawn.This.pRoute->HasMarker())
			m_pBehaviorSelector->OnTargetReached(pawn);

		IBehavior* pBehavior = pawn.pIntent3D->GetBehaviorPtr();
		if (pBehavior != nullptr)
			pBehavior->Update(pawn.This, deltaTime);
	}
}

void AISystem::_UpdatePawnTarget(Pawn& pawn, const Intent3D::View& view) {
	if (!pawn.pIntent3D->HasTarget()) {
		if (m_pTargetScanner->Scan(pawn, m_Targets, view))
			m_pBehaviorSelector->OnTargetFound(pawn);
		return;
	}

	const Bool isInView = 
		m_pTargetScanner->IsVisibleTarget(pawn, view, pawn.pIntent3D->GetTarget());

	if (isInView && !pawn.IsViewTarget) {
		m_pBehaviorSelector->OnEnterView(pawn);
	}
	else if (isInView && pawn.IsViewTarget) {
		m_pBehaviorSelector->OnInView(pawn);
	}
	else if (!isInView && pawn.IsViewTarget) {
		pawn.MemoryTimer.Restart();
		m_pBehaviorSelector->OnExitView(pawn);
	}
	else {
		const uInt elapsedTime = uInt(pawn.MemoryTimer.GetElapsedTimeMs());
		const uInt memoryTime = pawn.pIntent3D->GetMemoryTimeMs();

		if (elapsedTime >= memoryTime) {
			pawn.pIntent3D->SetTarget({ });
			m_pBehaviorSelector->OnTargetLost(pawn);
		}
	}

	pawn.IsViewTarget = isInView;
}


AISystemV2::AISystemV2(NotNull<IPhysicsOverlapper*> pOverlapper) noexcept :
	m_pPhysicsOverlapper(pOverlapper)
{
}

void AISystemV2::RegisterObject(GameObject& object) {
	AIObject ai = { };
	ai.pAI = object.GetComponent<AIComponent>();
	if (ai.pAI == nullptr)
		return;

	ai.pObject = &object;
	m_AIs.emplace_back(ai);
}

void AISystemV2::UnregisterObject(const GameObject& object) {
	std::erase_if(m_AIs, [&object] (const AIObject& ai) noexcept {
		return ai.pObject == &object;
		});
}

void AISystemV2::Update(const Float& deltaTime) {
	for (AIObject& ai : m_AIs) {
		ai.pAI->GetAI()->Scan(m_pPhysicsOverlapper, deltaTime);
		ai.pAI->GetAI()->Tick(deltaTime);
	}
}
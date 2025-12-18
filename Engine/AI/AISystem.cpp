#include <Engine/AI/AISystem.h>
#include <World/Components/AIComponent.h>

AISystem::AISystem(NotNull<IPhysicsOverlapper*> pOverlapper) noexcept :
	m_pPhysicsOverlapper(pOverlapper)
{
}

void AISystem::RegisterObject(GameObject& object) {
	AIComponent* pAIComponent = object.GetComponent<AIComponent>();
	if (pAIComponent == nullptr || pAIComponent->GetAI() == nullptr)
		return;

	AIObject ai = { };
	ai.pAI = pAIComponent->GetAI();
	ai.pIntent = object.GetComponent<Intent3D>();
	ai.pRoute = object.GetComponent<Route3D>();
	ai.pMovement = object.GetComponent<Movement3D>();
	ai.pObject = &object;
	m_AIs.emplace_back(ai);
}

void AISystem::UnregisterObject(const GameObject& object) {
	std::erase_if(m_AIs, [&object] (const AIObject& ai) noexcept {
		return ai.pObject == &object;
		});
}

void AISystem::Update(const Float& deltaTime) {
	for (AIObject& ai : m_AIs) {
		ai.pAI->Scan(m_pPhysicsOverlapper, deltaTime);

		if (ai.pIntent && ai.pRoute) {
			if (!ai.pIntent->HasTarget()) {
				const std::vector<LivingPawn>& visiblePawns = ai.pAI->GetContext().pSenses->GetVisiblePawns();
				if (!visiblePawns.empty())
					ai.pIntent->SetTarget(visiblePawns.back());
			}
			else {
				ai.pRoute->Clear();
				ai.pRoute->PushMarker({
					ai.pIntent->GetTarget().pTransform->Position(),
					ai.pMovement->Speed
					});
			}
		}

		ai.pAI->Tick(deltaTime);
	}
}
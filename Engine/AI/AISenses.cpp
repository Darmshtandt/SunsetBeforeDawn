#include <World/Components/GameComponents.h>
#include <World/Components/PhysicComponents.h>
#include <Engine/AI/AISenses.h>

AISenses::AISenses(GameObject& object, Float viewRadius, uLLong cooldownMs) :
	m_pTransform(RequireNotNull(object.GetComponent<Transform3D>())),
	m_pObject(&object),
	m_ViewRadius(viewRadius),
	m_CooldownMs(cooldownMs)
{
}

void AISenses::Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) {
	if (m_Timer.GetElapsedTimeMs() < m_CooldownMs)
		return;
	m_Timer.Restart();

	const std::vector<PhysicObject> allObjects = pOverlapper->OverlapSphere(
		m_pTransform->Position(), m_ViewRadius, 0);

	m_VisiblePawns.clear();
	for (const PhysicObject& object : allObjects) {
		if (object.pObject == m_pObject)
			continue;

		const LivingPawn pawn = {
			.pTransform = object.pTransform,
			.pHealth = object.pObject->GetComponent<Health>(),
			.pArmor = object.pObject->GetComponent<Armor>(),
			.pObject = object.pObject
		};

		if (!pawn.pHealth || !pawn.pArmor)
			continue;

		m_VisiblePawns.emplace_back(pawn);
	}
}

const std::vector<LivingPawn>& AISenses::GetVisiblePawns() const noexcept {
	return m_VisiblePawns;
}
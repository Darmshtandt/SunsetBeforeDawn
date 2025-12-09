#include <Engine/Combat/CombatSystem.h>
#include <Engine/Physics/Base/PhysicsInterfaces.h>
#include <World/Objects/Traps/Bomb.h>

CombatSystem::CombatSystem(NotNull<IPhysicsOverlapper*> pOverlapper) noexcept :
	m_pPhysicsOverlapper(pOverlapper)
{
}

void CombatSystem::RegisterObject(GameObject& object) {
	CombatPawn pawn = { };
	pawn.pCombat = object.GetComponent<Combat>();
	pawn.pObject = &object;
	if (pawn.pCombat == nullptr)
		return;

	pawn.pCombat->SetCombatDispatcher(this);
	m_CombatPawns.emplace_back(pawn);
}

void CombatSystem::UnregisterObject(const GameObject& object) {
	std::erase_if(
		m_CombatPawns,
		[&object] (const CombatPawn& pawn) {
			return pawn.pObject == &object;
		});
}

void CombatSystem::Dispatch(const DamageCommand& command) {
	std::vector<PhysicObject> physicObjects =
		command.pDamage->FindTargets(*command.pOwnerTransform, m_pPhysicsOverlapper);

	std::vector<GameObject*> objectPts;
	for (PhysicObject& object : physicObjects) {
		if (object.pObject == command.pOwner)
			continue;
		if (!object.pObject->HasComponent<Health>())
			continue;

		objectPts.emplace_back(object.pObject);
	}

	command.pDamage->Apply(objectPts);
}

void CombatSystem::Update(const Float& deltaTime) {

}
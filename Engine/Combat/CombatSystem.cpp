#include <Engine/Combat/CombatSystem.h>

void CombatSystem::RegisterObject(GameObject& object) {
	CombatPawn pawn;
	pawn.pCombat = object.GetComponent<Combat>();
	pawn.pObject = &object;

	if (pawn.pCombat == nullptr)
		return;

	m_CombatPawns.emplace_back(pawn);
}

void CombatSystem::UnregisterObject(const GameObject& object) {
	std::erase_if(
		m_CombatPawns,
		[&object] (const CombatPawn& pawn) {
			return pawn.pObject == &object;
		});
}

void CombatSystem::Update() {

}
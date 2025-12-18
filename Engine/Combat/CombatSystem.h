#pragma once

#include <World/Components/CombatComponents.h>
#include <Engine/ISystem.h>

struct IPhysicsOverlapper;

class CombatSystem final : public ICombatDispatcher, public ISystem {
public:
	struct CombatPawn final {
		Combat* pCombat = nullptr;
		const GameObject* pObject = nullptr;
	};

public:
	explicit CombatSystem(NotNull<IPhysicsOverlapper*> pOverlapper) noexcept;
	CombatSystem(const CombatSystem&) = delete;
	CombatSystem(CombatSystem&&) noexcept = default;
	~CombatSystem() noexcept override = default;

	CombatSystem& operator = (const CombatSystem&) = delete;
	CombatSystem& operator = (CombatSystem&&) noexcept = default;

	void RegisterObject(GameObject& object) override;
	void UnregisterObject(const GameObject& object) override;

	void Dispatch(const DamageCommand& command) override;

	void Update(const Float& deltaTime) override;

private:
	IPhysicsOverlapper* m_pPhysicsOverlapper;
	std::vector<CombatPawn> m_CombatPawns;
};
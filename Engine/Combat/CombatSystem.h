#pragma once

#include <World/Entities.h>
#include <World/Components/CombatComponents.h>

class CombatSystem final {
public:
	CombatSystem() noexcept = default;
	CombatSystem(const CombatSystem&) = delete;
	CombatSystem(CombatSystem&&) noexcept = default;
	~CombatSystem() noexcept = default;

	CombatSystem& operator = (const CombatSystem&) = delete;
	CombatSystem& operator = (CombatSystem&&) noexcept = default;

	void RegisterObject(GameObject& object);
	void UnregisterObject(const GameObject& object);

	void Update();

private:
	std::vector<CombatPawn> m_CombatPawns;
};
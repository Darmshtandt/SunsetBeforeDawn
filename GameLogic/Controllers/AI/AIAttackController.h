#pragma once

#include <World/Components/CombatComponents.h>
#include <Engine/AI/AIContext.h>

class AIAttackController : public IAttackController {
public:
	explicit AIAttackController(GameObject& object) noexcept :
		m_pCombat(RequireNotNull(object.GetComponent<Combat>())),
		m_Intent(RequireNotNull(object.GetComponent<Intent3D>()))
	{		
	}
	~AIAttackController() noexcept override = default;

	[[nodiscard]] Bool HasTarget() const noexcept override {
		return m_Intent->HasTarget();
	}

private:
	Combat* m_pCombat;
	Intent3D* m_Intent;
};

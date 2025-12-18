#pragma once

#include <World/Components/CombatComponents.h>
#include <Engine/AI/AIContext.h>

class AIAttackController : public IAttackController {
public:
	explicit AIAttackController(GameObject& object) :
		m_pCombat(RequireNotNull(object.GetComponent<Combat>())),
		m_Intent(RequireNotNull(object.GetComponent<Intent3D>())),
		m_pTransform(RequireNotNull(object.GetComponent<Transform3D>()))
	{		
	}
	~AIAttackController() noexcept override = default;

	void Attack() override {
		m_pCombat->PerformAttack(m_pTransform->Position(), m_pTransform->Rotation());
	}

	[[nodiscard]] uLLong GetCooldown() const noexcept override {
		return m_Cooldown;
	}
	[[nodiscard]] Bool HasTarget() const noexcept override {
		return m_Intent->HasTarget();
	}

private:
	uLLong m_Cooldown = 1500;
	Transform3D* m_pTransform;
	Combat* m_pCombat;
	Intent3D* m_Intent;
};

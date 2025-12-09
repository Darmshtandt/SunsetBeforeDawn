#pragma once

#include <Engine/AI/AIContext.h>

struct LivingPawn;
class GameObject;

class AISenses : public ISenses {
public:
	AISenses(GameObject& object, Float viewRadius, uLLong cooldownMs);
	~AISenses() noexcept override = default;

	void Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) override;

	[[nodiscard]] const std::vector<LivingPawn>& GetVisiblePawns() const noexcept override;

public:
	std::vector<LivingPawn> m_VisiblePawns;
	Transform3D* m_pTransform;
	GameObject* m_pObject;
	Nt::Timer m_Timer;
	uLLong m_CooldownMs;
	Float m_ViewRadius;
};
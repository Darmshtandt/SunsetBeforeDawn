#pragma once

#include <World/Components/IComponent.h>

struct IIntelligence {
	virtual ~IIntelligence() noexcept = default;

	virtual void Scan(NotNull<IPhysicsOverlapper*> pOverlapper, Float deltaTime) = 0;
	virtual void Tick(Float deltaTime) = 0;
};

class AIComponent final : public IComponent {
public:
	explicit AIComponent(NotNull<GameObject*> pOwner) :
		IComponent(pOwner, Class<AIComponent>::ID())
	{
	}
	~AIComponent() noexcept override = default;

	[[nodiscard]] IIntelligence* GetAI() noexcept {
		return m_pAI;
	}

	void SetIntelligence(IIntelligence* pAI) noexcept {
		m_pAI = pAI;
	}

private:
	IIntelligence* m_pAI = nullptr;
};
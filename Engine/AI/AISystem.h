#pragma once

#include <World/Objects/GameObject.h>
#include <Engine/ISystem.h>
#include <World/Components/GameComponents.h>

struct IIntelligence;
struct IPhysicsOverlapper;

class AISystem final : public ISystem {
public:
	struct AIObject final {
		IIntelligence* pAI = nullptr;
		Intent3D* pIntent = nullptr;
		Route3D* pRoute = nullptr;
		Movement3D* pMovement = nullptr;
		GameObject* pObject = nullptr;
	};

public:
	explicit AISystem(NotNull<IPhysicsOverlapper*> pOverlapper) noexcept;
	AISystem(const AISystem&) noexcept = default;
	AISystem(AISystem&&) noexcept = default;
	~AISystem() noexcept override = default;

	AISystem& operator = (const AISystem&) noexcept = default;
	AISystem& operator = (AISystem&&) noexcept = default;

	void RegisterObject(GameObject& object) override;
	void UnregisterObject(const GameObject& object) override;

	void Update(const Float& deltaTime) override;

private:
	IPhysicsOverlapper* m_pPhysicsOverlapper;
	std::vector<AIObject> m_AIs;
};
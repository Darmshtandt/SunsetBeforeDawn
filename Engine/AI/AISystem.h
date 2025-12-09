#pragma once

#include <World/Objects/GameObject.h>
#include <Engine/AI/ITargetScanner.h>
#include <Engine/AI/IBehaviorSelector.h>
#include <Engine/ISystem.h>
#include <Nt/Core/Log.h>

class AISystem final : public ISystem {
public:
	using Target = Intent3D::Target;

	struct Interval final {
		[[nodiscard]] Bool IsDone() const noexcept {
			if (Timer.GetElapsedTimeMs() < DelayMs)
				return false;

			Timer.Restart();
			return true;
		}

		mutable Nt::Timer Timer;
		uInt DelayMs = 0.f;
	};

public:
	AISystem();
	AISystem(const AISystem&) noexcept = default;
	AISystem(AISystem&&) noexcept = default;
	~AISystem() noexcept override = default;

	AISystem& operator = (const AISystem&) noexcept = default;
	AISystem& operator = (AISystem&&) noexcept = default;

	void RegisterObject(GameObject& object) override;
	void UnregisterObject(const GameObject& object) override;

	void Update(const Float& deltaTime) override;

	template <class _Ty> requires std::is_base_of_v<ITargetScanner, _Ty>
	void SetTargetScanner() {
		m_pTargetScanner = std::make_unique<_Ty>();
	}

	template <class _Ty> requires std::is_base_of_v<IBehaviorSelector, _Ty>
	void SetBehaviorSelector() {
		m_pBehaviorSelector = std::make_unique<_Ty>();
	}

private:
	std::unique_ptr<ITargetScanner> m_pTargetScanner;
	std::unique_ptr<IBehaviorSelector> m_pBehaviorSelector;
	std::vector<Pawn> m_Pawns;
	std::vector<Target> m_Targets;
	Interval m_ScanInterval;

private:
	void _UpdatePawnTarget(Pawn& pawn, const Intent3D::View& view);
};

namespace Nt {
	class Log;
}


struct IPhysicsOverlapper;
class AIComponent;

class AISystemV2 final : public ISystem {
public:
	struct AIObject final {
		AIComponent* pAI = nullptr;
		GameObject* pObject = nullptr;
	};

public:
	AISystemV2(NotNull<IPhysicsOverlapper*> pOverlapper) noexcept;
	AISystemV2(const AISystemV2&) noexcept = default;
	AISystemV2(AISystemV2&&) noexcept = default;
	~AISystemV2() noexcept override = default;

	AISystemV2& operator = (const AISystemV2&) noexcept = default;
	AISystemV2& operator = (AISystemV2&&) noexcept = default;

	void RegisterObject(GameObject& object) override;
	void UnregisterObject(const GameObject& object) override;

	void Update(const Float& deltaTime) override;

	template <class _Ty> requires std::is_base_of_v<ITargetScanner, _Ty>
	void SetTargetScanner() {
		Nt::Log::Warning("AISystemV2 does not has SetTargetScanner()");
	}

	template <class _Ty> requires std::is_base_of_v<IBehaviorSelector, _Ty>
	void SetBehaviorSelector() {
		Nt::Log::Warning("AISystemV2 does not has SetBehaviorSelector()");
	}

private:
	IPhysicsOverlapper* m_pPhysicsOverlapper;
	std::vector<AIObject> m_AIs;
};
#pragma once

#include <World/Objects/GameObject.h>
#include <Engine/AI/ITargetScanner.h>
#include <Engine/AI/IBehaviorSelector.h>

class AISystem final {
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
	~AISystem() noexcept = default;

	AISystem& operator = (const AISystem&) noexcept = default;
	AISystem& operator = (AISystem&&) noexcept = default;

	void RegisterObject(GameObject& object);
	void UnregisterObject(const GameObject& object);

	void Update(const Float& deltaTime);

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
#pragma once

#include <Engine/AI/IBehavior.h>

class BehaviorLocator final {
public:
	template <class _Ty> requires std::is_base_of_v<IBehavior, _Ty>
	static void Register() {
		constexpr ClassID id = Class<_Ty>::ID();
		if (!m_Map.contains(id))
			m_Map[id] = std::make_unique<_Ty>();
	}

	template <class _Ty> requires std::is_base_of_v<IBehavior, _Ty>
	static IBehavior* Get() noexcept {
		constexpr ClassID id = Class<_Ty>::ID();
		if (!m_Map.contains(id))
			return nullptr;
		return m_Map[id].get();
	}

private:
	inline static std::unordered_map<ClassID, BehaviorPtr> m_Map;
};
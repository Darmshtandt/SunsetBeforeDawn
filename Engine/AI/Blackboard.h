#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Nt/Core/Timer.h>
#include <unordered_map>
#include <variant>

namespace Temporary {
	class Blackboard final {
	public:
		using Value = std::variant<
			Int, uInt, Bool, Float, std::string,
			Nt::Float2D, Nt::Float3D, Nt::Float4D,
			Nt::Timer, GameObject*>;

		[[nodiscard]]
		Bool Contains(const std::string& name) const noexcept {
			return m_Map.contains(name);
		}
		void Erase(const std::string& name) noexcept {
			m_Map.erase(name);
		}
		void Clear() noexcept {
			m_Map.clear();
		}

		template <class _Ty>
		void Set(const std::string& name, _Ty&& value) {
			m_Map[name] = std::forward<_Ty>(value);
		}

		template <class _Ty>
		std::optional<_Ty> Get(const std::string& name) noexcept {
			auto it = m_Map.find(name);
			if (it == m_Map.cend())
				return std::nullopt;

			std::add_pointer_t<_Ty> pValue = std::get_if<_Ty>(&it->second);
			if (pValue == nullptr)
				return std::nullopt;

			return *pValue;
		}

	private:
		std::unordered_map<std::string, Value> m_Map;
	};
}
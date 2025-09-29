#pragma once

#include <functional>
#include <vector>

template <class... _Args>
class Signal {
public:
	using Slot = std::function<void(_Args...)>;

public:
	Signal() noexcept = default;

	void Connect(const Slot& slot) {
		m_Slots.push_back(slot);
	}

	void DisconnectAll() noexcept {
		m_Slots.clear();
	}

	void Emmit(_Args... args) {
		for (const Slot& slot : m_Slots)
			slot(args...);
	}

private:
	std::vector<Slot> m_Slots;
};
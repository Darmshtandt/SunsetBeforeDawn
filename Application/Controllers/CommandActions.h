#pragma once

#include <Nt/Core/Log.h>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <string>

class CommandActions {
public:
	using Action = std::function<void()>;

public:
	CommandActions() noexcept = default;

	void Execute(const std::string& command) const noexcept {
		if (!m_ActionMap.contains(command)) {
			Nt::Log::Warning("Unknown command: " + command);
			return;
		}

		const Action& action = m_ActionMap.at(command);
		if (action)
			action();
	}

	void Bind(const std::string& command, const Action& action) noexcept {
		m_ValidCommands.insert(command);
		m_ActionMap[command] = action;
	}

	[[nodiscard]] Bool IsBinded(const std::string& command) const noexcept {
		return m_ValidCommands.contains(command);
	}

private:
	std::unordered_map<std::string, Action> m_ActionMap;
	std::unordered_set<std::string> m_ValidCommands;
};
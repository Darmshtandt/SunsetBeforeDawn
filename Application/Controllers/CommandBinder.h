#pragma once

#include <Controllers/Command.h>
#include <Nt/Core/NtTypes.h>
#include <functional>

template <class _Ty>
class CommandBinder {
public:
	using State = Command::State;
	using Handler = std::function<void(const Command&)>;

public:
	CommandBinder(Handler handler) : m_Handler(std::move(handler)) {
		Assert(m_Handler, "Invalid handler");
	}

	void Handle(const _Ty& key, const State& state) const noexcept {
		if (!m_CommandMap.contains(key))
			return;

		const std::string& command = m_CommandMap.at(key);
		m_Handler({ command, state });
	}

	void Bind(const _Ty& key, const std::string& command) noexcept {
		m_CommandMap[key] = command;
	}

	void SetHandler(const Handler& handler) {
		m_Handler = handler;
		Assert(m_Handler, "Invalid handler");
	}

	[[nodiscard]] Bool IsBinded(const _Ty& key) const noexcept {
		return m_CommandMap.contains(key);
	}

private:
	std::unordered_map<_Ty, std::string> m_CommandMap;
	Handler m_Handler;
};
#pragma once

#include <Listeners/KeyboardHandler.h>
#include <Listeners/MouseHandler.h>
#include <Controllers/CommandActions.h>

class InputMapper {
public:
	using Button = Nt::Mouse::Button;
	using Key = Nt::Key;

public:
	InputMapper();

	void Register(NotNull<Nt::Window*> pWindow) const;
	void Unregister(NotNull<Nt::Window*> pWindow) const;

	void BindKeyCommand(const Nt::Key& key, const std::string& command) const noexcept;
	void BindMouseCommand(const Nt::Mouse::Button& button, const std::string& command) const noexcept;

	[[nodiscard]] Bool HasActive() const noexcept;
	[[nodiscard]] Bool IsActive(const std::string& command) const noexcept;

	[[nodiscard]] const std::unordered_set<std::string>& GetActiveCommands() const noexcept;

	void SetCommandActions(CommandActions* pCommandActions) noexcept;

private:
	std::unordered_set<std::string> m_ActiveCommands;
	std::unique_ptr<KeyboardHandler> m_pKeyboard;
	std::unique_ptr<MouseHandler> m_pMouse;
	CommandActions* m_pCommandActions;
};

#include <Controllers/InputMapper.h>

InputMapper::InputMapper() {
	auto handler = [this] (const Command& command) {
		if (command._State == Command::State::STARTED) {
			m_ActiveCommands.insert(command.Name);
		}
		else {
			m_ActiveCommands.erase(command.Name);

			if (m_pCommandActions != nullptr)
				m_pCommandActions->Execute(command.Name);
		}
	};

	m_pKeyboard = std::make_unique<KeyboardHandler>(handler);
	m_pMouse = std::make_unique<MouseHandler>(handler);
}

void InputMapper::Register(NotNull<Nt::Window*> pWindow) const {
	pWindow->RegisterKeyboardListener(m_pKeyboard.get());
	pWindow->RegisterMouseListener(m_pMouse.get());
}

void InputMapper::Unregister(NotNull<Nt::Window*> pWindow) const {
	pWindow->UnregisterKeyboardListener(m_pKeyboard.get());
	pWindow->UnregisterMouseListener(m_pMouse.get());
}

void InputMapper::BindKeyCommand(const Nt::Key& key, const std::string& command) const noexcept {
	m_pKeyboard->Bind(key, command);
}

void InputMapper::BindMouseCommand(const Nt::Mouse::Button& button, const std::string& command) const noexcept {
	m_pMouse->Bind(button, command);
}

Bool InputMapper::HasActive() const noexcept {
	return !m_ActiveCommands.empty();
}

Bool InputMapper::IsActive(const std::string& command) const noexcept {
	return m_ActiveCommands.contains(command);
}

const std::unordered_set<std::string>& InputMapper::GetActiveCommands() const noexcept {
	return m_ActiveCommands;
}

void InputMapper::SetCommandActions(CommandActions* pCommandActions) noexcept {
	m_pCommandActions = pCommandActions;
}

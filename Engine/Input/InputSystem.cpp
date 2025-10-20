#include <Engine/Input/InputSystem.h>
#include <Nt/Core/Input.h>

void InputSystem::LoadKeyBinds() {
	m_Binder.Context(InputContextID::Movement)
		.Bind(Nt::KEY_W, InputCommandID::Forward)
		.Bind(Nt::KEY_A, InputCommandID::Left)
		.Bind(Nt::KEY_S, InputCommandID::Back)
		.Bind(Nt::KEY_D, InputCommandID::Right)
		.Bind(Nt::KEY_SPACE, InputCommandID::Up)
		.Bind(Nt::KEY_SHIFT, InputCommandID::Down);

	m_Binder.Context(InputContextID::Combat)
		.Bind(Nt::BUTTON_LEFT, InputCommandID::Fire);
}

void InputSystem::TranslateInput(RawInputQueue queue) {
	while (!queue.empty()) {
		const RawInputEvent& event = queue.front();
		if (event.Pressed)
			m_ActiveKey.insert(event.Key);
		else
			m_ActiveKey.erase(event.Key);

		queue.pop();
	}

	m_Binder.HandleKeys(m_ActiveKey);
}
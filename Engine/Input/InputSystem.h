#pragma once

#include <Engine/Input/InputBinder.h>
#include <Core/Input/RawInput.h>

class InputSystem final {
public:
	InputSystem() noexcept = default;
	InputSystem(const InputSystem&) noexcept = default;
	InputSystem(InputSystem&&) noexcept = default;
	~InputSystem() noexcept = default;

	InputSystem& operator = (const InputSystem&) noexcept = default;
	InputSystem& operator = (InputSystem&&) noexcept = default;

	void LoadKeyBinds();
	void TranslateInput(RawInputQueue queue);

private:
	InputBinder m_Binder;
	std::unordered_set<Int> m_ActiveKey;
};
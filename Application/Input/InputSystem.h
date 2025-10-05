#pragma once

#include <Input/InputMapper.h>
#include <Input/RawInput.h>

class Scene;

class InputSystem final {
public:
	InputSystem() noexcept = default;
	InputSystem(const InputSystem&) noexcept = default;
	InputSystem(InputSystem&&) noexcept = default;
	~InputSystem() noexcept = default;

	InputSystem& operator = (const InputSystem&) noexcept = default;
	InputSystem& operator = (InputSystem&&) noexcept = default;

	void LoadKeyBinds();
	void Update(const Scene& scene, RawInputQueue queue);

private:
	InputMapper m_Mapper;
	std::unordered_set<Int> m_ActiveKey;
};
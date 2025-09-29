#pragma once

#include <Input/InputMapper.h>
#include <Input/RawInput.h>
#include <Objects/Components/InputComponent.h>
#include <World/Scene.h>

class GameObject;

class InputSystem final {
public:
	InputSystem();
	InputSystem(const InputSystem&) noexcept = default;
	InputSystem(InputSystem&&) noexcept = default;
	~InputSystem() noexcept = default;

	InputSystem& operator = (const InputSystem&) noexcept = default;
	InputSystem& operator = (InputSystem&&) noexcept = default;

	void Update(Scene& scene, RawInputQueue& queue);

private:
	InputMapper m_Mapper;
	std::unordered_set<Int> m_ActiveKey;
};

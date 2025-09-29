#pragma once

class InputSystem final {
public:
	struct MovementCommands {
		
	};
	struct InputCommands final : public MovementCommands {
		
	};

public:
	InputSystem() noexcept = default;
	InputSystem(const InputSystem&) noexcept = default;
	InputSystem(InputSystem&&) noexcept = default;
	~InputSystem() noexcept = default;

	InputSystem& operator = (const InputSystem&) noexcept = default;
	InputSystem& operator = (InputSystem&&) noexcept = default;


};
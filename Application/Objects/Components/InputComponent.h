#pragma once

#include <Input/Commands.h>
#include <Objects/Components/IComponent.h>
#include <unordered_set>

class InputComponent final : public IComponent {
public:
	InputComponent() noexcept;
	InputComponent(const InputComponent&) noexcept = default;
	InputComponent(InputComponent&&) noexcept = default;
	~InputComponent() noexcept override = default;

	InputComponent& operator = (const InputComponent&) noexcept = default;
	InputComponent& operator = (InputComponent&&) noexcept = default;

public:
	std::unordered_set<Command> Commands;
};
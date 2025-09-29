#pragma once

#include <Nt/Graphics/System/Window.h>

class MouseHandler final : public Nt::MouseListener {
public:
	using OnPressed = std::function<void(const Int& button)>;
	using OnReleased = std::function<void(const Int& button)>;

public:
	MouseHandler(OnPressed onPressed, OnReleased onReleased);

	MouseHandler() = delete;
	MouseHandler(const MouseHandler&) noexcept = default;
	MouseHandler(MouseHandler&&) noexcept = default;
	~MouseHandler() noexcept = default;

	MouseHandler& operator = (const MouseHandler&) noexcept = default;
	MouseHandler& operator = (MouseHandler&&) noexcept = default;

	void ButtonPressed(const Nt::Mouse::Button& button) override;
	void ButtonReleased(const Nt::Mouse::Button& button) override;

private:
	OnPressed m_OnPressed;
	OnReleased m_OnReleased;
};
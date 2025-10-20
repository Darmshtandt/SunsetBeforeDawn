#pragma once

#include <Nt/Graphics/System/Window.h>

class KeyboardHandler final : public Nt::KeyboardListener {
public:
	using OnPressed = std::function<void(const Int& key)>;
	using OnReleased = std::function<void(const Int& key)>;

public:
	KeyboardHandler(OnPressed onPressed, OnReleased onReleased);

	KeyboardHandler() = delete;
	KeyboardHandler(const KeyboardHandler&) noexcept = default;
	KeyboardHandler(KeyboardHandler&&) noexcept = default;
	~KeyboardHandler() noexcept = default;

	KeyboardHandler& operator = (const KeyboardHandler&) noexcept = default;
	KeyboardHandler& operator = (KeyboardHandler&&) noexcept = default;

	void KeyPressed(const Nt::Key& key) override;
	void KeyReleased(const Nt::Key& key) override;

private:
	OnPressed m_OnPressed;
	OnReleased m_OnReleased;
};
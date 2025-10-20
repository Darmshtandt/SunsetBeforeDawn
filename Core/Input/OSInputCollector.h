#pragma once

#include <Core/Input/MouseHandler.h>
#include <Core/Input/KeyboardHandler.h>
#include <Core/Input/RawInput.h>

class OSInputCollector final {
public:
	OSInputCollector();

	OSInputCollector(const OSInputCollector&) = delete;
	OSInputCollector(OSInputCollector&&) noexcept = default;
	~OSInputCollector() noexcept = default;

	OSInputCollector& operator = (const OSInputCollector&) = delete;
	OSInputCollector& operator = (OSInputCollector&&) noexcept = default;

	void Register(NotNull<Nt::Window*> pWindow) const;
	void Unregister(NotNull<Nt::Window*> pWindow) const;

	[[nodiscard]] RawInputQueue Poll() noexcept;

private:
	RawInputQueue m_RawInputQueue;
	std::unique_ptr<MouseHandler> m_pMouseHandler;
	std::unique_ptr<KeyboardHandler> m_pKeyboardHandler;

private:
	void _KeyboardHandler(const Int& key, const Bool& isPressed);
	void _MouseHandler(const Int& button, const Bool& isPressed);
};
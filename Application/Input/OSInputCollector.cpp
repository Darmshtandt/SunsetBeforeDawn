#include <Input/OSInputCollector.h>

OSInputCollector::OSInputCollector() {
	auto onMousePressed = [this] (const Int& button) {
		_MouseHandler(button, true);
		};
	auto onMouseReleased = [this] (const Int& button) {
		_MouseHandler(button, false);
		};

	m_pMouseHandler = std::make_unique<MouseHandler>(
		onMousePressed, onMouseReleased);


	auto onKeyPressed = [this] (const Int& key) {
		_KeyboardHandler(key, true);
		};
	auto onKeyReleased = [this] (const Int& key) {
		_KeyboardHandler(key, false);
		};

	m_pKeyboardHandler = std::make_unique<KeyboardHandler>(
		onKeyPressed, onKeyReleased);
}

void OSInputCollector::Register(NotNull<Nt::Window*> pWindow) const {
	pWindow->RegisterMouseListener(m_pMouseHandler.get());
	pWindow->RegisterKeyboardListener(m_pKeyboardHandler.get());
}

void OSInputCollector::Unregister(NotNull<Nt::Window*> pWindow) const {
	pWindow->UnregisterMouseListener(m_pMouseHandler.get());
	pWindow->UnregisterKeyboardListener(m_pKeyboardHandler.get());
}

RawInputQueue OSInputCollector::Poll() noexcept {
	RawInputQueue queue;
	queue.swap(m_RawInputQueue);
	return queue;
}

void OSInputCollector::_KeyboardHandler(const Int& key, const Bool& isPressed) {
	RawInputEvent input;
	input.Type = RawInputType::Keyboard;
	input.Key = key;
	input.Pressed = isPressed;

	m_RawInputQueue.push(input);
}

void OSInputCollector::_MouseHandler(const Int& button, const Bool& isPressed) {
	RawInputEvent input;
	input.Type = RawInputType::Mouse;
	input.Key = button;
	input.Pressed = isPressed;

	m_RawInputQueue.push(input);
}

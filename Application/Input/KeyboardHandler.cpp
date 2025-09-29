#include <Input/KeyboardHandler.h>

KeyboardHandler::KeyboardHandler(OnPressed onPressed, OnReleased onReleased) :
	m_OnPressed(std::move(onPressed)),
	m_OnReleased(std::move(onReleased))
{
	Assert(m_OnPressed, "Invalid OnPressed");
	Assert(m_OnReleased, "Invalid OnReleased");
}

void KeyboardHandler::KeyPressed(const Nt::Key& key) {
	m_OnPressed(static_cast<Int>(key));
}

void KeyboardHandler::KeyReleased(const Nt::Key& key) {
	m_OnReleased(static_cast<Int>(key));
}

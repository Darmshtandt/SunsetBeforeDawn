#include <Core/Input/MouseHandler.h>

MouseHandler::MouseHandler(OnPressed onPressed, OnReleased onReleased) :
	m_OnPressed(std::move(onPressed)),
	m_OnReleased(std::move(onReleased))
{
	Assert(m_OnPressed, "Invalid OnPressed");
	Assert(m_OnReleased, "Invalid OnReleased");
}

void MouseHandler::ButtonPressed(const Nt::Key& button) {
	m_OnPressed(button);
}

void MouseHandler::ButtonReleased(const Nt::Key& button) {
	m_OnReleased(button);
}

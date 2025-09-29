#pragma once

#include <Nt/Graphics/System/Window.h>
#include <Controllers/CommandBinder.h>

class MouseHandler :
	public CommandBinder<Nt::Mouse::Button>,
	public Nt::MouseListener {
public:
	MouseHandler(const Handler& handler) :
		CommandBinder(handler)
	{
	}

	void ButtonPressed(const Nt::Mouse::Button& button) override {
		if (IsBinded(button))
			Handle(button, State::STARTED);
	}
	void ButtonReleased(const Nt::Mouse::Button& button) override {
		if (IsBinded(button))
			Handle(button, State::STOPPED);
	}
};
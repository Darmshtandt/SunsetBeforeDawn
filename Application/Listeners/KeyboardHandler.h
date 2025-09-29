#pragma once

#include <Nt/Graphics/System/Window.h>
#include <Controllers/CommandBinder.h>

class KeyboardHandler : 
	public CommandBinder<Nt::Key>,
	public Nt::KeyboardListener {
public:
	KeyboardHandler(const Handler& handler) :
		CommandBinder(handler)
	{
	}

	void KeyPressed(const Nt::Key& key) override {
		if (IsBinded(key))
			Handle(key, State::STARTED);
	}
	void KeyReleased(const Nt::Key& key) override {
		if (IsBinded(key))
			Handle(key, State::STOPPED);
	}
};
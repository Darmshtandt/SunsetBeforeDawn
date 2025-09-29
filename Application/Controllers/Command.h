#pragma once

#include <string>

struct Command {
	enum State {
		STARTED,
		STOPPED
	};

	std::string Name;
	State _State;
};
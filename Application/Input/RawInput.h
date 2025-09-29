#pragma once

#include <Nt/Core/NtTypes.h>
#include <queue>

enum class RawInputType : Byte {
	Keyboard,
	Mouse
};

struct RawInputEvent {
	RawInputType Type;
	Int Key;
	Bool Pressed;
};

using RawInputQueue = std::queue<RawInputEvent>;
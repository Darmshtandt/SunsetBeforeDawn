#pragma once

#include <Nt/Core/NtTypes.h>

enum class Command : Byte {
	Left,
	Right,
	Up,
	Down,
	Forward,
	Back
};
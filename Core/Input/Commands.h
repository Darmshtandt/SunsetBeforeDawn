#pragma once

#include <Nt/Core/NtTypes.h>
#include <unordered_set>

enum class InputCommandID : Byte {
	Left,
	Right,
	Up,
	Down,
	Forward,
	Back,

	Fire
};
enum class InputContextID : Byte {
	Movement,
	Combat
};

struct InputCommand final {
	InputContextID ContextID;
	std::unordered_set<InputCommandID> Commands;
};
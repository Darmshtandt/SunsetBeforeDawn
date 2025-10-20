#pragma once

#include <World/Components/GameComponents.h>
#include <Engine/AI/IBehavior.h>
#include <Nt/Core/Timer.h>

class Combat;

struct Pawn final {
	Actor This = { };
	Intent3D* pIntent3D = nullptr;
	const GameObject* pObject = nullptr;

	Bool IsViewTarget = false;
	Nt::Timer MemoryTimer;
};

struct CombatPawn final {
	Combat* pCombat = nullptr;
	const GameObject* pObject = nullptr;
};
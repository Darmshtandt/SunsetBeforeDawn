#pragma once

#include <World/Components/GameComponents.h>
#include <Engine/AI/IBehavior.h>
#include <Nt/Core/Timer.h>

class Combat;

struct Pawn final {
	Actor This = { };
	Intent3D* pIntent3D = nullptr;

	Bool IsViewTarget = false;
	Nt::Timer MemoryTimer;
};

struct CombatPawn final {
	Combat* pCombat = nullptr;
	const GameObject* pObject = nullptr;
};

struct LivingPawn final {
	Transform3D* pTransform = nullptr;
	Health* pHealth = nullptr;
	Armor* pArmor = nullptr;
	const GameObject* pObject = nullptr;
};
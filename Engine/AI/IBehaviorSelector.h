#pragma once

#include <World/Entities.h>

struct IBehaviorSelector {
	virtual ~IBehaviorSelector() noexcept = default;

	virtual void OnSpawned(Pawn& pawn) = 0;
	virtual void OnTargetFound(Pawn& pawn) = 0;
	virtual void OnTargetLost(Pawn& pawn) = 0;
	virtual void OnTargetReached(Pawn& pawn) = 0;

	virtual void OnEnterView(Pawn& pawn) = 0;
	virtual void OnInView(Pawn& pawn) = 0;
	virtual void OnExitView(Pawn& pawn) = 0;
};
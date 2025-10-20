#pragma once

#include <Engine/AI/IBehaviorSelector.h>
#include <GameLogic/Behaviors/Wandering.h>
#include <GameLogic/Behaviors/Persecution.h>

struct DefaultBehaviorSelector : public IBehaviorSelector {
	~DefaultBehaviorSelector() noexcept override = default;

	void OnSpawned(Pawn& pawn) override {
		pawn.pIntent3D->SetBehavior(BehaviorLocator::Get<Wandering>());
	}
	void OnTargetFound(Pawn& pawn) override {
		const Intent3D::Target& target = pawn.pIntent3D->GetTarget();

		pawn.This.pRoute->Clear();
		pawn.This.pRoute->PushMarker({
			target.pTransform->Position(),
			pawn.This.pMovement->Speed
			});

		pawn.pIntent3D->SetBehavior(BehaviorLocator::Get<Persecution>());
	}
	void OnTargetLost(Pawn& pawn) override {
		pawn.pIntent3D->SetBehavior(BehaviorLocator::Get<Wandering>());
	}

	void OnEnterView(Pawn& pawn) {

	}
	void OnInView(Pawn& pawn) {
		pawn.This.pRoute->Clear();
		pawn.This.pRoute->PushMarker({
			pawn.pIntent3D->GetTarget().pTransform->Position(),
			pawn.This.pMovement->Speed
			});
	}
	void OnExitView(Pawn& pawn) {

	}
};
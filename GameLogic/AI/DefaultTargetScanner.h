#pragma once

#include <Engine/AI/ITargetScanner.h>

struct DefaultTargetScanner : public ITargetScanner {
	~DefaultTargetScanner() noexcept override = default;
	
	[[nodiscard]] Bool IsVisibleTarget(const Pawn& pawn, const View& view, const Target& target) const noexcept override {
		const Float distanceSq = pawn.This.pTransform->DistanceSquare(*target.pTransform);
		const Float thresholdSq = view.Range * view.Range;
		return distanceSq < thresholdSq;
	}

	[[nodiscard]] Bool Scan(Pawn& pawn, const std::vector<Target>& targets, const View& view) override {
		for (const Target& target : targets) {
			if (pawn.pObject == target.pObject)
				continue;

			if (!IsVisibleTarget(pawn, view, target))
				continue;

			pawn.pIntent3D->SetTarget(target);
			pawn.IsViewTarget = true;

			return true;
		}

		return false;
	}
};
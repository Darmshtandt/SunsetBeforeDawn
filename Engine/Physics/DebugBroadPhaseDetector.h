#pragma once

#include <Engine/Physics/Base/PhysicsInterfaces.h>

class DebugBroadPhaseDetector final : public IBroadPhaseDetector {
public:
	DebugBroadPhaseDetector() noexcept = default;
	~DebugBroadPhaseDetector() noexcept override = default;

	[[nodiscard]]
	std::vector<PhysicObjectPair> ComputePairs(const std::vector<PhysicObject>& bodies) const override {
		std::vector<PhysicObjectPair> result;
		for (uInt i = 0; i < bodies.size(); ++i) {
			const PhysicObject& objectA = bodies[i];
			if (!objectA.pCollider)
				continue;

			for (uInt j = i + 1; j < bodies.size(); ++j) {
				const PhysicObject& objectB = bodies[j];
				if (!objectB.pCollider)
					continue;
				if (!objectA.pRigidBody && !objectB.pRigidBody)
					continue;

				result.emplace_back(&objectA, &objectB);
			}
		}
		return result;
	}
};
#pragma once

#include <Engine/Physics/Base/PhysicsInterfaces.h>

class NarrowPhaseDetectorGJK final : public INarrowPhaseDetector {
public:
	NarrowPhaseDetectorGJK() noexcept = default;
	~NarrowPhaseDetectorGJK() noexcept override = default;

	[[nodiscard]]
	std::vector<CollisionContact> GenerateContacts(const std::vector<PhysicObjectPair>& pairs) const override;
};
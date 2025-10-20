#include <Engine/Physics/NarrowPhaseDetectorGJK.h>
#include <Core/Nt/Algorithms/GJK.h>
#include <Core/Nt/Algorithms/EPA.h>

std::vector<CollisionContact> NarrowPhaseDetectorGJK::
GenerateContacts(const std::vector<PhysicObjectPair>& pairs) const {
	std::vector<CollisionContact> result;
	for (const PhysicObjectPair& pair : pairs) {
		const ColliderPair colliderPair = {
			*pair.pA->pCollider,
			*pair.pB->pCollider
		};

		const std::pair<Bool, Nt::Simplex> gjkResult =
			Nt::GJK::CheckCollision(colliderPair);
		if (!gjkResult.first)
			continue;

		const Nt::CollisionPoint point =
			Nt::EPA::CheckCollision(gjkResult.second, colliderPair);

		result.emplace_back(pair.pA, pair.pB, Nt::Float3D(), point.Normal, point.Depth);
	}
	return result;
}

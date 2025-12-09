#pragma once

#include <Engine/Physics/Base/PhysicObject.h>

namespace Nt {
	struct Ray;
}

struct IPhysicsIntegrator {
	virtual ~IPhysicsIntegrator() noexcept = default;
	virtual void Integrate(const PhysicObject& object, const Float& deltaTime) const = 0;
	virtual void SetGravityVelocity(const Nt::Float3D& gravity) noexcept = 0;
};

struct IBroadPhaseDetector {
	virtual ~IBroadPhaseDetector() noexcept = default;
	[[nodiscard]]
	virtual std::vector<PhysicObjectPair> ComputePairs(const std::vector<PhysicObject>& bodies) const = 0;
};

struct INarrowPhaseDetector {
	virtual ~INarrowPhaseDetector() noexcept = default;
	[[nodiscard]]
	virtual std::vector<ObjectContactPair> GenerateContacts(const std::vector<PhysicObjectPair>& pairs) const = 0;
};

struct ICollisionResolver {
	virtual ~ICollisionResolver() noexcept = default;
	virtual void Resolve(const ObjectContactPair& contact) const = 0;
};

struct IPhysicsOverlapper {
	virtual ~IPhysicsOverlapper() noexcept = default;

	[[nodiscard]]
	virtual std::vector<PhysicObject> OverlapSphere(const Nt::Float3D& position, const Float& radius, const Int& layerMask) = 0;
	[[nodiscard]]
	virtual std::vector<PhysicObject> OverlapAABB(const Nt::Float3D& min, const Nt::Float3D& max, const Int& layerMask) = 0;
	[[nodiscard]]
	virtual std::vector<PhysicObject> OverlapRayCast(const Nt::Ray& ray, const Int& layerMask) = 0;
};
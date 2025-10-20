#pragma once

#include <Engine/Physics/Base/PhysicObject.h>

struct CollisionContact final {
	const PhysicObject* pA;
	const PhysicObject* pB;
	Nt::Float3D ContactPoint;
	Nt::Float3D Normal;
	Float Depth;
};

struct PhysicObjectPair final {
	PhysicObjectPair() noexcept = default;
	PhysicObjectPair(const CollisionContact& contact) noexcept :
		pA(contact.pA),
		pB(contact.pB)
	{
	}
	PhysicObjectPair(const PhysicObject* a, const PhysicObject* b) noexcept :
		pA(a),
		pB(b)
	{
	}

	[[nodiscard]] Bool operator == (const PhysicObjectPair& other) const noexcept {
		return (pA == other.pA && pB == other.pB)
			|| (pA == other.pB && pB == other.pA);
	}
	[[nodiscard]] Bool operator == (const CollisionContact& contact) const noexcept {
		return (pA == contact.pA && pB == contact.pB)
			|| (pA == contact.pB && pB == contact.pA);
	}

	const PhysicObject* pA;
	const PhysicObject* pB;
};
struct PhysicObjectHash final {
	[[nodiscard]] std::size_t operator () (const PhysicObjectPair& pair) const noexcept {
		return
			reinterpret_cast<std::size_t>(pair.pA) ^
			reinterpret_cast<std::size_t>(pair.pB);
	}
};

//[[nodiscard]] inline Bool operator == (const CollisionContact& contact, const PhysicObjectPair& pair) noexcept {
//	return (pair.pA == contact.pA && pair.pB == contact.pB)
//		|| (pair.pA == contact.pB && pair.pB == contact.pA);
//}


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
	virtual std::vector<CollisionContact> GenerateContacts(const std::vector<PhysicObjectPair>& pairs) const = 0;
};

struct ICollisionResolver {
	virtual ~ICollisionResolver() noexcept = default;
	virtual void Resolve(const CollisionContact& contact) const = 0;
};
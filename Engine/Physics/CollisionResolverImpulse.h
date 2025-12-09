#pragma once

#include <Engine/Physics/Base/PhysicsInterfaces.h>

class CollisionResolverImpulse final : public ICollisionResolver {
public:
	CollisionResolverImpulse() noexcept = default;
	~CollisionResolverImpulse() noexcept override = default;

	void Resolve(const ObjectContactPair& pair) const override;
};
#pragma once

#include <World/Components/PhysicComponents.h>

struct NearestPoint final {
	Nt::Float4D Point;
	Float Distance = FLT_MAX;
};

[[nodiscard]] inline
NearestPoint GetNearestPoint(const Nt::PointContainer& points, const Nt::Float3D& position) {
	NearestPoint nearest = { };
	for (const Nt::Float4D& point : points) {
		const Float distanceSq = position.GetSquareDistance(point.xyz);
		if (nearest.Distance > distanceSq) {
			nearest.Point = point;
			nearest.Distance = distanceSq;
		}
	}

	if (!points.empty())
		nearest.Distance = std::sqrtf(nearest.Distance);
	return nearest;
}

[[nodiscard]] inline
Bool HasColliderInRange(const Collider& collider, const Nt::Float3D& position, const Float& range) {
	const Nt::Matrix4x4& localToWorld = collider.LocalToWorld();
	const Nt::PointContainer& colliderPoints = collider.GetPoints();

	const Float rangeSq = range * range;
	for (const Nt::Float4D& localPoint : colliderPoints) {
		const Nt::Float4D realPoint = localToWorld * localPoint;

		const Float distanceSq = position.GetSquareDistance(realPoint.xyz);
		if (distanceSq <= rangeSq)
			return true;
	}

	return false;
}

[[nodiscard]] inline
Bool CheckAABBCollision(const Collider& aCollider, const AABBBox& bBox) {
	const AABBBox aBox = aCollider.GetAABBBox();
	return (aBox.Min <= bBox.Max && aBox.Max >= bBox.Min);
}
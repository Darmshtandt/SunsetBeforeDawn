#pragma once

#include <Core/Nt/HitBox.h>
#include <Nt/Graphics/Geometry/Primitives.h>

namespace Nt {
	struct BoundingBox final {
		NT_NODISCARD Bool HasIntersection(const BoundingBox& other) const noexcept {
			return other.Min <= Max && Min <= other.Max;
		}

		NT_NODISCARD Bool HasSphereIntersection(const Float3D& center, Float radius) const noexcept {
			const Float3D closest = Max.Min(Min.Max(center));
			const Float3D diff = center - closest;
			const Float distanceSq = diff.LengthSquare();

			return distanceSq <= radius * radius;
		}

		Float3D Min = { FLT_MAX, FLT_MAX, FLT_MAX };
		Float3D Max = { FLT_MIN, FLT_MIN, FLT_MIN };
	};


	__inline NT_NODISCARD BoundingBox CalculateBoundingBox(const PointContainer& points) noexcept {
		BoundingBox box;
		for (const Float4D& point : points) {
			box.Min = box.Min.Min(point.xyz);
			box.Max = box.Max.Max(point.xyz);
		}

		return box;
	}
	__inline NT_NODISCARD BoundingBox CalculateBoundingBox(const Shape& shape) noexcept {
		BoundingBox box;
		for (const Vertex& vertex : shape.Vertices) {
			box.Min = box.Min.Min(vertex.Position.xyz);
			box.Max = box.Max.Max(vertex.Position.xyz);
		}

		return box;
	}
}

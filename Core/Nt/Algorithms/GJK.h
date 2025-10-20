#pragma once

#include <World/Components/PhysicComponents.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Core/Math/Vector.h>

namespace Nt {
	class GJK final {
	public:
		[[nodiscard]] static Float3D Support(const ColliderPair& colliderPair, const Float3D& direction);

		[[nodiscard]] static
		std::pair<Bool, Simplex> CheckCollision(const ColliderPair& colliderPair);

	private:
		[[nodiscard]] static Float3D FindFurthestPoint(const Collider& collider, const Float3D& direction);

		[[nodiscard]] static Bool Line(Simplex& simplex, Float3D& direction);
		[[nodiscard]] static Bool Triangle(Simplex& simplex, Float3D& direction);
		[[nodiscard]] static Bool Polyhedron(Simplex& simplex, Float3D& direction);

		[[nodiscard]] static Bool NextSimplex(Simplex& simplex, Float3D& direction);

		[[nodiscard]] static Matrix4x4 TransformWorldMatrix(const Matrix4x4& world) noexcept;
	};
}
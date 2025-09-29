#pragma once

#include <Nt/CollisionAlgorithms/CollisionAlgorithm.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Core/Math/Vector.h>

namespace Nt {
	class GJK_EPA final : public CollisionAlgorithm {
	public:
		explicit GJK_EPA(NotNull<Collider*> pCollider) noexcept;

		GJK_EPA() = delete;
		GJK_EPA(const GJK_EPA&) noexcept = default;
		GJK_EPA(GJK_EPA&&) noexcept = default;
		virtual ~GJK_EPA() noexcept = default;

		GJK_EPA& operator = (const GJK_EPA&) noexcept = default;
		GJK_EPA& operator = (GJK_EPA&&) noexcept = default;

		[[nodiscard]] Bool CheckCollision(const Collider& collider, CollisionPoint* pPoint) override;

	private:
		[[nodiscard]] std::pair<Bool, Simplex> GJK(const Collider& collider) const;
		[[nodiscard]] CollisionPoint EPA(const Simplex& simplex, const Collider& collider) const;

		void _AddIfUniqueEdge(std::vector<std::pair<uInt, uInt>>& edges, const std::vector<uInt>& faces, const uInt& a, const uInt& b) const;

		[[nodiscard]] std::pair<FaceNormalContainer, uInt> GetFaceNormals(const PointContainer& points, const std::vector<uInt>& faces) const;
		[[nodiscard]] Float3D _FindFurthestPoint(const Collider& collider, const Float3D& direction) const;
		[[nodiscard]] Float3D _Support(const Collider& collider, const Float3D& direction) const;

		[[nodiscard]] Bool _Line(Simplex& simplex, Float3D& direction) const;
		[[nodiscard]] Bool _Triangle(Simplex& simplex, Float3D& direction) const;
		[[nodiscard]] Bool _Polyhedron(Simplex& simplex, Float3D& direction) const;

		[[nodiscard]] Bool _NextSimplex(Simplex& simplex, Float3D& direction) const;
	};
}
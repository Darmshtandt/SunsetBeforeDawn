#pragma once

#include <Nt/Collider.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Core/Math/Vector.h>

namespace Nt {
	struct CollisionPoint final {
		Float3D Normal;
		Float Depth;
	};
	struct FaceNormal final {
		Float3D Normal;
		Float Distance;
	};

	using FaceNormalContainer = std::vector<FaceNormal>;

	class EPA final {
	public:
		[[nodiscard]] static
		CollisionPoint CheckCollision(const Simplex& simplex, const ColliderPair& colliderPair);

	private:
		static void AddUniqueEdge(std::vector<std::pair<uInt, uInt>>& edges, const std::vector<uInt>& faces, const uInt& a, const uInt& b);
		[[nodiscard]] static std::pair<FaceNormalContainer, uInt> GetFaceNormals(const PointContainer& points, const std::vector<uInt>& faces);
	};
}
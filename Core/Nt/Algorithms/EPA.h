#pragma once

#include <World/Components/PhysicComponents.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Core/Math/Vector.h>

namespace Nt {
	struct CollisionPoint final {
		Float3D Normal;
		Float Depth = FLT_MIN;
	};
	struct FaceNormal final {
		Float3D Normal;
		Float Distance = FLT_MIN;
	};

	class EPA final {
	public:
		NT_CONSTEXPR static Float Epsilon = 0.000001f;

	public:
		struct NormalContainer final {
			NT_FORCE_INLINE
			void Reserve(const uInt& capacity) {
				Normals.reserve(capacity);
			}

			NT_FORCE_INLINE
			void EmplaceBack(const Float3D& normal, const Float& distance) {
				Normals.emplace_back(normal, distance);
			}

			NT_NODISCARD NT_FORCE_INLINE
			const FaceNormal& operator [] (const uInt& index) const {
				Assert(index < Normals.size(), "Out of range");
				return Normals[index];
			}

			std::vector<FaceNormal> Normals;
			uInt NearFaceIndex = uInt(-1);
		};

	public:
		[[nodiscard]] static
		CollisionPoint CheckCollision(const Simplex& simplex, const ColliderPair& colliderPair);

	private:
		static void AddUniqueEdge(std::vector<std::pair<uInt, uInt>>& edges, const std::vector<uInt>& faces, const uInt& a, const uInt& b);
		[[nodiscard]] static NormalContainer PolytopeToNormals(const Polytope& polytope, const std::vector<uInt>& faces);
	};
}
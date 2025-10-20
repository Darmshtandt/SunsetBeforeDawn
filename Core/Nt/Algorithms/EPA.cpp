#include <Core/Nt/Algorithms/EPA.h>
#include <Core/Nt/Algorithms/GJK.h>

namespace Nt {
	CollisionPoint EPA::CheckCollision(const Simplex& simplex, const ColliderPair& colliderPair) {
		Polytope polytope = simplex.ToPolytope();

		std::vector<uInt> faces = {
			0, 1, 2,
			0, 3, 1,
			0, 2, 3,
			1, 3, 2
		};

		NormalContainer normalContainer = PolytopeToNormals(polytope, faces);
		uInt& minFace = normalContainer.NearFaceIndex;

		Float3D minNormal = normalContainer[minFace].Normal;
		Float minDistance = FLT_MAX;

		const uInt pointCount = 
			colliderPair.first.GetPointCount() +
			colliderPair.second.GetPointCount();

		for (uInt i = 0; i < pointCount; ++i) {
			minNormal = normalContainer[minFace].Normal;
			minDistance = normalContainer[minFace].Distance;

			const Float3D support = GJK::Support(colliderPair, minNormal);
			const Float supportDistance = minNormal.Dot(support);
			if (std::abs(supportDistance - minDistance) <= Epsilon)
				break;

			minDistance = FLT_MAX;
			std::vector<std::pair<uInt, uInt>> uniqueEdges;

			uInt pointID = 0;
			for (const FaceNormal& faceNormal : normalContainer.Normals) {
				if (faceNormal.Normal.Dot(support) > 0) {
					AddUniqueEdge(uniqueEdges, faces, pointID + 0, pointID + 1);
					AddUniqueEdge(uniqueEdges, faces, pointID + 1, pointID + 2);
					AddUniqueEdge(uniqueEdges, faces, pointID + 2, pointID + 0);

					faces.erase(faces.begin() + pointID, faces.begin() + (pointID + 3));
				}
				else {
					pointID += 3;
				}
			}

			std::vector<uInt> newFaces;
			newFaces.reserve(uniqueEdges.size() * 3);
			for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
				newFaces.push_back(edgeIndex1);
				newFaces.push_back(edgeIndex2);
				newFaces.push_back(polytope.size());
			}
			polytope.push_back(support);

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());

			normalContainer = PolytopeToNormals(polytope, faces);
		}

		if (std::abs(minDistance - FLT_MAX) < Epsilon)
			return CollisionPoint { minNormal, 0.f };
		return CollisionPoint { minNormal, minDistance + Epsilon };
	}

	void EPA::AddUniqueEdge(std::vector<std::pair<uInt, uInt>>& edges, const std::vector<uInt>& faces, const uInt& a, const uInt& b) {
		auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));
		if (reverse != edges.end())
			edges.erase(reverse);
		else
			edges.emplace_back(faces[a], faces[b]);
	}

	EPA::NormalContainer EPA::PolytopeToNormals(const Polytope& polytope, const std::vector<uInt>& faces) {
		NormalContainer container;
		container.Reserve(faces.size() / 3);

		Float minDistance = FLT_MAX;
		for (uInt i = 0; i < faces.size(); i += 3) {
			const Float3D& a = polytope[faces[i + 0]];
			const Float3D& b = polytope[faces[i + 1]];
			const Float3D& c = polytope[faces[i + 2]];

			Float3D normal = (b - a).GetCross(c - a).GetNormalize();
			Float distance = normal.Dot(a);
			if (distance < -Epsilon) {
				normal = -normal;
				distance = -distance;
			}

			container.EmplaceBack(normal, distance);
			if (distance < minDistance) {
				container.NearFaceIndex = i / 3;
				minDistance = distance;
			}
		}

		return container;
	}
}
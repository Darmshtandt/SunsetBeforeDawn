#include <Nt/Algorithms/EPA.h>
#include <Nt/Algorithms/GJK.h>

namespace Nt {
	CollisionPoint EPA::CheckCollision(const Simplex& simplex, const ColliderPair& colliderPair) {
		std::vector<Float3D> polytope;
		for (uInt i = 0; i < simplex.GetSize(); ++i)
			polytope.push_back(simplex[i]);

		std::vector<uInt> faces = {
			0, 1, 2,
			0, 3, 1,
			0, 2, 3,
			1, 3, 2
		};

		std::pair<FaceNormalContainer, uInt> faceNormals = GetFaceNormals(polytope, faces);
		FaceNormalContainer& normalContainer = faceNormals.first;
		uInt& minFace = faceNormals.second;

		Float3D minNormal = normalContainer[minFace].Normal;
		Float minDistance = FLT_MAX;

		const uInt pointCount = 
			colliderPair.first.GetPoints().size() +
			colliderPair.second.GetPoints().size();

		for (uInt i = 0; i < pointCount; ++i) {
			minNormal = normalContainer[minFace].Normal;
			minDistance = normalContainer[minFace].Distance;

			const Float3D support = GJK::Support(colliderPair, minNormal);
			const Float supportDistance = minNormal.Dot(support);

			if (std::abs(supportDistance - minDistance) > 0.0001f) {
				minDistance = FLT_MAX;
				std::vector<std::pair<uInt, uInt>> uniqueEdges;

				uInt pointID = 0;
				for (FaceNormal& faceNormal : normalContainer) {
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

				auto newFaceNormals = GetFaceNormals(polytope, faces);

				normalContainer = std::move(newFaceNormals.first);
				minFace = newFaceNormals.second;
			}

			if (minDistance != FLT_MAX)
				break;
		}

		if (std::abs(minDistance - FLT_MAX) < FLT_EPSILON)
			return CollisionPoint { minNormal, 0 };
		return CollisionPoint { minNormal, minDistance + 0.0001f };
	}

	void EPA::AddUniqueEdge(std::vector<std::pair<uInt, uInt>>& edges, const std::vector<uInt>& faces, const uInt& a, const uInt& b) {
		auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));
		if (reverse != edges.end())
			edges.erase(reverse);
		else
			edges.emplace_back(faces[a], faces[b]);
	}

	std::pair<FaceNormalContainer, uInt> EPA::GetFaceNormals(const PointContainer& points, const std::vector<uInt>& faces) {
		FaceNormalContainer normalContainer;
		normalContainer.reserve(faces.size() / 3);

		uInt nearFaceIndex = 0;
		Float minDistance = FLT_MAX;
		for (uInt i = 0; i < faces.size(); i += 3) {
			const Float3D a = points[faces[i + 0]];
			const Float3D b = points[faces[i + 1]];
			const Float3D c = points[faces[i + 2]];

			Float3D normal = (b - a).GetCross(c - a).GetNormalize();
			Float distance = normal.Dot(a);
			if (distance < -0.000001f) {
				normal = -normal;
				distance = -distance;
			}

			normalContainer.emplace_back(FaceNormal(normal, distance));
			if (distance < minDistance) {
				nearFaceIndex = i / 3;
				minDistance = distance;
			}
		}
		return std::make_pair(normalContainer, nearFaceIndex);
	}
}
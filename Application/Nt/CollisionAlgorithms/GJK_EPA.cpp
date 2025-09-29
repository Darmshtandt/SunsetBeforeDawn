#include <Nt/CollisionAlgorithms/GJK_EPA.h>
#include <Nt/Collider.h>

namespace Nt {
	GJK_EPA::GJK_EPA(NotNull<Collider*> pCollider) noexcept :
		CollisionAlgorithm(pCollider)
	{
	}

	Bool GJK_EPA::CheckCollision(const Collider& collider, CollisionPoint* pPoint) {
		const std::pair<Bool, Simplex> result = GJK(collider);
		if (result.first && pPoint != nullptr)
			(*pPoint) = EPA(result.second, collider);
		return result.first;
	}

	Float3D GJK_EPA::_FindFurthestPoint(const Collider& collider, const Float3D& direction) const {
		Float3D maxPoint;
		Float maxDistance = -FLT_MAX;

		Matrix4x4 localToWorld = Matrix4x4::GetIdentity();
		localToWorld.Rows[3] = collider.LocalToWorld().Rows[3];

		Matrix4x4 transformedLocalWorld = localToWorld.Transposition();

		const Float3D left = localToWorld[0];
		const Float3D up = localToWorld[1];
		const Float3D lookAt = localToWorld[2];
		const Float3D eye = localToWorld[3];

		transformedLocalWorld[0].xyz = left / left.LengthSquare();
		transformedLocalWorld[0].w = -eye.Dot(left) / left.LengthSquare();

		transformedLocalWorld[1].xyz = up / up.LengthSquare();
		transformedLocalWorld[1].w = -eye.Dot(up) / up.LengthSquare();

		transformedLocalWorld[2].xyz = lookAt / lookAt.LengthSquare();
		transformedLocalWorld[2].w = -eye.Dot(lookAt) / lookAt.LengthSquare();
		transformedLocalWorld[3][3] = 1.f;

		transformedLocalWorld = localToWorld;

		const Float3D transformedDirection = (transformedLocalWorld * direction).GetNormalize().xyz;
		for (const Float3D& point : collider.GetPoints()) {
			const Float pointLength = point.Dot(transformedDirection);
			if (maxDistance < pointLength) {
				maxDistance = pointLength;
				maxPoint = point;
			}
		}
		return (localToWorld * maxPoint).xyz + localToWorld[3].xyz;
	}

	Float3D GJK_EPA::_Support(const Collider& collider, const Float3D& direction) const {
		return _FindFurthestPoint(*m_pCollider, direction) - _FindFurthestPoint(collider, -direction);
	}

	Bool GJK_EPA::_Line(Simplex& simplex, Float3D& direction) const {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];

		const Float3D ab = b - a;
		const Float3D ao = -a;

		if (ab.Dot(ao) > 0) {
			direction = ab.GetCross(ao).GetCross(ab);
		}
		else {
			simplex = { a };
			direction = ao;
		}
		return false;
	}

	Bool GJK_EPA::_Triangle(Simplex& simplex, Float3D& direction) const {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];
		const Float3D c = simplex[2];

		const Float3D ab = b - a;
		const Float3D ac = c - a;
		const Float3D ao = -a;

		const Float3D abc = ab.GetCross(ac);

		if (abc.GetCross(ac).Dot(ao) > 0) {
			if (ac.Dot(ao) > 0) {
				simplex = { a, c };
				direction = ac.GetCross(ao).GetCross(ac);
			}
			else {
				simplex = { a, b };
				return _Line(simplex, direction);
			}
		}
		else if (ab.GetCross(abc).Dot(ao) > 0) {
			simplex = { a, b };
			return _Line(simplex, direction);
		}
		else {
			if (abc.Dot(ao) > 0) {
				direction = abc;
			}
			else {
				simplex = { a, c, b };
				direction = -abc;
			}
		}
		return false;
	}

	Bool GJK_EPA::_Polyhedron(Simplex& simplex, Float3D& direction) const {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];
		const Float3D c = simplex[2];
		const Float3D d = simplex[3];

		const Float3D ab = b - a;
		const Float3D ac = c - a;
		const Float3D ad = d - a;
		const Float3D ao = -a;

		const Float3D abc = ab.GetCross(ac);
		const Float3D acd = ac.GetCross(ad);
		const Float3D adb = ad.GetCross(ab);

		if (abc.Dot(ao) > 0) {
			simplex = { a, b, c };
			return _Triangle(simplex, direction);
		}
		else if (acd.Dot(ao) > 0) {
			simplex = { a, c, d };
			return _Triangle(simplex, direction);
		}
		else if (adb.Dot(ao) > 0) {
			simplex = { a, d, b };
			return _Triangle(simplex, direction);
		}

		direction = { };
		return true;
	}

	Bool GJK_EPA::_NextSimplex(Simplex& simplex, Float3D& direction) const {
		switch (simplex.GetSize()) {
		case 2:
			return _Line(simplex, direction);
		case 3:
			return _Triangle(simplex, direction);
		case 4:
			return _Polyhedron(simplex, direction);
		}
		return true;
	}

	std::pair<Bool, Simplex> GJK_EPA::GJK(const Collider& collider) const {
		Simplex simplex;
		simplex.Add(_Support(collider, { 1.f, 0.f, 0.f }));

		Float3D direction = -simplex[0];
		do {
			Float3D point = _Support(collider, direction);
			if (point.Dot(direction) <= 0)
				return std::make_pair(false, Simplex());

			simplex.Add(point);
		}
		while (!_NextSimplex(simplex, direction));
		return std::make_pair(true, simplex);
	}

	std::pair<FaceNormalContainer, uInt> GJK_EPA::GetFaceNormals(const PointContainer& points, const std::vector<uInt>& faces) const {
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

	void GJK_EPA::_AddIfUniqueEdge(std::vector<std::pair<uInt, uInt>>& edges, const std::vector<uInt>& faces, const uInt& a, const uInt& b) const {
		auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));
		if (reverse != edges.end())
			edges.erase(reverse);
		else
			edges.emplace_back(faces[a], faces[b]);
	}

	CollisionPoint GJK_EPA::EPA(const Simplex& simplex, const Collider& collider) const {
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

		uInt iters = 0;
		while (minDistance == FLT_MAX && (iters++ < m_pCollider->GetPoints().size() + collider.GetPoints().size())) {
			minNormal = normalContainer[minFace].Normal;
			minDistance = normalContainer[minFace].Distance;

			Float3D support = _Support(collider, minNormal);
			Float supportDistance = minNormal.Dot(support);

			if (std::abs(supportDistance - minDistance) > 0.0001f) {
				minDistance = FLT_MAX;
				std::vector<std::pair<uInt, uInt>> uniqueEdges;

				uInt pointID = 0;
				for (auto& normal : normalContainer) {
					if (normal.Normal.Dot(support) > 0) {
						_AddIfUniqueEdge(uniqueEdges, faces, pointID + 0, pointID + 1);
						_AddIfUniqueEdge(uniqueEdges, faces, pointID + 1, pointID + 2);
						_AddIfUniqueEdge(uniqueEdges, faces, pointID + 2, pointID + 0);

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
		}

		if (std::abs(minDistance - FLT_MAX) < 0.000001f)
			return CollisionPoint { minNormal, 0 };
		return CollisionPoint { minNormal, minDistance + 0.0001f };
	}

}
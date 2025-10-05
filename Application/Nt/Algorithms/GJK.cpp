#include <Nt/Algorithms/GJK.h>

namespace Nt {
	std::pair<Bool, Simplex> GJK::CheckCollision(const ColliderPair& colliderPair) {
		Simplex simplex;
		simplex.Add(Support(colliderPair, { 1.f, 0.f, 0.f }));

		Float3D direction = -simplex[0];
		do {
			Float3D point = Support(colliderPair, direction);
			if (point.Dot(direction) <= FLT_EPSILON)
				return std::make_pair(false, Simplex());

			simplex.Add(point);
		}
		while (!NextSimplex(simplex, direction));
		return std::make_pair(true, simplex);
	}

	Float3D GJK::FindFurthestPoint(const Collider& collider, const Float3D& direction) {
		Matrix4x4 localToWorld = Matrix4x4::GetIdentity();
		localToWorld.Rows[3] = collider.GetLocalToWorld().Rows[3];

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

		Float3D maxPoint;
		Float maxDistance = -FLT_MAX;

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

	Float3D GJK::Support(const ColliderPair& colliderPair, const Float3D& direction) {
		return
			FindFurthestPoint(colliderPair.first, direction) -
			FindFurthestPoint(colliderPair.second, -direction);
	}

	Bool GJK::Line(Simplex& simplex, Float3D& direction) {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];

		const Float3D ab = b - a;
		const Float3D ao = -a;

		if (ab.Dot(ao) > FLT_EPSILON) {
			direction = ab.GetCross(ao).GetCross(ab);
		}
		else {
			simplex = { a };
			direction = ao;
		}
		return false;
	}

	Bool GJK::Triangle(Simplex& simplex, Float3D& direction) {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];
		const Float3D c = simplex[2];

		const Float3D ab = b - a;
		const Float3D ac = c - a;
		const Float3D ao = -a;

		const Float3D abc = ab.GetCross(ac);

		if (abc.GetCross(ac).Dot(ao) > FLT_EPSILON) {
			if (ac.Dot(ao) > FLT_EPSILON) {
				simplex = { a, c };
				direction = ac.GetCross(ao).GetCross(ac);
			}
			else {
				simplex = { a, b };
				return Line(simplex, direction);
			}
		}
		else if (ab.GetCross(abc).Dot(ao) > 0) {
			simplex = { a, b };
			return Line(simplex, direction);
		}
		else {
			if (abc.Dot(ao) > FLT_EPSILON) {
				direction = abc;
			}
			else {
				simplex = { a, c, b };
				direction = -abc;
			}
		}
		return false;
	}

	Bool GJK::Polyhedron(Simplex& simplex, Float3D& direction) {
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

		if (abc.Dot(ao) > FLT_EPSILON) {
			simplex = { a, b, c };
			return Triangle(simplex, direction);
		}
		if (acd.Dot(ao) > FLT_EPSILON) {
			simplex = { a, c, d };
			return Triangle(simplex, direction);
		}
		if (adb.Dot(ao) > FLT_EPSILON) {
			simplex = { a, d, b };
			return Triangle(simplex, direction);
		}

		direction = { };
		return true;
	}

	Bool GJK::NextSimplex(Simplex& simplex, Float3D& direction) {
		switch (simplex.GetSize()) {
		case 2:
			return Line(simplex, direction);
		case 3:
			return Triangle(simplex, direction);
		case 4:
			return Polyhedron(simplex, direction);
		default:
			break;
		}
		return true;
	}
}
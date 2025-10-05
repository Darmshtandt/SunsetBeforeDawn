#include <Nt/Algorithms/RayCast.h>

namespace Nt {
	RayCastResult RayCast::RayCastTest(const Collider& collider, const Ray& ray) {
		const PointContainer& points = collider.GetPoints();

		RayCastResult result = { };
		for (uInt i = 2; i < points.size(); i += 3) {
			const Double3D face[3] = {
				GetPointRealPosition(collider, points[i - 2]),
				GetPointRealPosition(collider, points[i - 1]),
				GetPointRealPosition(collider, points[i - 0]),
			};

			if (ray.IntersectTriangleTest(face, result.Point)) {
				result.FaceIndex = (i / 3);
				return result;
			}
		}

		return result;
	}

	Double3D RayCast::GetPointRealPosition(const Collider& collider, const Float3D& point) noexcept {
		const Matrix4x4& localToWorld = collider.GetLocalToWorld();

		Double3D position = Float3D(-localToWorld.Rows[3].xyz);
		//position.x -= Double((point.x * localToWorld._11) + (point.y * localToWorld._12) + (point.z * localToWorld._13));
		//position.y -= Double((point.x * localToWorld._21) + (point.y * localToWorld._22) + (point.z * localToWorld._23));
		//position.z -= Double((point.x * localToWorld._31) + (point.y * localToWorld._32) + (point.z * localToWorld._33));
		return position;
	}
}
#pragma once

#include <Nt/Collider.h>

namespace Nt {
	struct RayCastResult final {
		Float3D Point;
		Int FaceIndex = -1;
	};

	class RayCast final {
	public:
		[[nodiscard]] static RayCastResult RayCastTest(const Collider& collider, const Ray& ray);

	private:
		[[nodiscard]] static 
		Double3D GetPointRealPosition(const Collider& collider, const Float3D& point) noexcept;
	};
}
#pragma once

#include <Nt/Core/Utilities.h>
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

	using PointContainer = std::vector<Float3D>;
	using FaceNormalContainer = std::vector<FaceNormal>;
	//using uIntContainer = std::vector<uInt>;

	class Collider;

	class CollisionAlgorithm {
	protected:
		explicit CollisionAlgorithm(NotNull<Collider*> pCollider) noexcept;

	public:
		CollisionAlgorithm() = delete;
		CollisionAlgorithm(const CollisionAlgorithm&) noexcept = default;
		CollisionAlgorithm(CollisionAlgorithm&&) noexcept = default;
		virtual ~CollisionAlgorithm() noexcept = default;

		CollisionAlgorithm& operator = (const CollisionAlgorithm&) noexcept = default;
		CollisionAlgorithm& operator = (CollisionAlgorithm&&) noexcept = default;

		[[nodiscard]] virtual Bool CheckCollision(const Collider&, CollisionPoint*) = 0;

	protected:
		Collider* m_pCollider;
	};
}
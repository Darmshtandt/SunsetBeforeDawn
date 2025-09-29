#pragma once

#include <Nt/Graphics/Objects/Model.h>
#include <Nt/Graphics/Geometry/RayCast.h>
#include <Nt/Graphics/Renderer.h>
#include <Nt/CollisionAlgorithms/CollisionAlgorithm.h>

class GameObject;

namespace Nt {
	class Collider {
	public:
		explicit Collider(NotNull<GameObject*> pOwner);

		Collider() = delete;
		Collider(const Collider& collider) noexcept = default;
		Collider(Collider&&) noexcept = default;
		~Collider() noexcept = default;

		Collider& operator = (const Collider&) noexcept = default;
		Collider& operator = (Collider&&) noexcept = default;

		void Render(NotNull<Renderer*> pRenderer) const;

		void Show() noexcept;
		void Hide() noexcept;
		void ToggleVisible(const Bool& enabled) noexcept;
		void ToggleVisible() noexcept;

		[[nodiscard]] Bool CheckCollision(const Collider& collider, CollisionPoint* pPoint = nullptr);

		[[nodiscard]] Int RayCastTest(const Ray& ray, Float3D* pResultIntersectionPoint = nullptr) const;

		[[nodiscard]] Double3D GetPointRealPosition(const Float3D& point) const noexcept;
		[[nodiscard]] Matrix4x4 LocalToWorld() const noexcept;

		void SetShape(const Shape& shape);
		void SetPointContainer(const PointContainer& points);

		template <class _Ty, Requires(std::is_base_of_v<CollisionAlgorithm, _Ty>)>
		void SetAlgorithm() {
			m_pAlgorithm = std::make_unique<_Ty>(this);
		}

		[[nodiscard]] const PointContainer& GetPoints() const;
		[[nodiscard]] Bool IsVisible() const noexcept;

	private:
		std::unique_ptr<CollisionAlgorithm> m_pAlgorithm;
		GameObject* m_pOwner;
		Mesh m_Mesh;
		PointContainer m_Points;
		Model m_Model;
		Bool m_IsVisible = true;
	};
}
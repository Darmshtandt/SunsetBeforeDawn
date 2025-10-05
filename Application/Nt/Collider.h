#pragma once

#include <Nt/Graphics/Objects/Model.h>
#include <Nt/Graphics/Geometry/RayCast.h>
#include <Nt/Graphics/Renderer.h>

class GameObject;

namespace Nt {
	using PointContainer = std::vector<Float3D>;

	class Collider {
	public:
		Collider() noexcept = default;
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

		[[nodiscard]] Matrix4x4 GetLocalToWorld() const noexcept;

		void SetLocalToWorld(const Matrix4x4& matrix) noexcept;
		void SetShape(const Shape& shape);
		void SetPointContainer(const PointContainer& points);

		[[nodiscard]] const PointContainer& GetPoints() const;
		[[nodiscard]] Bool IsVisible() const noexcept;

	private:
		Matrix4x4 m_LocalToWorld;
		Mesh m_Mesh;
		PointContainer m_Points;
		Bool m_IsVisible = false;
	};

	using ColliderPair = std::pair<Collider, Collider>;
}
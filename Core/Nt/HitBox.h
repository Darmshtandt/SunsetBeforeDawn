#pragma once

#include <Nt/Graphics/Resources/Mesh.h>

namespace Nt {
	using PointContainer = std::vector<Float4D>;

	class HitBox {
	public:
		HitBox() noexcept = default;
		HitBox(const HitBox& collider) noexcept = default;
		HitBox(HitBox&&) noexcept = default;
		~HitBox() noexcept = default;

		HitBox& operator = (const HitBox&) noexcept = default;
		HitBox& operator = (HitBox&&) noexcept = default;

		void Show() noexcept;
		void Hide() noexcept;
		void ToggleVisible(const Bool& enabled) noexcept;
		void ToggleVisible() noexcept;
		
		void SetShape(const Shape& shape);
		void SetPointContainer(const PointContainer& points);

		[[nodiscard]] const PointContainer& GetPoints() const noexcept;
		[[nodiscard]] const Mesh* GetMeshPtr() const noexcept;
		[[nodiscard]] Bool IsVisible() const noexcept;

	private:
		Mesh m_Mesh;
		PointContainer m_Points;
		Bool m_IsVisible = false;
	};
}